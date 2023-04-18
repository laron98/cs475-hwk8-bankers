#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

// TODO - Define vector and matrix operations

/**
* Prints given matrix
*/
void printMatrix(int** matrix){
    printf("\n");
    for (int i = 0; i < NPROC; i++) { //loop through and print values from matrix
		for (int j = 0; j < NRES; j++){ 
			printf("%d ", matrix[i][j]);
		}
    printf("\n");
	}
}

/**
* Prints given vector
*/
void printVector(int* vector){
    printf("\n");
    for (int i = 0; i < NRES; i++){ //loop through and print vector
        printf("%d ", vector[i]);
    }
    printf("\n");
}

/**
* Clones given matrix
*/
int** cloneMatrix(int** matrix){
    //get space for the clone matrix 
    int** clone = (int**) malloc(sizeof(int*) * NPROC);
    for (int i = 0; i < NPROC; i++){
        clone[i] = (int*) malloc(sizeof(int) * NRES);
    }

    //clone the passed-in matrix
    for (int i = 0; i < NPROC; i++) {
        for (int j = 0; j < NRES; j++){ 
		    clone[i][j] = matrix[i][j];
		}
    }

  return clone; //return the cloned matrix
}

/**
* Clones given vector
*/
int* cloneVector(int* vector){
    //make space for the total resource vector
    int* clone = (int*) malloc(sizeof(int) * NRES);

    for (int i = 0; i < NRES; i++){ //clone the given vector
        clone[i] = vector[i];
    }

    return clone; //return the cloned vectors
}

/**
* Compares two vectors, if they are the same return 0 else return 1
*/
int compareVectors(int* vector1, int* vector2){
    for (int i = 0; i < NRES; i++){
        if (vector1[i] != vector2[i]){
            return 0;
        }
    }
    return 1;
}

/**
* Sums two vectors and returns vector with the summed results
*/
int* addVectors(int* vector1, int* vector2){
    //get space for vector in which sums will be held
    int* summed = (int*) malloc(sizeof(int) * NRES);

    for (int i = 0; i < NRES; i++){ //sum vectors and place result into new vector
        summed[i] = vector1[i] + vector2[i];
    }

    return summed;
}

/**
* Subtracts two vectors and returns vector with the results
*/
int* subVectors(int* vector1, int* vector2){
    //get space for vector in which sums will be held
    int* subtract = (int*) malloc(sizeof(int) * NRES);

    for (int i = 0; i < NRES; i++){ //sum vectors and place result into new vector
        subtract[i] = vector1[i] - vector2[i];
    }

    return subtract;
}

/**
* Adds two matrices and returns matrix with the results
*/
int** addMatrices(int** matrix1, int** matrix2){
    //get space for the matrix that will hold summed results
	int** add = (int**) malloc(sizeof(int*) * NPROC);
	for (int i = 0; i < NPROC; i++){
		add[i] = (int*) malloc(sizeof(int) * NRES);
	}

    //fill add matrix with summed results
    for (int i = 0; i < NPROC; i++) {
        for (int j = 0; j < NRES; j++){ 
			add[i][j] = matrix1[i][j] + matrix2[i][j];
		}
    }
    return add;
}

/**
* Subtracts two matrices and returns matrix with the results
*/
int** subMatrices(int** matrix1, int** matrix2){
    //get space for the matrix that will hold subtracted results
	int** sub = (int**) malloc(sizeof(int*) * NPROC);
	for (int i = 0; i < NPROC; i++){
		sub[i] = (int*) malloc(sizeof(int) * NRES);
	}

    //fill add matrix with subtracted results
    for (int i = 0; i < NPROC; i++) {
        for (int j = 0; j < NRES; j++){ 
			sub[i][j] = matrix1[i][j] - matrix2[i][j];
		}
    }
    return sub;
}