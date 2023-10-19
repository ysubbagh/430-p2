#ifndef SUDOKU_H
#define SUDOKU_H

// Sudoku puzzle verifier and solver

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//structure for passing data to threads
typedef struct params;

//functions
void checkPuzzle(int psize, int **grid, bool *complete, bool *valid);
int checkRows();
int checkColumns();
int checkBox();
int readSudokuPuzzle(char *filename, int ***grid);
void printSudokuPuzzle(int psize, int **grid);
void deleteSudokuPuzzle(int psize, int **grid);
int main(int argc, char **argv);


#endif 