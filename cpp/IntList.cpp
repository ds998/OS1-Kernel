/*
 * IntList.cpp
 *
 *  Created on: Aug 10, 2019
 *      Author: OS1
 */

#include "IntList.h"
bool intcomp(void* t1,void* t2){
	return *((int*)t1)<*((int*)t2);
}
IntList::IntList():List<int>(&intcomp){}
IntList::~IntList(){}
int* IntList::remove(int x){
	elock.lock();
		Node<int>* h=head;
		Node<int>* pret=nullptr;
		while(h!=nullptr && *(h->element)!=x){
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
			int* el=h->element;
			h->next=nullptr;
			delete h;
			h=nullptr;
			elock.unlock();
			return el;
		}
		else {
			elock.unlock();
			return nullptr;
		}
}


