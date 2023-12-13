/**
 * NVS Demo
 * Jon Durrant - 2021
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include "NVSOnboard.h"


int main() {

	char key[]="Demo";
	uint16_t v ;

	//Initialise IO as we are using printf for debug
	stdio_init_all();

	sleep_ms(2000);

	NVSOnboard * nvs = NVSOnboard::getInstance();

	nvs->printNVS();

	if (nvs->contains(key)){
		 nvs->get_u16(key, &v);
		 printf("%s = %u\n", key, v);
	} else {
		v = 0;
		printf("%s Key not present\n", key);
	}

	nvs->set_u16(key, v+1);
	nvs->commit();

	for (;;){
		sleep_ms(2000);
	}
}
