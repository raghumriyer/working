#include <iostream>
#include "ThreadPool.h"
#include "RequestWorkerThread.h"

using namespace std;

extern "C" void assignTasks(unsigned int);

RequestWorkerThread::RequestWorkerThread(unsigned int id):reqId(id)
{
}

unsigned RequestWorkerThread::executeThis()
{
    return(0);
}

void assignTasks(unsigned int reqId)
{
    ThreadPool* myPool = ThreadPool::getInstance();
    char buff[30]={0};
    sprintf(buff,"%u",reqId);
    myPool->getMsgQueue()->send(buff);
}

void mins()
{
    ThreadPool* myPool = ThreadPool::getInstance();
}
