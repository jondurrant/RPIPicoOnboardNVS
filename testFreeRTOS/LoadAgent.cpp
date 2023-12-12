/*
 * LoadAgent.cpp
 *
 *  Created on: 12 Dec 2023
 *      Author: jondurrant
 */

#include "LoadAgent.h"
#include <math.h>

LoadAgent::LoadAgent() {
	// TODO Auto-generated constructor stub

}

LoadAgent::~LoadAgent() {
	// TODO Auto-generated destructor stub
}


/***
* Run loop for the agent.
*/
void LoadAgent::run(){
	for (;;){
		for (int i=1; i < 0xFFFFFF; i++ ){
				double sq = (double)i;
				double r = sqrt(sq);
				taskYIELD();
			}
	}
}


/***
* Get the static depth required in words
* @return - words
*/
configSTACK_DEPTH_TYPE LoadAgent::getMaxStackSize(){
	return 128;
}

