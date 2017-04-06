/*
 *  Matrix routines
 *  Supports generation of random R x C matrices
 *  And operations on them
 * 
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <sched.h>
#include <string.h>
#include <time.h>

// MATRIX ROUTINES
int ** AllocMatrix(int r, int c)
{
  int ** a;
  int i;
  a = (int**) malloc(sizeof(int *) * r);
  assert(a != 0);
  for (i = 0; i < r; i++)
  {
    a[i] = (int *) malloc(c * sizeof(int));
    assert(a[i] != 0);
  }
  return a;
}

void FreeMatrix(int ** a, int r, int c)
{
  int i;
  for (i=0; i<r; i++)
  {
    free(a[i]);
  }
  free(a);
}

void GenMatrix(int ** matrix, const int height, const int width)
{
  int i, j;
  for (i = 0; i < height; i++)
  {
    for (j = 0; j < width; j++)
    {
      int * mm = matrix[i];
      //matrix[i * width + j] = rand() % 10;
      mm[j] = rand() % 10;
#if OUTPUT
      printf("matrix[%d][%d]=%d \n",i,j,mm[j]);
#endif
    }
  }
}

int AvgElement(int ** matrix, const int height, const int width)
{
  int x=0;
  int y=0;
  int ele=0;
  int i, j;
  for (i=0; i<height; i++)
    for (j=0; j<width; j++)
    {
      int *mm = matrix[i];
      y=mm[j];  
      x=x+y;
      ele++;
#if OUTPUT
      printf("[%d][%d]--%d x=%d ele=%d\n",i,j,mm[j],x,ele);
#endif
    }
  printf("x=%d ele=%d\n",x, ele);
  return x / ele; 
}

int SumMatrix(int ** matrix, const int height, const int width)
{
  int sum = 0;
  int i, j;
  for (i=0; i<height; i++)
    for (j=0; j<width; j++)
    {
      sum += matrix[i][j];
#if OUTPUT
      printf("[%d][%d]--%d x=%d ele=%d\n",i,j,mm[j],x,ele);
#endif
    }
  return sum; 
}


