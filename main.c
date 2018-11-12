#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int data[10]={0};
int read_txt(void);

int main(void)
{
	printf("hello,gavin!\n");
	read_txt();
	getchar();
	return 1;  
}


int read_txt(void)
{	
	FILE *fpRead=fopen("data.txt","r");
	if(fpRead==NULL)
	{
		return 0;
	}
	for(int i=0;i<10;i++)
	{
		fscanf(fpRead,"%d",&data[i]);
		printf("%d",data[i]);
	}
}

int selection_problem(void)
{
	

}

