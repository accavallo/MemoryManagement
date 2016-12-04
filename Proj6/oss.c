//  main.c
//  Proj6
//  Created by Tony on 11/28/16.
//  Copyright © 2016 Anthony Cavallo. All rights reserved.

#include "Proj6.h"

int main(int argc, const char * argv[]) {
    srand((unsigned)time(NULL));
    /* Create the processes with unique page requests within each process. */
    
    int **processArray = createProcessesWithPages(), *requests, i, j, option, faultCount = 0, successCount = 0, algorithm = 1, index = 0;
    requests = (int *)malloc(sizeof(int) * 150);
    
    while ((option = getopt(argc, (char **)argv, "hflp:")) != -1) {
        switch (option) {
            case 'h':
                printf("Display help\n");
                printf("'-h' displays this help menu.\n");
                printf("'-f' sets the page replacement algorithm to first in, first out. Default is set to clock replacement.\n");
                printf("'-l' sets the page replacement algorithm to least recently used. Default is set to clock replacement.\n");
                printf("'-p pages' sets the total number of pages within the frame. Default is set to 5.\n");
                break;
            case 'f':
                algorithm = 2;
                break;
            case 'l':
                algorithm = 3;
                break;
            case 'p':
                if (atoi(optarg)) {
                    frameSize = atoi(optarg);
                } else
                    printf("Expected Integer. Found %s instead.\n", optarg);
                break;
            case '?':
                if (optopt == 'p')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf(stderr, "Unknown option '-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character '%s'.\n", optarg);
                break;
            default:
                break;
        }
    }
    for (i = optind; i < argc; i++) {
        printf("Non-option argument \"%s\"\n", argv[i]);
    }
    printf("Frame size set to %i\n", frameSize);
    printf("Page replacement algorithm set to: ");
    if (algorithm == 1) {
        head = createClockMemoryFrame();
        printf("Clock.\n");
    } else if (algorithm == 2) {
        head = createFIFOAndLRUMemoryFrame();
        printf("First In, First Out.\n");
    } else {
        head = createFIFOAndLRUMemoryFrame();
        printf("Least Recently Used.\n");
    }
    printArray(processArray);
    sleep(2);
    
    pm_t *current = head;
    /* Regardless of which page replacement algorithm is used, the actual page request order won't change. */
    index = 0;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 15; j++) {
            int pageRequested = rand() % 8;
            requests[index] = processArray[i][pageRequested];
            index++;
        }
    }
    
    switch (algorithm) {
        case 1:
            /* This will probably need to change to accommodate the 15 page requests chosen at random per process */
            for (i = 0; i < 150; i++) {
                if (clockFaultOccurred(requests[i], current)) {
                    current = replaceClockPage(current, requests[i]);
                    faultCount++;
                } else
                    successCount++;
            }
            break;
        case 2:
            for (i = 0; i < 150; i++) {
                if (fifoFaultOccurred(requests[i])) {
                    replaceFifoPage(requests[i]);
                    faultCount++;
                } else
                    successCount++;
            }
            break;
        case 3:
            for (i = 0; i < 150; i++) {
                if (lruFaultOccurred(requests[i])) {
                    replaceLruPage(requests[i]);
                    faultCount++;
                } else
                    successCount++;
            }
            break;
        default:
            break;
    }
    
    printf("Faults:  %2i\n", faultCount);
    printf("Success: %2i\n", successCount);
    
    getOptimalSolution(requests);
    
    freeMemory(processArray, requests);
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
void getOptimalSolution(int *requests) {
    printf("\nThe optimal solution.\n");
    sleep(2);
    int i, j, successCount = 0, faultCount = 0, frame[frameSize], maxDistance = 0, currentDistance, maxLocation = 0, index = 0;
    for (i = 0; i < frameSize; i++)
        frame[i] = 0;
    
    printf("Page request order:\n");
    int newLine = 0;
    for (i = 0; i < 150; i++) {
        newLine++;
        printf("%2i ", requests[i]);
        if (newLine == 15) {
            newLine = 0;
            printf("\n");
        }
    }
    printf("\n");
    index = 0;
    /* Go through the entire request list to find the optimal solution. */
    for (i = 0; i < 150; i++) {
        //Check if current page is in the current frame
        if (pageIsInFrame(frame, requests[i])) {
            //Increase the success count
            successCount++;
        } else {
            //Increase the fault count
            faultCount++;
            maxDistance = 0;    //Reset max for the current iteration.
            //Go through each page in the current frame and find the distance to its next call.
            for (j = 0; j < frameSize; j++) {
                index = i;
                currentDistance = 0;
                if (frame[j] == 0) {    //I know the user won't notice a difference, but this will be quicker. Along with instances
                    maxLocation = j;    //where empty pages within the frame would persist while other pages would be replaced.
                    break;
                }
                while (requests[index] != frame[j] && index < 150) {
                    index++;
                    currentDistance++;
                }
                if (currentDistance > maxDistance) {
                    maxDistance = currentDistance;
                    maxLocation = j;
                }
            }
            //Replace the page with the greatest distance with the page requested.
            frame[maxLocation] = requests[i];
        }
    }
    
    printf("Faults:  %2i\n", faultCount);
    printf("Success: %2i\n", successCount);
}

