#ifndef TASK3_H
#define TASK3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
*Constant used by the program.
*TASK_COUNT is number of exam taks
*each task can receive up to MAX_POINTS_PER_TASK points
*max id length is set to 6, so the it will only register- 
*-5 digits and cut off the rest
*/
#define TASK_COUNT 5 /*5 tasks to grade, made it easier with 20 points per task*/
#define MAX_ID_LEN 6 /*only allows 5 numbers + 0 terminator*/
#define MAX_JUSTIFICATION_LEN 256
#define MAX_POINTS_PER_TASK 20 /*max 20 points per task, easier to add up to 100*/

/*
*this struct represent one exam candidate.
*part of the dll(doubly linked list)
*prev points to previous candidate.
*next points to next candidate.
*/

typedef struct ExamSubmission {
char candidateID[MAX_ID_LEN];

int points[TASK_COUNT];
char justification[TASK_COUNT][MAX_JUSTIFICATION_LEN];

int finaleScore;

struct ExamSubmission *prev;
struct ExamSubmission *next;
} ExamSubmission;

/*function prototypes*/
void calculateFinalScore(ExamSubmission *candidate);
char calculateGrade(int score);
int isFullyGraded(ExamSubmission *candidate);

ExamSubmission *findCandidate(ExamSubmission *head, const char *candidateId);

void addCandidate(ExamSubmission **head, const char *candidateId);
void addCandidatesFromFile(ExamSubmission **head, const char *filename);

void assesTask(ExamSubmission *head);

void printAllCandidates(ExamSubmission *head);
void printCandidateByGrade(ExamSubmission *head);
void printUngradedCandidates(ExamSubmission *head);
void printCandidateDetails(ExamSubmission *head);

void freeList(ExamSubmission *head);

#endif
