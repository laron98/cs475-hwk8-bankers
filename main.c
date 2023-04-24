#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"


/**
* Global defs
*/
int NPROC; //number of processes 
int NRES; //number of resource types

int main(int argc, char *argv[]){
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
  fscanf(file, "%d", &NRES);
  fscanf(file, "%d", &NPROC);

  //TEST
  //printf("Resource: %d\n", NRES);
  //printf("Process: %d\n", NPROC);

  //make space for the total resource vector
  int* totalResVector = (int*) malloc(sizeof(int) * NRES);
  for (int i = 0; i < NRES; i++){ //fill the totalResVector array
    int res;
    fscanf(file, "%d", &res);
    totalResVector[i] = res;
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

  //sanity checks:
  int passSanity = 1;

  //check that the currently allocated resources do not exceed the total number of resources
  for (int i = 0; i < NRES; i++){
    int sum = 0;
    for (int j = 0; j < NPROC; j++){ 
      sum = sum + alloc[j][i]; //sum up each column in alloc matrix
		}
    
    if(sum > totalResVector[i]){ //check if the total allocs exceed the total number of resources
    passSanity = 0;
      printf("Integrity test failed: allocated resources exceed total resources.\n");
      break;
    }
  }

  //ensure each thread’s needs do not exceed its max demands for each resource type
  int* result;
  for (int i = 0; i < NPROC; i++){
    result = subVectors(maxDemand[i], alloc[i]);
    for (int j = 0; j < NRES; j++){
      if (result[j] < 0){
        passSanity = 0;
        printf("Integrity test failed: allocated resources exceed demand for Thread %d.\n", i);
        printf("Need %d instances of resource %d.\n", result[j], j);
        break;
      }
    }
    free(result);
    result = NULL;
  }

  fclose(file); //close the file 

  //TODO: Run banker's safety algorithm
  if (passSanity == 1){ //if sanity checks are passed, then run safety algorithm
    isSafe(totalResVector, alloc, maxDemand, NPROC, NRES);
  }
    
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
  

  return 0;
}