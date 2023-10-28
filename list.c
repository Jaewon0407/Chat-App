#include "list.h"
#include <stdio.h>
#include <stdlib.h>

// Initialize the statically allocated list for storing list structs
static List listPool[LIST_MAX_NUM_HEADS];

// Initialize the statically allocated list for storing node structs
static Node nodePool[LIST_MAX_NUM_NODES];

// Initialize the number of lists
static int numLists = 0;

// Initialize the number of nodes
static int numNodes = 0;

// Make a new, empty list and return its reference on success.
// Returns a NULL pointer on failure
List* List_create() {

    // The number of list struct(s) cannot exceed the number of max heads
    if (numLists > LIST_MAX_NUM_HEADS) {
        return NULL;
    }
    
    // Allocate a new list struct from pre-allocated listPool
    List* pList = &listPool[numLists]; 

    // Increase the number of list struct by 1
    numLists++;

    // Initialize list struct
    pList->head = NULL;
    pList->tail = NULL;
    pList->current = NULL;
    pList->numNodes = 0;

    // Return the new list struct
    return pList;

}

int List_count(List* pList) {
    
    // If pList is NULL, return NULL
    if (pList == NULL) {
        return LIST_FAIL;
    }

    // Return the number of nodes in pList
    return pList->numNodes; 
}

void* List_first(List* pList) {

    // If pList is NULL, return NULL
    if (pList == NULL) {
        return NULL;
    }

    // If pList is empty, return NULL
    if (pList->head == NULL) {
        // Set current item to NULL
        pList->current->item = NULL;
        return NULL;
    }

    // Update the status to be in bounds 
    pList->status = 2;

    // Make the first item the current item
    pList->current = pList->head;

    // Return the pointer to the first item
    return pList->current->item;

}

void* List_last(List* pList) {

    // If pList is NULL, return NULL
    if (pList == NULL) {
        return NULL;
    }

    // If pList is empty, return NULL
    if (pList->head == NULL) {
        // Set current item to NULL
        pList->current->item = NULL;
        return NULL;
    }

    // Update the status to be in bounds 
    pList->status = 2;

    // Make the last item the current item
    pList->current = pList->tail;

    // Return the pointer to the last item
    return pList->current->item;
}

void* List_next(List* pList) {
    
    // If the list is empty, return NULL
    if (pList == NULL || pList->head == NULL) {
        return NULL;
    }

    // If the current pointer is beyond the end of the list 
    if (pList->status == LIST_OOB_END) {
        return NULL;
    }

    // If the current pointer is before the start of pList
    if (pList->current == NULL) {
        // Put the current pointer back into the bounds
        pList->current = pList->head;
        // Update the status to be in bounds 
        pList->status = 2;
    } else {
        // Update the status to be in bounds 
        pList->status = 2;
        pList->current = pList->current->next;
    }

    // If the current pointer BECOMES NULL after moving to the next node
    if (pList->current == NULL) {
        pList->status = LIST_OOB_END;
        return NULL;
    }

    // Return the item of the current pointer
    return pList->current->item;
}

void* List_prev(List* pList) {

    // If the list is empty, return NULL
    if (pList == NULL || pList->head == NULL) {
        return NULL;
    }

    // If the current pointer is before the start of the list 
    if (pList->status == LIST_OOB_START) {
        return NULL;
    }

    // If the current pointer is beyond the end of pList
    if (pList->current == NULL) {
        pList->current = pList->tail;
        // Update the status to be in bounds 
        pList->status = 2;
    } else {
        // Current pointer is in bounds 
        pList->status = 2;
        pList->current = pList->current->prev;
    }

    // If the current pointer BECOMES NULL after moving to the previous node
    if (pList->current == NULL) {
        pList->status = LIST_OOB_START;
        return NULL;
    }

    // Return the item of the current pointer
    return pList->current->item;
}

void* List_curr(List* pList) {

    // If the list is empty, return NULL
    if (pList == NULL || pList->head == NULL) {
        return NULL;
    }

    // If the pointer to the current item is NULL
    if (pList->current == NULL) {
        return NULL;
    }

    // Return the pointer to the current item
    return pList->current->item;
}

