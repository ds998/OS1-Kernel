/*
 * thread.cpp
 *
 *  Created on: Jun 28, 2019
 *      Author: OS1
 */
#include "Lock.h"
#include "PCB.h"
#include "thread.h"
#include "switch.h"
#include "iostream.h"
#include "idle.h"
#include "PCBList.h"
#include "KSem.h"
void dispatch(){
	if(lockCnt==0)context_switch();
	else context_switch_on_demand=1;
}
Thread::Thread(StackSize stackSize,Time timeSlice):myPCB(new PCB(stackSize, timeSlice, this)) {}

Thread::~Thread(){
	myPCB->owner=nullptr;
	elock.lock();
	delete myPCB;
	myPCB=nullptr;
	elock.unlock();
}

ID Thread::getId() {
  return myPCB->id;
}
Thread* Thread::getThreadById(ID id){
	elock.lock();
	PCB* p=PCB::pcbs.getbyId(id);
	if(p!=nullptr){
		Thread* t=p->owner;
        p=nullptr;
		elock.unlock();
		return t;
	}
	elock.unlock();
	return nullptr;
}
ID Thread::getRunningId(){
	return running->id;
}

int Thread::waitToComplete() {
	return myPCB->waitToComplete();
}

void Thread::start() {
	myPCB->start();
}

void Thread::exitThread(){
	if(myPCB->state!=PCB::NOSTART && myPCB->state!=PCB::FINISHED){
		elock.lock();
		myPCB->state = PCB::FINISHED;
		if(myPCB->parent!=-1){
			Thread* par=getThreadById(myPCB->parent);
			par->signal(1);
		}
		this->signal(2);
		myPCB->updateparent();
		myPCB->updatechildren();
		if(myPCB->mySem!=nullptr){
			myPCB->mySem->exit(myPCB->id);
		}
		myPCB->mySem=nullptr;
		myPCB->parent=-1;
		PCB::pcbs.removebyID(myPCB->id);
		KernelSem::timeblocked.notime_remove(myPCB->id);
		myPCB->release();
		if(myPCB->stack!=nullptr){
		    delete[] myPCB->stack;
			myPCB->stack=nullptr;
		}

		elock.unlock();
		if(running->id==getId() && running!=idlepointer && getId()!=maint->getId()){
			dispatch();
		}
	}
}
void Thread::signal(SignalId signal){
	elock.lock();
	myPCB->signal(signal);
	elock.unlock();
}
void Thread::registerHandler(SignalId signal,SignalHandler handler){
	elock.lock();
	myPCB->registerHandler(signal,handler);
	elock.unlock();

}
void Thread::unregisterAllHandlers(SignalId signal){
	elock.lock();
	myPCB->unregisterAllHandlers(signal);
	elock.unlock();
}
void Thread::swap(SignalId id,SignalHandler hand1,SignalHandler hand2){
	elock.lock();
	myPCB->swap(id,hand1,hand2);
	elock.unlock();
}
void Thread::blockSignal(SignalId signal){
	elock.lock();
	myPCB->blockSignal(signal);
	elock.unlock();
}
void Thread::blockSignalGlobally(SignalId signal){
	PCB::blockSignalGlobally(signal);
}
void Thread::unblockSignal(SignalId signal){
	elock.lock();
	myPCB->unblockSignal(signal);
	elock.unlock();
}
void Thread::unblockSignalGlobally(SignalId Signal){
	elock.lock();
	PCB::unblockSignalGlobally(Signal);
	elock.unlock();
}
