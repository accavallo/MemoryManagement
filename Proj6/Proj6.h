//  Proj6.h
//  Proj6
//  Created by Tony on 11/28/16.
//  Copyright © 2016 Anthony Cavallo. All rights reserved.

#ifndef Proj6_h
#define Proj6_h

#include <stdio.h>          /* Basic functions */
#include <stdlib.h>         /* Some other basic functions such as exit and EXIT_FAILURE and srand() */
#include <getopt.h>         /* For command line options */
#include <ctype.h>          /* For isprint, isdigit, atoi, etc in getopt */
#include <time.h>           /* srand(unsigned) */
#include <sys/time.h>       /* POSIX standard, same as time.h */
#include <stdbool.h>        /* For bool type because C doesn't include it automatically. I really hate this language */

/* Variables for OSS */
#define MILLION 1000000
typedef struct page_memory {
    int use;
    int pageNum;
    long unsigned lastUseTime;
    struct page_memory *nextPage;
}pm_t;
pm_t *head;
int currentPointer, frameSize = 5;
enum {used = 1, notUsed = 0};

/* Function prototypes for OSS */
/* General use methods that can be used regardless of the page replacement algorithm. */
int **createProcessesWithPages();
bool pageAlreadyRequested(int, int, int *);
void printArray(int **);
void freeMemory(int **, int *);
/* 3 methods for the clock algorithm. One to create the frame, one to check for a fault, and one to do a page replacement. */
pm_t *createClockMemoryFrame();
bool clockFaultOccurred(int, pm_t *);
pm_t *replaceClockPage(pm_t *, int);
/* 2 methods for the optimal solution */
void getOptimalSolution(int *);
bool pageIsInFrame(int [], int );
/* 3 methods for the FIFO algorithm. One to create the frame, one to check for a fault, and one to replace a page. Creating the frame is shared with the Least Recently Used algorithm. */
pm_t *createFIFOAndLRUMemoryFrame();
bool fifoFaultOccurred(int);
void replaceFifoPage(int);
/* 2 methods for the LRU algorithm. The LRU is created the same way as the FIFO, so those two are combined. The other two
   are specifically for the Least Recently Used algorithm. */
bool lruFaultOccurred(int);
void replaceLruPage(int);

#endif /* Proj6_h */
