/*
 * basic.cpp
 *
 *  Created on: Jun 27, 2019
 *      Author: OS1
 */
#include "malloc.h"
#include "iLock.h"
#include "dos.h"
#include <stdio.h>
#include <stdlib.h>
#include "basic.h"
#include "iostream.h"

volatile int alloc_cnt=0;
void* operator new(unsigned size) {

  ilock

  void* ret = malloc(size);

  if(ret == nullptr && size > 0) {
    exit(1);
  }
  alloc_cnt++;
  return ret;
}

void operator delete(void* loc) {

  ilock

  if(loc != nullptr){
	  free(loc);
	  alloc_cnt--;
  }

}


