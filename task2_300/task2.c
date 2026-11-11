#include <stdio.h>
#include "task2_numbers_metadata.h"
#include "number_utilities.h"

int main()
{
	FILE *fin;
	FILE *fout;
	struct TASK2_NUMBERS_METADATA data[11];
	int n;
	int i;

	fin = fopen("pgexam26_test.txt", "r");
	if(!fin)
	{
		perror("failed to load input file");
		return 1;
	}

	char line[100]; /*buffer for lines*/
	i = 0;
	while (i < 11 && fgets(line, sizeof(line), fin) != NULL)
	{
			if(sscanf(line, "%d", &n) !=1)
			continue; /*skips invalid lines*/
		data[i].iIndex = i; /* + 1 removed to make the program run propperly;*/
		data[i].iNumber = n;

		data[i].bIsFibonacci = isFibonacci(n);
		data[i].bIsPrimeNumber = isPrime(n);
		data[i].bIsSquareNumber = isSquareNumber(n);
		data[i].bIsCubeNumber = isCubeNumber(n);
		data[i].bIsPerfectNumber = isPerfectNumber(n);
		data[i].bIsAbundantNumber = isAbundantNumber(n);
		data[i].bIsDeficientNumber = isDeficientNumber(n);
		data[i].bIsOddNumber = isOdd(n);

		i++;
	}

	fclose(fin);

	fout = fopen("pgexam26_output.bin" ,"wb");
	if (!fout)
	{
		perror("could not open output file");
		return 1;
	}

	fwrite(data, sizeof(struct TASK2_NUMBERS_METADATA) , i, fout);
	fclose(fout);

	return 0;
	}
