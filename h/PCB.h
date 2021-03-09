/*
 * PCB.h
 *
 *  Created on: Jun 26, 2019
 *      Author: OS1
 */


#ifndef PCB_H_
#define PCB_H_
#include "basic.h"
#include "thread.h"
#include "List.h"


class KernelSem;
class PCBList;
class SigHandList;
class IntList;
void sigend();

class PCB{
public:
	static int ID;
	static PCBList pcbs;
	static bool globsigperm[16];
	int id;
	KernelSem* mySem;
	PCBList* join_blocked;
	static enum State{NOSTART,READY,RUNNING,BLOCKED,FINISHED};
	StackSize size;
	unsigned sp;
	unsigned bp;
	unsigned ss;
	int lCnt;
	int csdemand;
	int sem_saved;
	int join_num;
	int time_unblocked;
	Time time;
	Time waiting_time;
	unsigned* stack;
	State state;
	Thread* owner;
	int parent;
	IntList* siglist;
	IntList* childlist;
	bool sigperm[16];
	SigHandList* sighandlist[16];
	PCB(StackSize _stackSize= defaultStackSize,
	         Time timeSlice = defaultTimeSlice,Thread* own=nullptr);
	~PCB();
	static void initRunning();
	static void initIdle();
	int waitToComplete();
	void start();
	static void wrapper();
	static void (*foo) ();
	void createstack();
    void release();
    void sigact();
    friend ostream& operator<<(ostream& os,const PCB &pcb);
    void updateparent();
    void updatechildren();


    void signal(SignalId signal);
    void registerHandler(SignalId signal,SignalHandler handler);
    void unregisterAllHandlers(SignalId id);
    void swap(SignalId id,SignalHandler hand1,SignalHandler hand2);

    void blockSignal(SignalId signal);
    static void blockSignalGlobally(SignalId signal);
    void unblockSignal(SignalId signal);
    static void unblockSignalGlobally(SignalId Signal);


};


#endif /* PCB_H_ */


