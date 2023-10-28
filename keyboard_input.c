#include <pthread.h>
#include "shared_data.h"
#include "keyboard_input.h"
#include "screen_output.h"
#include "list.h"
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void* keyboardInputThread(void* arg) 
{   
    SharedData* inputData = (SharedData*) arg;

    while (1) {
        
        char buffer[100];   
        ssize_t bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer));
        
        if (bytesRead <= 0 ) {
            perror("Error reading from keyboard");
            return NULL;
        }
  
        buffer[bytesRead] = '\0';

        // Check if the input is empty or contains only whitespace
        if (strspn(buffer, " \t\n\r\f\v") == bytesRead) {
            // Input is empty, ignore and continue the loop
            continue;
        }

        if (strcmp(buffer, "!\n") == 0) {
            inputData->workToDo = 1;
            pthread_cond_signal(&inputData->condVar1);
            exit(0);
        }

        pthread_mutex_lock(&inputData->mutex1);
        List_append(inputData->messageList1, buffer);
        pthread_cond_signal(&inputData->condVar1);
        pthread_mutex_unlock(&inputData->mutex1);

    }

    return NULL;
}