int List_insert_after(List* pList, void* pItem) {

    // The number of node struct(s) cannot exceed the number of max nodes
    if (numNodes > LIST_MAX_NUM_NODES) {
        return LIST_FAIL;
    }

    // Allocate a node struct from pre-allocated nodePool
    Node* pNode = &nodePool[numNodes];

    // Increase the number of node struct by 1
    numNodes++;
    
    // Initialize node struct
    pNode->item = pItem;
    pNode->next = NULL;
    pNode->prev = NULL; 

    // If the list is empty or null, return null
    if (pList == NULL || pList->head == NULL) {
        return LIST_FAIL;
    }

    // If the current pointer is out of bounds 
    if (pList->current == NULL) {

        // If the current pointer is before the start of pList
        if (pList->status == LIST_OOB_START) {
            // Insert the node struct into the start of pList
            pNode->next = pList->head;
            pList->head->prev = pNode;
            pList->head = pNode;

            // Make item the current item
            pList->current = pNode;

            // Increase the number of node(s) by 1
            pList->numNodes++;
            return LIST_SUCCESS;

        // If the current pointer is beyond the end of pList
        } else if (pList->status == LIST_OOB_END) {
            pNode->prev = pList->tail;
            pList->tail->next = pNode;
            pList->tail = pNode;
            pList->current = pNode;

        
            pList->numNodes++;
            return LIST_SUCCESS;

        }

    } 

    // Update the status to be in bounds 
    pList->status = 2;

    // If the current pointer is at the tail
    if (pList->current->next == NULL) {
        // Insert the node struct at the end of pList
        pNode->prev = pList->tail;
        pList->tail->next = pNode;
        pList->tail = pNode;

        // Make item the current item
        pList->current = pNode;

        // Increase the number of node(s) by 1
        pList->numNodes++;
        return LIST_SUCCESS;
    }
    
    // Insert the node struct after the current item
    pNode->prev = pList->current;
    pNode->next = pList->current->next;
    pList->current->next->prev = pNode;
    pList->current->next = pNode;

    // Make item the current item
    pList->current = pNode;

    // Increase the number of node(s) by 1
    pList->numNodes++;

    return LIST_SUCCESS;

}

int List_insert_before(List* pList, void* pItem) {

    // The number of node struct(s) cannot exceed the number of max nodes
    if (numNodes > LIST_MAX_NUM_NODES) {
        return LIST_FAIL;
    }

    // Allocate a node struct from pre-allocated nodePool
    Node* pNode = &nodePool[numNodes];

    // Increase the number of node struct by 1
    numNodes++;
    
    // Initialize node struct
    pNode->item = pItem;
    pNode->next = NULL;
    pNode->prev = NULL; 

    // If the list is empty or null, return null
    if (pList == NULL || pList->head == NULL) {
        return LIST_FAIL;
    }

    // If the current pointer is out of bounds 
    if (pList->current == NULL) {

        // If the current pointer is before the start of pList
        if (pList->status == LIST_OOB_START) {
            // Insert the node struct into the start of pList
            pNode->next = pList->head;
            pList->head->prev = pNode;
            pList->head = pNode;

            // Make item the current item
            pList->current = pNode;

            // Increase the number of node(s) by 1
            pList->numNodes++;
            return LIST_SUCCESS;

        // If the current pointer is beyond the end of pList
        } else if (pList->status == LIST_OOB_END) {
            // Insert the node struct at the end of pList
            pNode->prev = pList->tail;
            pList->tail->next = pNode;
            pList->tail = pNode;

            // Make item the current item
            pList->current = pNode;

            // Increase the number of node(s) by 1
            pList->numNodes++;
            return LIST_SUCCESS;

        }

    } 

    // Update the status to be in bounds 
    pList->status = 2;

    // If the current pointer is at the head
    if (pList->current->prev == NULL) {
        // Insert the node struct at the start of pList
        pNode->next = pList->head;
        pList->head->prev = pNode;
        pList->head = pNode;

        pList->current = pNode;

        pList->numNodes++;
        return LIST_SUCCESS;
    }

    // Insert the node struct before the current item
    pNode->next = pList->current;
    pNode->prev = pList->current->prev;
    pList->current->prev->next = pNode;
    pList->current->prev = pNode;

    // Make item the current item
    pList->current = pNode;

    // Increase the number of node(s) by 1
    pList->numNodes++;

    return LIST_SUCCESS;

}

int List_append(List* pList, void* pItem) {
    
    // The number of node struct(s) cannot exceed the number of max nodes
    if (numNodes > LIST_MAX_NUM_NODES) {
        return LIST_FAIL;
    }

    // Allocate a node struct from pre-allocated nodePool
    Node* pNode = &nodePool[numNodes];

    // Increase the number of node struct by 1
    numNodes++;
    
    // Initialize node struct
    pNode->item = pItem;
    pNode->next = NULL;
    pNode->prev = NULL;

    // If pList is empty
    if (pList->head == NULL) {
        pList->head = pNode;
        pList->current = pNode;
        pList->tail = pNode;

        // Increase the number of node(s) by 1
        pList->numNodes++;
        return LIST_SUCCESS;
    }

    // Insert the node struct into the end of pList
    pNode->prev = pList->tail;
    pList->tail->next = pNode;
    pList->tail = pNode;

    // Update the status to be in bounds 
    pList->status = 2;

    // Make the new node the current node
    pList->current = pNode;

    // Increase the number of node(s) by 1
    pList->numNodes++;
    return LIST_SUCCESS;

}

