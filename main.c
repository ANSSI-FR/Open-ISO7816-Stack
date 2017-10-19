#include <avr/io.h>
#include "test.h"
#include "const_defines.h"

int main(void){
	setup();
	
	while(1){
		loop();
	}
	
	return 0;
}
