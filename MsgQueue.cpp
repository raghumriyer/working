#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include "MsgQueue.h"
#include "RequestWorkerThread.h"
#include "ThreadPool.h"

int MsgQueue::initialize()
{
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    if ((qd_rec = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, 0666, &attr)) == -1) 
    {
        perror ("Server: mq_open (server)");
        exit (1);
    }
}

int MsgQueue::receive(char* in_buffer)
{
    int ret=-1;
    while (1) 
    {
        ret = mq_receive (qd_rec , in_buffer, MSG_BUFFER_SIZE, NULL);
        if ( ret == -1 )
        {
            perror ("Server: mq_receive");
            exit (1);
        }
        char *ptr;
        unsigned int reqId;

        char dest[MSG_BUFFER_SIZE];
        memset(dest, '\0', sizeof(dest));
        strncpy(dest, in_buffer,ret);
        reqId = strtoul(dest,&ptr,10);
        ThreadPool* myPool = ThreadPool::getInstance(); 
        WorkerThread* worker = myPool->getWorkerThread(reqId);
        if(!worker)
        {
            perror("Error getting the worker thread\n");
        }
        worker->PostMsg(reqId);
    }
    return 0;
}

int MsgQueue::send(const char* data)
{
    if ((qd_send = mq_open (SERVER_QUEUE_NAME, O_WRONLY)) == -1) 
    {
        perror ("Error opening mq_open ");
    }

    if (mq_send (qd_send, data, strlen (data), 0) == -1) 
    {
        perror ("Error sending qd_send");
    }
    return 0;
}
