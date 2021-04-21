#include <stdio.h>
#include <string.h>
#include "structures.h"


//calculation for fcfs
void fcfs_values(int nArray[][6], int nTurnAround[] , int *nTotalTime, int n) {
	int i;
  for(i = 1; i < n; i++) {
    //update start time.
	    if(i==1){
	      nArray[i][3] = nArray[i][1];
	    } else{
	    	if(nArray[i-1][4]>nArray[i][1]){
        nArray[i][3] = nArray[i-1][4];
      }else{
        nArray[i][3] = nArray[i][1];
      	}
	}
    //update end time.
    nArray[i][4] = nArray[i][3] + nArray[i][2];
    //update turnaround time.
    nTurnAround[i] = nArray[i][4] - nArray[i][1];
    // Update waiting time.
    nArray[i][5] = nTurnAround[i] - nArray[i][2];
    //update total waiting time.
    *nTotalTime += nArray[i][5];
  }
}

void fcfs(int nArray[][6], int n) {

  int nTotalTime = 0;
  int nTurnAround[n];

  initialize_zero(nTurnAround, n, 0);

  sort_array(n, 6, nArray, 1);

  fcfs_values(nArray, nTurnAround, &nTotalTime, n);

  // print results
  print(nArray, n, nTotalTime, nTurnAround);
}
