#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERVER_QUEUE_NAME   "/request-server2"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 20
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10


class MsgQueue
{
private:
    struct mq_attr attr;
    mqd_t qd_rec, qd_send;  
public:
    int initialize();
    int send(const char* data);
    int receive(char* in_buffer);
};

#endif //MSG_QUEUE_H
