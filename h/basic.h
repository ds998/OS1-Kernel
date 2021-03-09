/*
 * basic.h
 *
 *  Created on: Jun 27, 2019
 *      Author: OS1
 */

#ifndef BASIC_H_
#define BASIC_H_
typedef void interrupt (*pInterrupt)(...);

#define true 1
#define false 0
#define nullptr 0L
typedef int bool;

typedef unsigned char IVTNo;
typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms


typedef int ID;

extern volatile int alloc_cnt;

void* operator new(unsigned size);
void operator delete(void *);


#endif /* BASIC_H_ */
