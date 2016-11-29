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
//#include <getopt.h>         /* For command line options */
//#include <string.h>         /* atoi(string) */
//#include <ctype.h>          /* For isprint in getopt */
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
    struct page_memory *nextPage;
}pm_t;
pm_t *head;
int currentPointer;
enum {used = 1, notUsed = 0};

/* Function prototypes for OSS */
int **createProcessesWithPages();
void printArray(int **);
bool pageAlreadyRequested(int, int, int *);
pm_t *createMemoryFrame();

#endif /* Proj6_h */
