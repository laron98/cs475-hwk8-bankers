#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "banker.h"

void vectorPlusEquals(int*v1,int*v2,int len){for(int i=-1;++i<len;v1[i]+=v2[i]);}
void vectorMinusEquals(int*v1,int*v2,int len){for(int i=-1;++i<len;v1[i]-=v2[i]);}
int vectorLessThan(int*v1,int*v2,int len){int b=1;for(int i=-1;++i<len;b*=(v1[i]<v2[i])); return b;}
int vectorLessThanEq(int*v1,int*v2,int len){int b=1;for(int i=-1;++i<len;b*=(v1[i]<=v2[i])); return b;}
int vectorGreaterThan(int*v1,int*v2,int len){int b=1; for(int i=-1;++i<len;b=b&&v1[i]>v2[i]); return b;}
void printVec(int*v,int len){printf("[%d",v[0]);for(int i=0;++i<len;) printf(",%d",v[i]);printf("]\n");}
int isDone(int*v,int len){int b=1;for(int i=-1;++i<len;b*=v[i]);return b;}
void printMat(int**mat,int n,int m){for(int i=-1;++i<n;printVec(mat[i],m));}

// TODO - Safety Algorithm goes here
int isSafeRecursive(int *available, int **alloc, int **need, int n, int m, char* outputString, int *finish){
    if(isDone(finish,n)){
        printf("SAFE: %s\n",outputString);
        return 1;
    }

    int safe = 0;
    int noProgress = 1;
    for(int i=-1;++i<n;){
        if(finish[i]) continue; // if I is finished, ignore
        // printf("available: ");
        // printVec(available,m);
        // printf("need[%u]: ",i);
        // printVec(need[i],m);
        if(vectorLessThan(need[i],available,m)){ // all values in (need - alloc) are less than or equal to available
            vectorPlusEquals(available,alloc[i],m);
            finish[i] = 1;

            sprintf(outputString,"%sT%u ",outputString,i);

            safe = isSafeRecursive(available,alloc,need,n,m,outputString,finish)||safe;
            noProgress = 0;
            //undo modifications, so as not to mess up other recursions
            outputString[strlen(outputString)-3] = '\0';
            finish[i] = 0;
            vectorMinusEquals(available,need[i],m);
        }
    }

    if(noProgress){
        safe = 0;
        printf("UNSAFE: ");
        for(int i = -1; ++i<n;printf(!finish[i] ? "T%u ":"",i));
        printf("can't finish\n");
    }
    return safe;
}

int isSafe(int *available, int **alloc, int **maxDemand, int n, int m){

    int* work = (int*)malloc(m*sizeof(int)); // m is the number of resources
    for(int i = -1; ++i < m; work[i]=available[i]); // clones available to work
    int* finish = (int*)calloc(n, sizeof(int)); // n is number of threads, all are set to 0
    int** need = (int**)malloc(n*sizeof(int*)); // m is the number of resources
    for(int i = -1; ++i<n;){need[i]=(int*)malloc(m*sizeof(int));for(int j=-1;++j<m;need[i][j]=maxDemand[i][j]-alloc[i][j]);} // makes need by subtracting alloc from maxDemand

    //is need defined as all the resources needed, or number of remaining resources?
    //if it's all the resources needed:
    //for(int i = -1; ++i<n; vectorMinusEquals(need[i],alloc[i],m));

    char* outputString = (char *)calloc((n*3),sizeof(char));

    int safe = isSafeRecursive(work,alloc,need,n,m,outputString,finish);
    free(outputString); free(work); free(finish); for(int i=-1; ++i<n;free(need[i])); free(need);

    return safe;
}
//      while (exists unfinished thread i && Need[i] <= Work) {
//              // pretend that thread i finishes execution
//              // then OS can reclaim thread i's allocated resources
//              Work += Alloc[i]
//              Finish[i] = 1
//      }
//      // there's an execution order in which all threads
//      if (Finish == [1, 1, ..., 1])
//              return true     // safe!
//      return false            // unsafe!