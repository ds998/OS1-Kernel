/*
 * iLock.cpp
 *
 *  Created on: Jul 6, 2019
 *      Author: OS1
 */




#include "iLock.h"

ILock::ILock(){
	inte=0;
	static unsigned tmp;
	asm{
		pushf
		cli
		pop tmp;
	};
	if(tmp & 0x200){
		inte=1;
	}
}

ILock::~ILock(){
	if(inte){
		asm sti;
	}
}
