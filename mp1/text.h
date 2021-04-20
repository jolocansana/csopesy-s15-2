#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

  char sSampleStr[250] = "0 12 2\n1 1 1\n1 0 0\n1 0 1\n1 1 1\n1 0 0\n1 0 1\n1 1 1\n1 0 0\n1 0 1\n1 1 1\n1 0 0\n1 0 1";
  char* token;
  char* line;
  char *token_r, *line_r;
  char sDelim[3] = "\n";
  int nArray[50][3];
  int i, j;
  int nAlgorithm, nSize, nQuantum;

  // get first row
  int firstRow[3];

  // put points in nArray
  token = strtok_r(sSampleStr, sDelim, &token_r);
  i = 0;

  while(token){
    line = strtok_r(token, " ", &line_r);
    for (j = 0; j < 3; j++) {
      nArray[i][j] = atoi(line);
      line = strtok_r(NULL, " ", &line_r);
    }
    token = strtok_r(NULL, sDelim, &token_r);
    i++;
    // printf("i: %d\n", i);
  }

  // printf("Contents of array: \n");

  for(i = 0; i <= nArray[0][1]; i++) {
    for (j = 0; j < 3; j++) {
      printf("%d ", nArray[i][j]);
    }
    printf("\n");
  }

  return 0;
}