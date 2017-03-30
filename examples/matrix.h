#include <stdio.h>

typedef struct matrix {
    int nrows, ncols, *data;
} matrix;

matrix* allocate(matrix *mat, int nrows, int ncols);
int *cell(const matrix *mat, int row, int col);
matrix *addrow(matrix *mat);
matrix *addcol(matrix *mat);
matrix *print(FILE *f, matrix *mat);
