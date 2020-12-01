# ISO7816 smartcard reader
==========================

## Introduction

This project aims to provide an open-source implementation of the ISO7816-3 communication protocol from the reader side.
This protocol is ruling the interactions between a smartcard and a card reader when using its contacts to communicate.
It is defined by the ISO/IEC 7816-3 specification which can be found [here](https://www.iso.org/standard/38770.html).

This code is designed to be run on an embedded target and is organized according to several abstraction layers, thus allowing you to easily make your own implementation for a target which might not be supported yet.
The software is written in pure C code and is thoroughly tested to asses its compliance with the protocol specification.

At its higher abstraction level, this librairy provides an API allowing you to send and receive APDUs to/from a smartcard.
At the same time, the lower level APIs are allowing you to interract directly with the arcanes of the T=0 and T=1 underlying protocols.


## Supported targets

The currently supported targets are :
* stm32f407
* stm32f411

It is recommended to use the developpment boards associated to those components (discovery boards).
So you won't have to change some settings in the code (especially oscillator frequency provided by the board).



## Getting project's code and cross-compiling it on your local machine

First, clone project's repository on your local machine :

``` shell
> git clone git@git.bouffard.info:Apprentissage-Boris/iso7816-reader.git
> git submodule init
> git submodule update
```

The project is relying on the following submodules : 
* *Unity*, is a unit-test framework for testing the project.
* *CMock*, is a mocking framework used jointly with the unity framework. Usefull for testing parts of code relying on hardware functions.


### Installing toolchain and dependencies

You have to install the ARM toolchain and cross-compiler on your local machine.
Typically can be achieved by doing :
``` shell
$ apt-get install arm-none-eabi-*
```

You need as well to install the ST toolchain for flashing the target (stlink), especially the *st-flash* executable.
You can get it fron the ST website, or on fedora distributions you can do :

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

### Selecting the target

To select the target for which the firmware has to be compiled you have to follow those steps :

1. In the main directory open the Makefile and uncomment the lines corresponding to your target :

``` shell
# Target can be stm32f407, stm32f411.
#TARGET=stm32f411
#TARGET=stm32f407

# Linker script file, has to be changed with target
#LDFILE=ld/STM32F411VEHx_FLASH.ld
#LDFILE=ld/STM32F407VGTx_FLASH.ld

# Name of the startup file in the startup/ folder (without the .s extension)
#STARTUP_FILE=startup_stm32f411xe
#STARTUP_FILE=startup_stm32f407xx

# Value of the proprocessor constant (defining the target) given to $(CC) at compile time
#TARGET_DEFINE=TARGET_STM32F411
#TARGET_DEFINE=TARGET_STM32F407
```

2. In the ./lib directory open the Makefile and uncomment the lines corresponding to your target :

``` shell
# Target can be stm32f407, stm32f411.
#TARGET=stm32f411
#TARGET=stm32f407

# Value of the proprocessor constant (defining the target) given to $(CC) at compile time
#TARGET_DEFINE=TARGET_STM32F411
#TARGET_DEFINE=TARGET_STM32F407
```


### Compiling and uploading the project to the target

Use the following commands from the main directory to compile the project and upload it to the target.

``` shell
$ make all
$ make upload
```

### Building a static librairy and linking it to another project
----------------------------------------------------------------

#### Building the static library
--------------------------------
In the reader local directory execute the following command:

``` shell
	> make clean
	> make reader
```
	
A file libreader.a containing the static library is then generated.

#### Including the library to your own projet and compiling it
--------------------------------------------------------------

To compile your projet with the reader library, the static library and all the
headers in *reader/inc* must be accessible by your projet.

Your projet should include reader_lib.h file as:

``` c
	#include "reader_lib.h"
```
	
To build your project, all *reader/inc* headers should be indicated at the
compilation time with the gcc *-I* option by the following way :

``` shell
	$ gcc -Ireader/inc
``` 
	
When linking your own projet you have to point out the path to the static library :

``` shell
	$ ld -Lpath/to/staticlib/folder/ -lreader
``` 




## Developpement informations and contributing

### Testing the code

The reader's code has to pass the tests described in the *./tests* directory.
Tests are comiled and run on the local developpment machine.

For running the tests, make sure you have the following tools installed on your local machine :
* gcc
* lcov
* ruby (CMock framework needs it for the scripts which are building mock's code)
* libc of the arm cross-compiler ?
``` shell
$ dnf install arm-none-eabi-newlib.noarch
```
* genhtml

Eventually setup the path to ruby:
``` shell
	$ export RUBY=/path/to/ruby
```

Set up the following parameters in the *Makefile_tests* file to the correct values according to the setup of your local machine :

``` shell
CC=gcc
LD=gcc
RUBY=ruby
LCOV=lcov
GCOV=gcov-7
GENHTML=genhtml
```


Finally, you can compile all the test executables by doing the following from the main project directory :
``` shell
$ make tests
```
then, you execute them by typing :
``` shell
$ make test
```

You can obtain a code coverage report by using the following make istruction :
``` shell
$ make report
```
A report in an HTML format is then available in the *tests/cov* directory.


note : for the tests the specfified gcc and gcov versions are gcc-7 and gcov-7. Seems to be compatibility problems with gcc-9 and gcov-9. Still needs to be tested for gcc-8 and gcov-8.


### On-target debugging

You need to install *openocd* tool on your local machine, then run the following bash script to run the openocd server.

``` shell
$ sudo apt-get install openocd
$ ./start_openocd.sh <target>
```

*target* is a value depending on the target you are debugging.
Possible values are :

*stm32f411
*stm32f407


To manage the openocd server :
``` shell
$ telnet localhost 4444
```

Basic commands : shutdown
For more commands see : http://openocd.org/doc/html/General-Commands.html


Debugger :

``` shell
$ sudo apt-get install gdb-multiarch
```
(or arm-none-eabi-gdb)


run :
``` shell
$ gdb-multiarch out/test.elf
$ target remote localhost:3333
$ monitor reset halt
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
	
## General code organization
----------------------------

The code is splitted in two main parts. The first one is the hardware dependant
code. It is an hardware abstraction layer composed of all the functions that are
directly interacting with the hardware. Typically it is the set of functions
purposed to the emission and reception of characters. The whole hardware
depenent code is located in the files : *reader_hal_basis.c* and
*reader_hal_comm_settings.c* in the *./src* directory. The orther part is the
logic (hardware independant code) state machine to make the ISO7816 protocol
work.

## Currently supported features of the ISO7816-3 protocol
	
| Feature                          | Implemented? | Tested? |
| -------------------------------- | ------------ | ------- |
| ATR reception and decoding       | X            |         |
| PPS Negociation                  |              |         |
| T=0 Case 1                       | X            | X       |
| T=0 Case 2S                      | X            | X       |
| T=0 Case 3S                      | X            | X       |
| T=0 Case 4S                      | X            | X       |
| T=0 Case 2E                      | X            |         |
| T=0 Case 3E                      | X            |         |
| T=0 Case 4E                      |              |         |
| T=1 Normal operation             | X            | X       |
| T=1 Chaning                      | X            | X       |
| T=1 Error handling               | X            | X       |
| T=1 IFS Adjustement              | X            | X       |
| T=1 WTX Adjustement              | X            | X       |
| T=1 ABORT Request                | X            |         |
| T=1 RESYNCH Request              | X            |         |
	

## Using the API
----------------

### Data structures
-------------------

```c
READER_HAL_CommSettings settings;
```

*settings* is a data structre which contains all the parameters values currently used by READER_HAL hardware abstraction layer used by the library for low level communications (emission and reception of characters). 
The user should modify directly the values into this structure. It's recommended to use the functions exposed in the file *reader_hal_comm_settings.h*.


```c
READER_APDU_Command apduCmd;
READER_APDU_Response apduResp;
```

*apduCmd* and *apduResp* are data structures respectively representing an APDU command and an APDU response.

```c
READER_T0_ContextHandler context1;
READER_T1_ContextHandler context2;
```

*context1* is a data structure which contains the current communication context when using the protocol T=0.
*context2* is a data structure which contains the current communication context when using the protocol T=1.


### Initalizing the API with default parameters values
------------------------------------------------------

```c
/* Initialization of the hardware abstraction layer with default values ...  */
READER_HAL_InitWithDefaults(&settings);
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
---------------------

```c
/* Executing an APDU when using the protocol T=0 */
READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
```

```c
/* Executing an APDU when using the protocol T=1 */
READER_T1_APDU_Execute(&context, &apduCmd, &apduResp);
```
	
### Extracting informations form the APDU Response structure
------------------------------------------------------------

```c
uint8_t *dataPtr;
uint32_t dataSize;
uint8_t SW1, SW2;

/* Getting the SW from the APDU Response data structure ...   */
READER_APDU_ExtractRespSW(&apduResp, &SW1, &SW2);

/* Getting the data caontained into the APDU Response data structure ...  */
READER_APDU_ExtractRespDataPtr(&apduResp, &dataPtr, &dataSize);
```

	
### Full example when using T=0 protocol
----------------------------------------

```c
/* Initialization of the hardware abstraction layer with default values ...  */
READER_HAL_InitWithDefaults(&settings);
READER_HAL_DoColdReset();	

/* Initialization of the APDU API when using the protocol T=0 ...  */
READER_T0_APDU_Init(&context, &settings);

/* Executing an APDU when using the protocol T=0 */
READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
```

	
## To Do
--------

