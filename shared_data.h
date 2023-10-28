#include <pthread.h>
#include "list.h"

#ifndef _SHARED_DATA_H_
#define _SHARED_DATA_H_

typedef struct SharedData_s SharedData;
struct SharedData_s {
    pthread_t threadID;
    List* messageList1;
    List* messageList2;
    pthread_mutex_t mutex1;
    pthread_cond_t condVar1;
    pthread_mutex_t mutex2;
    pthread_cond_t condVar2;
    int workToDo;
    char* localMachine;
    char* myPort;
    char* remoteMachine;
    char* remotePort;
};

#endif