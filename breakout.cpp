/**
 * @file breakout.cpp
 * @author Sharan Sajiv Menon
 * @brief Actual logic of breakout. My Breakout is a multicore implementation.
 * @version 0.1
 * @date 2022-06-22
 *
 * @copyright Copyright (c) 2022 Sharan Sajiv Menon
 *
 */
#include "breakout.h"
#include <circle/string.h>
#include <circle/util.h>
#include <circle/alloc.h>

#include <circle/string.h>
#include <assert.h>

#include "sprintf.h"
#include "logic.h"
#include "text.h"

static const char FromKernel[] = "kernel";

CBreakOut *CBreakOut::s_pThis = 0;

CBreakOut::CBreakOut(
    C2DGraphics *p_graphics,
    CScreenDevice *p_screen,
    CDeviceNameService *p_devicenameservice,
    // CLogger *p_Logger,
    CTimer *p_Timer,
    CUSBHCIDevice *p_USBHCI,
    CUSBKeyboardDevice *volatile pKeyboard,
    CMemorySystem *pMemorySystem)
    : CMultiCoreSupport(pMemorySystem),
      m_pgraphics(p_graphics),
      m_pscreen(p_screen),
      m_pDeviceNameService(p_devicenameservice),
      //   m_pLogger(p_Logger),
      m_pTimer(p_Timer),
      m_pUSBHCI(p_USBHCI),
      m_pKeyboard(pKeyboard),
      m_ShutdownMode(ShutdownNone)

{
    // int width = (m_pgraphics->GetWidth() > MAX_WIDTH ? MAX_WIDTH : m_pgraphics->GetWidth()) - X_OFFSET * 2;
    blocks = new int[BLOCKS_PER_ROW * 5];
    gameOver = false;
    lives = 3;
    exitGame = false;
    boxDirection = 0;
    boxPosition = 0;
    circleX = 0;
    score = 0;
    circleY = 0;
    velocityX = CIRCLE_SPEED;
    velocityY = -CIRCLE_SPEED;
    direction = -1;
    s_pThis = this;
    paddleColDirection = -1;
}

CBreakOut::~CBreakOut(void)
{
    m_pgraphics = 0;
    m_pscreen = 0;
    m_pUSBHCI = 0;
    m_pKeyboard = 0;
    s_pThis = 0;
}
/**
 * @brief The screen manager function. This is core 1's job, to draw the screen.
 *
 */
void CBreakOut::drawBlocks()
{
    int width = (m_pgraphics->GetWidth() > MAX_WIDTH ? MAX_WIDTH : m_pgraphics->GetWidth()) - X_OFFSET * 2;
    int height = m_pgraphics->GetHeight();
    int blockWidth = width / BLOCKS_PER_ROW;
    circleY += velocityY;
    circleX += velocityX;
    // move block/circle
    boxPosition = boxPosition + getSpeed(width, boxPosition, &boxDirection); // uncomment this.
    // boxPosition = 250; // development purposes
    int colors[] = {BRIGHT_RED_COLOR, BRIGHT_GREEN_COLOR, BRIGHT_BLUE_COLOR, BRIGHT_YELLOW_COLOR, BRIGHT_CYAN_COLOR};
    if (!gameOver)
    {
        // Colored blocks at top
        m_pgraphics->ClearScreen(BLACK_COLOR);

        char *s = (char *)"Breakout";
        char scoreC[80];
        char livesStr[80];
        sprintf(livesStr, (char *)"Lives: %d", lives);
            sprintf(scoreC, (char *)"Score: %d", score);
        drawString(m_pgraphics, X_OFFSET, 0, s, WHITE_COLOR, 2);
        drawString(m_pgraphics, X_OFFSET + 200, 0, livesStr, WHITE_COLOR, 2);
        drawString(m_pgraphics, X_OFFSET + 400, 0, scoreC, WHITE_COLOR, 2);

        for (int i = 0; i < BLOCKS_PER_ROW * 5; i++)
        {
            int row = i / 10;
            if (blocks[i] == 0)
            {
                m_pgraphics->DrawRect(X_OFFSET + blockWidth * (i % 10), Y_OFFSET + HEIGHT * row, blockWidth, HEIGHT, colors[row]);
            }
        }
        // Actual Circle
        m_pgraphics->DrawCircle(circleX, circleY, CIRCLE_RADIUS, BRIGHT_WHITE_COLOR);
        // Paddle
        m_pgraphics->DrawRect(X_OFFSET + boxPosition,
                              m_pgraphics->GetHeight() - Y_OFFSET,
                              BOX_WIDTH, HEIGHT, BRIGHT_WHITE_COLOR);
        // update
        m_pgraphics->UpdateDisplay();
    }
    if (gameOver) {
        m_pgraphics->ClearScreen(BLACK_COLOR);
        char *gameover = (char *)"Game Over!";
        char finalS[80];
        sprintf(finalS, (char *)"Final Score: %d", score);
        drawString(m_pgraphics, width / 2, height / 2 - Y_OFFSET, gameover, BRIGHT_RED_COLOR, 4);
        drawString(m_pgraphics, width / 2, height/2 + Y_OFFSET, finalS, WHITE_COLOR, 3);
        m_pgraphics->UpdateDisplay();
    }
    collisons();
    if (circleY > m_pgraphics->GetHeight())
    {
        circleY = m_pgraphics->GetHeight()/2;
        circleX = width / 2;
        lives -= 1;
    }
    if (lives == 0) {
        gameOver = true;
    }
}

