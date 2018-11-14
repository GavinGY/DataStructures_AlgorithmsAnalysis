#ifndef _SORTING_ALGORITHM_H_
#define _SORTING_ALGORITHM_H_

#include <stdio.h>
#include <stdlib.h>

int bubbleSort(int *arr, int n, int dir);
int bubbleSort_Pro(int *arr, int n, int dir);
int quickSort(int *a,int left,int right);

int InsertSort(int *a, int n ,int dir);
int ShellSort(int *a, int n,int dir);

int SelectSort(int *a, int n,int dir);
int HeapSort(int a[], int n,int dir);

int MergeSort(int a[], int left, int right);

#endif