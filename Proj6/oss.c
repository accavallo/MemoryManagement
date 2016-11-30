//  main.c
//  Proj6
//  Created by Tony on 11/28/16.
//  Copyright © 2016 Anthony Cavallo. All rights reserved.

#include "Proj6.h"

int main(int argc, const char * argv[]) {
    srand((unsigned)time(NULL));
    /* Create the processes with unique page requests within each process. */
    
    int **processArray = createProcessesWithPages(), *requests, i, j, option, faultCount = 0, successCount = 0, algorithm = 1, index = 0;
    printArray(processArray);
    requests = (int *)malloc(sizeof(int) * 150);
    
    while ((option = getopt(argc, (char **)argv, "hflp:")) != -1) {
        switch (option) {
            case 'h':
                printf("Display help\n");
                printf("-h displays this help menu.\n");
                printf("-f sets the page replacement algorithm to first in, first out. Default is set to clock replacement.\n");
                printf("-l sets the page replacement algorithm to least recently used. Default is set to clock replacement.\n");
                printf("-p pages sets the total number of pages within the frame. Default is set to 5.\n");
                break;
            case 'f':
                algorithm = 2;
                break;
            case 'l':
                algorithm = 3;
                break;
            case 'p':
                if (isdigit(atoi(optarg))) {
                    frameSize = atoi(optarg);
                }
            default:
                break;
        }
    }
    for (i = optind; i < argc; i++) {
        printf("Non-option argument \"%s\"\n", argv[i]);
    }
    printf("Page replacement algorithm set to ");
    if (algorithm == 1) {
        printf("clock.\n");
    } else if (algorithm == 2) {
        printf("first in, first out.\n");
    } else {
        printf("least recently used.\n");
    }
    sleep(2);
    
    head = createClockMemoryFrame();
    pm_t *current = head;
    /*
    printf("Pages in current frame:\n");
    for (i = 0; i < totalPages; i++) {
        printf("%i ", current->pageNum);
        current = current->nextPage;
    }
    printf("\n");
    */
    
    switch (algorithm) {
        case 1:
            /* This will probably need to change to accommodate the 15 page requests chosen at random per process */
            for (i = 0; i < 10; i++) {
                for (j = 0; j < 15; j++) {
                    int pageRequested = rand() % 8;
                    if (clockFaultOccurred(processArray[i][pageRequested], current)) {
                        current = replaceClockPage(current, processArray[i][pageRequested]);
                        faultCount++;
                    } else {
                        successCount++;
                    }
                    requests[index] = processArray[i][pageRequested];
                    index++;
                }
            }
            break;
        case 2:
            printf("First in, first out not set up yet.\n");
            break;
        case 3:
            printf("Least recently used not set up yet.\n");
            break;
        default:
            break;
    }
    
    printf("Faults:  %2i\n", faultCount);
    printf("Success: %2i\n", successCount);
    
    /*
    printf("Pages in current frame after:\n");
    for (i = 0; i < totalPages; i++) {
        printf("%i ", current->pageNum);
        current = current->nextPage;
    }
    printf("\n");
    */
    
    getOptimalSolution(processArray, requests);
    
    freeMemory(processArray);
    return 0;
}

/* Method to create the initial memory frame with no pages loaded. */
pm_t *createClockMemoryFrame() {
    pm_t *frame = (pm_t *)malloc(sizeof(pm_t)), *current;
    frame->use = notUsed;
    frame->nextPage = NULL;
    frame->pageNum = 0;
    current = frame;
    int i;
    for (i = 0; i < frameSize - 1; i++) {
        pm_t *temp = (pm_t *)malloc(sizeof(pm_t));
        temp->pageNum = 0;
        current->nextPage = temp;
        current = current->nextPage;
    }
    current->nextPage = frame;
    return frame;
}

