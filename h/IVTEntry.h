/*
 * IVTEntry.h
 *
 *  Created on: Jul 4, 2019
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "basic.h"
class KernelEv;


class IVTEntry{
public:
	pInterrupt old;
	pInterrupt newx;
	KernelEv* event;
	IVTNo iv;
	static IVTEntry* array[256];
	IVTEntry(IVTNo ivn,pInterrupt n);
	void signal();
	void setevent(KernelEv* k);
	~IVTEntry();



};





#endif /* IVTENTRY_H_ */