/* FOR THE OPTIMAL SOLUTON ONLY. The other 3 are using a queue, therefore won't be useable for Clock, LRU, or FIFO. */
bool pageIsInFrame(int frame[], int page) {
    int i = 0;
    for (; i < frameSize; i++) {
        if (page == frame[i]) {
            return true;
        }
    }
    
    return false;
}

/* Create the First in, first out frame */
pm_t *createFIFOAndLRUMemoryFrame() {
    pm_t *frame = (pm_t *)malloc(sizeof(pm_t)), *current;
    frame->pageNum = 0;
    frame->use = notUsed;
    frame->lastUseTime = 999999999999999999L;
    frame->nextPage = NULL;
    current = frame;
    int i;
    for (i = 0; i < frameSize - 1; i++) {
        pm_t *temp = (pm_t *)malloc(sizeof(pm_t));
        temp->nextPage = NULL;
        temp->use = notUsed;
        temp->pageNum = 0;
        current->nextPage = temp;
        current = current->nextPage;
    }
    return frame;
}

/* Check if the page is in the current frame for the First In, First Out algorithm. */
bool fifoFaultOccurred(int page) {
    pm_t *current = head;
    while (current) {
        if (current->pageNum == page) {
            return false;
        }
        current = current->nextPage;
    }
    return true;
}

/* A fault occurred for the first in, first out algorithm. Now we need to replace the first page and add one to the end. */
void replaceFifoPage(int page) {
    pm_t *delNode = head, *current = head, *temp;
    head = head->nextPage;
    free(delNode);
    while (current->nextPage) {
        current = current->nextPage;
    }
    temp = (pm_t *)malloc(sizeof(pm_t));
    temp->nextPage = NULL;
    temp->pageNum = page;
    temp->use = notUsed;
    current->nextPage = temp;
}

/* Check if a page fault has occurred with the Least Recently Used algorithm. */
bool lruFaultOccurred(int page) {
    pm_t *current = head;
    int i = 0;
    struct timeval myTime;
    for (; i < frameSize; i++) {
        if (current->pageNum == page) {
            gettimeofday(&myTime, NULL);
            current->lastUseTime = myTime.tv_sec * MILLION + myTime.tv_usec;
            return false;
        }
        current = current->nextPage;
    }
    return true;
}

/* A fault has occurred, so we need to determine which page has been used the least recent and replace it. */
void replaceLruPage(int page) {
    pm_t *ptr = head;
    int i, pageToReplace = 0;
    long greatestTimeDifference = 0, currentTimeDifference = 0;
    struct timeval myTime;
    gettimeofday(&myTime, NULL);
    for (i = 0; i < frameSize; i++) {
        if (ptr->pageNum == 0) {
            pageToReplace = 0;
            break;
        }
        currentTimeDifference = (myTime.tv_sec * MILLION + myTime.tv_usec) - ptr->lastUseTime;
        if (currentTimeDifference > greatestTimeDifference) {
            greatestTimeDifference = currentTimeDifference;
            pageToReplace = ptr->pageNum;
        }
        ptr = ptr->nextPage;
    }
    ptr = head;
    while (ptr) {
        if (ptr->pageNum == pageToReplace) {
            ptr->pageNum = page;
            ptr->lastUseTime = myTime.tv_sec * MILLION + myTime.tv_usec;
//            printf("%lu\n", ptr->lastUseTime);
            break;
        }
        ptr = ptr->nextPage;
    }
}

/* Free up the memory that's allocated. */
void freeMemory(int **processArray, int *requests) {
    int i;
    /* Free the memory allocated for the processes */
    for (i = 0; i < 10; i++)
        free(processArray[i]);
    free(processArray);
    /* Free the memory allocated for the page request array */
    free(requests);
    /* Free the memory allocated for the frame */
    pm_t *deleteNode = head;
    i = 0;
    while (i < frameSize) {
        pm_t *pointer = deleteNode->nextPage;
        free(deleteNode);
        deleteNode = pointer;
        i++;
    }
    head = NULL;
    deleteNode = NULL;
}
