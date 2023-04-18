#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"

void printMatrix(int** matrix, int NPROC, int NRES){
  printf("\n");
  for (int i = 0; i < NPROC; i++) {
		for (int j = 0; j < NRES; j++){ 
			printf("%d ", matrix[i][j]);
		}
    printf("\n");
	}

}

int main(int argc, char *argv[])
{
  // TODO: attempt to open scenario file and scan data into allocated structures

  FILE *file; 
  if (argv[1] == NULL){ //if file is not given, print an error message
    printf("Error: file not given.\n");
    return 0;
  }
  else{
    file = fopen(argv[1], "r"); //open the file for reading
    
    if (file == NULL){ //if file was not found, print error message
      printf("Error: file not found.\n");
      return 0;
    }
  }

  //get the number of resource types and processes
  int NRES; //number of resource types
  int NPROC; //number of processes
  fscanf(file, "%d", &NRES);
  fscanf(file, "%d", &NPROC);

  //TEST
  printf("Resource: %d\n", NRES);
  printf("Process: %d\n", NPROC);

  //make space for the total resource vector
  int* totalResVector = (int*) malloc(sizeof(int) * NRES);
  for (int i = 0; i < NRES; i++){ //fill the totalResVector array
    int res;
    fscanf(file, "%d", &res);
    totalResVector[i] = res;
  }

  //TEST
  printf("Total resource vector: ");
  for (int i = 0; i < NRES; i++){
    printf("%d ", totalResVector[i]);
  }

  //get space for the max demand matrix
	int** maxDemand = (int**) malloc(sizeof(int*) * NPROC);
	for (int i = 0; i < NPROC; i++){
		maxDemand[i] = (int*) malloc(sizeof(int) * NRES);
	}

  //fill max demand matrix with values from file
  for (int i = 0; i < NPROC; i++) {
    int maxRes;
		for (int j = 0; j < NRES; j++){ 
      fscanf(file, "%d", &maxRes);
			maxDemand[i][j] = maxRes;
		}
  }

  //TEST
  printf("\nMax demand matrix:");
  printMatrix(maxDemand, NPROC, NRES);

  //get space for the allocation matrix
	int** alloc = (int**) malloc(sizeof(int*) * NPROC);
	for (int i = 0; i < NPROC; i++){
		alloc[i] = (int*) malloc(sizeof(int) * NRES);
	}

  //fill allocation matrix with values from file
  for (int i = 0; i < NPROC; i++) {
    int allocRes;
		for (int j = 0; j < NRES; j++){ 
      fscanf(file, "%d", &allocRes);
			alloc[i][j] = allocRes;
		}
  }

  //TEST
  printf("\nAllocation matrix:");
  printMatrix(alloc, NPROC, NRES);

  fclose(file); //close the file 

  //free up everything:
  
  for (int i = 0; i < NPROC; i++) { //free each row in maxDemand
    free(maxDemand[i]);
    maxDemand[i] = NULL;  //remove dangling pointer
	}
  free(maxDemand);
  maxDemand = NULL;

  for (int i = 0; i < NPROC; i++) { //free each row in alloc
    free(alloc[i]);
    alloc[i] = NULL;  //remove dangling pointer
	}
  free(alloc);
  alloc = NULL;

  free(totalResVector); //free totalResVector array
  totalResVector = NULL; //no dangling pointers
  
  //TODO: Run banker's safety algorithm

  return 0;
}