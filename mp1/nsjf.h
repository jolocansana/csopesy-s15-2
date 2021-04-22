
#include <stdio.h>
#include <string.h>
#include "structures.h"

//find minimum burst time
void find_minimum(int nArray[][6], int burst[], int status[], int n, int running_time, int *value, int *minimum) {
	int i;
  for( i = 1; i < n; i++) {
    if(nArray[i][1] <= running_time && status[i] == 0) {
      if(burst[i] < *minimum) {
        *minimum = burst[i];
        *value = i;
      }
      if(burst[i] == *minimum)
      if(nArray[i][1] < nArray[*value][1]) {
        *minimum = burst[i];
        *value = i;
      }
    }
  }
}

//calculation for nsfj
void nsjf_values(int nArray[][6], int running_time, int nTurnAround[] , int *nTotalTime, int status[], int value) {
  nArray[value][3] = running_time;
  nArray[value][4] = nArray[value][3] + nArray[value][2];
  nTurnAround[value] = nArray[value][4] - nArray[value][1];
  nArray[value][5] = nTurnAround[value] - nArray[value][2];
  *nTotalTime += nArray[value][5];
  status[value] = 1;
}


void nsjf(int nArray[][6], int n) {

  int burst[250];
  int i;

  int nTotalTime = 0, running_time = 0, process = 0;
  int nTurnAround[n], status[n];

  initialize_zero(status, n, 0);
  initialize_zero(nTurnAround, n, 0);

  // copy burst time in an array for finding minimum
  for( i = 1; i < n; i++)
    burst[i] = nArray[i][2];

  // NSJF execution
  while(process != n-1) {
    int value = 0;
    int minimum = 10000000;

    find_minimum(nArray, burst, status, n, running_time, &value, &minimum);

    // check if nArray has been found
    if(value != 0) {
      // Calculate the nArray values.
      nsjf_values(nArray, running_time, nTurnAround, &nTotalTime, status, value);

      process++;
      running_time = nArray[value][4];
    }
    // if not found, increment running time
    else
    running_time++;
  }

  // Sort the nArrayes by start time.
  sort_array(n, 6, nArray, 3);

  // Print the result.
 print(nArray, n, nTotalTime, nTurnAround);
}

