// Sudoku puzzle verifier and solver

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//define the variables / structure for the parameters passed to the threads
typedef struct Params{
  int row;
  int column;
  int size;
  int **agrid;
}Params;

//checks every row for the correct values
void* checkRows(struct Params *param){
  printf("size: %d\n", param -> size);
  int valid = 1;
  bool stop = false;
  //traverse through the grid
  for(int i = 1; i <= param -> size; i++){ //traverse through the rows
    int* vals = (int*)calloc(param -> size, sizeof(int));
    for(int j = 1; j <= param -> size; j++){ //traverse through the columns
      int cellNum = param -> agrid[i][j];
      if(cellNum == 0){
        valid = -1;
        stop = true;
      }else if(vals[cellNum] != 0){
        valid = 0;
        stop = true;
      }else{
        vals[cellNum] = j;
      }
    }
    free(vals);
    if(stop){break;}
  }
  return (void*)(uintptr_t)valid;
}

//checks every column for the correct values
void* checkCol(struct Params *param){
  int valid = 1;
  bool stop = false;
  //traverse through the grid
  for(int i = 1; i <= param -> size; i++){ //traverse through the rows
    int* vals = (int*)calloc(param -> size, sizeof(int));
    for(int j = 1; j <= param -> size; j++){ //traverse through the columns
      int cellNum = param -> agrid[j][i];
      if(cellNum == 0){
        valid = -1;
        stop = true;
      }else if(vals[cellNum] != 0){
        valid = 0;
        stop = true;
      }else{
        vals[cellNum] = i;
      }
    }
    free(vals);
    if(stop){break;}
  }
  return (void*)(uintptr_t)valid;
}

// takes puzzle size and grid[][] representing sudoku puzzle
// and tow booleans to be assigned: complete and valid.
// row-0 and column-0 is ignored for convenience, so a 9x9 puzzle
// has grid[1][1] as the top-left element and grid[9]9] as bottom right
// A puzzle is complete if it can be completed with no 0s in it
// If complete, a puzzle is valid if all rows/columns/boxes have numbers from 1
// to psize For incomplete puzzles, we cannot say anything about validity
void checkPuzzle(int psize, int **grid, bool *complete, bool *valid) {
  int result[2+psize];
  //initalize threads
  pthread_t rowThread;
  pthread_t colThread;
  pthread_t boxThread;
  //create attributes for threads
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  //create params//
  //row
  Params *rowParams = (Params*)malloc(sizeof(Params));
  rowParams -> size = psize;
  rowParams -> agrid = grid;
  //column
  Params *colParams = (Params*)malloc(sizeof(Params));
  colParams -> size = psize;
  colParams -> agrid = grid;

  /*CREATE THREADS*/
  //rows
  pthread_create(&rowThread, &attr, checkRows, rowParams);
  void* funResult;
  pthread_join(rowThread, &funResult);
  result[0] = (int)(uintptr_t)funResult;
  //columns
  pthread_create(&colThread, &attr, checkCol, colParams);
  void* colResult;
  pthread_join(colThread, &funResult);
  result[1] = (int)(uintptr_t)funResult;

  //base cases for results
  *valid = true;
  *complete = true;
  /*
  //check the results from the threads
  for(int i = 0; i < 2+psize; i++){
    if(result[i] == -1){ //a 0 was found, not complete, no need to check for valid
      *complete = false;
      break;
    }
    if(result[i] == 0){ //at least one on the threads was not valid
      *valid = false;
    }
  }
  if(!*complete){*valid = false;} //valid cannot be true if complete is false
  */
  if(result[0] != 1 && result[1] != 1){
    *valid = false;
  }


  //free pointers
  free(rowParams);
  free(colParams);
  pthread_attr_destroy(&attr);
}

// takes filename and pointer to grid[][]
// returns size of Sudoku puzzle and fills grid
int readSudokuPuzzle(char *filename, int ***grid) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Could not open file %s\n", filename);
    exit(EXIT_FAILURE);
  }
  int psize;
  fscanf(fp, "%d", &psize);
  int **agrid = (int **)malloc((psize + 1) * sizeof(int *));
  for (int row = 1; row <= psize; row++) {
    agrid[row] = (int *)malloc((psize + 1) * sizeof(int));
    for (int col = 1; col <= psize; col++) {
      fscanf(fp, "%d", &agrid[row][col]);
    }
  }
  fclose(fp);
  *grid = agrid;
  return psize;
}

// takes puzzle size and grid[][]
// prints the puzzle
void printSudokuPuzzle(int psize, int **grid) {
  printf("%d\n", psize);
  for (int row = 1; row <= psize; row++) {
    for (int col = 1; col <= psize; col++) {
      printf("%d ", grid[row][col]);
    }
    printf("\n");
  }
  printf("\n");
}

// takes puzzle size and grid[][]
// frees the memory allocated
void deleteSudokuPuzzle(int psize, int **grid) {
  for (int row = 1; row <= psize; row++) {
    free(grid[row]);
  }
  free(grid);
}

// expects file name of the puzzle as argument in command line
int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: ./sudoku puzzle.txt\n");
    return EXIT_FAILURE;
  }
  // grid is a 2D array
  int **grid = NULL;
  // find grid size and fill grid
  int sudokuSize = readSudokuPuzzle(argv[1], &grid);
  bool valid = false;
  bool complete = false;
  checkPuzzle(sudokuSize, grid, &complete, &valid);
  printf("Complete puzzle? ");
  printf(complete ? "true\n" : "false\n");
  if (complete) {
    printf("Valid puzzle? ");
    printf(valid ? "true\n" : "false\n");
  }
  printSudokuPuzzle(sudokuSize, grid);
  deleteSudokuPuzzle(sudokuSize, grid);
  return EXIT_SUCCESS;
}
