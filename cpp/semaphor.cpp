/*
 * semaphor.cpp
 *
 *  Created on: Jul 3, 2019
 *      Author: OS1
 */
#include "Lock.h"
#include "semaphor.h"
#include "basic.h"
#include "KSem.h"
#include "switch.h"

Semaphore::Semaphore(int init){
	myImpl=new KernelSem(init);
}

int Semaphore::wait(Time maxTimetoWait){
	return myImpl->wait(maxTimetoWait);
}
int Semaphore::signal(int n){
	return myImpl->signal(n);
}
int Semaphore::val() const{
	return myImpl->val();
}

Semaphore::~Semaphore(){
	elock.lock();
	delete myImpl;
	myImpl=nullptr;
	elock.unlock();
}



