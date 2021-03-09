/*
 * PCBList.h
 *
 *  Created on: Jul 1, 2019
 *      Author: OS1
 */

#ifndef PCBLIST_H_
#define PCBLIST_H_

#include "List.h"
class PCB;
bool pcbcomp(void*,void*);
class PCBList:public List<PCB>{
public:
	PCBList();
	int removebyID(int id);
	PCB* getheadpcb();
	void time_add(PCB* newx);
	void notime_remove(ID id);
	PCB* getbyId(ID id);
	~PCBList();
	void decrease();
	bool popzero();
};


#endif /* PCBLIST_H_ */
