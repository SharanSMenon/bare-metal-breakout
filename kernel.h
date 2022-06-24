#ifndef _kernel_h
#define _kernel_h

#include <circle/actled.h>
#include <circle/koptions.h>
#include <circle/devicenameservice.h>
#include <circle/screen.h>
#include <circle/serial.h>
#include <circle/exceptionhandler.h>
#include <circle/2dgraphics.h>
#include <circle/interrupt.h>
#include <circle/timer.h>
#include <circle/logger.h>
#include <circle/usb/usbhcidevice.h>
#include <circle/usb/usbkeyboard.h>
#include <circle/types.h>
#include "breakout.h"

class CKernel
{
public:
	CKernel(void);
	~CKernel(void);

	boolean Initialize(void);

	TShutdownMode Run(void);

private:
	static void ShutdownHandler (void);

private:
	// do not change this order
	CActLED m_ActLED;
	CKernelOptions m_Options;
	CDeviceNameService m_DeviceNameService;
	CScreenDevice m_Screen;
	C2DGraphics m_graphics;
	CSerialDevice m_Serial;
	CExceptionHandler m_ExceptionHandler;
	CInterruptSystem m_Interrupt;
	CTimer m_Timer;
	CLogger m_Logger;
	CUSBHCIDevice m_USBHCI;

	CUSBKeyboardDevice *volatile m_pKeyboard;
	CBreakOut m_breakout;

	volatile TShutdownMode m_ShutdownMode;

	static CKernel *s_pThis;
};

#endif
