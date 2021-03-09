/*
 * IntList.h
 *
 *  Created on: Aug 10, 2019
 *      Author: OS1
 */

#ifndef INTLIST_H_
#define INTLIST_H_

#include "List.h"
bool intcomp(void*,void*);
class IntList:public List<int>
{
public:
	IntList();
	int* remove(int x);
	~IntList();
};


#endif /* INTLIST_H_ */
