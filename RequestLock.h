#ifndef REQUEST_LOCK
#define REQUEST_LOCK

#include <pthread.h>
#include <unistd.h>
#include <deque>
#include <iostream>
#include <vector>
#include <errno.h>
#include <string.h>

using namespace std;

class RequestLock 
{
public:
  RequestLock();
  ~RequestLock();
  void lock();
  void unlock();
  pthread_mutex_t* getMutexPtr();

private:
  pthread_mutex_t m_lock;
  volatile bool isLocked;
};

#endif  //REQUEST_LOCK
