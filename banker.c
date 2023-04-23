#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "banker.h"

void vectorPlusEquals(int*v1,int*v2,int len){for(int i=-1;++i<len;v1[i]+=v2[i]);}
void vectorMinusEquals(int*v1,int*v2,int len){for(int i=-1;++i<len;v1[i]-=v2[i]);}
int vectorLessThan(int*v1,int*v2,int len){int b=1;for(int i=-1;++i<len;b*=v1[i]<v2[i]); return b;}
int vectorGreaterThan(int*v1,int*v2,int len){int b=1; for(int i=-1;++i<len;b*=v1[i]>v2[i]); return b;}

// TODO - Safety Algorithm goes here
int isSafeRecursive(int *available, int **alloc, int **need, int n, int m, char* outputString, int *finish,int depth){
    if(depth == n){
        printf("%s",outputString);
        return 1;
    }
    int safe = 0;
    int noProgress = 1;
    for(int i=-1;++i<n;){
        if(finish[i]) continue; // if I is finished, ignore
        if(!vectorGreaterThan(available,need[i],m)){
            vectorPlusEquals(available,need[i],m);
            finish[i] = 1;
            safe = safe||isSafeRecursive(available,alloc,need,n,m,outputString,finish,depth+1);
            noProgress = 0;
            //undo modifications, so as not to mess up other recursions
            finish[i] = 0;
            vectorMinusEquals(available,need[i],m);
        }
    }
    if(noProgress){
        printf("UNSAFE: ");
        for(int i = -1; ++i<n;printf(!finish[i] ? "T%u ":"",i));
        printf("can't finish\n");
    }
    return safe;
}

int isSafe(int *available, int **alloc, int **need, int n, int m){

    //int work[m]; // m is the number of resources
    //for(int i = -1; ++i < m; work[i]=available[i]); // clones available to work
    int* finish = (int*)malloc(n * sizeof(int)); // n is number of threads, all are set to 0
    for(int i = -1; ++i < m; finish[i]=0); // sets finish to 0s

    //is need defined as all the resources needed, or number of remaining resources?
    //if it's all the resources needed:
    for(int i = -1; ++i<n; vectorMinusEquals(need[i],alloc[i],m));

    char* outputString = (char *)malloc((n*3-1) * sizeof(char));

    int safe = isSafeRecursive(available,alloc,need,n,m,outputString,finish,0);
    free(outputString);
    return safe;

// 	while (exists unfinished thread i && Need[i] <= Work) {
// 		// pretend that thread i finishes execution
// 		// then OS can reclaim thread i's allocated resources
// 		Work += Alloc[i]
// 		Finish[i] = 1
// 	}
// 	// there's an execution order in which all threads
// 	if (Finish == [1, 1, ..., 1])
// 		return true	// safe!
// 	return false		// unsafe!
}

/**
 * Tells whether all matricies have finished
*/
int isDone(int* f, int n){int curr;for(int i=0; ++i < n; curr = curr*f[n]); return curr;}