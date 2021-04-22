typedef struct Process
{
	int doneQueue, waitTime, numOfIterations;
  int activeTimes[MAX][2]; // 0 - start; 1 - end
} Process;