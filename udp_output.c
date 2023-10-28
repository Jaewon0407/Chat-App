#include <pthread.h>
#include "udp_input.h"
#include "udp_output.h"
#include "list.h"
#include "shared_data.h"
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

void* udpOutputThread(void* arg) {
    
    SharedData* udpOutputData = (SharedData*)arg;

    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    
    if (getaddrinfo(udpOutputData->remoteMachine, udpOutputData->remotePort, &hints, &res) != 0) {
        perror("Error getting address info in udp output");
        return NULL;
    }
    
    int udpSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (udpSocket < 0) {
        perror("Error creating socket");
        freeaddrinfo(res);
        return NULL;
    }

    while (1) {

        pthread_mutex_lock(&udpOutputData->mutex1);
        while (List_count(udpOutputData->messageList1) == 0 && udpOutputData->workToDo == 0) {
            pthread_cond_wait(&udpOutputData->condVar1, &udpOutputData->mutex1);
        }

        if (udpOutputData->workToDo == 1) {
            pthread_mutex_unlock(&udpOutputData->mutex1);
            break;
        }
        
        char* message = List_trim(udpOutputData->messageList1);

        int bytesSent = sendto(
            udpSocket, 
            message, 
            strlen(message), 
            0,
            res->ai_addr,
            res->ai_addrlen
        );

        if (bytesSent <= 0) {
            perror("Error sending UDP output data");
            break;
        }

        pthread_mutex_unlock(&udpOutputData->mutex1);

    }

    freeaddrinfo(res);
    close(udpSocket);
    return NULL;
}