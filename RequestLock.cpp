#include "RequestLock.h"

RequestLock::RequestLock() 
{
  pthread_mutex_init(&m_lock, NULL);
  isLocked = false;
}

RequestLock::~RequestLock() 
{
  while(isLocked);
  unlock(); 
  pthread_mutex_destroy(&m_lock);
}

void RequestLock::lock() 
{
  pthread_mutex_lock(&m_lock);
  isLocked = true;
}

void RequestLock::unlock() 
{
  isLocked = false;
  pthread_mutex_unlock(&m_lock);
}

pthread_mutex_t* RequestLock::getMutexPtr()
{
  return &m_lock;
}

