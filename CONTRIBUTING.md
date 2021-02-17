# Developpement informations and contributing


## General code organization

The code is splitted in two main parts. The first one is the hardware dependant
code. It is an hardware abstraction layer composed of all the functions that are
directly interacting with the hardware. Typically it is the set of functions
in charge of emission and reception of characters. The whole hardware
depenent code is located in the files : *reader_hal_basis.c* and
*reader_hal_comm_settings.c* in the *./src* directory. The orther part is the
logic (hardware independant code) state machine to make the ISO7816 protocol
work.


## Testing the code

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
 * stm32f411
 * stm32f407


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


	
## To Do

* Implementing PPS negociation. See ISO/IEC7816-3 section 9.
* Implementing inverted convention for the character level transmission/reception. See ISO/IEC7816-3 section 8.1.
* Implementing CRC (only LRC implemented yet) for the T=1 blocks.
* Translating some remaining doxygen comments from french to english
