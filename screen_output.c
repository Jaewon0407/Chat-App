#include <pthread.h>
#include "list.h"
#include "screen_output.h"
#include "shared_data.h"
#include "udp_output.h"
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void* screenOutputThread(void* arg) 
{           
    SharedData* outputData = (SharedData*) arg;
    while (1) {

        pthread_mutex_lock(&outputData->mutex2);
        while(List_count(outputData->messageList2) == 0 && outputData->workToDo == 0) {
            pthread_cond_wait(&outputData->condVar2, &outputData->mutex2);
        }

        if (outputData->workToDo == 1) {
            pthread_mutex_unlock(&outputData->mutex2);
            break;
        }

        char* result = List_trim(outputData->messageList2);
        printf("%s: %s", outputData->localMachine, result);
        pthread_mutex_unlock(&outputData->mutex2);

    }
    
    return NULL;
}

