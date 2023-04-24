#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "banker.h"

/**
 * updates each value of v1 with v1+v2
 * 
 * @param v1 Vector 1
 * @param v2 Vector 2
 * @param len length of the arrays
*/
void vectorPlusEquals(int*v1,int*v2,int len){
    for(int i=0;i<len;i++) v1[i]+=v2[i];
}
/**
 * updates each value of v1 with v1-v2
 * 
 * @param v1 Vector 1
 * @param v2 Vector 2
 * @param len length of the arrays
*/
void vectorMinusEquals(int*v1,int*v2,int len){
    for(int i=0;i<len;i++)v1[i]-=v2[i];
}
/**
 * returns whether all values of v1 are less than or equal to v2
 * 
 * @param v1 Vector 1
 * @param v2 Vector 2
 * @param len length of the arrays
 * @return whether all values of v1 are less than or equal to v2 
*/
int vectorLessThanEq(int*v1,int*v2,int len){
    int b=1;
    for(int i=0;i<len;i++) b=b&&(v1[i]<=v2[i]); 
    return b;
}
/**
 * prints a vector
 * 
 * @param v Vector you'd like to print
 * @param len length of the arrays
*/
void printVec(int*v,int len){
    printf("[%d",v[0]);
    for(int i=0;i<len;i++) printf(",%d",v[i]);
    printf("]\n");
}
/**
 * returns whether all values of v1 are less than or equal to v2
 * 
 * @param v1 Vector 1
 * @param v2 Vector 2
 * @param len length of the arrays
 * @return whether all values of v1 are less than or equal to v2 
*/
int isDone(int*v,int len){
    int b=1;
    for(int i=0;i<len;i++) b=b&&v[i];
    return b;
}
/**
 * prints a vector
 * 
 * @param mat Matrix you'd like to print
 * @param n num cols
 * @param m num rows
*/
void printMat(int**mat,int n,int m){
    for(int i=0;i<n;i++) printVec(mat[i],m);
}
/** checks each thread ordering to tell if the path is safe */
int isSafeRecursive(int *work, int **alloc, int **need, int n, int m, char* outputString, int *finish,int depth){

    if(isDone(finish,n)){ //if there's an exectution order where all threads finish
        printf("SAFE: %s\n",outputString); // print order
        return 1;
    }

    int safe = 0;
    int noProgress = 1;

    for(int i=0;i<n;i++){ // for each thread
        if(finish[i]) continue; // if I is finished, ignore
            if(vectorLessThanEq(need[i],work,m)){ // all values in need are less than or equal to work
            // pretend that thread i finishes execution
		    // then OS can reclaim thread i's allocated resources
            vectorPlusEquals(work,alloc[i],m); //work += alloc[i]
            finish[i] = 1;

            char* currOutput = outputString;
            sprintf(outputString,"%sT%u ",currOutput,i);

            safe = isSafeRecursive(work,alloc,need,n,m,outputString,finish,depth+1) || safe; // if this — or any other path — has been safe, return safe
            noProgress = 0;
            //undo modifications, allowing other recursion branches to work with the same data
            outputString[strlen(outputString)-3] = '\0';
            finish[i] = 0;
            vectorMinusEquals(work,alloc[i],m);
        }
    }
    if(noProgress){
        safe = 0;
        printf("UNSAFE: ");
        for(int i = 0; i<n;i++) printf(!finish[i] ? "T%u ":"",i);
        printf("can't finish\n");
    }
    return safe;
}

/**
 * checks whether an allocation of resources is safe
 * 
 * @param available total resources available vector
 * @param alloc how resources are currently allocated
 * @param maxDemand how many total resources threads want
 * @param n num threads
 * @param m num resources
*/
int isSafe(int *available, int **alloc, int **maxDemand, int n, int m){
    /** preprocessing */
    int* work = (int*)malloc(m*sizeof(int)); // m is the number of resources
    for(int i = 0;i<m;i++) work[i]=available[i]; // clones available to work

    int* finish = (int*)calloc(n,sizeof(int)); // n is number of threads, all are set to 0
    int** need = (int**)malloc(n*sizeof(int*)); // allocate need

    for(int i = 0; i<n;i++){
        need[i]=(int*)malloc(m*sizeof(int));
        for(int j=0;j<m;j++){
            need[i][j]=maxDemand[i][j]-alloc[i][j];     // makes need by subtracting alloc from maxDemand
        } 
    }
    for(int i=0; i<n; i++) vectorMinusEquals(work,alloc[i],m); // account for resources already in use

    char* outputString = (char *)calloc((n*3),sizeof(char)); // makes outputString

    int safe = isSafeRecursive(work,alloc,need,n,m,outputString,finish,0);

    free(outputString); 
    free(work); 
    free(finish); 
    for(int i=0; i<n;i++) free(need[i]); 
    free(need);

    return safe;
}
