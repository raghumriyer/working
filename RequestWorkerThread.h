#ifndef REQUEST_WORKER_THREAD
#define REQUEST_WORKER_THREAD

class WorkerThreads
{
    const char* reqId;
public:
    unsigned virtual executeThis()
    {
        return 0;
    }
};

class RequestWorkerThread : public WorkerThreads
{
public:
    RequestWorkerThread(unsigned int id);
    unsigned int reqId;
    unsigned virtual executeThis();
};

void mins();

#endif  //REQUEST_WORKER_THREAD
