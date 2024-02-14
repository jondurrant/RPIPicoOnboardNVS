/**
 * NVS Demo using FreeRTOS SMP
 * Jon Durrant - 2021
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include "NVSOnboard.h"

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#include "BlinkAgent.h"
#include "NVSAgent.h"

#define TASK_PRIORITY     ( tskIDLE_PRIORITY + 1UL )

void main_task(void* params){

	printf("Start Blink\n");
	BlinkAgent blinkA(15);
	blinkA.setSpeed(120);
	blinkA.blinkOn();
	blinkA.start("Blink A", TASK_PRIORITY);
	UBaseType_t coreMask = 0x1;
	vTaskCoreAffinitySet( blinkA.getTask(), coreMask );

	BlinkAgent blinkB(14);
	blinkB.setSpeed(120);
	blinkB.blinkOn();
	blinkB.start("Blink B", TASK_PRIORITY);
	coreMask = 0x2;
	vTaskCoreAffinitySet( blinkB.getTask(), coreMask );

	NVSAgent::getInstance()->start();

	char key[]="Demo";
	uint16_t v ;

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
	NVSAgent::getInstance()->commit();

	for (;;){
		sleep_ms(2000);
	}
}

void vLaunch(void) {
  TaskHandle_t task;

  xTaskCreate(main_task, "MainThread", 3000, NULL, TASK_PRIORITY, &task);

  /* Start the tasks and timer running. */
  vTaskStartScheduler();
}


int main(void) {
  stdio_init_all();
  sleep_ms(2000);
  printf("GO\n");

  /* Configure the hardware ready to run the demo. */
  const char* rtos_name;
#if ( portSUPPORT_SMP == 1 )
    rtos_name = "FreeRTOS SMP";
#else
    rtos_name = "FreeRTOS";
#endif

#if ( portSUPPORT_SMP == 1 ) && ( configNUM_CORES == 2 )
    printf("%s on both cores:\n", rtos_name);
#else
    printf("Starting %s on core 0:\n", rtos_name);
#endif

    vLaunch();

  return 0;
}


