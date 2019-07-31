# ISO7816 smartcard reader
==========================

## Getting started

The ISO7816 smartcard reader is a pure C library designed for embedded systems.
It allows communicating APDUs with a smartcard following the ISO7816-3 specification.
The library supports sending and receiving APDUs using protocols T=0 and T=1.


## Target devices
-----------------

The code is designed to be easilly adaptable to a big range of microcontrollers.
For the moment the supported devices are :

- STM32F407 

## Prerequisites
----------------


- STLINK tool
- RUBY (for the tests)
- Compiler for your local machine (for the tests)
- Cross compiler for your target

	
	
## Build instructions
---------------------
In a first time, you should clone the code as:

``` shell
	> git clone git@bouffard.info:iso7816-reader
```

### Building and linking external projet with the reader library
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

#### Uploading the binay to the board
-------------------------------------

Before uploading the developper shoud configure the Makefile in the following way :

``` shell
	$ export STLINK=~/stlink/build/Release/
```

The variable STLINK have to be setp with the path to stlink tool directory.

Then, the developper should execute in the command line in the current directory the following commands:

``` shell
	$ make clean
	$ make all
	$ make upload
```
	
### Building tests
------------------

Tests are implemented to insure that library doe not include code regression.
They implement the ISO7816-3 standard. Before compiling and running the tests,
the developper should update the Makefile_tests in the following way if ruby is
not in the $PATH:

``` shell
	$ export RUBY=/path/to/ruby  (path to the ruby executable)
```

Then run the following commands in the library directory:

``` shell
	$ cd Unity
	$ git submodule init
	$ git submodule update
	
	$ cd ../CMock
	$ git submodule init
	$ git submodule update
```

Finaly, the developper is able to run the tests in the following way:
	
```shell
	$ make clean
	$ make tests
	$ make test
```
	
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

## Features
	
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

