/*
 * IVTEntry.cpp
 *
 *  Created on: Jul 4, 2019
 *      Author: OS1
 */

#include "switch.h"
#include "Lock.h"
#include "IVTEntry.h"
#include "dos.h"
#include "KEvent.h"
IVTEntry* IVTEntry::array[256];
IVTEntry::IVTEntry(IVTNo ivn,pInterrupt n):iv(ivn),newx(n){
	old=getvect(iv);
	setvect(iv,newx);
	event=nullptr;
	array[iv]=this;
}
IVTEntry::~IVTEntry(){
	elock.lock();
	setvect(iv,old);
	old=nullptr;
	newx=nullptr;
	event=nullptr;
	elock.unlock();
}
void IVTEntry::signal(){event->signal();}
void IVTEntry::setevent(KernelEv* k){event=k;}

