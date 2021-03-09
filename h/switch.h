/*
 * switch.h
 *
 *  Created on: Jun 27, 2019
 *      Author: OS1
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include "basic.h"

class Thread;
class PCB;
class Idle;
class Lock;
extern Lock elock;
extern Thread* maint;
extern Idle* idle;
extern volatile PCB* running;
extern volatile PCB* idlepointer;
extern unsigned tsp;
extern unsigned tss;
extern unsigned tbp;
extern volatile int brojac;
extern int lockCnt;
extern int context_switch_on_demand;
extern volatile int tend;
void interrupt context_switch();
void interrupt timer(...);
void interrupt timer_tick();

#endif /* SWITCH_H_ */
