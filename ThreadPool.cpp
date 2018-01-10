#include <stdlib.h>
#include "ThreadPool.h"
#include "WorkerThread.h"

using namespace std;

ThreadPool* ThreadPool::instanceS = NULL;

extern "C"
void* start_msg_thread(void* arg)
{
    ThreadPool* tp = (ThreadPool*) arg;
    tp->threadRecvMsg();
    return NULL;
}


ThreadPool::ThreadPool()
{
    ThreadPool(2);
}

MsgQueue* ThreadPool::getMsgQueue()
{
    return &msgQueue;
}

ThreadPool::ThreadPool(int maxThreads)
{
    // Create the pool of worker Threads
    char* buff;
    buff = (char*)malloc(sizeof(char)*20);
    for (int i=0; i<maxThreads; ++i)
    {
        sprintf( buff,"workerThread%d",i);
        WorkerThread* workerThreads= new WorkerThread((const char*)buff);
        vectorOfWorkers.push_back(workerThreads);
    }
    msgQueue.initialize();
    if (maxThreads < 1)  
    {
        maxThreads=1;
    } 
    this->maxThreads = maxThreads;
    this->queueSize = maxThreads;
    initializeThreads();
    initializeWorkerThread();
}

void ThreadPool::initializeWorkerThread()
{
    for (std::vector<WorkerThread*>::iterator it = vectorOfWorkers.begin() ; it != vectorOfWorkers.end(); ++it)
    {
        std::cout << "The worker thread ID =" << (*it)->getThreadId() << "thread Name =" << (*it)->getThreadName() << endl;
        string threadName = (*it)->getThreadName();
        vectorTakenMap.insert( std::pair<string, bool>(threadName,false));
    }
}


void ThreadPool::initializeThreads()
{
    int ret = -1;

    // This thread receives the msg from the http server throu and IPC msgQueue
    ret = pthread_create(&msgThread, NULL, start_msg_thread, (void *) this );
    if (ret != 0)
    {
        cerr << "pthread_create() failed: " << ret << endl;
    }
    pthread_cond_init(&cond_var, NULL);
}

ThreadPool* ThreadPool::getInstance()
{
    if(!instanceS)
    {
        fprintf(stderr,"Creating the list of threads \n");
        instanceS = new ThreadPool(DEFAULT_POOL_SIZE);
    }
    return instanceS;
}


ThreadPool::~ThreadPool()
{
  if (m_pool_state != STOPPED) {
    destroyPool(1);
  }
  delete instanceS;
}

void *ThreadPool::threadRecvMsg()
{
    char in_buff[33]={0};
    memset(in_buff,0,33);
    msgQueue.receive(in_buff);
    return NULL;
}

void ThreadPool::destroyPool(int maxPollSecs = 1)
{
    m_pool_state = STOPPED;
    vectorOfWorkers.erase( vectorOfWorkers.begin(),vectorOfWorkers.end() );
    vectorTakenMap.erase( vectorTakenMap.begin(),vectorTakenMap.end() );
    requestThreadMap.erase( requestThreadMap.begin(),requestThreadMap.end() );
}

void ThreadPool::insertThreadMap(unsigned int rId, const char* name)
{
    cout << "Inserting reqId:" << rId << " To name:" << name << endl;
    requestThreadMap.insert( std::pair<unsigned int, const char*>(rId,name));
}

WorkerThread* ThreadPool::getWorkerThread(unsigned int reqId)
{
    // Check if the worker thread is assigned to a request ID
    const char* name =  requestThreadMap.find(reqId)->second;
    if( (!name) && (!(vectorOfWorkers.empty())) )
    {
        //If there is no thread assigned to the request, take the one available and mark it taken
        for (std::vector<WorkerThread*>::iterator it = vectorOfWorkers.begin() ; it != vectorOfWorkers.end(); ++it)
        {
            string threadName = (*it)->getThreadName();

            int taken = 0;
            taken = vectorTakenMap.at(threadName);

            if( 0 == taken)
            {
                insertThreadMap(reqId, threadName.c_str());
                vectorTakenMap.at(threadName) = 1;

                for( map< string,int >::iterator ii=vectorTakenMap.begin(); ii!=vectorTakenMap.end(); ++ii)
                {
                    //cout << (*ii).first << ": " << (*ii).second << endl;
                }

               return (*it);
            }
        }
        
        // If all the threads are taken, then take the fornt thread from the vector which is empty
        WorkerThread* worker;
        for (std::vector<WorkerThread*>::iterator it = vectorOfWorkers.begin() ; it != vectorOfWorkers.end(); ++it)
        {
            if((*it)->isMsgQueueEmpty())
            {
                insertThreadMap(reqId, (*it)->getThreadName());
                return (*it);
            }
        }
    }

    for (std::vector<WorkerThread*>::iterator it = vectorOfWorkers.begin() ; it != vectorOfWorkers.end(); ++it)
    {
        //std::cout << "The worker thread ID =" << (*it)->getThreadId() << endl;
        if(strcmp(name, (*it)->getThreadName() ) == 0)
        {
            return (*it);
        }
    }
    return NULL;
}

bool ThreadPool::addTask(RequestWorkerThread* request)
{
    requestLock.lock();
    requestQueue.push_back(request);
    m_task_cond_var.signal();
    requestLock.unlock();
}
