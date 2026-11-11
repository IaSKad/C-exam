#include "task3.h"

/*--------------------Functions-----------------------*/

/*
*Calculate the final score for one candidate
*only graded tasks where points 0 or higher are included
*/

void calculateFinalScore(ExamSubmission *candidate){
int i;
int sum;

sum = 0;

for (i = 0; i < TASK_COUNT; i++) {
        if (candidate->points[i] >= 0) {
                sum = sum + candidate->points[i];
                }
        }
        candidate->finaleScore = sum;
}

/*----------------------------------------------------*/
/*
*code under converts points to grades
*grade is based on percentage of maximum possible sccore
*/

char calculateGrade(int score)
{
        int maxScore;
        double percent;

        maxScore = TASK_COUNT * MAX_POINTS_PER_TASK;
        percent = ((double)score / (double)maxScore) * 100.0;

        if (percent >= 90.0){
        return 'a';
        } else if (percent >=80.0){
        return 'b';
        } else if (percent >=60.0){
        return 'c';
        } else if (percent >=50.0){
        return 'd';
        } else if (percent >=40.0){
        return 'e';
        }

        return 'f';
        }

/*----------------------------------------------------*/

/*
*check if all tasks have been graded
*the taks is considered graded if it has points and justification
*/

int isFullyGraded(ExamSubmission *candidate)
{
	int i;

	for (i= 0; i < TASK_COUNT; i++) {
		if (candidate->points[i] < 0 ||
			strlen(candidate->justification[i]) == 0) {
			return 0;
		}
	}
        return 1;
}

/*----------------------------------------------------*/
/*
* search in the list for candidate id
* returns a pointer to the candidate found
*returns NULL if not found
*/

ExamSubmission *findCandidate(ExamSubmission *head, const char *candidateId)
{
	ExamSubmission *current;
	current = head;

	while (current != NULL) {
		if (strcmp(current->candidateID, candidateId) == 0) {
			return current;
		}

		current = current->next;
		}
	return NULL;
}

/*----------------------------------------------------*/
/*
*add a candidate to the end off dll
*all tasks start as not graded, marked by -1
*/

void addCandidate(ExamSubmission **head, const char *candidateId)
{
	ExamSubmission *newCandidate;
	ExamSubmission *current;
	int i;

	if (findCandidate(*head, candidateId) != NULL) {
		printf("candidate %s already exists\n", candidateId);
		return;
		}

	newCandidate = (ExamSubmission *)malloc(sizeof(ExamSubmission));

	if (newCandidate == NULL) {
		printf("memory allocation falied\n");
		return;
	}

/*strncpy is somewhat problematic as it might not null terminate the string
*and it pads with 0 so its not ver efficient. 
*However, for this small program with limited input I find it okay to use
*/
	strncpy(newCandidate->candidateID, candidateId, MAX_ID_LEN - 1);
	newCandidate->candidateID[MAX_ID_LEN - 1] = '\0';

	for (i=0; i < TASK_COUNT; i++) {
		newCandidate->points[i] = -1;
		newCandidate->justification[i][0] = '\0';
	}

	newCandidate->finaleScore = 0;
	newCandidate->prev = NULL;
	newCandidate->next = NULL;

        if (*head == NULL) {
                *head = newCandidate;
        }else {
                current = *head;

                while (current->next != NULL) {
                        current = current->next;
                }

                current->next = newCandidate;
                newCandidate->prev = current;
                }
        printf("candidate %s added\n", candidateId);

        }

/*----------------------------------------------------*/
/*
*reads candidate from file
*the file contains one candidate id pr line
*/

void addCandidatesFromFile(ExamSubmission **head, const char *filename)
{
	FILE *file;
	char candidateId[MAX_ID_LEN];

	file = fopen(filename, "r");

        if (file == NULL) {
		printf("could not open file: %s\n", filename);
		return;
	}

	while (fgets(candidateId, MAX_ID_LEN, file) != NULL){
		/*removes trailing newline if it exists*/
		candidateId[strcspn(candidateId, "\n")] = '\0';

		if (candidateId[0] != '\0'){
			/*ignore empty lines*/
			addCandidate(head, candidateId);
		}
	}

        fclose(file);
}

/*----------------------------------------------------*/
/*
* add justification and score for one task
*the user can here select candidate, task number, points, and write justification
*/

