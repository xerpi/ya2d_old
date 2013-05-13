TARGET = ya2d

INCLUDES = include
SOURCES = source

PSP_FW_VERSION = 370
BUILD_PRX = 1

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))

OBJS = $(addprefix source/, $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) \
       $(sFILES:.s=.o) $(SFILES:.S=.o))

LIBDIR =
LDFLAGS =
LIBS= -lpng -lz -lpspgum -lpspvram -lpspgu -ljpeg -lm -lpsprtc

CFLAGS = -O2 -G0 -Wall -I$(INCLUDES)
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)


EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = $(TARGET)

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

copy:
	cp EBOOT.PBP /media/xerpi/disk/PSP/GAME/YA2D/EBOOT.PBP
	umount /media/xerpi/disk