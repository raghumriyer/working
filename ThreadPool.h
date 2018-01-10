#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
#include <semaphore.h>
#include <deque>
#include <iostream>
#include <vector>
#include <map>
#include "RequestLock.h"
#include "RequestWorkerThread.h"
#include "CondVar.h"
#include "MsgQueue.h"
#include "WorkerThread.h"

const int DEFAULT_POOL_SIZE = 10;
const int STARTED = 0;
const int STOPPED = 1;


class ThreadPool
{
public:
    virtual ~ThreadPool();
    
    void destroyPool(int maxPollSecs);

    bool assignWorker();
    bool addTask(RequestWorkerThread* worker);
    bool fetchWork(WorkerThreads **worker);

    void initializeThreads();
    void initializeWorkerThread();
    
    static void *threadExecute1(void *param);
    void *threadExecute();
    void *threadRecvMsg();
    static ThreadPool* getInstance();
    MsgQueue* getMsgQueue();
    void insertThreadMap(unsigned int rId, const char* name);
    void handlePriority(unsigned int req);
    WorkerThread* getWorkerThread(unsigned int req);

private:
    ThreadPool();
    ThreadPool(int maxThreads);
    static ThreadPool* instanceS;
    int maxThreads;
    pthread_mutex_t mutexSync, mutexWorkCompletion;

    RequestLock requestLock;
    pthread_t msgThread;
    pthread_mutex_t m_task_mutex;
    std::vector<WorkerThread*> vectorOfWorkers;
    CondVar m_task_cond_var;
    std::deque<RequestWorkerThread*> requestQueue;
    std::map<string, int> vectorTakenMap;
    std::map<unsigned int, const char*> requestThreadMap;
    pthread_cond_t cond_var;
    int queueSize;
    volatile int m_pool_state;
    MsgQueue msgQueue;
};

#endif //THREAD_POOL_H
