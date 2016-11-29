//  main.c
//  Proj6
//  Created by Tony on 11/28/16.
//  Copyright © 2016 Anthony Cavallo. All rights reserved.

#include "Proj6.h"

int main(int argc, const char * argv[]) {
    srand((unsigned)time(NULL));
    /* Create the processes with unique page requests within each process. */
    int **processArray = createProcessesWithPages(), i;
//    printArray(processArray);
    
    head = createMemoryFrame();
//    pm_t *current = head;
//    for (i = 0; i < 10; i++) {
//        printf("%i ", current->pageNum);
//        current = current->nextPage;
//    }
    
    /* Free the memory allocated for the processes */
    for (i = 0; i < 10; i++)
        free(processArray[i]);
    free(processArray);
    return 0;
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

/* Create a 2D array of "processes" that has "page requests" */
int **createProcessesWithPages() {
    int **array, i, j;
    
    array = (int **)malloc(sizeof(int *) * 10);
    
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

/* Each process can only have 1 of each page request, so this method ensures that there are no duplicate requests within that process */
bool pageAlreadyRequested(int page, int index, int* process) {
    if (index == 0)
        return false;
    int i;
    for (i = 0; i < index; i++)
        if (process[i] == page)
            return true;
    
    return false;
}

pm_t *createMemoryFrame() {
    pm_t *frame = (pm_t *)malloc(sizeof(pm_t)), *current;
    frame->use = notUsed;
    frame->nextPage = NULL;
    frame->pageNum = 69;
    current = frame;
    int i;
    for (i = 0; i < 4; i++) {
        pm_t *temp = (pm_t *)malloc(sizeof(pm_t));
        temp->pageNum = frame->pageNum + i + 1;
        current->nextPage = temp;
        current = current->nextPage;
    }
    current->nextPage = frame;
    return frame;
}
