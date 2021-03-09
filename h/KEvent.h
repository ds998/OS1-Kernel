/*
 * KEvent.h
 *
 *  Created on: Jul 4, 2019
 *      Author: OS1
 */

#ifndef KEVENT_H_
#define KEVENT_H_

#include "basic.h"
class PCB;
class Semaphore;
class KernelEv{
public:
	KernelEv(IVTNo ivn,PCB* run);
	~KernelEv();
	void wait();
	void signal();
	IVTNo getiv();
private:
	Semaphore *sem;
	IVTNo iv;
	PCB* owner;
};



#endif /* KEVENT_H_ */
