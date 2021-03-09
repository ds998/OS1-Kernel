/*
 * KEvent.cpp
 *
 *  Created on: Jul 4, 2019
 *      Author: OS1
 */

#include "Lock.h"
#include "PCB.h"
#include "KEvent.h"
#include "Semaphor.h"
#include "switch.h"
#include "iostream.h"
KernelEv::KernelEv(IVTNo ivn,PCB* run):iv(ivn),owner(run){
	sem=new Semaphore(0);
}
void KernelEv::wait(){
	if(running->id==owner->id){
		sem->wait(0);
	}
}

void KernelEv::signal(){
	if(sem->val()<0){
		sem->signal();
	}
}
IVTNo KernelEv::getiv(){
	return iv;
}
KernelEv::~KernelEv(){
	elock.lock();
	delete sem;
	sem=nullptr;
	owner=nullptr;
	elock.unlock();
}



