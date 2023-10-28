#include "shared_data.h"
#include "udp_input.h"
#include "udp_output.h"
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

void* udpInputThread(void* arg) 
{     
    SharedData* udpInputData = (SharedData*)arg;

    int udpSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(udpInputData->myPort));
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket == -1) {
        perror("Error creating UDP socket");
        return NULL;
    }

    if (bind(udpSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error binding UDP socket");
        close(udpSocket);
        return NULL;
    }

    while (1) {
        
        char buffer[100];
        ssize_t bytesReceived = recvfrom(
            udpSocket, 
            buffer,
            sizeof(buffer),
            0,
            (struct sockaddr*)&clientAddr,
            &clientAddrLen
        );

        if (bytesReceived <= 0) {
            perror("Error receiving UDP input data");
            break;
        }

        if (udpInputData->workToDo == 1) {
            pthread_cond_signal(&udpInputData->condVar2);
            break;
        }

        pthread_mutex_lock(&udpInputData->mutex2);
        List_append(udpInputData->messageList2, buffer);
        pthread_cond_signal(&udpInputData->condVar2);
        pthread_mutex_unlock(&udpInputData->mutex2);  
    }

    close(udpSocket);
    return NULL;
}