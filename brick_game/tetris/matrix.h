/**
 * @file matrix.h
 * @brief Header file for all matrix function
 */

#ifndef BRICKGAME_BRICK_GAME_TETRIS_MATRIX_H
#define BRICKGAME_BRICK_GAME_TETRIS_MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/**
 * @brief Codes for matrix functions results
 */
typedef enum { kOk = 0, kError = 1 } StatusCode;

/**
 * @brief Allocate memory for matrix and fills with zero values
 * @param rows - number of rows
 * @param columns - number of columns
 * @param code - pointer to status code
 * @return kOk if memory allocated successfully and kError if didn't
 */
int **createMatrix(int rows, int columns, int *code);

/**
 * @brief Turn matrix counterclockwise
 * @param rows - number of rows
 * @param columns - number of columns
 * @param result - resulting turned matrix
 * @return kOk if there are no errors inside the function and kError
 * otherwise
 * @warning Works only with square matrix, otherwise may be segfault
 */
int turnLeftMatrix(int rows, int columns, int **result);

/**
 * @brief Turn matrix clockwise
 * @param rows - number of rows
 * @param columns - number of columns
 * @param result - resulting turned matrix
 * @return kOk if there are no errors inside the function and kError
 * otherwise
 * @warning Works only with square matrix, otherwise may be segfault
 */
int turnRightMatrix(int rows, int columns, int **result);

#ifdef __cplusplus
}
#endif

#endif