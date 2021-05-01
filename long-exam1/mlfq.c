#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 200
#define COLUMNS 6


typedef struct MLFQProcess
{
	int processID, arrivalTime, totalExecutionTime, IOLength, IOBurstCooldown;
  int doneQueue, waitTime, numOfIterations;
  int printingQueue[MAX]; // [FOR PRINTING] -1 for IO, queueID for the rest
  int printingStartTime[MAX];
  int printingEndTime[MAX];
  int priority;
  int remainingTimeQuantum;
  int isIOBurstable;
  int remainingTimeUntilIOBurst;
  int remainingTimeInIO;
} MLFQProcess;

typedef struct queueStruct
{
  int timeQuantum;
  int queueID;
  int queue[MAX];
  int head;
  int tail;
} queueStruct;


// Used for 1.1 Place items for text file to array
// inputFile: File variable for text file
// nArray: dest array
void parseText (FILE *inputFile, int nArray[][COLUMNS])
{
	char cDump;
	int i, j;
  char *token;
  char line[MAX];

	
  i = 0;

	while (fgets (line, MAX, inputFile))
	{
    j = 0;
     //printf ("%s", line);
    token = strtok(line, " ");

		while(token)
		{
      nArray[i][j] = atoi(token);
			token = strtok(NULL, " ");
      j++;
		}
    i++;
  }  
}


// Used for 2. Sorting
// startRow: what row to start sorting
// endRow: what row to stop sorting
// column: number of columns
// array: source array
// index: on what column index to sort
void sort_array(int startRow, int endRow, int column, int array[][COLUMNS], int index) {
	int i,j,k,temp;
  // printf("%d %d\n",startRow, endRow);
  for(i = startRow; i < endRow; i++){
    // printf("OUTER: %d\n",i);
    for(j = startRow; j < endRow - (i-startRow) - 1; j++){
      // printf("INNER: %d\n",j);
      if(array[j][index] > array[j + 1][index]) {
        // Flips all positions in array
        // printf("FLIP!\n");
        for( k = 0; k < column; k++) {
          temp = array[j][k];
          array[j][k] = array[j + 1][k];
          array[j + 1][k] = temp;
        }
      }
    }
  }
}

