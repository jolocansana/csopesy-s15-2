#include <stdio.h>

int main () {

  // 1. User inputs the text file name as string
  char* filename;

  printf("Please enter a valid file name: ");
  scanf("%s", filename);

  // 1.1 Place items from text file to array
  

  // 2. Sorting
  // 2.1 Sort processes based on arrival time

  // 2.2 Sort queues by priority



  return 0;
}

// Used for 2. Sorting
void sort_array(int row, int column, int array[][6], int index) {
	int i,j,k,temp;
  for(i = 1; i <= row; i++){
    for( j = 1; j <= row - i - 1; j++){
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


