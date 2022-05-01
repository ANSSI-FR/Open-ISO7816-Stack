CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar
OBJCOPY=arm-none-eabi-objcopy



# TARGET DEFINITION STARTS HERE

# Target can be stm32f407, stm32f411.
#TARGET=stm32f411
TARGET=stm32f407


ifeq ($(TARGET), stm32f411)
	LIBDIR=./$(TARGET)
	HALDIR=$(LIBDIR)/HAL
	CMSISDIR=$(LIBDIR)/CMSIS/CMSIS
	CMSISDEVICEDIR=$(LIBDIR)/CMSIS_DEVICE
	
	# Value of the preprocessor constant (defining the target) given to $(CC) at compile time
	TARGET_DEFINE=TARGET_STM32F411
	TARGET_DEFINE_CMSIS=STM32F411xE
	
else ifeq ($(TARGET), stm32f407)
	LIBDIR=./$(TARGET)
	HALDIR=$(LIBDIR)/HAL
	CMSISDIR=$(LIBDIR)/CMSIS/CMSIS
	CMSISDEVICEDIR=$(LIBDIR)/CMSIS_DEVICE
	
	TARGET_DEFINE=TARGET_STM32F407
	TARGET_DEFINE_CMSIS=STM32F407xx
	
else
	@echo The TARGET parameter has been wrongly defined in the Makefile. Target does not exist. Build failed.
	exit 1 
endif

# TARGET DEFINITION ENDS HERE


CPU_CIBLE=cortex-m4

LIBPREFIX=lib
LIBSUFIX=.a
LIBAR=$(LIBPREFIX)$(TARGET)_hal$(LIBSUFIX)


INCS=-I$(HALDIR)/Inc
INCS+=-I$(CMSISDIR)/Include
INCS+=-I$(CMSISDEVICEDIR)/Include
INCS+=-I../inc


DEFS+= -DUSE_HAL_DRIVER
DEFS+= -D$(TARGET_DEFINE)
DEFS+= -D$(TARGET_DEFINE_CMSIS)


CFLAGS= -mcpu=$(CPU_CIBLE)
CFLAGS+= -mlittle-endian
CFLAGS+= -mthumb
CFLAGS+= -Wall
CFLAGS+= -ffreestanding 
CFLAGS+= -nostdlib
CFLAGS+= -ffunction-sections
CFLAGS+= -Os
#CFLAGS+= -std=c89
#CFLAGS+= -pedantic-errors
CFLAGS+= $(DEFS)
CFLAGS+= $(INCS)


#SRCS=$(shell ls $(HALDIR)/Src)
SRCS=stm32f4xx_hal.c
SRCS+=stm32f4xx_hal_gpio.c
SRCS+=stm32f4xx_hal_i2c.c
SRCS+=stm32f4xx_hal_spi.c
SRCS+=stm32f4xx_hal_pwr.c
SRCS+=stm32f4xx_hal_rcc.c
SRCS+=stm32f4xx_hal_dma.c
SRCS+=stm32f4xx_hal_flash.c
SRCS+=stm32f4xx_hal_cortex.c
SRCS+=stm32f4xx_hal_usart.c
SRCS+=stm32f4xx_hal_uart.c
SRCS+=stm32f4xx_hal_smartcard.c

DEPS=$(addprefix dep/,$(SRCS:.c=.d))
OBJS=$(addprefix obj/,$(SRCS:.c=.o))



.PHONY: all dirs clean

all:dirs $(LIBAR)


clean:
	rm -rf dep obj $(LIBAR)


dirs:
	mkdir -p dep obj


$(LIBAR):$(OBJS)
	$(AR) -r $@ $(OBJS)
	


obj/%.o:$(HALDIR)/Src/%.c dep/%.d
	$(CC) $(CFLAGS) -c -MD -MT $*.o -MF dep/$*.d $< -o $@
	
	

dep/%.d: ;
	
	
	
-include $(DEPS)