int main () {

  // Variables
  int nArray[MAX][COLUMNS];
  // COLUMNS:
  // 0 - F
  // 1 - G
  // 2 - H
  // 3 - I
  // 4 - J

  // Variables
  int i,j,k, index;
  int isComplete = 0;
  int timeElapsed;
  int firstRowProcess;
  int arraySize, numOfProcesses, numOfQueues;
  int remainingPrioBoost;
  int currProcessID, isCurrFound, prevCurrProcessID;
  int doneQueue;
  int isBoost;
  int isActive;
  int foundFirst;

  // DEBUGGING
  int isVerbose = 1; // set to 1 if you want to see step by step, if you want results only, set to 0
  
  
  // 1. User inputs the text file name as string
  char filename[100] = "input.txt"; // TODO: Remove for text input

  // printf("Please enter a valid file name: ");
  // scanf("%s", filename);

  // 1.1 Place items from text file to array
  FILE *file = fopen(filename, "rt");
  
  if(file == NULL) {
    printf("ERROR: %s not found.", filename);
    return(1);
  }

  parseText(file, nArray);

  
  
  
  // 2. Sorting
  // 2.1 Sort processes based on arrival time
  sort_array(nArray[0][0]+1, nArray[0][0]+1+nArray[0][1], COLUMNS, nArray, 1);

  // 2.2 Sort queues by priority
  sort_array(1, nArray[0][0]+1, COLUMNS, nArray, 1);

  // Change to 0 to disable printing of nArray
  if(0) {
    for (i = 0; i <= nArray[0][0] + nArray[0][1]; i++) {
      printf("R[%d] ", i);
      for (j = 0; j < 5; j++){
        printf("%d ", nArray[i][j]);
      }
      printf("\n");
    }
  }




  // 3. Setup queueStructures and other variables
  // IO Queue
  int IOQueue[MAX][2] = {};
  for (i = 0; i < nArray[0][1]; i++){
    IOQueue[i][0] = i; // COLUMN 0: process ID
    IOQueue[i][1] = 0; // COLUMN 1: 0 for not in IO, 1 for in IO 
  }

  // RR Queues
  queueStruct queues[MAX] = {}; // Queues will be arranged based on priority and not queueID
  for(i = 1; i < nArray[0][0]+1; i++){
    queues[i-1].queueID = nArray[i][0];
    queues[i-1].timeQuantum = nArray[i][2];
    queues[i-1].head = 0;
    queues[i-1].tail = 0;
  }

  // Processes
  MLFQProcess processes[MAX] = {};

  // Variables
  timeElapsed = 0;
  firstRowProcess = nArray[0][0]+1;
  arraySize =  nArray[0][0]+nArray[0][1]+1;
  remainingPrioBoost = nArray[0][2];
  numOfProcesses = nArray[0][1];
  numOfQueues = nArray[0][0];
  isActive = 0;
  isBoost = 0;
  currProcessID = -1;
  prevCurrProcessID = -1;
  doneQueue = 0;
  foundFirst = 0;


  // 4. Multilevel Feedback Queue
  while(!isComplete && timeElapsed < 50){ // TODO remove timeElapsed here later

    // Check if isBoost is true to perform priority boosting.
    // DO ONLY IF AFTER ACTIVE PROCESS IS DONE AND isBoost is true. Based an queue priority, transfer processes back to highest priority in the same order as in original queue
    if (!isActive && isBoost){
      isBoost = 0;
      if(isVerbose) printf("[[[PRIORITY BOOST]]]\n");
      for (i = 1; i < numOfQueues; i++){ // 1 and not 0 because we are moving everything to highest priority which is 0
        while(queues[i].head != queues[i].tail){
          processes[queues[i].queue[queues[i].head]-1].priority = 0;
          processes[queues[i].queue[queues[i].head]-1].remainingTimeQuantum = queues[0].timeQuantum;


          queues[0].queue[queues[0].tail] = queues[i].queue[queues[i].head];
          queues[i].head++;
          queues[0].tail++;

          if(isVerbose) printf("Tail of Q0: %d\n", queues[0].queue[queues[0].tail-1]);
        }
      }
      if(isVerbose) printf("---------------\n\n");
    }

    // Check for arrivals, if exists, add to highest priority queue and increment tail of queue
    // Arriving Processes are added first before IO returnees.
    for(i = firstRowProcess; i < arraySize; i++){
      if(nArray[i][1] == timeElapsed){
        queues[0].queue[queues[0].tail] = nArray[i][0];
        queues[0].tail++;
        // Setup Active Process:      SAVE PROCESS IN ARRAY BY ITS ID - 1
        index = nArray[i][0] - 1;
        processes[index].numOfIterations = 0;
        processes[index].processID = nArray[i][0];
        processes[index].arrivalTime = nArray[i][1];
        processes[index].totalExecutionTime = nArray[i][2];
        processes[index].IOLength = nArray[i][3];
        processes[index].IOBurstCooldown = nArray[i][4];
        processes[index].priority = 0; // assign priority
        processes[index].remainingTimeQuantum = queues[0].timeQuantum; // assign remaintingTimeQuantum of process (a custom array variable) to the timeQuantum of queue
        if (processes[index].IOBurstCooldown == 0){
          processes[index].isIOBurstable = 0; // Since no cooldown, doesn't IO burst
          processes[index].remainingTimeUntilIOBurst = -1;
        } else {
          processes[index].isIOBurstable = 1;
          processes[index].remainingTimeUntilIOBurst = processes[index].IOBurstCooldown;
        }

        // printf("NEW PROCESS: ID %d at %d index with Quantum %d\n", processes[index].processID, index, processes[index].remainingTimeQuantum);
      }
    }

    // ================
    // ACTUAL RR
    // ================
    // Check queues based on priority if empty; if not, execute processes in that queue for this time unit (this preempts less prioritized processes)
    // Check the heads of each queue based on prioity of queue and assign currProcess to the highest priority head.
    isCurrFound = 0;
    for(i = 0; i < numOfQueues && !isCurrFound; i++){
      if(queues[i].head != queues[i].tail) { // check if queue is not empty
        // printf("BeforeChange: Proccess: %d Prio: %d Time: %d\n", currProcessID, processes[currProcessID-1].priority, processes[currProcessID-1].remainingTimeQuantum);
        if(i < processes[currProcessID-1].priority && isActive) // check if pre-empted by a higher priority job
        {
          queues[processes[currProcessID-1].priority].queue[queues[processes[currProcessID-1].priority].tail] = currProcessID;
          queues[processes[currProcessID-1].priority].tail++;
          queues[processes[currProcessID-1].priority].head++;
          processes[currProcessID-1].remainingTimeQuantum = queues[processes[currProcessID-1].priority].timeQuantum;
          if(isVerbose) printf("BAZINGA, PROCESS GOT PREEMPTED! P[%d] now has TimeQ %d\n", currProcessID, processes[currProcessID-1].remainingTimeQuantum);

          processes[currProcessID-1].printingEndTime[processes[currProcessID-1].numOfIterations] = timeElapsed;
          processes[currProcessID-1].numOfIterations++;
        }

        prevCurrProcessID = currProcessID;
        currProcessID = queues[i].queue[queues[i].head];
        isCurrFound = 1;
        isActive = 1;
        foundFirst = 1;

        if(currProcessID != prevCurrProcessID) {
          // PRINTING VARIABLES
          processes[currProcessID-1].printingQueue[processes[currProcessID-1].numOfIterations] = queues[processes[currProcessID-1].priority].queueID;
          processes[currProcessID-1].printingStartTime[processes[currProcessID-1].numOfIterations] = timeElapsed;
          
          if(isVerbose) printf("NEW PROCESS MATCHED at time %d!\n", processes[currProcessID-1].printingStartTime[processes[currProcessID-1].numOfIterations]);
        }

        if(isVerbose) printf("TEMP CHECK ON START TIME OF PROCESS %d: %d with num %d\n", currProcessID, processes[currProcessID-1].printingStartTime[processes[currProcessID-1].numOfIterations], processes[currProcessID-1].numOfIterations);

      }
    }

    if(isCurrFound){
      // TEST PRINT
      if(isVerbose) printf("P[%d] at %d with prio %d\n", currProcessID, timeElapsed, processes[currProcessID-1].priority);
      if(isVerbose) printf("Q[0] head: %d tail %d\n", queues[0].head, queues[0].tail);
      if(isVerbose) printf("Q[1] head: %d tail %d\n", queues[1].head, queues[1].tail);

      // Decrement H, remainingTimeUntilIOBurst, remainingTimeQuantum of current process.
      processes[currProcessID-1].totalExecutionTime--;
      processes[currProcessID-1].remainingTimeQuantum--;
      if(processes[currProcessID-1].isIOBurstable) processes[currProcessID-1].remainingTimeUntilIOBurst--;

      if(isVerbose) printf("Time Left: %d\n", processes[currProcessID-1].totalExecutionTime);

      // Decrement all processes in IO array
      for(i = 0; i < numOfProcesses; i++) {
        if (IOQueue[i][1] == 1){ // if there is a process in IO
          processes[i].remainingTimeInIO--;

          
          // Check in IO array for any process remainingTimeInIO = 0; if 0, add process to its priorityQueue (a custom array variable) and increment tail of queue
          // NOTE: priorityQueue gets the priority of the queue and NOT its queueID since priority is unique but save queueID in queuea array in structure for printing
          if(processes[i].remainingTimeInIO == 0){
            if(isVerbose) printf("PROCESS %d IS DONE IO BURSTING AT %d!\n", i+1, timeElapsed+1);
            queues[processes[i].priority].queue[queues[processes[i].priority].tail] = processes[i].processID;
            queues[processes[i].priority].tail++;

            // Reset remainingTimeUntilBurst to J (how often IO) of process
            processes[i].remainingTimeUntilIOBurst = processes[i].IOBurstCooldown;

            // IOQueue column 1 back to 0
            IOQueue[i][1] = 0; 

            // PRINTING VARIABLES
            processes[i].printingEndTime[processes[i].numOfIterations] = timeElapsed+1;
            processes[i].numOfIterations++;
          }
        }
      }

      // printf("REMAINING TQ OF P%d: %d\n", currProcessID, processes[currProcessID-1].remainingTimeQuantum);

      // If 1 || 2 || 3, increment head of queue, and [FOR PRINTING] add timeElapse to endTimes of the process. Increment numOfIterations as well for the process.        
      if(processes[currProcessID-1].totalExecutionTime == 0 || processes[currProcessID-1].remainingTimeQuantum == 0 || processes[currProcessID-1].remainingTimeUntilIOBurst == 0) {
        queues[processes[currProcessID-1].priority].head++;
        isActive = 0;

        // PRINTING VARIABLES
        processes[currProcessID-1].printingEndTime[processes[currProcessID-1].numOfIterations] = timeElapsed+1;
        processes[currProcessID-1].numOfIterations++;

        if(isVerbose) printf("PROCESS ITER: %d to %d (numOfIteration: %d)\n", processes[currProcessID-1].printingStartTime[processes[currProcessID-1].numOfIterations-1] , processes[currProcessID-1].printingEndTime[processes[currProcessID-1].numOfIterations-1] ,processes[currProcessID-1].numOfIterations);

        // (1) If H is 0, add doneQueue to doneQueue field of struct and increment doneQueue;
        if (processes[currProcessID-1].totalExecutionTime == 0){
          processes[currProcessID-1].doneQueue = doneQueue; // doneQueue the order of display
          doneQueue++;
          if(isVerbose) printf("PROCESS %d IS DONE AT %d WITH DONE QUEUE %d\n", currProcessID, timeElapsed, doneQueue);
        } 
        else if (processes[currProcessID-1].remainingTimeQuantum == 0 && processes[currProcessID-1].remainingTimeUntilIOBurst == 0) {
          if(processes[currProcessID-1].priority != numOfQueues-1) processes[currProcessID-1].priority++;
          IOQueue[processes[currProcessID-1].processID-1][1] = 1;
          processes[currProcessID-1].remainingTimeInIO = processes[currProcessID-1].IOLength;
          if(isVerbose) printf("PROCESS %d IS IO BURSTING AT %d! IT IS ALSO DEMOTED!\n", currProcessID, timeElapsed+1);

          // PRINTING VARIABLES
          processes[currProcessID-1].printingQueue[processes[currProcessID-1].numOfIterations] = -1;
          processes[currProcessID-1].printingStartTime[processes[currProcessID-1].numOfIterations] = timeElapsed+1;
        }
        // (2) If remainingTimeQuantum is 0, assign priority of process to priority++ and increment tail of new priorityQueue;
        else if (processes[currProcessID-1].remainingTimeQuantum == 0) {
          if(processes[currProcessID-1].priority != numOfQueues-1) processes[currProcessID-1].priority++;
          queues[processes[currProcessID-1].priority].queue[queues[processes[currProcessID-1].priority].tail] = processes[currProcessID-1].processID;
          queues[processes[currProcessID-1].priority].tail++;
          processes[currProcessID-1].remainingTimeQuantum = queues[processes[currProcessID-1].priority].timeQuantum;
          if(isVerbose) printf("PROCESS %d DEMOTED TO PRIORITY: %d with TimeQ %d\n", currProcessID, processes[currProcessID-1].priority, queues[processes[currProcessID-1].priority].timeQuantum);
        } 
        // (3) If remainingTimeUntilBurst is 0, add process to IO queue, add remainingTimeInIO to process
        else {
          IOQueue[processes[currProcessID-1].processID-1][1] = 1;
          processes[currProcessID-1].remainingTimeInIO = processes[currProcessID-1].IOLength;
          if(isVerbose) printf("PROCESS %d IS IO BURSTING AT %d!\n", currProcessID, timeElapsed+1);
          
          // PRINTING VARIABLES
          processes[currProcessID-1].printingQueue[processes[currProcessID-1].numOfIterations] = -1;
          processes[currProcessID-1].printingStartTime[processes[currProcessID-1].numOfIterations] = timeElapsed+1;
        }
      }
    }


    // ==================
    // POST ROUND CLEAN UP
    // ==================
    // Increment timeElapsed
    timeElapsed++;

    // Only start prioboost counter until first encounter
    if (foundFirst) {
      // Decrement remainingPrioBoost;
      remainingPrioBoost--;
      if(isVerbose) printf("TIME TO BOOST: %d\n", remainingPrioBoost);
      
      // Check if remainingPrioBoost is 0; if true, set isBoost true
      if (remainingPrioBoost == 0) {
        isBoost = 1;
        remainingPrioBoost = nArray[0][2];
        if(isVerbose) printf("PRIO BOOST RESET TO: %d\n", remainingPrioBoost);
      }
    }

    // Check nArray processes if all H are 0; if true, isComplete = 1 to end MLFQ;
    if (doneQueue == numOfProcesses){
      isComplete = 1;
      if(isVerbose) printf("-----[[END]]-----\n\n");
    } else {
      if(isVerbose) printf("---------------\n\n");
    }

  }


  // 5. Print Output
  for(i = 0; i < doneQueue; i++){
    for(j = 0; j < numOfProcesses; j++){
      if (i == processes[j].doneQueue){
        printf("P[%d]\n", processes[j].processID);

        // Loop through and display queue, start time, and end time of process
        // format: Q[X]/[IO] Start time: <S1> End time: <E1>
        for (k = 0; k < processes[j].numOfIterations; k++) {
          if (processes[j].printingQueue[k] == -1) {
            printf("[IO] ");
          } else {
            printf("Q[%d] ", processes[j].printingQueue[k]);
          }

          printf("Start time: %d End time: %d\n", processes[j].printingStartTime[k], processes[j].printingEndTime[k]);
        }
        
        // Compute and display waiting time
        // format: Waiting Time: <WT>

        // Computer and display turnaround time
        // format: Turnaround Time: <TT>

        printf("**********************************\n");
      }
    }
  }


  return 0;
}


