#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int data[10]={0};
int data_show[10]={0};
int read_txt2array(char *filename,int number,int *array_addr);
int write_array2txt(char *filename,int number,int *array_addr);
int generating_random_Numbers(int start,int end,int number,int *array_addr);
int selection_problem(int max_k,int number,int *array_addr);

int main(void)
{
    printf("hello,gavin!\n");
    srand((unsigned)time(NULL));
    generating_random_Numbers(100,999,10,&data[0]);
    write_array2txt("data.txt",10,&data[0]);
    read_txt2array("data.txt",8,&data_show[0]);
    getchar();
    return 1;
}

int read_txt2array(char *filename,int number,int *array_addr)
{	
    FILE *fpRead=fopen(filename,"r");
    if(fpRead==NULL)
    {
        return 0;
    }
    for(int i=0;i<number;i++)
    {
        fscanf(fpRead,"%d",(array_addr+i)); 
        printf("Line %d: %d\n",i,*(array_addr+i));
    }
}

int write_array2txt(char *filename,int number,int *array_addr)
{
    FILE *fpWrite=fopen(filename,"w");
	if(fpWrite==NULL)
	{
        return 0;
	}
    for(int i=0;i<number;i++)
		fprintf(fpWrite,"%d\n",*(array_addr+i));
	fclose(fpWrite);
}

int generating_random_Numbers(int start,int end,int number,int *array_addr)
{
    int leng=(end-start)+1;
    int random_Numbers=0;
    //random_Numbers = rand()%leng+start;
    for (int i=0; i<number; i++)
    {
        *(array_addr+i)=rand()%leng+start;;
    }
    //printf("%d \n", random_Numbers);
    //return random_Numbers;
}

int selection_problem(int max_k,int number,int *array_addr)
{
    for (int i=0; i<number; i++)
    {
        if(*(array_addr+i)>*(array_addr+max_k))
        {

        }
        else
        {

        }
    }
    return 0;
}