void CBreakOut::handleKeyboard()
{
    for (unsigned nCount = 0; m_ShutdownMode == ShutdownNone; nCount++)
    {
        if (exitGame)
        {
            break;
        }
        boolean bUpdated = m_pUSBHCI->UpdatePlugAndPlay();
        if (bUpdated && m_pKeyboard == 0)
        {
            m_pKeyboard = (CUSBKeyboardDevice *)m_pDeviceNameService->GetDevice("ukbd1", FALSE);
            if (m_pKeyboard != 0)
            {
                m_pKeyboard->RegisterRemovedHandler(KeyboardRemovedHandler);
                m_pKeyboard->RegisterKeyPressedHandler(KeyPressedHandler);
            }
        }
        if (m_pKeyboard != 0)
        {
            m_pKeyboard->UpdateLEDs();
        }
    }
}

void CBreakOut::KeyPressedHandler(const char *pString)
{
    assert(s_pThis != 0);
    if (strlen(pString))
    {
        s_pThis->boxDirection = s_pThis->boxDirection == 1 ? 0 : 1;
    }
}

void CBreakOut::KeyboardRemovedHandler(CDevice *pDevice, void *pContext)
{
    assert(s_pThis != 0);
    // CLogger::Get()->Write(FromKernel, LogDebug, "Keyboard removed");
    s_pThis->m_pKeyboard = 0;
}

void CBreakOut::Run(unsigned nCore)
{
    //
    for (int i = 0; i < BLOCKS_PER_ROW * 5; i++)
    {
        blocks[i] = 0;
    }
    int width = (m_pgraphics->GetWidth() > MAX_WIDTH ? MAX_WIDTH : m_pgraphics->GetWidth()) - X_OFFSET * 2;
    int height = m_pgraphics->GetHeight();
    circleX = width / 2;
    circleY = height - Y_OFFSET * 2;
    switch (nCore)
    {
    case 0: // first core handles drawing to the screen
        while (!exitGame)
        {
            drawBlocks();
            m_pTimer->MsDelay(10);
        }
    case 1: // second core handles the keyboard
        handleKeyboard();
    case 2:
        doNothing();
    case 3:
        doNothing();
    }
}

/**
 * @brief Handles collisions. This function runs on the 3rd core.
 *
 */
void CBreakOut::collisons()
{
    int width = (m_pgraphics->GetWidth() > MAX_WIDTH ? MAX_WIDTH : m_pgraphics->GetWidth()) - X_OFFSET * 2;
    int height = m_pgraphics->GetHeight();

    int collideObj = detectCollision(blocks, circleX, circleY, width, &direction);
    int paddleCollide = detectPaddleCollision(circleX, circleY, boxPosition, height, &paddleColDirection);
    int wallCollide = detectCornerCollison(circleX, circleY, width, height);
    if (wallCollide >= 0)
    {
        if (wallCollide == 0)
        {
            velocityY = -velocityY;
        }
        if (wallCollide == 1 || wallCollide == 2)
        {
            velocityX = -velocityX;
        }
    }
    if (paddleCollide > 0)
    {
        velocityY = -velocityY;
    }
    if (collideObj > -1)
    {
        if (direction == 0 || direction == 1)
        {
            velocityX = -velocityX;
        }
        if (direction == 2 || direction == 3)
        {
            velocityY = -velocityY;
        }

        blocks[collideObj] = 1; // remove block from existence.
        score += 100;
        direction = -1;
    }
}