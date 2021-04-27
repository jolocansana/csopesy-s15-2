// ***************************************************************
// Names:  CANSANA, Jose Lorenzo      JAMALUL, Gabriel Rasheed
// Group:   Group 2
// Section:  S15
// ***************************************************************


#include <stdio.h>

int rr(int nArray[][6]) {

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
  int Queue[10] = {};
  int nQueueSize = 0;
  int currProcess[6] = {};
  int foundFirst = 0;

  // calculate for total time expected
  for(i = 1; i <= nArray[0][1]; i++) {
    nTotalTime += nArray[i][2];
  }

  // printf("%d", nTotalTime);

  nIndex = 0;
  i = 1;
  while(!foundFirst && i <= nArray[0][1]) {
    // printf("%d\n", i);
    if (nArray[i][1] == 0){
        Queue[nIndex] = i;
        foundFirst = 1;
    }
    i++;
  }
  // printf("%d\n", Queue[0]);
  
  // printf("%d", Queue[nIndex]);

  while(nTotalTime > 1) {
    nTime = nQuantum;

    // X TODO: Change all calls of nArray[nIndex] to the Array assigned above
    if(nArray[Queue[nIndex]][2] != 0 && nArray[Queue[nIndex]][1] <= nTimeElapsed) {
      // printf("P[%d] Start at %d\n", Queue[nIndex], nTimeElapsed);
      
      // add start time of this iteration
      processes[Queue[nIndex]-1].activeTimes[processes[Queue[nIndex]-1].numOfIterations][0] = nTimeElapsed;

      while(nTime > 0 && nArray[Queue[nIndex]][2] > 0) {
        // X TODO: Insert process to queue on arrival
        for (i = 1; i <= nArray[0][1]; i++)
        {
          if (nArray[i][1] == nTimeElapsed)
          {
            Queue[nQueueSize] = i;
            nQueueSize++;
          }
        }
        nArray[Queue[nIndex]][2]--;
        nTimeElapsed++;
        nTime--;
        nTotalTime--;
      }
      // add end time of this iteration
      processes[Queue[nIndex]-1].activeTimes[processes[Queue[nIndex]-1].numOfIterations][1] = nTimeElapsed;
      processes[Queue[nIndex]-1].numOfIterations++;
    }

    if (nArray[Queue[nIndex]][2] == 0) { // completed task; add doneQueue
          processes[Queue[nIndex]-1].doneQueue = doneQueue;
          // printf("SADSD %d\n", Queue[nIndex]-1);
          doneQueue++;
          // printf("P[%d] Complete at time %d\n", nIndex, nTimeElapsed);
    } else {
      // X TODO: Add this process to queue
      Queue[nQueueSize] = Queue[nIndex];
      nQueueSize++;
    }

    // for(i = 0; i < nQueueSize; i++){
    //   printf("%d", Queue[i]);
    // }
    // printf("\n");

    nIndex++;
  }

  // printf("DONE QUEUE: %d\n", doneQueue);

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