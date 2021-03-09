/*
 * Event.cpp
 *
 *  Created on: Jul 4, 2019
 *      Author: OS1
 */


#include "Event.h"
#include "KEvent.h"
#include "IVTEntry.h"
#include "switch.h"
#include "Lock.h"
#include "iostream.h"
Event::Event(IVTNo ivtNo):myImpl(new KernelEv(ivtNo,(PCB*)running)){
	IVTEntry::array[ivtNo]->setevent(myImpl);
}

void Event::wait(){
	elock.lock();
	myImpl->wait();
	elock.unlock();
}

void Event::signal(){
	elock.lock();
	myImpl->signal();
	elock.unlock();
}

Event::~Event(){
	elock.lock();
	delete IVTEntry::array[myImpl->getiv()];
	IVTEntry::array[myImpl->getiv()]=nullptr;
	delete myImpl;
	myImpl=nullptr;
	elock.unlock();

}



