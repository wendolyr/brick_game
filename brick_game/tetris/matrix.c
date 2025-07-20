#include "matrix.h"

int **createMatrix(int rows, int columns, int *code) {
  *code = kOk;
  int **matrix = malloc(rows * sizeof(int *) + columns * rows * sizeof(int));

  if (matrix == NULL)
    *code = kError;
  else {
    for (int i = 0; i < rows; i++) {
      matrix[i] = (int *)(matrix + rows) + columns * i;
    }

    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        matrix[i][j] = 0;
      }
    }
  }

  return matrix;
}

int turnLeftMatrix(int rows, int columns, int **result) {
  int code = kOk;
  if (result == NULL) {
    code = kError;
  } else {
    int **temp = createMatrix(rows, columns, &code);
    if (temp == NULL)
      code = kError;
    else {
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
          temp[i][j] = result[j][columns - 1 - i];
        }
      }

      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
          result[i][j] = temp[i][j];
        }
      }

      free(temp);
    }
  }
  return code;
}

int turnRightMatrix(int rows, int columns, int **result) {
  int code = kOk;
  if (result == NULL) {
    code = kError;
  } else {
    int **temp = createMatrix(rows, columns, &code);
    if (temp == NULL)
      code = kError;
    else {
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
          temp[i][j] = result[rows - 1 - j][i];
        }
      }

      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
          result[i][j] = temp[i][j];
        }
      }

      free(temp);
    }
  }
  return code;
}