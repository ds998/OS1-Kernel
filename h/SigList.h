/*
 * SigList.h
 *
 *  Created on: Aug 19, 2019
 *      Author: OS1
 */

#ifndef SIGLIST_H_
#define SIGLIST_H_


#include "List.h"
#include "thread.h"

bool sigcomp(void*,void*);

class SigHandList:public List<SignalHandler>{
public:
	SigHandList();
	void swap(SignalHandler h1,SignalHandler h2);
	void gothrough();
	void makeempty();
	~SigHandList();
};



#endif /* SIGLIST_H_ */
