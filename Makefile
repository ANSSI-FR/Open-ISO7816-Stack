CC=/usr/bin/arm-none-eabi-gcc
AR=/usr/bin/arm-none-eabi-ar
OBJCOPY=/usr/bin/arm-none-eabi-objcopy
STLINK=~/stlink/build/Release/
STFLASH=st-flash


# TARGET CONFIG HERE

# Target can be stm32f407, stm32f411.
TARGET=stm32f411
#TARGET=stm32f407


ifeq ($(TARGET), stm32f411)

# Linker script file, has to be changed with target
LDFILE=ld/STM32F411VEHx_FLASH.ld
# Name of the startup file in the startup/ folder (without the .s extension)
STARTUP_FILE=startup_stm32f411xe
# Value of the preprocessor constant (defining the target) given to $(CC) at compile time
TARGET_DEFINE=TARGET_STM32F411
TARGET_DEFINE_CMSIS=STM32F411xE


else ifeq($(TARGET), stm32f407)

LDFILE=ld/STM32F407VGTx_FLASH.ld
STARTUP_FILE=startup_stm32f407xx
TARGET_DEFINE=TARGET_STM32F407
TARGET_DEFINE_CMSIS=STM32F407xx

else

@echo The TARGET parameter has been wrongly defined in the Makefile. Target does not exist. Build failed.
exit 1 

endif


# END OF TARGET CONFIG


CPU_CIBLE=cortex-m4



MAKEFILE_TESTS=Makefile_tests

INCDIR=inc
SRCDIR=src
LIBDIR=lib
OUTDIR=out
DEPDIR=dep
OBJDIR=obj
TESTDIR=tests
STARTUPDIR=startup


LIB=$(TARGET)_hal
HALDIR=$(LIBDIR)/$(TARGET)/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver
CMSISDIR=$(LIBDIR)/$(TARGET)/STM32CubeF4/Drivers/CMSIS
BSPDIR=$(LIBDIR)/$(TARGET)/STM32CubeF4/Drivers/BSP





OUTPUT_NAME=reader
OUTPUT_BIN=$(OUTPUT_NAME).bin
OUTPUT_ELF=$(OUTPUT_NAME).elf
OUTPUT_HEX=$(OUTPUT_NAME).hex
OUTPUT_MAP=$(OUTPUT_NAME).map
OUTPUT_LST=$(OUTPUT_NAME).lst
OUTPUT_AR=lib$(OUTPUT_NAME).a



DEFS+= -DUSE_HAL_DRIVER
DEFS+= -D$(TARGET_DEFINE)
DEFS+= -D$(TARGET_DEFINE_CMSIS)
#DEFS+= -DUSE_HAL_GPIO_MODULE


INCS= -I$(INCDIR)
INCS+= -I$(LIBDIR)/$(TARGET)
INCS+= -I$(HALDIR)/Inc
INCS+= -I$(HALDIR)/Inc/Legacy
INCS+= -I$(CMSISDIR)/Include
INCS+= -I$(CMSISDIR)/Device/ST/STM32F4xx/Include
INCS+= -I./src_hw_dependent


CFLAGS= -mcpu=$(CPU_CIBLE)
CFLAGS+= -mlittle-endian
CFLAGS+= -mthumb
CFLAGS+= -Wall
CFLAGS+= -O0
#CFLAGS+= -ffunction-sections -fdata-sections
#CFLAGS+= -Os
#CFLAGS+= -std=c89
#CFLAGS+= -pedantic-errors
CFLAGS+= $(DEFS)
CFLAGS+= $(INCS)
CFLAGS+= -g     #informations de debug


LDFLAGS= -Wl,--gc-sections
LDFLAGS+= -Wl,-Map=$(OUTDIR)/$(OUTPUT_MAP),--cref,--no-warn-mismatch
LDFLAGS+= -L$(LIBDIR)
LDFLAGS+= -l$(LIB)
LDFLAGS+= -T$(LDFILE)



SRCS=$(shell ls $(SRCDIR) | sed -e 's/\.s/\.c/g')
SRCS+= $(STARTUP_FILE).c
OBJS=$(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
DEPS=$(addprefix $(DEPDIR)/,$(SRCS:.c=.d))




.PHONY: all dirs clean upload library tests test report reader



all:dirs library $(OUTDIR)/$(OUTPUT_ELF) $(OUTDIR)/$(OUTPUT_BIN) $(OUTDIR)/$(OUTPUT_HEX)


reader:all
	$(AR) -r $(OUTPUT_AR) $(subst $(OBJDIR)/main.o,,$(OBJS)) $(LIBDIR)/obj/*.o

upload:all
	$(STFLASH) write $(OUTDIR)/$(OUTPUT_BIN) 0x8000000
	
	
clean:
	rm -rf $(OUTDIR)
	rm -rf $(DEPDIR)
	rm -rf $(OBJDIR)
	$(MAKE) clean -C $(LIBDIR)
	$(MAKE) --file $(MAKEFILE_TESTS) clean
	rm -f $(OUTPUT_AR)


dirs:
	echo $(OBJS)
	mkdir -p $(OBJDIR) $(DEPDIR) $(OUTDIR)


library:
	$(MAKE) all -C $(LIBDIR)
	
tests:
	$(MAKE) --file $(MAKEFILE_TESTS) all
	
test:tests
	$(MAKE) --file $(MAKEFILE_TESTS) test
	
report:
	$(MAKE) --file $(MAKEFILE_TESTS) report


$(OUTDIR)/$(OUTPUT_BIN):$(OUTDIR)/$(OUTPUT_ELF)
	$(OBJCOPY) -O binary $< $@


$(OUTDIR)/$(OUTPUT_HEX):$(OUTDIR)/$(OUTPUT_ELF)
	$(OBJCOPY) -O ihex $< $@


#ajout $(DEPS) en dependances ??
$(OUTDIR)/$(OUTPUT_ELF):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	
	

$(OBJDIR)/%.o:$(SRCDIR)/%.c $(DEPDIR)/%.d
	$(CC) $(CFLAGS) -c -MD -MT $*.o -MF $(DEPDIR)/$*.d $< -o $@
	
	
#$(OBJDIR)/%.o:$(SRCDIR)/%.s $(DEPDIR)/%.d
#	$(CC) $(CFLAGS) -c -MD -MT $*.o -MF $(DEPDIR)/$*.d $< -o $@

$(OBJDIR)/$(STARTUP_FILE).o:$(STARTUPDIR)/$(STARTUP_FILE).s $(DEPDIR)/%.d
	$(CC) $(CFLAGS) -c -MD -MT $*.o -MF $(DEPDIR)/$*.d $< -o $@
		
		
$(DEPDIR)/%.d: ;


	
-include $(DEPS)
