#include "config.h"

#define test_number 8000

int data[test_number]={0};
int data_show[test_number]={0},data_show1[test_number]={0},data_show2[test_number]={0},data_show3[test_number]={0};
int data_show4[test_number]={0},data_show5[test_number]={0},data_show6[test_number]={0},data_show7[test_number]={0};
int read_txt2array(char *filename,int number,int *array_addr);
int write_array2txt(char *filename,int number,int *array_addr);
int generating_random_Numbers(int start,int end,int number,int *array_addr);
int selection_problem(int *array_addr,int number,int max_k);
int execution_time(int (*function)(int *,int,int),int *a,int b,int c);
void show_array(int leng,int *array_addr);

int main(void)
{
    printf("hello,gavin!\n");

    srand((unsigned)time(NULL));
    generating_random_Numbers(0,9999,test_number,data);

    int n = sizeof(data_show) / sizeof(int);

    write_array2txt("data.txt",sizeof(data) / sizeof(int),data);
    //show_array(sizeof(data) / sizeof(int),data);
    read_txt2array("data.txt",n,data_show);
    read_txt2array("data.txt",n,data_show1);
    read_txt2array("data.txt",n,data_show2);
    read_txt2array("data.txt",n,data_show3);
    read_txt2array("data.txt",n,data_show4);
    read_txt2array("data.txt",n,data_show5);
    read_txt2array("data.txt",n,data_show6);
    read_txt2array("data.txt",n,data_show7);

    execution_time(bubbleSort, data_show, n, 0);
    // printf("正向排序后的数组为：\n");
    // show_array(n,data_show);

    execution_time(bubbleSort_Pro, data_show1, n, 0);
    // printf("正向排序后的数组为：\n");
    // show_array(n,data_show1);

    //execution_time(quickSort, data_show2, 0, n-1);
    // printf("正向排序后的数组为：\n");
    // show_array(n,data_show2);

    execution_time(InsertSort, data_show3, n, 0);
    // printf("正向排序后的数组为：\n");
    // show_array(n,data_show3);

    execution_time(ShellSort, data_show4, n, 0);
    // printf("正向排序后的数组为：\n");
    // show_array(n,data_show4);

    execution_time(SelectSort, data_show5, n, 0);
    // printf("正向排序后的数组为：\n");
    // show_array(n,data_show5);

    execution_time(HeapSort, data_show6, n, 0);
    // printf("正向排序后的数组为：\n");
    // show_array(n,data_show6);

    execution_time(MergeSort, data_show7, 0, n-1);
    // printf("正向排序后的数组为：\n");
    // show_array(n,data_show7);

    printf("选择问题：%d \n",execution_time(selection_problem,data_show2,n,3));
    // show_array(n,data_show2);

    getchar();
    return 1;
}

int execution_time(int (*function)(int *,int,int),int *a,int b,int c)
{
    clock_t start, finish;
    double  duration; 
    start = clock(); 
    int re = function(a,b,c);
    finish = clock(); 
    duration = (double)(finish - start) / CLOCKS_PER_SEC;  
    printf("Function Execution Time is: %f seconds\n", duration );
    return re;
}

void show_array(int leng,int *array_addr)
{
    for (int j = 0; j<leng; j++)
        printf("%d ", array_addr[j]);
    printf("\n"); 
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
        //printf("Line %d: %d\n",i,*(array_addr+i));
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

int selection_problem(int *array_addr,int number,int max_k)
{
    quickSort(array_addr, 0, number-1);
    return array_addr[number-max_k];
}

