/*
 *  Matrix header
 *  Function prototypes, data, and constants for matrix module
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

#define ROW 5
#define COL 5

// MATRIX ROUTINES
int ** AllocMatrix(int r, int c);
void FreeMatrix(int ** a, int r, int c);
void GenMatrix(int ** matrix, const int height, const int width);
int AvgElement(int ** matrix, const int height, const int width);
int SumMatrix(int ** matrix, const int height, const int width);

