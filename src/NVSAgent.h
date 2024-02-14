/*
 * NVSAgent.h
 *
 * NVS Agent to manage commits when FreeRTOS SMP is being used.
 * In this situation the writes are always done via Core 0 and Core 1 is paused during
 * the write to flash.
 * An instance of NVSAgent should be created and started prior to commit
 *
 *
 *  Created on: 14 Feb 2024
 *      Author: jondurrant
 */

#ifndef SRC_NVSAGENT_H_
#define SRC_NVSAGENT_H_

#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "semphr.h"

class NVSAgent {
public:
	/***
	 * Singleton pattern to access the object
	 * @return
	 */
	static NVSAgent * getInstance();
	virtual ~NVSAgent();

	/***
	 * Starts the agent - this creates tasks on both core0 and core1 to manage
	 * a write to flash
	 * @param priority - defaults to max priority
	 */
	void start(UBaseType_t priority = (configMAX_PRIORITIES - 1));

	/***
	 * Stop task
	 * @return
	 */
	virtual void stop();


	/***
	 * Commit NVS by writing it to the flash
	 * The function blocks until the commit has completed.
	 */
	void commit();



protected:
	/***
	 * Start the task via static function
	 * @param pvParameters - will be the Agent object
	 */
	static void vTask0( void * pvParameters );

	/***
	 * Start the task via static function
	 * @param pvParameters - will be the Agent object
	 */
	static void vTask1( void * pvParameters );

	/***
	 * Task main run loop
	 */
	void run0();

	/***
	 * Task main run loop
	 */
	void __not_in_flash_func(run1)();

	/***
	 * Pause core 1
	 */
	void pause1();

	/***
	 * Resume core 1
	 */
	void resume1();

	//Handles for the two tasks
	TaskHandle_t xHandle0 = NULL;
	TaskHandle_t xHandle1 = NULL;

	//Semaphore used to block the commit method
	SemaphoreHandle_t xCommitSem = NULL;

	const char pName0[10] = "NVSAgent0";
	const char pName1[10] = "NVSAgent1";

	static NVSAgent *pSingleton;


	//The state of the Core1 process
	bool xCore1Paused = false;

	//Request to resume on core1
	bool xCore1Resume = false;

private:
	NVSAgent();
};

#endif /* SRC_NVSAGENT_H_ */
