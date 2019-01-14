#ifndef _ARRY_REVERSE_H_
#define _ARRY_REVERSE_H_

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int file_size(char* filename);   
int file_reverse_4ByteA(char* Infile, char* Outfile);
int file_reverse_4ByteB(char* Infile, char* Outfile);
int file_reverse_4ByteC(char* Infile, char* Outfile);
int checkSum(char* File);

#endif

