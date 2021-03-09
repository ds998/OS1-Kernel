/*
 * List.h
 *
 *  Created on: Jun 26, 2019
 *      Author: OS1
 */

#ifndef LIST_H_
#define LIST_H_
#include "Node.h"
#include "iostream.h"
#include "basic.h"
#include "Lock.h"
#include "switch.h"
template<class T>
class List{
public:
	List(bool (*c)(void*,void*));
	List(const List<T> &l);
	~List();
	void push_back(T* Element);
	T* pop_back();
	void push_begin(T* Element);
    T* pop_begin();
	void sort_add(T* Element);
	void cond_remove(T* Element);
	bool isempty();
	friend ostream& operator<<(ostream& os,const List<T> &l);
	Node<T>* head;
	Node<T>* tail;
	bool (*comp)(void*,void*);
};
template<class T>
List<T>::List(bool (*c)(void*,void*)):comp(c){
	head=tail=nullptr;
}
template<class T>
List<T>::List(const List<T> &l)
{
	    this->comp=l.comp;
		if (l.head == nullptr) {
		    head = nullptr;
		}
		else {
			T elem=*(l.head->element);
			head = new Node<T>(new T(elem));
			Node<T> *current = head;
			Node<T> *currentObj = l.head;
			while (currentObj->next != nullptr){
				T el=*(currentObj->next->element);
			    current->next = new Node<T>(new T(el));
				currentObj = currentObj->next;
				current = current->next;
			}
			tail=current;

		}
}
template<class T>
List<T>::~List(){
	elock.lock();
	Node<T>* old=nullptr;
	Node<T>* cur=head;
	while(cur!=nullptr){
		old=cur;
		cur=cur->next;
		delete old;
	}
	head=tail=nullptr;
	comp=nullptr;
	elock.unlock();
}
template<class T>
void List<T>::push_back(T* Element){
	elock.lock();
	Node<T>* node=new Node<T>(Element);
	if(head==nullptr) head=node;
	else tail->next=node;
	tail=node;
	elock.unlock();
}
template<class T>
void List<T>::push_begin(T* Element){
	elock.lock();
	Node<T>* node=new Node<T>(Element);
	node->next=head;
	head=node;
	if(tail==nullptr)tail=node;
	elock.unlock();
}
template<class T>
T* List<T>::pop_back(){
	elock.lock();
	if(tail!=nullptr){
	     Node<T>* cur=head;
	     if(head==tail)cur=nullptr;
	     else{
	    	 while(cur->next!=tail)cur=cur->next;
	     }
	     T* el=tail->element;
	     delete tail;
	     tail=cur;
	     if(tail!=nullptr){
	    	 tail->next=nullptr;
	     }
	     else{
	    	 head=nullptr;
	     }
	     cur=nullptr;
	     elock.unlock();
	     return el;
	}
	elock.unlock();
	return nullptr;
}
template<class T>
T* List<T>::pop_begin(){
	elock.lock();
	if(head!=nullptr){
		T* el=head->element;
		Node<T>* cur=head->next;
		if(tail==head)tail=nullptr;
		head->next=nullptr;
		delete head;
		head=cur;
		cur=nullptr;
		elock.unlock();
		return el;
	}
	elock.unlock();
	return nullptr;
}

template<class T>
void List<T>::sort_add(T* Element){
	elock.lock();
	Node<T>* node=new Node<T>(Element);
	if(head==nullptr){
		head=tail=node;
	}
	else{
		Node<T>* pret=nullptr;
		Node<T>* cur=head;
		while(cur!=nullptr && comp((void*)cur->element,(void*)node->element)){
			pret=cur;
			cur=cur->next;
		}
		if(cur==nullptr){
			delete node;
			node=nullptr;
			cur=nullptr;
			pret=nullptr;
			push_back(Element);
		}
		else{
			if(pret!=nullptr)pret->next=node;
			node->next=cur;
			if(head==cur){
				head=node;
			}
			pret=nullptr;
			cur=nullptr;
		}
	}
	elock.unlock();
}
template<class T>
void List<T>::cond_remove(T* Element){
	elock.lock();
	if(head!=nullptr){
		Node<T>* cur=head;
		Node<T>* pret=nullptr;
		while(cur!=nullptr && comp((void*)cur->element,(void*)Element)){
			head=cur->next;
			delete cur;
			cur=head;

		}
		if(head==nullptr)tail=nullptr;
		while(cur!=nullptr){
			while(cur!=nullptr && !comp((void*)cur->element,(void*)Element)){
				pret=cur;
				cur=cur->next;
			}
			if(cur==nullptr) return;
			if(tail==cur){
				tail=pret;
			}
			pret->next=cur->next;
			Node<T>* old=cur;
			cur=cur->next;
			delete old;
			old=nullptr;
		}
		pret=nullptr;

	}
	elock.unlock();
}
template<class T>
bool List<T>::isempty(){
	return head==nullptr;
}
template<class T>
ostream& operator<<(ostream& os,const List<T> &l){
	elock.lock();
	Node<T>* cur=l.head;
	while(cur!=nullptr){
		os<<(*cur->element)<<" ";
		cur=cur->next;
	}
	elock.unlock();
	return os<<"\n";
}


#endif /* LIST_H_ */
