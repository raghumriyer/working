#include "WorkerThread.h"
#include <iostream>

using namespace std;


WorkerThread::WorkerThread(const char* threadName) : m_thread(0), m_threadName(threadName)
{
    //printf("Calling the worker Thread = %s\n",m_threadName.c_str());
    createThread();
}

void* WorkerThread::Process(void* ptr)
{
    while (1)
    {
        unsigned int msg;
        {
            ((WorkerThread*)ptr)->requestLock.lock();

            // Block the thread and wait till the request is added to queue
            while (((WorkerThread*)ptr)->m_queue.empty())
                ((WorkerThread*)ptr)->m_cv.wait(((WorkerThread*)ptr)->requestLock.getMutexPtr());

            if (((WorkerThread*)ptr)->m_queue.empty())
                continue;

            msg = ((WorkerThread*)ptr)->m_queue.front();
            printf("The threadName=%s and data receve = %d\n",((WorkerThread*)ptr)->getThreadName(),msg);
            ((WorkerThread*)ptr)->m_queue.pop();

            ((WorkerThread*)ptr)->requestLock.unlock();
        }
    }
    return NULL;
}

WorkerThread::~WorkerThread()
{
    exitThread();
}

bool WorkerThread::createThread()
{
    int ret = -1;
    ret = pthread_create(&m_thread, NULL, WorkerThread::Process, (void *)this ); 
    
    if (ret != 0)
    {
        cerr << "pthread_create() failed: " << ret << endl;
    }

    return true;
}

const char* WorkerThread::getThreadName()
{
  return m_threadName.c_str();
}

pthread_t WorkerThread::getThreadId()
{
    return m_thread;
}

void WorkerThread::exitThread()
{
    if (!m_thread)
        return;
}

void WorkerThread::PostMsg(unsigned int req)
{
    // When you receive the message push the msg to queue for processing, 
    // and signal the waiting thread
    requestLock.lock();
    m_queue.push(req);
    m_cv.signal();
    requestLock.unlock();
}

bool WorkerThread::isMsgQueueEmpty()
{
    return m_queue.empty();
}
