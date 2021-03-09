/*
 * Lock.cpp
 *
 *  Created on: Jun 26, 2019
 *      Author: OS1
 */

#include "basic.h"
#include "Lock.h"
#include "thread.h"
#include "switch.h"
#include "iLock.h"
#include "iostream.h"

Lock::Lock(bool _lock){
  if(_lock) lock();
}

void Lock::lock() {
	ilock
	lockCnt--;
}

void Lock::unlock() {
	ilock
	lockCnt++;
	if(lockCnt==0 && context_switch_on_demand){
		dispatch();

	}
	if(lockCnt>0) lockCnt=0;
}

Lock::~Lock(){}


