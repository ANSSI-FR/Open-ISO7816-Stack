CC=arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY=arm-none-eabi-objcopy

CPU_CIBLE=cortex-m4
UC_CIBLE=STM32F407xx

LIBPREFIX=lib
LIBSUFIX=.a
LIBAR=$(LIBPREFIX)stm32f407_hal$(LIBSUFIX)



LIBDIR=.
HALDIR=$(LIBDIR)/HAL
CMSISDIR=$(LIBDIR)/CMSIS
BSPDIR=$(LIBDIR)/BSP


INCS=-I$(HALDIR)/Inc
INCS+=-I$(CMSISDIR)/Include
INCS+=-I$(CMSISDIR)/Device/ST/STM32F4xx/Include
INCS+=-I../inc


DEFS= -DSTM32F407xx
DEFS+= -DUSE_HAL_DRIVER
#DEFS+= -DUSE_HAL_GPIO_MODULE


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
