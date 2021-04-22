#include "text.h"
#include "structures.h"

#include "fcfs.h"
#include "nsjf.h"
#include "psjf.h"
#include "rr.h"


int main () {

  // Read txt file and place in nArray
  char *fileName = "sampleInput-rr2.txt";
  FILE *inputFile = fopen (fileName, "rt");

  // CONTAINS THE VALUES IN int
  int nArray[250][6];

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
      printf("First-Come First-Serve (FCFS)\n");
      fcfs(nArray, nArray[0][1] + 1);
      break;
    case 1:
      printf("Non-preemptive Shortest-Job First (NSJF) \n");
      nsjf(nArray, nArray[0][1] + 1);
      break;
    case 2:
      printf("Preemptive Shortest-Job First (PSJF)\n");
      psjf(nArray);
      break;
    case 3:
      printf("Round-Robin (RR)\n");
      rr(nArray);
      break;
    default:
      printf("ERROR: No such algorithm.\n");
      return 0;
  }

  return 0;
}
