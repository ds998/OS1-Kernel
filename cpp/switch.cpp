/*
 * switch.cpp
 *
 *  Created on: Jun 27, 2019
 *      Author: OS1
 */
#include "basic.h"
#include "switch.h"
#include "schedule.h"
#include "PCB.h"
#include "Lock.h"
#include "KSem.h"
#include "PCBList.h"
#include "idle.h"
#include <iostream.h>
volatile int tend=0;
volatile int brojac=20;
volatile PCB* running;
volatile PCB* idlepointer;
Thread* maint=nullptr;
Idle* idle=nullptr;
unsigned tsp;
unsigned tss;
unsigned tbp;
int lockCnt=0;
int context_switch_on_demand=0;
Lock elock(false);

void interrupt context_switch(){
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
		running->sp = tsp;
		running->ss = tss;
		running->bp = tbp;
        if(running->sem_saved){
        	running->lCnt=lockCnt;
        	running->csdemand=context_switch_on_demand;
        }
		if(running->state==PCB::RUNNING){

            running->state=PCB::READY;
			if(running!=idlepointer)Scheduler::put((PCB*) running);
		}

		running=Scheduler::get();

		if(running==nullptr || running->state!=PCB::READY){
			running=idlepointer;
		}
        running->state=PCB::RUNNING;
		tsp = running->sp;
		tss = running->ss;
		tbp = running->bp;
        if(running!=idlepointer && running->sem_saved){
        	lockCnt=running->lCnt;
        	context_switch_on_demand=running->csdemand;
        }
        if(running!=idlepointer){
        	idlepointer->state=PCB::READY;
        }
        brojac = running->time;
        context_switch_on_demand=0;
		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp, tbp
		}
		if(running!=idlepointer){
			((PCB*)running)->sigact();
		}

}
void interrupt timer_tick(){
	KernelSem::timeblocked.decrease();
	while(KernelSem::timeblocked.popzero()){
		PCB* fovx=KernelSem::timeblocked.pop_begin();
		fovx->mySem->exit(fovx->id);
		fovx->time_unblocked=0;
		fovx->sem_saved=0;
		fovx->state=PCB::READY;
		fovx->mySem=nullptr;
		Scheduler::put(fovx);
	}
	void tick();
	tick();
}
void interrupt timer(...){	// prekidna rutina
	asm int 60h;
	timer_tick();
	if(brojac>0){
	    brojac--;
	    if (brojac == 0) {
	    	if(lockCnt==0)context_switch();
	    	else context_switch_on_demand=1;
	    }
	}
}

