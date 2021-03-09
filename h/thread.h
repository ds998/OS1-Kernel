/*
 * thread.h
 *
 *  Created on: Jun 28, 2019
 *      Author: OS1
 */

#ifndef THREAD_H_
#define THREAD_H_


#include "basic.h"
void dispatch();

class PCB; // Kernel's implementation of a user's thread

typedef void (*SignalHandler)();

typedef unsigned SignalId;
class Thread {
public:

  void start();
  int waitToComplete();
  virtual ~Thread();
  ID getId();
  static ID getRunningId();
  static Thread* getThreadById(ID id);
  void exitThread();


  void signal(SignalId signal);
  void registerHandler(SignalId signal,SignalHandler handler);
  void unregisterAllHandlers(SignalId id);
  void swap(SignalId id,SignalHandler hand1,SignalHandler hand2);

  void blockSignal(SignalId signal);
  static void blockSignalGlobally(SignalId signal);
  void unblockSignal(SignalId signal);
  static void unblockSignalGlobally(SignalId Signal);

protected:
  friend class PCB;
  Thread(StackSize stackSize = defaultStackSize,
         Time timeSlice = defaultTimeSlice);
  virtual void run() {}

private:
  PCB* myPCB;
};


#endif /* THREAD_H_ */
