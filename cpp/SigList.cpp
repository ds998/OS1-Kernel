/*
 * SigList.cpp
 *
 *  Created on: Aug 19, 2019
 *      Author: OS1
 */


#include "SigList.h"
#include "Lock.h"
bool sigcomp(void* t1,void* t2){
	return false;
}
SigHandList::SigHandList():List<SignalHandler>(&sigcomp){};
SigHandList::~SigHandList(){}
void SigHandList::swap(SignalHandler h1,SignalHandler h2){
	elock.lock();
	Node<SignalHandler>* h=head;
	Node<SignalHandler>* p1=nullptr;
	Node<SignalHandler>* p2=nullptr;
	bool f1=false;
	bool f2=false;
	while(h!=nullptr){
		if(h1==*(h->element)){
			f1=true;
			p1=h;
			break;
		}
		h=h->next;
	}
	h=head;
	while(h!=nullptr){
		if(h2==*(h->element)){
			f2=true;
			p2=h;
			break;
		}
		h=h->next;
	}
	if(f1 && f2){
		SignalHandler* temp=p1->element;
		p1->element=p2->element;
		p2->element=temp;
	}
	elock.unlock();
}
void SigHandList::gothrough(){
	elock.lock();
	Node<SignalHandler>* h=head;
	while(h!=nullptr){
		(*(h->element))();
		h=h->next;
	}
	elock.unlock();


}
void SigHandList::makeempty(){
	elock.lock();
	while(!isempty()){
		SignalHandler* sh=pop_begin();
		delete sh;
		sh=nullptr;
	}
	elock.unlock();
}



