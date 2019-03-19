#!/bin/bash

openocd -f openocd/stm32f4discovery.cfg -c 'init' -c 'reset halt'
