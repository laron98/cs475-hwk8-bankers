
// TODO: function prototypes of vector and matrix operations

extern int NRES;
extern int NPROC;

void printMatrix(int** matrix);

void printVector(int* vector);

int** cloneMatrix(int** matrix);

int* cloneVector(int* vector);

int compareVectors(int* vector1, int* vector2);

int* addVectors(int* vector1, int* vector2);

int* subVectors(int* vector1, int* vector2);

int** addMatrices(int** matrix1, int** matrix2);

int** subMatrices(int** matrix1, int** matrix2);