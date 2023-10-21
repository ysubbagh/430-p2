#ifndef SUDOKU_H
#define SUDOKU_H

// Sudoku puzzle verifier and solver

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//functions
void checkPuzzle(int psize, int **grid, bool *complete, bool *valid);
void checkRows(int tnum, int psize, struct Params* param, int **grid);
int checkColumns();
int checkBox();
int readSudokuPuzzle(char *filename, int ***grid);
void printSudokuPuzzle(int psize, int **grid);
void deleteSudokuPuzzle(int psize, int **grid);
int main(int argc, char **argv);


#endif 