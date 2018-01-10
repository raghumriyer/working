#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H


//The worker thread class handles all the incoming http requests

#include <pthread.h>
#include <queue>
#include <string.h>
#include "CondVar.h"
#include "RequestLock.h"

class WorkerThread 
{
public:
    WorkerThread(const char* threadName);

    ~WorkerThread();

    bool createThread();

    void exitThread();

    pthread_t getThreadId();

    const char* getThreadName();

    void PostMsg(unsigned int data);

    bool isMsgQueueEmpty();

private:
    WorkerThread(const WorkerThread&);
    WorkerThread& operator=(const WorkerThread&);

    static void* Process(void* ptr);
    RequestLock requestLock; 
    pthread_t m_thread;
    std::queue<unsigned int> m_queue;
    pthread_mutex_t m_mutex;
    CondVar m_cv;
    string m_threadName;
};

#endif //WORKER_THREAD
