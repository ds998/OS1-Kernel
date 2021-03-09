/*
 * PCBList.cpp
 *
 *  Created on: Jul 1, 2019
 *      Author: OS1
 */

#include "Lock.h"
#include "PCBList.h"
#include "PCB.h"
#include "basic.h"

bool pcbcomp(void* t1,void* t2){
	return ((PCB*)t1)->waiting_time<((PCB*)t2)->waiting_time;
}
PCBList::PCBList():List<PCB>(&pcbcomp){}
int PCBList::removebyID(int id){
	elock.lock();
	Node<PCB>* h=head;
	Node<PCB>* pret=nullptr;
	while(h!=nullptr && h->element->id!=id){
		pret=h;
		h=h->next;
	}
	if(h!=nullptr){
		if(pret!=nullptr){
			pret->next=h->next;
			if(h->next==nullptr){
				tail=pret;
			}
		}
		else{
			if(head==tail){
				head=nullptr;
				tail=nullptr;

			}
			else{
				head=h->next;
			}
		}
		h->next=nullptr;
		delete h;
		h=nullptr;
		elock.unlock();
		return 1;
	}
	else {
		elock.unlock();
		return 0;
	}


}
void PCBList::decrease(){
	elock.lock();
	if(head!=nullptr){
		head->element->waiting_time--;
	}
	elock.unlock();
}
bool PCBList::popzero(){
	elock.lock();
	if(head!=nullptr){
		if(head->element->waiting_time==0){
			elock.unlock();
			return true;
		}
	}
	elock.unlock();
	return false;
}
void PCBList::time_add(PCB* newx){
	elock.lock();
	Node<PCB>* h=head;
	int x=0;
	int y=0;
	while(h!=nullptr){
		y=x;
		x+=h->element->waiting_time;
		h->element->waiting_time+=y;
		h=h->next;
	}
	this->sort_add(newx);
	h=head;
	x=0;
	y=0;
	while(h!=nullptr){
		y=x;
		h->element->waiting_time-=y;
		x+=h->element->waiting_time;
		h=h->next;
	}
	elock.unlock();

}
void PCBList::notime_remove(ID id){
	elock.lock();
	Node<PCB>* h=head;
	int x=0;
	int y=0;
	while(h!=nullptr){
		y=x;
		x+=h->element->waiting_time;
		h->element->waiting_time+=y;
		h=h->next;
	}
	this->removebyID(id);
	h=head;
	x=0;
	y=0;
	while(h!=nullptr){
		y=x;
		h->element->waiting_time-=y;
		x+=h->element->waiting_time;
		h=h->next;
	}
	elock.unlock();
}
PCB* PCBList::getbyId(ID id){
	elock.lock();
	Node<PCB>* h=head;
	while(h!=nullptr && h->element->id!=id){
		h=h->next;
	}
	if(h!=nullptr){
		PCB* ret=h->element;
		h=nullptr;
		elock.unlock();
		return ret;
	}
	elock.unlock();
	return nullptr;
}
PCB* PCBList::getheadpcb(){
	elock.lock();
	if(head!=nullptr){
		elock.unlock();
		return head->element;
	}
	elock.unlock();
	return nullptr;
}
PCBList::~PCBList(){}
