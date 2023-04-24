/* declarations related to banker's algorithm */
#define TRUE 1
#define FALSE 0

int isSafe(int* available, int** alloc, int** need, int n, int m);
void printMat(int**mat,int n,int m);