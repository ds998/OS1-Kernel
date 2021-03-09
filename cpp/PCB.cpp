/*
 * PCB.cpp
 *
 *  Created on: Jun 27, 2019
 *      Author: OS1
 */

#include "Lock.h"
#include "ILock.h"
#include "Idle.h"
#include "PCBList.h"
#include "PCB.h"
#include "basic.h"
#include "schedule.h"
#include "switch.h"
#include "iostream.h"
#include "dos.h"
#include "KSem.h"
#include "Intlist.h"
#include "SigList.h"
int PCB::ID=0;
PCBList PCB::pcbs;
bool PCB::globsigperm[16];
void sigend(){
	running->owner->exitThread();
}
PCB::PCB(StackSize sSize,Time times,Thread* own):size(sSize),time(times),owner(own),sp(0),bp(0),ss(0),id(ID++),state(PCB::NOSTART),stack(0),waiting_time(0),mySem(0),time_unblocked(0),parent(-1){
	lCnt=0;
	csdemand=0;
	sem_saved=0;
	join_blocked=new PCBList();
	join_num=0;
	parent=-1;
	siglist=new IntList();
	childlist=new IntList();
	for(int i=0;i<16;i++){
		sigperm[i]=true;
	}
	if(id>=2){
		for(int i=0;i<16;i++){
			sighandlist[i]=new SigHandList(*(((PCB*)running)->sighandlist[i]));
		}
	}
	else{
		for(int i=0;i<16;i++){
			sighandlist[i]=new SigHandList();
		}
	}
	if(id>=2){
		parent=running->id;
		running->childlist->push_back(new int(id));

	}
	else{
		registerHandler(0,sigend);
	}
	pcbs.push_back(this);
}
void PCB::initRunning(){
	maint=new Thread();
	running=maint->myPCB;
	running->state=PCB::RUNNING;
}
void PCB::initIdle(){
	idle=new Idle();
	idlepointer=idle->myPCB;
	idlepointer->state=PCB::READY;
	idle->start();

}
int PCB::waitToComplete(){
	elock.lock();
	if(state!=PCB::FINISHED && state!=PCB::NOSTART && running!=this && idlepointer!=this && id!=maint->getId()){
		join_num++;
		running->state=PCB::BLOCKED;
		join_blocked->push_back((PCB*)running);
		elock.unlock();
		dispatch();
	}
	elock.unlock();
	return 1;
}
void PCB::wrapper(){
	running->owner->run();
	running->owner->exitThread();
}
void PCB::release(){
	elock.lock();
	for(int i=0;i<join_num;i++){
		PCB* newx=join_blocked->pop_begin();
		newx->state=PCB::READY;
		Scheduler::put(newx);
		newx=nullptr;

	}
	join_num=0;
	elock.unlock();
}
void (*PCB::foo)() =&PCB::wrapper;
void PCB::createstack(){
	stack=new unsigned[size/sizeof(unsigned)];
	unsigned top=(size/sizeof(unsigned))-1;
	stack[top--]=0x200;
	stack[top--] = FP_SEG(foo);
	stack[top--] = FP_OFF(foo);
	this->sp = FP_OFF(stack+top-8);
	this->ss = FP_SEG(stack+top-8);
	this->bp = FP_OFF(stack+top-8);
}
void PCB::start(){
	createstack();
	if(running!=this && idlepointer!=this){
		this->state=PCB::READY;
		Scheduler::put(this);
	}
}
void PCB::updateparent(){
	elock.lock();
	if(parent!=-1){
		Thread* par=Thread::getThreadById(parent);
		PCB* parentpcb=par->myPCB;
		int *y=parentpcb->childlist->remove(id);
		delete y;
		y=nullptr;
		par=nullptr;
		parentpcb=nullptr;
	}
	elock.unlock();

}
void PCB::updatechildren(){
	elock.lock();
	while(!childlist->isempty()){
		int *x=childlist->pop_begin();
		Thread* child=Thread::getThreadById(*x);
		PCB* childpcb=child->myPCB;
		childpcb->parent=parent;
		if(parent!=-1){
			Thread* par=Thread::getThreadById(parent);
			PCB* parentpcb=par->myPCB;
			parentpcb->childlist->push_back(x);
			par=nullptr;
			parentpcb=nullptr;
		}
		child=nullptr;
		childpcb=nullptr;

	}
	elock.unlock();

}
PCB::~PCB(){
	elock.lock();
	if(state!=PCB::FINISHED){
		updateparent();
		updatechildren();
		pcbs.removebyID(id);
		if(mySem!=nullptr){
			mySem->exit(id);
		}
		mySem=nullptr;
		if(stack!=nullptr){
			delete[] stack;
			stack=nullptr;
		}
	}
	for(int i=0;i<16;i++){
		unregisterAllHandlers(i);
		delete sighandlist[i];
		sighandlist[i]=nullptr;
	}
	delete join_blocked;
	join_blocked=nullptr;
	delete siglist;
	siglist=nullptr;
	delete childlist;
	childlist=nullptr;
	elock.unlock();

}
void PCB::signal(SignalId signal){
	elock.lock();
	if((signal!=1 && signal!=2 && state!=PCB::NOSTART && state!=PCB::FINISHED) || ((signal==1 || signal==2) && state!=PCB::NOSTART && running->state==PCB::FINISHED)){
			if(id==running->id)sighandlist[(int)signal]->gothrough();
			else siglist->push_back(new int(signal));
	}
	elock.unlock();

}
void PCB::registerHandler(SignalId signal,SignalHandler handler){
	sighandlist[(int)signal]->push_back(new SignalHandler(handler));
}
void PCB::unregisterAllHandlers(SignalId id){
	sighandlist[(int)id]->makeempty();
}
void PCB::swap(SignalId id,SignalHandler hand1,SignalHandler hand2){
	sighandlist[(int)id]->swap(hand1,hand2);
}
void PCB::blockSignal(SignalId signal){
	sigperm[(int)signal]=false;
}
void PCB::blockSignalGlobally(SignalId signal){
	PCB::globsigperm[(int)signal]=false;
}
void PCB::unblockSignal(SignalId signal){
	sigperm[(int)signal]=true;
}
void PCB::unblockSignalGlobally(SignalId Signal){
	globsigperm[(int)Signal]=true;
}
void PCB::sigact(){
	elock.lock();
	IntList backup_list;
	while(!siglist->isempty()){
		int* y=siglist->pop_begin();
		if(sigperm[*y] && globsigperm[*y]){
			sighandlist[*y]->gothrough();
			delete y;
		}
		else{
			backup_list.push_back(y);
		}
    }
	while(!backup_list.isempty()){
		siglist->push_back(backup_list.pop_begin());
	}
	elock.unlock();
}
ostream& operator<<(ostream& os,const PCB &pcb){
	elock.lock();
	os<<"pcb "<<pcb.id<<" "<<pcb.waiting_time<<" | ";
	elock.unlock();
	return os;
}

