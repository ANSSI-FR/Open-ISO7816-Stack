# ISO7816 smartcard reader
==========================

## Getting started


## Target devices
-----------------

The code is designed to be easilly adaptable to a big range of microcontrollers.
For the moment the supported devices are :

	- STM32F407 



## Prerequisites
----------------

	- STLINK tool
	- RUBY (for the tests)
	- GCC (for the tests)
	- Compiler for the target
	
	
## Build instructions
---------------------

	> git clone git@bouffard.info:/iso7816-reader
	
### Building tests
------------------

Before compiling and running the tests, the developper should configure the Makefile_tests in the following way :

	RUBY=ruby  (path to the ruby executable)

Then run the following commands :

	> cd Unity
	> git submodule init
	> git submodule update
	
	> cd ../CMock
	> git submodule init
	> git submodule update
	
Then, the developper is able to make and run the tests in the following way :
	
	> make clean
	> make tests
	> make test
	
	
### Building and flashing on a board
------------------------------------

Before uploading the developper shoud configure the Makefile in the following way :

	STLINK=~/stlink/build/Release/
	
The variable STLINK have to be filled with the path to the directory of the stlink tool.


Then, the developper should execute in the command line in the current directory the following commands :

	> make clean
	> make all
	> make upload
	
	
	
## General code organization

	The code is splitted in two main parts.
	The first one is the hardware dependant code. 
	It is an hardware abstraction layer composed of all the functions that are directly interacting with the hardware.
	Typically it is the set of functions purposed to the emission and reception of characters.
	The whole hardware depenent code is located in the files : *reader_hal_basis.c* and *reader_hal_comm_settings.c* in the *./src* directory.
	The orther part is the logic (hardware independant code) state machine to make the ISO7816 protocol work. 

## Features
	
	| Feature                          | Developpment status | Testing status |
	| -------------------------------- | ------------------- | -------------- |
	| ATR reception and decoding       | Implemented         | Not tested     |
	| PPS Negociation                  | Not implemented     | Not tested     |
	| T=0 Case 1                       | Implemented         | Tested         |
	| T=0 Case 2S                      | Implemented         | Tested         |
	| T=0 Case 3S                      | Implemented         | Tested         |
	| T=0 Case 4S                      | Implemented         | Tested         |
	| T=0 Case 2E                      | Implemented         | Not tested     |
	| T=0 Case 3E                      | Implemented         | Not tested     |
	| T=0 Case 4E                      | Not implemented     | Not tested     |
	| T=1 Normal operation             | Implemented         | Tested         |
	| T=1 Chaning                      | Implemented         | Tested         |
	| T=1 Error handling               | Implemented         | Tested         |
	| T=1 IFS Adjustement              | Implemented         | Tested         |
	| T=1 WTX Adjustement              | Implemented         | Tested         |
	| T=1 ABORT Request                | Implemented         | Not tested     |
	| T=1 RESYNCH Request              | Implemented         | Not tested     |
	

## API Documentation
--------------------




## To Do
--------

