/*
 * idle.cpp
 *
 *  Created on: Jul 6, 2019
 *      Author: OS1
 */

#include "Lock.h"
#include "idle.h"
#include <iostream.h>
#include "switch.h"
Idle::Idle(StackSize stackSize,Time timeSlice):Thread(stackSize,timeSlice){}
Idle::~Idle(){waitToComplete();}
void Idle::run(){

	while(1);
}
