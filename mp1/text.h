#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000

int parseText (FILE *inputFile, int nArray[][4])
{
	char cDump;
	int i, j;
  char *token;
  char line[MAX];

	
  i = 0;

	while (fgets (line, MAX, inputFile))
	{
    j = 0;
    // printf ("%s", line);
    token = strtok(line, " ");

		while(token)
		{
      nArray[i][j] = atoi(token);
			token = strtok(NULL, " ");
      j++;
		}
    i++;
  }  
  
  return 0;
}