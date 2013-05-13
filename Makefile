TARGET = ya2d

INCLUDES = include
SOURCES  = source
DATA     = data

PSP_FW_VERSION = 370
BUILD_PRX = 1

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
PNGFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.png)))
JPGFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.jpg)))

OBJS := $(addprefix $(SOURCES)/, $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(sFILES:.s=.o) $(SFILES:.S=.o) )
OBJS += $(addprefix $(DATA)/, $(PNGFILES:.png=.o) $(JPGFILES:.jpg=.o) )  
      
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

copy: EBOOT.PBP
	cp EBOOT.PBP /media/xerpi/disk/PSP/GAME/YA2D/EBOOT.PBP
	umount /media/xerpi/disk

%.o	: %.png
	bin2o -i $< $@ $(addsuffix _png, $(basename $(notdir $<) ))
	
%.o	: %.jpg
	bin2o -i $< $@ $(addsuffix _jpg, $(basename $(notdir $<) ))
	
