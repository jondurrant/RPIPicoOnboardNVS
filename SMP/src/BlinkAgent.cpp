/*
 * BlinkAgent.cpp
 *
 *  Created on: 15 Aug 2022
 *      Author: jondurrant
 */

#include "BlinkAgent.h"

#include "stdio.h"
#include "hardware/structs/sio.h"



//Local enumerator of the actions to be queued
enum BlinkAction {BlinkOff, BlinkOn, BlinkSpeed};

//Queue cmd structure
struct BlinkCmd {
	BlinkAction 	action;
	uint 				bpm;
};

//Type def for the queue command
typedef struct BlinkCmd BlinkCmdT;

/***
 * Constructor
 * @param gp - GPIO Pad number for LED
 */
BlinkAgent::BlinkAgent(uint8_t gp) {
	xLedPad = gp;

	xCmdQ = xQueueCreate( BLINK_QUEUE_LEN, sizeof(BlinkCmdT));
	if (xCmdQ == NULL){
		printf("ERROR: Unable to create Queue\n");
	}
}

/***
 * Destructor
 */
BlinkAgent::~BlinkAgent() {
	stop();
	if (xCmdQ != NULL){
		vQueueDelete(xCmdQ);
	}
}


 /***
  * Main Run Task for agent
  */
 void BlinkAgent::run(){
    BlinkCmdT cmd;
    BaseType_t res;
	printf("Blink Started\n");

	gpio_init(xLedPad);

	gpio_set_dir(xLedPad, GPIO_OUT);

	if (xCmdQ == NULL){
		return;
	}

	while (true) { // Loop forever
		res = xQueueReceive(xCmdQ, (void *)&cmd, 10);
		if (res == pdTRUE){
			switch(cmd.action){
			case BlinkOff:
				xBlinkOn = false;
				xLedOn = false;
				gpio_put(xLedPad, xLedOn);
				break;
			case BlinkOn:
				xBlinkOn = true;
				break;
			case BlinkSpeed:
				xDelayTime =((1000 * 60) / cmd.bpm)/2;
				break;
			}
		}

		if (xBlinkOn){
			uint32_t now = to_ms_since_boot(get_absolute_time());
			if (now > (xLastAction + xDelayTime)){
				  xLedOn = ! xLedOn;
                  gpio_put(xLedPad, xLedOn);
                  xLastAction = now;

                  //printf("%s on %u\n",pName,  (uint8_t)sio_hw->cpuid);

                  if (!xLedOn){
                	  xBlinkCount ++;
                  }
			}
		}
	}

 }

/***
 * Get the static depth required in words
 * @return - words
 */
configSTACK_DEPTH_TYPE BlinkAgent::getMaxStackSize(){
	return 256;
}


/***
 * Get the number of blinks that have been completed
 * @return
 */
uint BlinkAgent::getBlinkCount(){
	return xBlinkCount;
}

/***
 * Set the Blink Speed
 * @param bpm - Blinks Per Minute
 */
void BlinkAgent::setSpeed(uint bpm) {
	BaseType_t res;

	BlinkCmdT cmd;
	cmd.action = BlinkSpeed;
	cmd.bpm = bpm;

	if (xCmdQ != NULL){
		res = xQueueSendToBack(xCmdQ, (void *)&cmd, 0);
		if (res != pdTRUE){
			printf("WARNING: Queue is full\n");
		}
	}
}


/***
 * Turn Blink On
 */
void BlinkAgent::blinkOn(){
	BaseType_t res;

	BlinkCmdT cmd;
	cmd.action = BlinkOn;

	if (xCmdQ != NULL){
		res = xQueueSendToBack(xCmdQ, (void *)&cmd, 0);
		if (res != pdTRUE){
			printf("WARNING: Queue is full\n");
		}
	}
}

/***
 * turn blink off
 */
void BlinkAgent::blinkOff(){
	BaseType_t res;

	BlinkCmdT cmd;
	cmd.action = BlinkOff;

	if (xCmdQ != NULL){
		res = xQueueSendToBack(xCmdQ, (void *)&cmd, 0);
		if (res != pdTRUE){
			printf("WARNING: Queue is full\n");
		}
	}
}
