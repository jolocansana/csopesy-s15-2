#ifndef structures_h
#define structures_h
#define MAX 250


typedef struct Process
{
	int doneQueue, waitTime, numOfIterations;
  int activeTimes[MAX][2]; // 0 - start; 1 - end
} Process;

//initializes array to 0
void initialize_zero(int array[], int size, int value) {
	int i;
  for(i = 1; i < size; i++){
    	array[i] = value;
	}
}

//sort the processes by its arrival time 
void sort_array(int row, int column, int array[][6], int index) {
	int i,j,k,temp;
  for(i = 1; i < row; i++){
    for( j = 1; j < row - i - 1; j++){
      if(array[j][index] > array[j + 1][index]) {
        for( k = 0; k < column; k++) {
           temp = array[j][k];
          array[j][k] = array[j + 1][k];
          array[j + 1][k] = temp;
        }
      }
    }
  }
}

//print results
void print(int nArray[][6], int n, int nTotalTime, int nTurnAround[]) {
	int i;
  for(i = 1; i < n; i++) {
    printf("[%d] Start time: %d End time: %d | Waiting time: %d\n", nArray[i][0], nArray[i][3], nArray[i][4], nArray[i][5]);
    printf("Turn Around Time: %d \n", nTurnAround[i]);
    printf("************************************\n");
  }

  printf("Average waiting time: %.1f", (float)nTotalTime / ((float)n-1) );
  printf("\n");
}
#endif
