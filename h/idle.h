/*
 * idle.h
 *
 *  Created on: Jul 6, 2019
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_


#include "basic.h"
#include "thread.h"

class Idle:public Thread{
public:
	Idle(StackSize stackSize = defaultStackSize,Time timeSlice = defaultTimeSlice);
	~Idle();
	void run();
};


#endif /* IDLE_H_ */
