#include <stdio.h>
#include "structures.h"

int rr(int nArray[][4]) {

  int nQuantum = nArray[0][2];
  int i, j, k;
  int nTime = 0;
  int nTotalTime = 0;
  int nTotalTurnaround = 0;
  int nIndex = 0;
  int nTimeElapsed = 0;
  int doneQueue = 0;
  Process processes[50] = {};

  // calculate for total time expected
  for(i = 0; i < nArray[0][1]; i++) {
    nTotalTime += nArray[i][2];
  }

  nIndex = 1;

  while(nTotalTime > 1) {
    nTime = nQuantum;
    if(nArray[nIndex][2] != 0) {
      // printf("P[%d] Start at %d\n", nIndex, nTimeElapsed);
      
      // add start time of this iteration
      processes[nIndex-1].activeTimes[processes[nIndex-1].numOfIterations][0] = nTimeElapsed;

      while(nTime > 0 && nArray[nIndex][2] > 0) {
        nArray[nIndex][2]--;
        nTimeElapsed++;
        nTime--;
        if (nArray[nIndex][2] == 0) { // completed task; add doneQueue
          processes[nIndex-1].doneQueue = doneQueue;
          doneQueue++;
          // printf("P[%d] Complete at time %d\n", nIndex, nTimeElapsed);
        }
        nTotalTime--;
      }
      // add end time of this iteration
      processes[nIndex-1].activeTimes[processes[nIndex-1].numOfIterations][1] = nTimeElapsed;
      processes[nIndex-1].numOfIterations++;
    }

    if (nIndex >= nArray[0][1]) {
      nIndex = 1;
    } else {
      nIndex++;
    }
  }

  for (i = 0; i < doneQueue; i++) {
    for (j = 0; j < doneQueue; j++) {
      if (i == processes[j].doneQueue){
        nTotalTurnaround = 0;
        printf("P[%d]\n", j+1);
        for (k = 0; k < processes[j].numOfIterations; k++){
          printf("Start time: %d End time: %d\n", processes[j].activeTimes[k][0], processes[j].activeTimes[k][1]);
          if(k + 1 < processes[j].numOfIterations) 
          {
            nTotalTurnaround += processes[j].activeTimes[k+1][0] - processes[j].activeTimes[k][1];
          }
        }
        printf("Waiting time: %d\n", processes[j].activeTimes[0][0]-nArray[j+1][1]);
        printf("Turnaround time: %d\n", nTotalTurnaround);
        printf("************************************\n");
      }
    }
  }

}