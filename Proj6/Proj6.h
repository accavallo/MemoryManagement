//
//  Proj6.h
//  Proj6
//
//  Created by Tony on 11/28/16.
//  Copyright © 2016 Anthony Cavallo. All rights reserved.
//

#ifndef Proj6_h
#define Proj6_h

#include <stdio.h>          /* Basic functions */
#include <stdlib.h>         /* Some other basic functions such as exit and EXIT_FAILURE and srand() */
//#include <unistd.h>         /* For execl */
//#include <signal.h>         /* For signal handling */
#include <getopt.h>         /* For command line options */
//#include <string.h>         /* atoi(string) */
#include <ctype.h>          /* For isprint, isdigit, atoi, etc in getopt */
#include <time.h>           /* srand(unsigned) */
//#include <sys/time.h>       /* POSIX standard, same as time.h */
//#include <sys/shm.h>        /* For shared memory */
//#include <sys/msg.h>        /* For message passing */
//#include <semaphore.h>      /* For semaphore usage */
#include <stdbool.h>        /* For bool type because C doesn't include it automatically. I really hate this language */
//#include <sys/ipc.h>        /* For inter-process communication */
//#include <sys/types.h>      /*  */
//#include <errno.h>          /* To set errno for perror */
//#include <fcntl.h>          /* For O_* constants */
//#include <sys/stat.h>       /* For mode constants */

/* Variables for OSS */
typedef struct page_memory {
    int use;
    int pageNum;
    long long unsigned lastUseTime;
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
/*  */
void getOptimalSolution(int *);
bool pageIsInFrame(int [], int );
/* 3 methods for the FIFO algorithm. One to create the frame, one to check for a fault, and one to replace a page. */
pm_t *createFIFOMemoryFrame();
bool fifoFaultOccurred(pm_t *, int );
void replaceFifoPage(int);

#endif /* Proj6_h */
