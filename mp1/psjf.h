#include <stdio.h>

int psjf(int nArray[][4]) {
  int i, j, k;
  int nTime = 0;
  int nTotalTime = 0;
  int nActiveTime = 0;
  int nIndex = 0;
  int nNewIndex = 0;
  int nSmallestBurst = 0;
  int nWaitTime = 0;
  int nTotalWaitTime = 0;
  int nTimeElapsed = 0;
  int doneQueue = 0;
  Process processes[50] = {};

  // calculate for total time expected
  for(i = 1; i <= nArray[0][1]; i++) {
    nTotalTime += nArray[i][2];
  }

  // printf("Total Time: %d\n", nTotalTime);

  nIndex = 0;
  nSmallestBurst = 1000;
  // printf("%d\n", nSmallestBurst);

  while(nTotalTime > 0) {
    // check if smallest burst time and arrived already
    for (i = 1; i <= nArray[0][1]; i++){
      if (nArray[i][2] < nSmallestBurst && nArray[i][2] > 0 && nArray[i][1] <= nTimeElapsed) {
        // printf("%d < %d\n", nArray[i][2], nSmallestBurst);
        // printf("NEW: p[%d] at time %d\n", i, nTimeElapsed);
        nNewIndex = i;
        nSmallestBurst = nArray[i][2];
      }
    }

    // if new
    if (nIndex != nNewIndex){
      if (nArray[nIndex][2] != 0) {
        processes[nIndex-1].activeTimes[processes[nIndex-1].numOfIterations][1] = nTimeElapsed;
        processes[nIndex-1].numOfIterations++;
      }
      printf("in at time %d\n", nTimeElapsed);
      nIndex = nNewIndex;
      processes[nIndex-1].activeTimes[processes[nIndex-1].numOfIterations][0] = nTimeElapsed;
    }

    // drecrease curr burst time
    nArray[nIndex][2]--;
    nSmallestBurst = nArray[nIndex][2];
    // printf("p[%d] time left: %d\n", nIndex, nArray[nIndex][2]);

    // change other variables
    nTimeElapsed++;
    nTotalTime--;
    // printf("STATS: Time Elapsed %d Total Time Left %d\n", nTimeElapsed, nTotalTime);

    if (nArray[nIndex][2] == 0) // if it is not done
    {
      // printf("p[%d] is done at time %d\n", nIndex, nTimeElapsed);
      processes[nIndex-1].doneQueue = doneQueue;
      doneQueue++;
      nSmallestBurst = 1000; // change this to max int

      processes[nIndex-1].activeTimes[processes[nIndex-1].numOfIterations][1] = nTimeElapsed;
      processes[nIndex-1].numOfIterations++;
    }
  }

  // printf("Done Queue: %d\n", doneQueue);

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
        nActiveTime = 0;
      }
    }
  }

  printf("Average waiting time: %.1f\n", nTotalWaitTime/(float)doneQueue);
}