/* Create a 2D array of "processes" that has "page requests". */
int **createProcessesWithPages() {
    int **array, i, j;
    
    array = (int **)malloc(sizeof(int *) * 10);
    /******NOTE******
     * These two numbers MUST remain at 10 and 8 respectively.
     ****************/
    for (i = 0; i < 10; i++) {
        array[i] = (int *)malloc(sizeof(int) * 8);
        for (j = 0; j < 8; j++) {
            int page;
            do {
                page = 1 + rand() % 50;
            } while (pageAlreadyRequested(page, j, array[i]));
            array[i][j] = page;
        }
    }
    return array;
}

/* Each process can only have 1 of each page request, so this method ensures that there are no duplicate requests within that process. */
bool pageAlreadyRequested(int page, int index, int* process) {
    if (index == 0)
        return false;
    int i;
    for (i = 0; i < index; i++)
        if (process[i] == page)
            return true;
    
    return false;
}

/* Method to print the processArray when I need it to */
void printArray(int **array) {
    int i, j;
    for (i = 0; i < 10; i++) {
        printf("Process %2i memory pages: ", i+1);
        for (j = 0; j < 8; j++) {
            printf("%2i ", array[i][j]);
        }
        printf("\n");
    }
}

/* Determine whether a page fault has occurred based on the page the current process is requesting and what is currently in memory. */
bool clockFaultOccurred(int page, pm_t *frame) {
//    int count = 0;
    pm_t *start = frame;
    while (1) {
        if (page == frame->pageNum) {
            frame->use = used;
            return false;
        }
        
        frame = frame->nextPage;
        if (frame == start) {
            break;
        }
    }
    return true;
}

/* Page fault occurred for the clock algorithm, now a page needs to be replaced within the frame. */
pm_t *replaceClockPage(pm_t *frame, int pageNeeded) {
    /* Go through the frame until we find a page that hasn't been used in a bit. */
    while (frame->use != notUsed) {
        frame->use = notUsed;
        frame = frame->nextPage;
    }
    /* Replace the current page with the page that is needed, set the use bit to 1 and advance to the next page in the frame */
    frame->pageNum = pageNeeded;
    frame->use = used;
    frame = frame->nextPage;
    
    return frame;
}

/* Find and show the optimal page replacement solution. */
void getOptimalSolution(int **processes, int *requests) {
    printf("The optimal solution.\n");
    sleep(2);
    int i, j, successCount = 0, faultCount = 0, totals[50] = {0}, frame[5] = {0}, maxDistance = 0, currentDistance, maxLocation, allPages[80], index = 0;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 8; j++) {
            allPages[index] = processes[i][j];
//            printf("%i ", processes[i][j]);
            index++;
//            totals[processes[i][j]-1]++;
        }
    }
//    printf("\n");
//    for (i = 0; i < 80; i++) {
//        printf("%i ", allPages[i]);
//    }
//    printf("\n");
    index = 0;
    //TODO: Calculate the distance between every element that is currently in the frame and its next occurrence. Then replace the element with the greatest distance between it and it's next occurrence.
    for (i = 0; i < 80; i++) {
//        index = 0;
        for (j = 0; j < 5; j++) {
            if (allPages[i] == frame[j]) {
                successCount++;
            } else {
                index = i;
                while (index < 150) {
                    
                    
                    index++;
                }
            }

        }
    }
    
    printf("Faults:  %2i\n", faultCount);
    printf("Success: %2i\n", successCount);
}

/* Free up the memory that's allocated. */
void freeMemory(int **array) {
    int i;
    /* Free the memory allocated for the processes */
    for (i = 0; i < 10; i++)
        free(array[i]);
    free(array);
    /* Free the memory allocated for the frame */
    pm_t *deleteNode = head;
    i = 0;
    while (i < frameSize) {
//        printf("deleteNode->pageNum: %i\n", deleteNode->pageNum);
        pm_t *pointer = deleteNode->nextPage;
        free(deleteNode);
        deleteNode = pointer;
        i++;
    }
    head = NULL;
    deleteNode = NULL;
}
