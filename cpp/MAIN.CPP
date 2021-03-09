//pretpostavljeni memorijski model: huge
#include "List.h"
#include "Lock.h"
#include "switch.h"
#include "schedule.h"
#include "iostream.h"
#include "IVTEntry.h"
#include "Event.h"
#include "idle.h"
#include "user.h"
#include "PCB.h"
#include "semaphor.h"
#include "PCBList.h"
#include "thread.h"
#include "IntList.h"
#include "KSem.h"
#include <dos.h>




pInterrupt oldRoutine=getvect(0x08);



void inic(){
	setvect(0x08,timer);
	setvect(0x60,oldRoutine);
    PCB::initRunning();
    PCB::initIdle();
    for(int i=0;i<16;i++){
    	PCB::globsigperm[i]=true;
    }

}
void restore(){
	setvect(0x08,oldRoutine);
	idle->exitThread();
	maint->exitThread();
	delete idle;
	delete maint;
	maint=nullptr;
	idle=nullptr;
	idlepointer=nullptr;
	running=nullptr;
	oldRoutine=nullptr;
}
int main(int argc,char* argv[]){
	//cout<<"Alloc_cnt: "<<alloc_cnt<<endl;
	inic();
	int userMain(int,char**);
	userMain(argc,argv);
	restore();
	//cout<<"Alloc_cnt: "<<alloc_cnt<<endl;
	return 0;
}

