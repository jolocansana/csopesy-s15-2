#include <stdio.h>

int rr(int nArray[][4]) {

  int nQuantum = nArray[0][2];
  int i, j, k;
  int nTime = 0;
  int nTotalTime = 0;
  int nTotalTurnaround = 0;
  int nIndex = 0;
  int nActiveTime = 0;
  int nWaitTime = 0;
  int nTotalWaitTime = 0;
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
    if(nArray[nIndex][2] != 0 && nArray[nIndex][1] <= nTimeElapsed) {
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
        nActiveTime = 0;
        printf("P[%d]\n", j+1);
        for (k = 0; k < processes[j].numOfIterations; k++){
          printf("Start time: %d ", processes[j].activeTimes[k][0]);
          printf("End time: %d\n", processes[j].activeTimes[k][1]);
          nActiveTime += processes[j].activeTimes[k][1] - processes[j].activeTimes[k][0];
          if(k + 1 < processes[j].numOfIterations) 
          {
            nWaitTime += processes[j].activeTimes[k+1][0] - processes[j].activeTimes[k][1];
          }
        }
        nWaitTime += processes[j].activeTimes[0][0]-nArray[j+1][1];
        nTotalWaitTime += nWaitTime;
        printf("Waiting time: %d\n", nWaitTime);
        printf("Turnaround time: %d\n", nActiveTime+nWaitTime);
        printf("************************************\n");
        nWaitTime = 0;
      }
    }
  }

  printf("Average waiting time: %.1f\n", nTotalWaitTime/(float)doneQueue);

}