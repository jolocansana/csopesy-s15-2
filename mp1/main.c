#include "text.h"
#include "fcfs.h"
#include "nsjf.h"
#include "psjf.h"
#include "rr.h"

int main () {

  // Read txt file and place in nArray
  char *fileName = "sampleInput.txt";
  FILE *inputFile = fopen (fileName, "rt");

  // CONTAINS THE VALUES IN int
  int nArray[250][4];

  int i,j;

  if(inputFile == NULL) {
    printf("ERROR: %s not found.", fileName);
    return(1);
  }

  parseText(inputFile, nArray);

  fclose(inputFile);

  
  // Checking if read was successful

  // Uncomment to see contents of nArray
  // printf("In nArray:\n");

  // for (i = 0; i <= nArray[0][1]; i++) {
  //   for (j = 0; j < 3; j++){
  //     printf("%d ", nArray[i][j]);
  //   }
  //   printf("\n");
  // }

  
  // Pick scheduling algorithm based on input

  switch (nArray[0][0]) {
    case 0:
      printf("First-Come First-Serve (FCFS)");
      break;
    case 1:
      printf("Non-preemptive Shortest-Job First (NSJF) ");
      break;
    case 2:
      printf("Preemptive Shortest-Job First (PSJF)");
      break;
    case 3:
      printf("Round-Robin (RR)");
      rr(nArray);
      break;
    default:
      printf("ERROR: No such algorithm.");
      return 0;
  }

  return 0;
}