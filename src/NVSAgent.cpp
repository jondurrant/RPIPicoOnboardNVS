/*
 * NVSAgent.cpp
 *
 *  Created on: 14 Feb 2024
 *      Author: jondurrant
 */

#include "NVSAgent.h"
#include <cstdio>
#include "pico/stdlib.h"
#include "NVSOnboard.h"

#define STACK1 256
#define STACK0 256


NVSAgent *NVSAgent::pSingleton = NULL;

NVSAgent::NVSAgent() {
	xCommitSem = xSemaphoreCreateBinary();
	if (xCommitSem == NULL){
		printf("Create Semipher failed for NVSAgent\n");
	}
}

NVSAgent::~NVSAgent() {
	if (xCommitSem != NULL){
		vSemaphoreDelete(xCommitSem);
	}
}


NVSAgent * NVSAgent::getInstance(){
	if (NVSAgent::pSingleton == NULL){
		NVSAgent::pSingleton  = new NVSAgent;
	}
	return NVSAgent::pSingleton;
}


void NVSAgent::start(UBaseType_t priority ){
	BaseType_t res;
	UBaseType_t coreMask ;

	//Core 0 task
	res = xTaskCreate(
				NVSAgent::vTask0,
				pName0,
				STACK0,
				( void * ) this,
				priority,
				&xHandle0
			);
   coreMask = 0x1;
   vTaskCoreAffinitySet(xHandle0, coreMask );

   //Core 1 task
	res = xTaskCreate(
			NVSAgent::vTask1,
			pName1,
			STACK1,
			( void * ) this,
			priority,
			&xHandle1
		);
	coreMask = 0x2;
	vTaskCoreAffinitySet(xHandle1, coreMask );
}

/***
* Stop task
* @return
*/
void NVSAgent::stop(){
	if (xHandle0 != NULL){
		vTaskDelete(  xHandle0 );
		xHandle0 = NULL;
	}
	if (xHandle1 != NULL){
		vTaskDelete(  xHandle1 );
		xHandle1 = NULL;
	}
}

/***
* Start the task via static function
* @param pvParameters - will be the Agent object
*/
void NVSAgent::vTask0( void * pvParameters ){
	NVSAgent *task = (NVSAgent *) pvParameters;
	 if (task != NULL){
		 task->run0();
	 }
}

/***
* Start the task via static function
* @param pvParameters - will be the Agent object
*/
void NVSAgent::vTask1( void * pvParameters ){
	NVSAgent *task = (NVSAgent *) pvParameters;
	 if (task != NULL){
		 task->run1();
	 }
}

/***
* Task main run loop
*/
void NVSAgent::run0(){
	printf("NVSAgent0 started\n");
	for (;;){
		//Wait for notification to commit NVS to flash
		if (ulTaskNotifyTake( pdTRUE, 3000) != 0){

			pause1(); //pause core 1
			NVSOnboard::getInstance()->commit();
			resume1(); //resume core 1
			if (xCommitSem != NULL){
				//Signal the commit function to unblock and return
				xSemaphoreGive( xCommitSem);
			}
		 }
	}
}

/***
* Task main run loop
*/
void NVSAgent::run1(){
	printf("NVSAgent1 started\n");
	for (;;){
		// Wait for signal to pause
		 if (ulTaskNotifyTake( pdTRUE, 3000) != 0){
			 //Let the caller know that we have paused
			 xCore1Paused = true;
			 //Hold the core in busy state doing nothing
			 while (! xCore1Resume) {
				 	;
			 }
			 //Clear does the signal variables
			 xCore1Paused = false;
			 xCore1Resume = false;
		 }
	}
}

void NVSAgent::pause1(){
	//Notify core1 process to pause
	xTaskNotifyGive(  xHandle1 );
	//Wait until the pause is acknowledged
	while (!xCore1Paused){
		vTaskDelay(2);
	}
}

void NVSAgent::resume1(){
	//Signal the core1 to resume by chanign the bool
	xCore1Resume=true;
}

void NVSAgent::commit(){
	//Notify the core0 process to commit
	xTaskNotifyGive(  xHandle0 );
	//Wait for semaphore that commit has completed
	if (xCommitSem != NULL){
		xSemaphoreTake(  xCommitSem, 10000 );
	}
}