void assesTask(ExamSubmission *head)
{
	char candidateID[MAX_ID_LEN];
	int taskNumber;
	int points;
	char justification[MAX_JUSTIFICATION_LEN];
	ExamSubmission *candidate;

	printf("Candidate id: ");
	scanf("%31s", candidateID);

        candidate = findCandidate(head, candidateID);

        if (candidate == NULL) {
		printf("not found\n");
        	return;
        }

	printf("task number (1-%d): ", TASK_COUNT);
        scanf("%d", &taskNumber);

        if(taskNumber < 1 || taskNumber > TASK_COUNT) {
        	printf("invalid tasknumber\n");
                return;
        }

        printf("points (0-%d): ", MAX_POINTS_PER_TASK);
        scanf("%d", &points);

        if (points < 0 || points > MAX_POINTS_PER_TASK) {
        	printf("invalid points, must be 0 or more and less than max points(20)\n");
        	return;
	}

/*
*getchar() removes the new line in the input buffer after scanf
*it is lowlevel to use and prefferably fgets should be used. However, I feel like it can 
*safely be used in this instance without "bugs of skipping lines"
*/
        getchar();

        printf("justification: ");
        fgets(justification, MAX_JUSTIFICATION_LEN, stdin);

        justification[strcspn(justification, "\n")] = '\0';

        candidate->points[taskNumber - 1] = points;

        strncpy(candidate->justification[taskNumber - 1],
        justification,
        MAX_JUSTIFICATION_LEN -1);

        candidate->justification[taskNumber - 1][MAX_JUSTIFICATION_LEN - 1] = '\0';

        calculateFinalScore(candidate);

        printf("assessment saved");
}

/*----------------------------------------------------------------------------------------------*/

/*
*print all candidates with final score and grades
*/

void printAllCandidates(ExamSubmission *head)
{
	ExamSubmission *current;

        current = head;

        printf("\nall candidates:\n");


        while (current != NULL) {
                printf("candidates: %s | Score %d | grade: %c\n",
                current->candidateID,
                current->finaleScore,
                calculateGrade(current->finaleScore));

        current = current->next;
                }
}

/*---------------------------------------------------------------------------------------------*/
/*
* print all candidates with a given grade
*/

void printCandidateByGrade(ExamSubmission *head)
{
	ExamSubmission *current;
        char grade;

        printf("enter a grade A-F: ");
        scanf(" %c", &grade);

        current = head;

        printf("\ncandidates with grades %c:\n", grade);

        while (current != NULL) {
		if(calculateGrade(current->finaleScore) == grade){
                                printf("candidate: %s | score: %d\n",
                                current->candidateID,
                                current->finaleScore);
                }
                        current = current->next;
	}
}


/*---------------------------------------------------------------------------------------------*/
/*
*cthis prints candidates that are not fully graded
*/

void printUngradedCandidates(ExamSubmission *head)
{
	ExamSubmission *current;

        current = head;

        printf("\nnot fully graded candidates\n");

        while (current != NULL) {
        	if (!isFullyGraded(current)) {
                	printf("candidate: %s\n", current->candidateID);
                	}

                        current = current->next;
        	}
        }

/*---------------------------------------------------------------------------------------------*/
/*
* detailed report for one candidate
* show score, grade, points, and justification for every task
*/

void printCandidateDetails(ExamSubmission *head)
{
	char candidateID[MAX_ID_LEN];
        ExamSubmission *candidate;
        int i;

        printf("candidate Id: ");
        scanf("%31s", candidateID);

        candidate = findCandidate(head, candidateID);

        if (candidate == NULL) {
        	printf("candidate not found\n");
                return;
                }

	printf("\ncandidate: %s\n", candidate->candidateID);
        printf("\nfinale score: %d\n", candidate->finaleScore);
        printf("\ngrade: %c\n", calculateGrade(candidate->finaleScore));

        for (i=0; i < TASK_COUNT; i++) {
        	printf("\ntask %d:\n", i+1);

        if (candidate->points[i]<0) {
        	printf("points: not graded\n");
        }else{
        	printf("points: %d\n", candidate->points[i]);
		}


	if (strlen(candidate->justification[i])==0) {
		printf("justification: not given\n");
        }else{
        	printf("justification: %s\n", candidate->justification[i]);
        	}
	}
}
/*---------------------------------------------------------------------------------------------*/
/*
*free all memory allocated
*gets called before program exit
*/

void freeList(ExamSubmission *head)
{
	ExamSubmission *current;
        ExamSubmission *next;

        current = head;

        while (current != NULL) {
        	next = current->next;
                free(current);
                current = next;
        }
}

