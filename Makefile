### ProcessorExpert installation directory
CWDIR = ../../dev/Freescale/CW_MCU_v10.5
PExDIR = $(CWDIR)/MCU/ProcessorExpert
EWLDIR = $(CWDIR)/MCU/ARM_GCC_Support/ewl

### Source file directory
SOURCEDIR = sources
DRIVERDIR = drivers
LIBDRIVER = lib

OBJECTDIR = build

### Compiler definitions
COMPILER = arm-none-eabi
AS       = $(COMPILER)-as
AR       = $(COMPILER)-ar
CC       = $(COMPILER)-gcc
LD       = $(COMPILER)-gcc
OBJCOPY  = $(COMPILER)-objcopy
STRIP    = $(COMPILER)-strip

SUFFIX = -cm0

CFLAGS += -O0 -mcpu=cortex-m0 -mthumb -mlittle-endian
CFLAGS += -fshort-enums -fomit-frame-pointer -fno-strict-aliasing
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wall  -g3

CFLAGS += -I $(DRIVERDIR)
CFLAGS += -I $(SOURCEDIR)
CFLAGS += -I $(PExDIR)/lib/Kinetis/pdd/inc
CFLAGS += -I $(PExDIR)/lib/Kinetis/iofiles
CFLAGS += -I $(EWLDIR)/EWL_C/include
CFLAGS += -I $(EWLDIR)/EWL_Runtime/include

AROPTS = rcf


### The default rule, which causes the driver library to be built.
all: $(OBJECTDIR) $(LIBDRIVER)
all: $(LIBDRIVER)/libdriver.a


#### The rule to clean out all the build products.
clean:
	@rm -rf $(OBJECTDIR) $(wildcard *~})
	@rm -rf $(LIBDRIVER) $(wildcard *~})

### Rules for building the driver library.
SOURCEFILES += $(wildcard $(DRIVERDIR)/*.c)
#SOURCEFILES += $(SOURCEDIR)/Events.c
OBJECTFILES += $(patsubst %.c,$(OBJECTDIR)/%.o,$(notdir $(SOURCEFILES)))

define FINALIZE_DEPENDENCY
cp $(@:.o=.d) $(@:.o=.$$$$); \
sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
    -e '/^$$/ d' -e 's/$$/ :/' < $(@:.o=.$$$$) >> $(@:.o=.d); \
rm -f $(@:.o=.$$$$)
endef

vpath %.c $(DRIVERDIR)
vpath %.c $(SOURCEDIR)

### The rule to create the target directory.
$(OBJECTDIR):
	@mkdir -p $(OBJECTDIR)
	
$(LIBDRIVER):
	@mkdir -p $(LIBDRIVER)
	

### The rule for building the object file from each C source file.
$(OBJECTDIR)/%.o: %.c | $(OBJECTDIR)
	@echo "  CC       " $<
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
	@$(FINALIZE_DEPENDENCY)
	
### The rule for creating an object library.
$(LIBDRIVER)/libdriver.a: $(OBJECTFILES)
	@echo "  AR       " $@
	@$(AR) $(AROPTS) $@ $^
