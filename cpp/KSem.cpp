/*
 * KSem.cpp
 *
 *  Created on: Jul 2, 2019
 *      Author: OS1
 */

#include "Lock.h"
#include "PCB.h"
#include "PCBList.h"
#include "KSem.h"
#include "switch.h"
#include "iLock.h"
#include "Schedule.h"
#include "iostream.h"
PCBList KernelSem::timeblocked;


KernelSem::KernelSem(int init):value(init){
	blocked=new PCBList();
}
KernelSem::~KernelSem(){
	elock.lock();
	delete blocked;
	blocked=nullptr;
	elock.unlock();
}

int KernelSem::wait(Time maxTimeToWait){
	int x=1;
	if(--value<0){
		elock.lock();
		running->state=PCB::BLOCKED;
		running->mySem=this;
		running->sem_saved=1;
		if(maxTimeToWait>0){
			running->waiting_time=maxTimeToWait;
			timeblocked.time_add((PCB*)running);
		}
		blocked->push_back((PCB*)running);
		elock.unlock();
		dispatch();
        x=running->time_unblocked;
	}
	return x;
}
int KernelSem::signal(int n){
	elock.lock();
	int x=0;
	if(n<0){return n;}
	else if(n==0){
		if(value++<0){
			PCB* newx=blocked->pop_begin();
			timeblocked.notime_remove(newx->id);
			newx->state=PCB::READY;
			newx->time_unblocked=1;
			newx->sem_saved=0;
			newx->mySem=nullptr;
			Scheduler::put(newx);
		}
	}
	else{
		for(int i=0;i<n;i++){
			if(value++<0){
				PCB* newx=blocked->pop_begin();
				x++;
				timeblocked.notime_remove(newx->id);
				newx->state=PCB::READY;
				newx->time_unblocked=1;
				newx->sem_saved=0;
				newx->mySem=nullptr;
				Scheduler::put(newx);

			}
		}
	}
	elock.unlock();
	return x;
}
int KernelSem::val() const{
	return value;
}
void KernelSem::exit(int i){
	elock.lock();
	if(this->blocked->removebyID(i)==1){
		this->value++;
	}
	elock.unlock();
}


