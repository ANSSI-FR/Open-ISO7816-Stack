#!/bin/bash

if [[ $1 = "stm32f411" ]]
then
	openocd -f openocd/stm32f411/stm32f4discovery.cfg -c 'init' -c 'reset halt'
elif [[ $1 = "stm32f407" ]]
then
	openocd -f openocd/stm32f407/stm32f4discovery.cfg -c 'init' -c 'reset halt'
else
	echo '[ERROR] : Please specify the target to connect to. Currently supported targets are : stm32f411 and stm32f407.'
fi
