CC=arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY=arm-none-eabi-objcopy
STLINK=~/stlink/build/Release/

MAKEFILE_TESTS=Makefile_tests

INCDIR=inc
SRCDIR=src
LIBDIR=lib
OUTDIR=out
DEPDIR=dep
OBJDIR=obj
TESTDIR=tests


LIB=stm32f407_hal
HALDIR=$(LIBDIR)/HAL
CMSISDIR=$(LIBDIR)/CMSIS
BSPDIR=$(LIBDIR)/BSP




LDFILE=STM32F407VGTx_FLASH.ld



OUTPUT_NAME=test
OUTPUT_BIN=$(OUTPUT_NAME).bin
OUTPUT_ELF=$(OUTPUT_NAME).elf
OUTPUT_HEX=$(OUTPUT_NAME).hex
OUTPUT_MAP=$(OUTPUT_NAME).map
OUTPUT_LST=$(OUTPUT_NAME).lst
OUTPUT_AR=lib$(OUTPUT_NAME).a

CPU_CIBLE=cortex-m4
UC_CIBLE=STM32F407xx


DEFS = -D$(UC_CIBLE)
DEFS+= -DUSE_HAL_DRIVER
#DEFS+= -DUSE_HAL_GPIO_MODULE


INCS= -I$(INCDIR)
INCS+= -I$(LIBDIR)
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
OBJS=$(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
DEPS=$(addprefix $(DEPDIR)/,$(SRCS:.c=.d))




.PHONY: all dirs clean upload library tests test report reader



all:dirs library $(OUTDIR)/$(OUTPUT_ELF) $(OUTDIR)/$(OUTPUT_BIN) $(OUTDIR)/$(OUTPUT_HEX)


reader:all
	$(AR) -r $(OUTPUT_AR) $(subst $(OBJDIR)/main.o,,$(OBJS))

upload:all
	$(STLINK)/st-flash write $(OUTDIR)/$(OUTPUT_BIN) 0x8000000
	
	
clean:
	rm -rf $(OUTDIR)
	rm -rf $(DEPDIR)
	rm -rf $(OBJDIR)
	$(MAKE) clean -C $(LIBDIR)
	$(MAKE) --file $(MAKEFILE_TESTS) clean
	rm $(OUTPUT_AR)


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
	
	
$(OBJDIR)/%.o:$(SRCDIR)/%.s $(DEPDIR)/%.d
	$(CC) $(CFLAGS) -c -MD -MT $*.o -MF $(DEPDIR)/$*.d $< -o $@
		
		
$(DEPDIR)/%.d: ;


	
-include $(DEPS)
