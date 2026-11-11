#include <stdio.h>
#include <stdlib.h>
#include "task3.h"
/*
*main menue
*/

int main(void)
{
	ExamSubmission *head;
        int choice;
        char candidateId[MAX_ID_LEN];
        char filename[128];

        head = NULL;
        choice =-1;

        while(choice !=0) {
                printf("\n====exam grading system====\n");
                printf("1: add a candidate manually\n");
                printf("2: add candidates from file\n");
                printf("3: asses one task\n");
                printf("4: print all candidates\n");
                printf("5: print candidates by grade\n");
                printf("6: print candiates not fullt graded\n");
                printf("7: print detailed candidate report\n");
                printf("0: exit\n");
                printf("choice: ");

                scanf("%d", &choice);

/*running the diffrent functions depending on users input choice*/
        if (choice == 1) {
                printf("candidate id: ");
                scanf("%31s", candidateId);
                addCandidate(&head, candidateId);
        } else if (choice == 2) {
                printf("filename: ");
                scanf("%127s", filename);
                addCandidatesFromFile(&head, filename);
        } else if (choice == 3) {
                assesTask(head);
        } else if (choice == 4) {
                printAllCandidates(head);
        } else if (choice == 5) {
                printCandidateByGrade(head);
        } else if (choice == 6) {
                printUngradedCandidates(head);
        } else if (choice == 7) {
                printCandidateDetails(head);
        } else if (choice == 0) {
                printf("exiting program\n");
        } else {
                printf("invalid choice\n");
                }
        }
/*tells the program to go through my linked list and free every node*/
	freeList(head);
        return 0;
}

