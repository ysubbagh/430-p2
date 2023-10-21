// Sudoku puzzle verifier and solver

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int **regions;

//define the variables / structure for the parameters passed to the threads
typedef struct Params{
  int row;
  int column;
}Params;

// takes puzzle size and grid[][] representing sudoku puzzle
// and tow booleans to be assigned: complete and valid.
// row-0 and column-0 is ignored for convenience, so a 9x9 puzzle
// has grid[1][1] as the top-left element and grid[9]9] as bottom right
// A puzzle is complete if it can be completed with no 0s in it
// If complete, a puzzle is valid if all rows/columns/boxes have numbers from 1
// to psize For incomplete puzzles, we cannot say anything about validity
void checkPuzzle(int psize, int **grid, bool *complete, bool *valid) {
  //creare threads
  int THREAD_COUNT = psize * 3;
  regions = (int*)malloc(THREAD_COUNT * sizeof(int));
  pthread_t threads[THREAD_COUNT];
  //create attributes for threads
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  //pthread_create(&threads, &attr, checkBox, NULL);
  //create params for row checkers
  Params *rowParam = (Params*)malloc(sizeof(Params));
  rowParam -> row = 0;
  rowParam -> column = 0;

  for(int i = 0; i < THREAD_COUNT; i++){
    if(i < psize) { //check rows
      rowParam -> column = 0;
      rowParam -> row++;
    }else if (i > psize * 2){ //check columns

    }else{ //check boxes

    }
  }

  //clear memory
  free(rowParam);

  //check the array that all were valid
  *valid = true;
  for(int i = 0; i < THREAD_COUNT; i++){
    if(regions[i] != 1){ // a region was not valid
      *valid = false;
      break;
    }
  }
  *complete = true;
  free(regions);
}

void checkRows(int tnum, int psize, struct Params *param, int **grid){
  int valid = 1;
  //build array to check for validity in the row
  int* validityArr = (int*)malloc(psize * sizeof(int));
  memset(validityArr, 0, psize *sizeof(int));
  //traverse through the row
  for(int i = 0; i < psize; i++){
    int value = grid[param -> row][param -> column];
    if(validityArr[value] != 0){//number was already found in that row
      valid = 0;
      break;
    }else{
      validityArr[value] = 1;
    }
    param -> column++;
  }
  regions[tnum] = valid;
  free(validityArr);
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
