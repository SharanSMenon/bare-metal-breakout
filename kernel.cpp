/**
 * @file kernel.cpp
 * @author Sharan Sajiv Menon
 * @brief Main kernel file. The game is loaded and initialized and control is handed over to breakout.cpp
 * @version 0.1
 * @date 2022-06-22
 *
 * @copyright Copyright (c) 2022 Sharan Sajiv Menon
 *
 */

#include "kernel.h"
#include <circle/string.h>
#include <circle/util.h>
#include <assert.h>
#include <circle/memory.h>
#include "text.h"

static const char FromKernel[] = "kernel";

CKernel *CKernel::s_pThis = 0;

CKernel::CKernel(void)
	: m_Screen(m_Options.GetWidth(), m_Options.GetHeight()),
	  m_graphics(m_Options.GetWidth(), m_Options.GetHeight()),
	  m_Timer(&m_Interrupt),
	  m_Logger(m_Options.GetLogLevel(), &m_Timer),
	  m_USBHCI(&m_Interrupt, &m_Timer, TRUE), // TRUE: enable plug-and-play
	  m_pKeyboard(0),
	  m_breakout(&m_graphics,
				 &m_Screen, &m_DeviceNameService,
				 //  &m_Logger,
				 &m_Timer,
				 &m_USBHCI, m_pKeyboard,
				 CMemorySystem::Get()),
	  m_ShutdownMode(ShutdownNone)
{
	s_pThis = this;

	m_ActLED.Blink(5); // show we are alive
}

CKernel::~CKernel(void)
{
	s_pThis = 0;
}

boolean CKernel::Initialize(void)
{
	boolean bOK = TRUE;

	if (bOK)
	{
		bOK = m_Screen.Initialize();
	}
	if (bOK)
	{
		bOK = m_graphics.Initialize();
	}

	if (bOK)
	{
		bOK = m_Serial.Initialize(115200);
	}

	if (bOK)
	{
		CDevice *pTarget = m_DeviceNameService.GetDevice(m_Options.GetLogDevice(), FALSE);
		if (pTarget == 0)
		{
			pTarget = &m_Screen;
		}

		// bOK = m_Logger.Initialize(pTarget);
	}

	if (bOK)
	{
		bOK = m_Interrupt.Initialize();
	}

	if (bOK)
	{
		bOK = m_Timer.Initialize();
	}

	if (bOK)
	{
		bOK = m_USBHCI.Initialize();
	}
	if (bOK)
	{
		bOK = m_breakout.Initialize();
	}

	return bOK;
}

TShutdownMode CKernel::Run(void)
{
	int width = m_graphics.GetWidth() / 2;
	int height = m_graphics.GetHeight() / 2;
	char *introText = (char *)"Breakout";
	char *name = (char *)"Sharan Sajiv menon";

	for (int i = 0; i < 100; i++)
	{
		drawString(&m_graphics, width / 2, height / 2, introText, BRIGHT_RED_COLOR, 5);
		drawString(&m_graphics, width / 2 - 50, height / 2 + 100, name, BRIGHT_RED_COLOR, 3);
		m_graphics.UpdateDisplay();
		m_Timer.MsDelay(20);
	}
	m_graphics.ClearScreen(BLACK_COLOR);
	m_graphics.UpdateDisplay();
	char *instructions = (char *)"Press any key to switch paddle direction.";
	for (int i = 0; i < 100; i++)
	{
		m_graphics.ClearScreen(BLACK_COLOR);
		m_graphics.UpdateDisplay();
		drawString(&m_graphics, width / 3, height / 2, instructions, BRIGHT_RED_COLOR, 2);
		m_graphics.UpdateDisplay();
		m_Timer.MsDelay(20);
	}
	m_breakout.Run(0);

	return m_ShutdownMode;
}

void CKernel::ShutdownHandler(void)
{
	assert(s_pThis != 0);
	s_pThis->m_ShutdownMode = ShutdownReboot;
}
