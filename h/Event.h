/*
 * Event.h
 *
 *  Created on: Jul 4, 2019
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "basic.h"
#include "IVTEntry.h"
class KernelEv;
class Event{
public:
     Event (IVTNo ivtNo);
     ~Event ();
     void wait ();
protected:
     friend class KernelEv;
     void signal(); // can call KernelEv private:
     KernelEv* myImpl;
};

#define PREPAREENTRY(No,flag)\
	void interrupt intr##No(...){\
	    IVTEntry::array[No]->signal();\
		if(flag){\
			IVTEntry::array[No]->old();\
		}\
    }\
    IVTEntry* IVTEntry##No=new IVTEntry(No,&intr##No);
#endif /* EVENT_H_ */
