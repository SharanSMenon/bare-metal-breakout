CIRCLEHOME = ./circle

OBJS	= main.o kernel.o breakout.o sprintf.o logic.o text.o

LIBS	= $(CIRCLEHOME)/lib/usb/libusb.a \
	  $(CIRCLEHOME)/lib/input/libinput.a \
	  $(CIRCLEHOME)/lib/fs/libfs.a \
	  $(CIRCLEHOME)/lib/libcircle.a

include Rules.mk

-include $(DEPS)

run:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img  -serial stdio