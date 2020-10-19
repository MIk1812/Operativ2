#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mymem.h"

//Global data
memoryNode* head;
void* memoryBlock;

size_t mySize;
strategies myStrategy = NotSet;

void myfreemem(){

    if(memoryBlock != NULL)
        free(memoryBlock);

    if(head != NULL){

        //Traverse data structure and free nodes
        memoryNode* currentNode = head;
        memoryNode* nextNode;

        while(currentNode != NULL){

            nextNode = currentNode -> right;
            free(currentNode);
            currentNode = nextNode;
        }
    }
}

void initmem(strategies strategy, size_t size){

    mySize = size;
    myStrategy = strategy;

    //First, free any previously allocated memory
    myfreemem();

    //Initialize memory block to manage
    memoryBlock = malloc(size);

    //Initialize dll to manage with
    head = (memoryNode*) malloc(1 * sizeof(memoryNode));
    head -> right = NULL;
    head -> left = NULL;
    head -> size = size;
    head -> alloc = 0;
    head -> memoryAddress =  memoryBlock;

}

//Insert node to the left
void* insertNewNode(memoryNode* nodeToSplit, size_t sizeRequested){

    //Create new node to insert
    memoryNode* newMemoryNode = (memoryNode*) malloc(sizeof(memoryNode));

    //Update head if necessary
    if(nodeToSplit == head)
        head = newMemoryNode;

    //Init new node
    newMemoryNode -> right = nodeToSplit;
    newMemoryNode -> left = nodeToSplit -> left;
    newMemoryNode -> size = sizeRequested;
    newMemoryNode -> alloc = 1;
    newMemoryNode -> memoryAddress = nodeToSplit -> memoryAddress;

    //Update to the left of new node
    if(nodeToSplit -> left != NULL)
        nodeToSplit -> left -> right = newMemoryNode;

    //Update to the right of new node
    nodeToSplit -> size = nodeToSplit -> size - sizeRequested;
    nodeToSplit -> memoryAddress = nodeToSplit -> memoryAddress + sizeRequested;
    nodeToSplit -> left = newMemoryNode;

    return newMemoryNode -> memoryAddress;
}

void* mymalloc(size_t sizeRequested){

    assert((int)myStrategy > 0);

    memoryNode* currentNode = head;

    while(currentNode != NULL){

        //If currentNode node is equal to requested size
        if(currentNode -> size == sizeRequested && currentNode -> alloc != 1){

            //Allocate the whole node
            currentNode -> alloc = 1;
            return currentNode -> memoryAddress;
        }

        //If currentNode node is bigger than requested size
        if(currentNode -> size > sizeRequested  && currentNode -> alloc != 1)
            return insertNewNode(currentNode, sizeRequested);

        //Traverse the dll if possible
        currentNode = currentNode -> right;

    }
    return NULL;
}

//Merges right node into left
memoryNode* combineNodes(memoryNode* leftNode, memoryNode* rightNode){

    //Update leftNode
    leftNode -> right = rightNode -> right;
    leftNode -> size = leftNode -> size + rightNode -> size;
    leftNode -> alloc = 0;
    leftNode -> memoryAddress = leftNode -> memoryAddress;

    //Update to the right of rightNode
    if(rightNode -> right != NULL)
        rightNode -> right -> left = leftNode;

    free(rightNode);
    return leftNode;

}

void myfree(void* nodeToFree){

    memoryNode* currentNode = head;

    while(currentNode != NULL){

        //If the corresponding memoryNode was found
        if(currentNode -> memoryAddress == nodeToFree){

            currentNode -> alloc = 0;

            //Check to the left
            if(currentNode -> left != NULL && currentNode -> left -> alloc == 0)
                currentNode = combineNodes(currentNode -> left, currentNode);

            //Check to the right
            if(currentNode -> right != NULL && currentNode -> right -> alloc == 0)
                combineNodes(currentNode, currentNode -> right);

            return;
        }

        //Traverse the dll
        currentNode = currentNode -> right;
    }
}

//How many free blocks are in memory?
int mem_holes(){

    //Traverse the dll
    int counter = 0;
    memoryNode* currentNode = head;

    while(currentNode != NULL){

        if(currentNode -> alloc == 0)
            counter++;

        currentNode = currentNode -> right;
    }
    return counter;
}

//How much memory is currently allocated?
int mem_allocated(){

    //Traverse the dll
    int sum = 0;
    memoryNode* currentNode = head;

    while(currentNode != NULL){

        if(currentNode -> alloc == 1)
            sum = sum + currentNode -> size;

        currentNode = currentNode -> right;
    }
    return sum;
}

//How much memory is NOT allocated?
int mem_free(){

    //Traverse the dll
    int sum = 0;
    memoryNode* currentNode = head;

    while(currentNode != NULL){

        if(currentNode -> alloc == 0)
            sum = sum + currentNode -> size;

        currentNode = currentNode -> right;
    }
    return sum;
}

//How large is the largest free block?
int mem_largest_free(){

    //Traverse the dll
    int max = 0;
    memoryNode* currentNode = head;

    while(currentNode != NULL){

        if(currentNode -> alloc == 0 && currentNode -> size > max)
            max = currentNode -> size;

        currentNode = currentNode -> right;
    }
    return max;
}

//How many small unallocated blocks are currently in memory?
int mem_small_free(int size){

    //Traverse the dll
    int counter = 0;
    memoryNode* currentNode = head;

    while(currentNode != NULL){

        if(currentNode -> alloc == 0 && currentNode -> size <= size)
            counter++;

        currentNode = currentNode -> right;
    }
    return counter;
}

//Is a particular byte allocated or not?
char mem_is_alloc(void* ptr){

    //Traverse the dll
    memoryNode* currentNode = head;

    while(currentNode != NULL){

        if(currentNode -> memoryAddress == ptr)
            if(currentNode -> alloc == 1)
                return 1;
            else
                return 0;

        currentNode = currentNode -> right;
    }
    return 0;
}

void try_mymem(int argc, char **argv) {
    return;
}

// #################################################################### //

/*
 * Feel free to use these functions, but do not modify them.
 * The test code uses them, but you may find them useful.
 */

//Returns a pointer to the memory pool.
void *mem_pool(){
    return memoryBlock;
}

// Returns the total number of bytes in the memory pool. */
int mem_total(){
    return mySize;
}


// Get string name for a strategy.
char *strategy_name(strategies strategy){

    switch (strategy){
        case Best:
            return "best";
        case Worst:
            return "worst";
        case First:
            return "first";
        case Next:
            return "next";
        default:
            return "unknown";
    }
}

// Get strategy from name.
strategies strategyFromString(char* strategy){

    if (!strcmp(strategy,"best"))
        return Best;
    else if (!strcmp(strategy,"worst"))
        return Worst;
    else if (!strcmp(strategy,"first"))
        return First;
    else if (!strcmp(strategy,"next"))
        return Next;
    else
        return 0;
}


