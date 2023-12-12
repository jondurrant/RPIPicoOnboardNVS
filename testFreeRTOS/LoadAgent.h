/*
 * LoadAgent.h
 * Agent to cause load on system and
 * require NVS to take action to gain sole control
 *
 *  Created on: 12 Dec 2023
 *      Author: jondurrant
 */

#ifndef TESTFREERTOS_LOADAGENT_H_
#define TESTFREERTOS_LOADAGENT_H_

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

#include "Agent.h"

class LoadAgent : public Agent {
public:
	LoadAgent();
	virtual ~LoadAgent();


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
};

#endif /* TESTFREERTOS_LOADAGENT_H_ */
