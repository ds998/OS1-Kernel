/*
 * Lock.h
 *
 *  Created on: Jun 26, 2019
 *      Author: OS1
 */

#ifndef LOCK_H_
#define LOCK_H_
#include "basic.h"

class Lock {

public:
  Lock(bool lock);
  ~Lock();

  void lock();
  void unlock();
};



#endif /* LOCK_H_ */
