/* This file has been created by EWA, and is part of task 4 on the exam for PG3401 2026*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "task4_prime.h"  /* Include the header where isPrime is declared */

/* Structure for linked list of prime numbers */
struct PGPRIMENUMBER
{
    unsigned int uiPrime;
    struct PGPRIMENUMBER* next;
};

/* Structure to pass parameters to threads */
struct thread_data 
{
    FILE* fInputFile;
    struct PGPRIMENUMBER** pPrimeList;
    pthread_mutex_t* mutex;
};

/* Thread function to process numbers */
void* threadFunction(void* arg) 
{
    struct thread_data* data;
    unsigned int auiNumbers[10] = {0};
    int iNumbersRead = 0;
    int iIndex = 0;
    struct PGPRIMENUMBER* newNode = NULL;

    data = (struct thread_data*)arg;

    while (1) 
	{
        iNumbersRead = 0;
        while (iNumbersRead < 10) {
            if (fscanf(data->fInputFile, "%u", &(auiNumbers[iNumbersRead])) == 1) {
                iNumbersRead++;
            } else {
                break;
            }
        }

        if (iNumbersRead == 0) {
            break;  /* End of file or no more numbers */
        }

        /* Process each number and check if it's prime */
        for (iIndex = 0; iIndex < iNumbersRead; iIndex++) {
            if (isPrime(auiNumbers[iIndex])) {  /* Call the external isPrime function */
                newNode = (struct PGPRIMENUMBER*)malloc(sizeof(struct PGPRIMENUMBER));
                if (newNode != NULL) {
                    newNode->uiPrime = auiNumbers[iIndex];

                    /* Lock the mutex before modifying the shared prime list */
                    pthread_mutex_lock(data->mutex);
                    newNode->next = *(data->pPrimeList);
                    *(data->pPrimeList) = newNode;
                    pthread_mutex_unlock(data->mutex);
                }
            }
        }
    }

    return NULL;
}

/* Main function: accepts file name as command-line argument and starts threads */
int main(int argc, char* argv[]) {
    FILE* fInputFile;
    struct PGPRIMENUMBER* pPrimeList = NULL;
    pthread_mutex_t primeListMutex;
    pthread_t thread1, thread2;
    struct thread_data data1, data2;
    int rc1, rc2;
    struct PGPRIMENUMBER* pPtr;  /* Move this declaration to the top */

    /* Check if the file name is provided as a command-line argument */
    if (argc != 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    /* Open the input file passed from the command line */
    fInputFile = fopen(argv[1], "r");
    if (fInputFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    /* Initialize the mutex for the prime list */
    pthread_mutex_init(&primeListMutex, NULL);

    /* Initialize thread data */
    data1.fInputFile = fInputFile;
    data1.pPrimeList = &pPrimeList;
    data1.mutex = &primeListMutex;

    data2.fInputFile = fInputFile;
    data2.pPrimeList = &pPrimeList;
    data2.mutex = &primeListMutex;

    /* Create two threads */
    rc1 = pthread_create(&thread1, NULL, threadFunction, (void*)&data1);
    if (rc1 != 0) {
        perror("Error creating thread 1");
        fclose(fInputFile);
        return 1;
    }

    rc2 = pthread_create(&thread2, NULL, threadFunction, (void*)&data2);
    if (rc2 != 0) {
        perror("Error creating thread 2");
        pthread_join(thread1, NULL);  /* Wait for thread 1 to finish */
        fclose(fInputFile);
        return 1;
    }

    /* Wait for both threads to finish */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    /* Close the input file after processing */
    fclose(fInputFile);

    /* Print the prime numbers found */
    printf("\r\nPrime numbers found:\r\n");
    pPtr = pPrimeList;  /* Now pPtr is declared at the top */
    while (pPtr != NULL) {
        printf("%d\r\n", pPtr->uiPrime);
        pPtr = pPtr->next;
    }

    /* Free the allocated memory for the linked list */
    while (pPrimeList != NULL) {
        pPtr = pPrimeList;
        pPrimeList = pPrimeList->next;
        free(pPtr);
    }

    /* Destroy the mutex */
    pthread_mutex_destroy(&primeListMutex);

    return 0;
}
