/***
 * main.cpp - Test Runner
 * Jon Durrant
 * 4-Oct-2022
 *
 *
 */

#include "pico/stdlib.h"
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "BlinkAgent.h"
#include "LoadAgent.h"



#define TASK_PRIORITY     ( tskIDLE_PRIORITY + 1UL )


void runTimeStats(){
  TaskStatus_t         * pxTaskStatusArray;
  volatile UBaseType_t uxArraySize, x;
  unsigned long        ulTotalRunTime;


  /* Take a snapshot of the number of tasks in case it changes while this
  function is executing. */
  uxArraySize = uxTaskGetNumberOfTasks();
  printf("Number of tasks %d\n", uxArraySize);

  /* Allocate a TaskStatus_t structure for each task.  An array could be
  allocated statically at compile time. */
  pxTaskStatusArray = (TaskStatus_t*) pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));

  if (pxTaskStatusArray != NULL){
    /* Generate raw status information about each task. */
    uxArraySize = uxTaskGetSystemState(pxTaskStatusArray,
                                       uxArraySize,
                                       &ulTotalRunTime);



    /* For each populated position in the pxTaskStatusArray array,
    format the raw data as human readable ASCII data. */
    for (x = 0; x < uxArraySize; x++){
      printf("Task: %d \t cPri:%d \t bPri:%d \t hw:%d \t%s\n",
             pxTaskStatusArray[x].xTaskNumber,
             pxTaskStatusArray[x].uxCurrentPriority,
             pxTaskStatusArray[x].uxBasePriority,
             pxTaskStatusArray[x].usStackHighWaterMark,
             pxTaskStatusArray[x].pcTaskName
      );
    }


    /* The array is no longer needed, free the memory it consumes. */
    vPortFree(pxTaskStatusArray);
  } else{
    printf("Failed to allocate space for stats\n");
  }

  HeapStats_t heapStats;
  vPortGetHeapStats(&heapStats);
  printf("HEAP avl: %d, blocks %d, alloc: %d, free: %d\n",
         heapStats.xAvailableHeapSpaceInBytes,
         heapStats.xNumberOfFreeBlocks,
         heapStats.xNumberOfSuccessfulAllocations,
         heapStats.xNumberOfSuccessfulFrees
  );

}


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



	LoadAgent load;
	load.start("Load", TASK_PRIORITY);

	printf("Start Tests\n");

	//Run Tests
	const char *av[1] = {"test"};
	const  int ac = 1;
	//int res = CommandLineTestRunner::RunAllTests(ac, av);

	while (true){

	//runTimeStats();

	vTaskDelay(3000);

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
