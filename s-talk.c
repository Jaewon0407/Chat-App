#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "keyboard_input.h"
#include "screen_output.h"
#include "udp_input.h"
#include "udp_output.h"
#include "list.h"
#include "shared_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_MACHINE_NAME_LEN 256
#define MAX_PORT_LEN 6

pthread_mutex_t mutex1;
pthread_cond_t cond1;
pthread_mutex_t mutex2;
pthread_cond_t cond2;

// ./s-talk [My Port #] [Remote Machine] [Remote Port #]
// From Left: ./s-talk 6000 d207-023-213-177.wireless.sfu.ca 6060
// From Right: ./s-talk 6060 d207-023-213-177.wireless.sfu.ca 6000
int main (int argc, char* argv[]) 
{

    char localMachine[256];
    if (gethostname(localMachine, sizeof(localMachine)) != 0) {
        perror("Error, could not retrieve localMachine");
        return -1;
    }

    char* myPort = argv[1];
    char* remoteMachine = argv[2];
    char* remotePort = argv[3];

    List* list1 = List_create();
    List* list2 = List_create();

    pthread_mutex_init(&mutex1, NULL);
    pthread_cond_init(&cond1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_cond_init(&cond2, NULL);

    SharedData sharedData;

    int workToDo = 0;

    sharedData.messageList1 = list1;
    sharedData.messageList2 = list2;
    sharedData.mutex1 = mutex1;
    sharedData.condVar1 = cond1;
    sharedData.mutex2 = mutex2;
    sharedData.condVar2 = cond2;
    sharedData.workToDo = workToDo;

    sharedData.myPort = myPort;
    sharedData.remoteMachine = remoteMachine;
    sharedData.remotePort = remotePort;
    sharedData.localMachine = localMachine;

    pthread_t keyboardInputThreadID;
    pthread_t screenOutputThreadID;
    pthread_t udpInputThreadID;
    pthread_t udpOutputThreadID;

    if (pthread_create(&keyboardInputThreadID, NULL, keyboardInputThread, (void*)&sharedData) != 0) {
        perror("Error creating keyboard input thread");
        return -1;
    }

    if (pthread_create(&udpOutputThreadID, NULL, udpOutputThread, (void*)&sharedData) != 0) {
        perror("Error creating udp output thread");
        return -1;
    }

    if (pthread_create(&udpInputThreadID, NULL, udpInputThread, (void*)&sharedData) != 0) {
        perror("Error creating udp input thread");
        return -1;
    }

    if (pthread_create(&screenOutputThreadID, NULL, screenOutputThread, (void*)&sharedData) != 0) {
        perror("Error creating screen output thread");
        return -1;
    }

    pthread_join(keyboardInputThreadID, NULL);
    pthread_join(udpOutputThreadID, NULL);
    pthread_join(udpInputThreadID, NULL);
    pthread_join(screenOutputThreadID, NULL);

    pthread_mutex_destroy(&mutex1);
    pthread_cond_destroy(&cond1);
    pthread_mutex_destroy(&mutex2);
    pthread_cond_destroy(&cond2);

    return 0;
}