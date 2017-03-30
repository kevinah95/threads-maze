/*
 * http://stackoverflow.com/questions/9853411/variable-size-matrix-in-c
 * */
#include "matrix.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

matrix* allocate(matrix *mat, int nrows, int ncols)
{
  assert(nrows > 0 && ncols > 0);
  mat->nrows = nrows;
  mat->ncols = ncols;
  mat->data = malloc(sizeof(int) * nrows * ncols);
  return mat;
}

int *cell(const matrix *mat, int row, int col)
{
  assert(row >= 0 && row < mat->nrows);
  assert(col >= 0 && col < mat->ncols);
  return mat->data + row * mat->ncols + col;
}

matrix *addrow(matrix *mat)
{
  mat->nrows++;
  mat->data = realloc(mat->data, sizeof(int) * mat->nrows * mat->ncols);
  return mat;
}

/* adding a column it's an expensive operation */
matrix *addcol(matrix *mat)
{
  mat->ncols++;
  mat->data = realloc(mat->data, sizeof(int) * mat->nrows * mat->ncols);

  /* shift rows' elements, to make room for last column */
  for (int r = mat->nrows - 1; r > 0; --r)
  {
    int *dest = mat->data + r * mat->ncols,
        *orig = mat->data + r * (mat->ncols - 1);
    memmove(dest, orig, sizeof(int) * (mat->ncols - 1));
  }
  return mat;
}

matrix *print(FILE *f, matrix *mat)
{
    for (int r = 0; r < mat->nrows; ++r)
    {
        for (int c = 0; c < mat->ncols; ++c)
            fprintf(f, "%4d ", *cell(mat, r, c));
        fprintf(f, "\n");
    }
    return mat;
}

int main(int argc, char **argv)
{
    matrix m;
    allocate(&m, 3, 5);

    for (int r = 0; r < m.nrows; ++r)
        for (int c = 0; c < m.ncols; ++c)
            *cell(&m, r, c) = 35;
    print(stdout, &m);
    fprintf(stdout, "\n");

    addrow(&m);
    for (int c = 0; c < m.ncols; ++c)
        *cell(&m, m.nrows - 1, c) = 45;
    print(stdout, &m);
    fprintf(stdout, "\n");

    addcol(&m);
    for (int r = 0; r < m.nrows; ++r)
        *cell(&m, r, m.ncols - 1) = 46;
    print(stdout, &m);
    fprintf(stdout, "\n");

    // remember to free memory
    free(m.data);

    return argc;
}