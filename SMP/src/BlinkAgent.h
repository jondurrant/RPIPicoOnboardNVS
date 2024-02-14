/*
 * BlinkAgent.h
 *
 * Active agent to run as task and blink and LED on the given GPIO pad
 *
 *  Created on: 15 Aug 2022
 *      Author: jondurrant
 */

#ifndef BLINKAGENT_H_
#define BLINKAGENT_H_

#include "Agent.h"
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

#include "queue.h"


#define BLINK_QUEUE_LEN 	10


class BlinkAgent: public Agent {
public:
	/***
	 * Constructor
	 * @param gp - GPIO Pad number for LED
	 */
	BlinkAgent(uint8_t gp=0);

	/***
	 * Destructor
	 */
	virtual ~BlinkAgent();

	/***
	 * Set the Blink Speed
	 * @param bpm - Blinks Per Minute
	 */
	void setSpeed(uint bpm);

	/***
	 * Get the number of blinks that have been completed
	 * @return
	 */
	uint getBlinkCount();

	/***
	 * Turn Blink On
	 */
	void blinkOn();

	/***
	 * turn blink off
	 */
	void blinkOff();


protected:

	/***
	 * Run loop for the agent.
	 */
	virtual void run();


	/***
	 * Get the static depth required in words
	 * @return - words
	 */
	virtual configSTACK_DEPTH_TYPE getMaxStackSize();

	//GPIO PAD for LED
	uint8_t xLedPad = 0;

	uint xBlinkCount = 0;

	//Queue of commands
	QueueHandle_t xCmdQ;

	bool xLedOn	= false;
	bool xBlinkOn = true;

	uint32_t xLastAction = 0;
	uint32_t xDelayTime = 500;
};


#endif /* BLINKAGENT_H_ */
