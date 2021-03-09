/*
 * KSem.h
 *
 *  Created on: Jul 2, 2019
 *      Author: OS1
 */

#ifndef KSEM_H_
#define KSEM_H_

class PCBList;
class KernelSem{
public:
	static PCBList timeblocked;
	KernelSem(int init=1);
	virtual ~KernelSem ();
	virtual int wait (Time maxTimeToWait);
	virtual int signal(int n=0);
	int val () const;
	void exit(int i);
private:
	int value;
	PCBList* blocked;

};




#endif /* KSEM_H_ */
