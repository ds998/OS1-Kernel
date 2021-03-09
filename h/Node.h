/*
 * Node.h
 *
 *  Created on: Jun 26, 2019
 *      Author: OS1
 */

#ifndef NODE_H_
#define NODE_H_

#include "basic.h"
template <class T>
class Node{
public:
	T* element;
	Node* next;
	Node(T* elem):element(elem){
		next=nullptr;
	}
	~Node(){
		element=nullptr;
	}



};



#endif /* NODE_H_ */
