# Open-sourced ISO7816-3 protocol implementation


## Introduction

This project aims to provide an open-source implementation of the ISO7816-3 communication protocol from the reader side.
This protocol is ruling the interactions between a smartcard and a card reader when using its contacts to communicate.
It is defined by the ISO/IEC 7816-3 specification which can be found [here](https://www.iso.org/standard/38770.html).

This code is designed to be run on an embedded target and is organized accordingly to several abstraction layers, thus allowing you to easily make your own implementation for a target which might not be supported yet.
The software is written in pure C code and is thoroughly tested to asses its compliance with the protocol specification.

At its higher abstraction level, this library provides an API allowing you to send and receive APDUs to/from a smartcard.
At the same time, the lower level APIs are allowing you to interact directly with the arcanes of the T=0 and T=1 underlying protocols.


## Copyright and license

Copyright ANSSI (2017-2021)

This software is licensed under a MIT license. See LICENSE file at the root folder of the project.

## Authors

  * Boris SIMUNOVIC (<mailto:boris.simunovic@ssi.gouv.fr>)
  * Guillaume BOUFFARD (<mailto:guillaume.bouffard@ssi.gouv.fr>)

This project has been developed as a part of an internship project supervised by Guillaume BOUFFARD ([ANSSI](https://www.ssi.gouv.fr/)) and carried out by Boris SIMUNOVIC (Grenoble Institute of Technology - [ESISAR](https://esisar.grenoble-inp.fr/))


## Supported targets

The currently supported targets are :
* stm32f407
* stm32f411

It is recommended to use the development boards associated to those components (discovery boards).
So you won't have to change some settings in the code (especially oscillator frequency provided by the board).



## Getting project's code and cross-compiling it on your local machine

First, clone project's repository on your local machine :

``` shell
$ git clone git@git.bouffard.info:Apprentissage-Boris/iso7816-reader.git
$ git submodule init
$ git submodule update
```

The project is relying on the following submodules : 
* *Unity*, is a unit-test framework for testing the project. (see project's page [here](http://www.throwtheswitch.org/unity))
* *CMock*, is a mocking framework used jointly with the unity framework. Useful for testing parts of code relying on hardware peripherals. (see project's page [here](http://www.throwtheswitch.org/cmock))


### Installing tool-chain and dependencies

You have to install the ARM tool-chain and cross-compiler on your local machine.
Typically can be achieved by doing :
``` shell
$ apt-get install arm-none-eabi-*
```

You need as well to install the ST tool-chain for flashing the target (stlink), especially the *st-flash* executable.
You can get it from the ST website, or on fedora distributions you can do :

``` shell
$ dnf install stlink-devel
```

### Setting-up the Makefile

In the main directory Makefile set the following parameters to the correct values :

``` shell
CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
AR=arm-none-eabi-ar
OBJCOPY=arm-none-eabi-objcopy
STFLASH=st-flash
```

Then, do the same for the Makefile in the *./lib* directory :

``` shell
CC=arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY=arm-none-eabi-objcopy
STFLASH=st-flash
```

NOTE: if you have several ARM cross-compilers installed on your local machine (typically if you have a concurrent Xilinx toolchain)
it is recommended to explicitly indicate the absolute path to the correct compiler to avoid problems.

### Selecting the target

To select the target for which the firmware has to be compiled you have to follow those steps :

1. In the main directory open the Makefile and uncoment the lines corresponding to your target :

``` shell
# Target can be stm32f407, stm32f411.
#TARGET=stm32f411
#TARGET=stm32f407

# Linker script file, has to be changed accordingly to the target
#LDFILE=ld/STM32F411VEHx_FLASH.ld
#LDFILE=ld/STM32F407VGTx_FLASH.ld

# Name of the startup file in the startup/ folder (without the .s extension)
#STARTUP_FILE=startup_stm32f411xe
#STARTUP_FILE=startup_stm32f407xx

# Value of the preprocessor constant (defining the target) given to $(CC) at compile time
#TARGET_DEFINE=TARGET_STM32F411
#TARGET_DEFINE=TARGET_STM32F407
```

2. In the ./lib directory open the Makefile and uncoment the lines corresponding to your target :

``` shell
# Target can be stm32f407, stm32f411.
#TARGET=stm32f411
#TARGET=stm32f407

# Value of the preprocessor constant (defining the target) given to $(CC) at compile time
#TARGET_DEFINE=TARGET_STM32F411
#TARGET_DEFINE=TARGET_STM32F407
```


### Compiling and uploading the project to the target

Use the following commands from the main directory to compile the project and upload it to the target.

``` shell
$ make all
$ make upload
```

## Building a static library and linking it to another project

### Building the static library

In the reader local directory execute the following command:

``` shell
$ make clean
$ make reader
```
	
A file libreader.a containing the static library is then generated.

### Including the library to your own project and compiling it


To compile your project with the reader library, the static library and all the
headers in *reader/inc* must be accessible by your project.

Your project should include reader_lib.h file as:

``` c
#include "reader_lib.h"
```
	
To build your project, all *reader/inc* headers should be indicated at the
compilation time with the gcc *-I* option by the following way :

``` shell
$ gcc -Ireader/inc
``` 
	
When linking your own project you have to point out the path to the static library :

``` shell
$ ld -Lpath/to/staticlib/folder/ -lreader
``` 

## Connecting the reader to the smartcard

For the currently supported stm32 targets, the wiring of the smartcard to the reader is always the same.
The follwing signals have to be wired to the discovery board :

| Smartcard signal   | stm32 port   | stm32 pin number |
| ------------------ | ------------ | ---------------- |
| Clock              | PORTA        | GPIO PIN 4       |
| I/O                | PORTA        | GPIO PIN 2       |
| Power              | PORTA        | GPIO PIN 6       |
| Reset              | PORTA        | GPIO PIN 5       |
| Ground             | Ground       | Ground           |

	

## Currently supported features of the ISO7816-3 protocol
	
| Feature                          | Implemented? | Tested? |
| -------------------------------- | ------------ | ------- |
| ATR reception and decoding       | X            |         |
| PPS Negotiation                  |              |         |
| T=0 Case 1                       | X            | X       |
| T=0 Case 2S                      | X            | X       |
| T=0 Case 3S                      | X            | X       |
| T=0 Case 4S                      | X            | X       |
| T=0 Case 2E                      | X            |         |
| T=0 Case 3E                      | X            |         |
| T=0 Case 4E                      |              |         |
| T=1 Normal operation             | X            | X       |
| T=1 Changing                      | X            | X       |
| T=1 Error handling               | X            | X       |
| T=1 IFS Adjustment              | X            | X       |
| T=1 WTX Adjustment              | X            | X       |
| T=1 ABORT Request                | X            |         |
| T=1 RESYNCH Request              | X            |         |
	

## Using the API

### Data structures

```c
READER_HAL_CommSettings settings;
```

*settings* is a data structure which contains all the parameters values currently used by READER_HAL hardware abstraction layer used by the library for low level communications (emission and reception of characters). 
The user should not modify directly the values present in this structure. 
It's recommended to use the functions exposed in the file *reader_hal_comm_settings.h* to operate safely on it.


```c
READER_APDU_Command apduCmd;
READER_APDU_Response apduResp;
```

*apduCmd* and *apduResp* are data structures respectively representing an APDU command and an APDU response objects defined in the ISO/IEC7816-3.

```c
READER_T0_ContextHandler context1;
READER_T1_ContextHandler context2;
```

*context1* is a data structure which contains the current communication context (current state of all the variables and protocol parameters) when using the protocol T=0.
*context2* is a data structure which contains the current communication context when using the protocol T=1.


### Initializing the API with default parameters values

```c
/* Initialization of the hardware abstraction layer with default values ...  */
READER_HAL_InitWithDefaults(&settings);
/* Resetting the card ...  */
READER_HAL_DoColdReset();	
```

```c
/* Initialization of the APDU API when using the protocol T=0 ...  */
READER_T0_APDU_Init(&context, &settings);
```

```c
/* Initialization of the APDU API when using the protocol T=1 ...  */
READER_T1_APDU_Init(&context, &settings);
```

### Executing an APDU

```c
/* Executing an APDU when using the protocol T=0 */
READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
```

```c
/* Executing an APDU when using the protocol T=1 */
READER_T1_APDU_Execute(&context, &apduCmd, &apduResp);
```
	
### Extracting information form the APDU Response structure

```c
uint8_t *dataPtr;
uint32_t dataSize;
uint8_t SW1, SW2;

/* Getting the SW from the APDU Response data structure ...   */
READER_APDU_ExtractRespSW(&apduResp, &SW1, &SW2);

/* Getting the data contained in the APDU Response data structure ...  */
READER_APDU_ExtractRespDataPtr(&apduResp, &dataPtr, &dataSize);
```

	
### Full example when using T=0 protocol

```c
READER_HAL_CommSettings settings;
READER_T0_ContextHandler context;
READER_APDU_Command apduCmd;
READER_APDU_Response apduResp;
uint8_t buffer[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};

/* Initialization of the hardware abstraction layer with default values ...  */
READER_HAL_InitWithDefaults(&settings);
READER_HAL_DoColdReset();	

/* Initialization of the APDU communication context when using the protocol T=0 ...  */
READER_T0_APDU_Init(&context, &settings);

/* Forging the APDU command ...  */
READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 10, buffer, 0);

/* Executing an APDU when using the protocol T=0 */
READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
```


## Contributing

Pull requests are welcome. See `CONTRIBUTING.md`.


## Community

If you’ve found a bug, or have an idea/suggestion/request, fill an issue
here on GitHub or contact the authors by email.
