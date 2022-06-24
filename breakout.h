/**
 * @file breakout.h
 * @author Sharan Sajiv Menon
 * @brief Header file for the breakout game.
 * @version 0.1
 * @date 2022-06-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _breakout_h
#define _breakout_h


#include <circle/multicore.h>
#include <circle/screen.h>
#include <circle/memory.h>
#include <circle/types.h>
#include <circle/logger.h>
#include <circle/timer.h>
#include <circle/devicenameservice.h>
#include <circle/2dgraphics.h>
#include <circle/usb/usbhcidevice.h>
#include <circle/usb/usbkeyboard.h>
#include "shutdownmode.h"


class CBreakOut : public CMultiCoreSupport
{
public:
    CBreakOut(C2DGraphics *p_graphics,
              CScreenDevice *p_screen,
              CDeviceNameService *p_devicenameservice,
            //   CLogger *p_Logger,
              CTimer *p_Timer,
              CUSBHCIDevice *p_USBHCI,
              CUSBKeyboardDevice *volatile pKeyboard,
              CMemorySystem *pMemorySystem);
    ~CBreakOut(void);
    void Run(unsigned nCore);

private:
    static void KeyPressedHandler(const char *pString);
    static void KeyboardRemovedHandler(CDevice *pDevice, void *pContext);

private:
    void drawBlocks();
    void handleKeyboard();
    void collisons();
    void reset();

private:
    C2DGraphics *m_pgraphics;
    CScreenDevice *m_pscreen;
    CDeviceNameService *m_pDeviceNameService;
    // CLogger *m_pLogger;
    CTimer *m_pTimer;
    CUSBHCIDevice *m_pUSBHCI;

    CUSBKeyboardDevice *volatile m_pKeyboard;

    volatile TShutdownMode m_ShutdownMode;
    static CBreakOut *s_pThis;

    int boxDirection; // 0 - right 1 - left
    int boxPosition;
    int* blocks; // 50 blocks | 10 per row
    bool gameOver; // true if player won or lost, otherwise false
    int lives;
    bool exitGame; // if true - exit the game and shutdown
    int score;

    int circleX;
    int circleY;
    int velocityX; // 0 - right 1 - left
    int velocityY; // 0 - up 1 - bottom

    int direction;
    int paddleColDirection;
};

#endif