int List_prepend(List* pList, void* pItem) {

    // The number of node struct(s) cannot exceed the number of max nodes
    if (numNodes > LIST_MAX_NUM_NODES) {
        return LIST_FAIL;
    }

    // Allocate a node struct from pre-allocated nodePool
    Node* pNode = &nodePool[numNodes];

    // Increase the number of node struct by 1
    numNodes++;
    
    // Initialize node struct
    pNode->item = pItem;
    pNode->next = NULL;
    pNode->prev = NULL;

    // If pList is empty
    if (pList->head == NULL) {
        pList->head = pNode;
        pList->current = pNode;
        pList->tail = pNode;

        // Increase the number of node(s) by 1
        pList->numNodes++;
        return LIST_SUCCESS;
    }

    // Insert the node struct into the start of pList
    pNode->next = pList->head;
    pList->head->prev = pNode;
    pList->head = pNode;

    // Update the status to be in bounds 
    pList->status = 2;

    // Make the new node the current node
    pList->current = pNode;

    // Increase the number of node(s) by 1
    pList->numNodes++;
    return LIST_SUCCESS;

}

void* List_remove(List* pList) {

    if (pList == NULL || pList->head == NULL) {
        return NULL;
    }

    if (pList->current == NULL) {
        return NULL;
    }

    // Set a temp pointing to the current node
    Node* removedItem = List_curr(pList);

    // If the current node is at the head 
    if (pList->current->prev == NULL) {
        pList->head = pList->current->next;
    } else {
        pList->current->prev->next = pList->current->next;
    }

    // If the current node is at the tail
    if (pList->current->next == NULL) {
        pList->tail = pList->current->prev;
    } else {
        pList->current->next->prev = pList->current->prev;
    }

    // Make the next item the current one
    pList->current = pList->current->next;

    // Decrease the number of nodes
    pList->numNodes--;

    // If the list is empty after removal, update status
    if (pList->head == NULL) {
        pList->status = LIST_OOB_END;
    }

    // Return the current item
    return removedItem->item;
}

void* List_trim(List* pList) {

    if (pList == NULL || pList->head == NULL) {
        return NULL;
    }

    // Check if the last node exists 
    if (pList->tail == NULL) {
        return NULL;
    }

    // If one node exists in the list
    if (pList->tail->prev == NULL) {
        pList->head = NULL;
        pList->status = LIST_OOB_END;
    } else {
        pList->tail->prev->next = NULL;
        pList->tail = pList->tail->prev;
        pList->status = 2;
    }

    pList->numNodes--;

    // if the current pointer is pointing to the last node
    if (pList->current == pList->tail) {
        pList->current = NULL;
    }

    return pList->tail->item;
}

void List_concat(List* pList1, List* pList2) {
    
    // If pList1 or pList2 are null, return null
    if (pList1 == NULL || pList2 == NULL) {
        return;
    }

    /* If pList2 is empty, nothing would change */

    // If pList1 is empty and pList2 is not empty
    if (pList1->head == NULL && pList2->head != NULL) {
        // Set pList1's head and tail to pList2's head and tail
        pList1->head = pList2->head;
        pList1->tail = pList2->tail;
    // If pList1 is not empty and pList2 is not empty
    } else if (pList1->head != NULL && pList2->head != NULL) {
        // Add pList2 to the end of pList1
        pList1->tail->next = pList2->head;
        pList2->head->prev = pList1->tail;
        pList1->tail = pList2->tail;
    }

    // Update the number of nodes in pList1
    pList1->numNodes += pList2->numNodes;

    // pList2 no longer exists; its head is available for future operations.
    pList2->numNodes = 0;
    pList2->head = NULL;
    pList2->tail = NULL;
    pList2->current = NULL;

}

void List_free(List* pList, FREE_FN pItemFreeFn) {
    
    if (pList == NULL || pList->head == NULL) {
        return;
    }

    // Start from head 
    Node* currentNode = pList->head;

    while (currentNode != NULL) {

        // Free the item in the node
        if (pItemFreeFn != NULL) {
            pItemFreeFn(currentNode->item);
        }

        // Free the node itself
        free(currentNode);

        // Move to next node
        currentNode = currentNode->next;
    }

    free(pList);
}

void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg) {
    if (pList == NULL || pList->head == NULL|| pComparator == NULL) {
        return NULL;
    }

    Node* currentNode = pList->current;

    // If the current pointer is out of bounds
    if (currentNode == NULL) {
        // If the current pointer is before the start of pList
        if (pList->status == LIST_OOB_START) {
            currentNode = pList->head;
        } 
    }

    while (currentNode != NULL) {
        // 1 - match, 0 - not match
        if (pComparator(currentNode->item, pComparisonArg)) {
            // The current pointer is left at the matched item 
            pList->current = currentNode;
            // update the status of the bounds
            pList->status = 2;
            return pList->current->item;
        } 

        currentNode = currentNode->next;
    }

    // If the match is not found, set the current pointer to be beyond the end of the list
    pList->status = LIST_OOB_END;
    pList->current = NULL;
    return NULL;

}