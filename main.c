#include "config.h"

#define test_number 10

int data[test_number]={0};
int data_show[test_number]={0},data_show1[test_number]={0},data_show2[test_number]={0},data_show3[test_number]={0};
int data_show4[test_number]={0},data_show5[test_number]={0},data_show6[test_number]={0},data_show7[test_number]={0};
int read_txt2array(char *filename,int number,int *array_addr);
int write_array2txt(char *filename,int number,int *array_addr);
int generating_random_Numbers(int start,int end,int number,int *array_addr);
int selection_problem(int *array_addr,int number,int max_k);
int execution_time(char *label,int (*function)(int *,int,int),int *a,int b,int c);
int show_array(int *array_addr,int leng,int reserve);/* 测试for循环和函数递归的速度 */
int show_array_recursion(int *array_addr,int leng,int reserve);/* 测试for循环和函数递归的速度 */
int print_array(char *label,int *array_addr,int leng);
int search_problem_dichotomy(int *arry, int leng, int item);

int main(void)
{
    printf("hello,gavin!\n");

    srand((unsigned)time(NULL));
    generating_random_Numbers(0,9999,test_number,data);
    #ifdef PRINT
    print_array("原始数组为：",data,sizeof(data) / sizeof(int));
    #endif
    write_array2txt("data.txt",sizeof(data) / sizeof(int),data);
    
    int n = sizeof(data_show) / sizeof(int);
    read_txt2array("data.txt",n,data_show);
    read_txt2array("data.txt",n,data_show1);
    read_txt2array("data.txt",n,data_show2);
    read_txt2array("data.txt",n,data_show3);
    read_txt2array("data.txt",n,data_show4);
    read_txt2array("data.txt",n,data_show5);
    read_txt2array("data.txt",n,data_show6);
    read_txt2array("data.txt",n,data_show7);

    execution_time("bubbleSort",bubbleSort, data_show, n, 0);
    #ifdef PRINT
    print_array("bubbleSort 正向排序后的数组为：",data_show,n);
    #endif

    execution_time("bubbleSort_Pro",bubbleSort_Pro, data_show1, n, 0);
    #ifdef PRINT
    print_array("bubbleSort_Pro 正向排序后的数组为：",data_show1,n);
    #endif

    // execution_time("quickSort",quickSort, data_show2, 0, n-1);
    // #ifdef PRINT
    // print_array("quickSort 正向排序后的数组为：",data_show2,n);
    // #endif

    execution_time("InsertSort",InsertSort, data_show3, n, 0);
    #ifdef PRINT
    print_array("InsertSort 正向排序后的数组为：",data_show3,n);
    #endif

    execution_time("ShellSort",ShellSort, data_show4, n, 0);
    #ifdef PRINT
    print_array("ShellSort 正向排序后的数组为：",data_show4,n);
    #endif

    execution_time("SelectSort",SelectSort, data_show5, n, 0);
    #ifdef PRINT
    print_array("SelectSort 正向排序后的数组为：",data_show5,n);
    #endif

    execution_time("HeapSort",HeapSort, data_show6, n, 0);
    #ifdef PRINT
    print_array("HeapSort 正向排序后的数组为：",data_show6,n);
    #endif

    execution_time("MergeSort",MergeSort, data_show7, 0, n-1);
    #ifdef PRINT
    print_array("MergeSort 正向排序后的数组为：",data_show7,n);
    #endif

    /* 测试for循环和函数递归的速度 */
    // execution_time("show_array",show_array, NULL, n, 0);
    // execution_time("show_array_recursion",show_array_recursion, NULL, n, 0);

    /* 选择问题的算法实现 */
    int target_num = 6;
    int num_value = execution_time("selection_problem", selection_problem, data_show2, n, target_num);
    printf("选择问题 - 选出第 %d 个最大数为：%d （从 1 开始）\n",target_num, num_value);
    #ifdef PRINT
    print_array("选择问题 - 正向排序后的数组为：",data_show2,n);
    #endif

    /* 查找问题的算法实现：二分法 */ // 使用有序数组或者先对数组进行排序 
    int item = execution_time("search_problem_dichotomy", search_problem_dichotomy, data_show2, n, num_value);
    printf("查找问题 - 查找目标 %d 的数组位置为：第 %d 个（从 0 开始）\n", num_value, item);
    #ifdef PRINT
    print_array("查找问题 - 正向排序后的数组为：",data_show2,n);
    #endif
	
    getchar();
    return 1;
}

int execution_time(char *label,int (*function)(int *,int,int),int *a,int b,int c)
{
    clock_t start, finish;
    double  duration; 
    start = clock(); 
    int re = function(a,b,c);
    finish = clock(); 
    duration = (double)(finish - start) / CLOCKS_PER_SEC;  
    printf("Function %s Execution Time is: %f seconds\n",label ,duration);
    return re;
}

int a=0;

int print_array(char *label,int *array_addr,int leng)
{
    printf("%s ", label);
    for (int j = 0; j<leng; j++)
        printf("%d ", array_addr[j]);
    printf("\n");
    return 0;
}

int show_array(int *array_addr,int leng,int reserve)
{
    for (int j = 0; j<leng; j++)
        printf("%d ", array_addr[j]);
        //a=j%50;   /* 测试for循环和函数递归的速度 */
    return 0;
}

//递归实现
int show_array_recursion(int *array_addr,int leng,int reserve) 
{
    if(!leng) //基准情形出现
        return 0;
    else{
        printf("%d ", array_addr[leng-1]);
        //a=leng%50; /* 测试for循环和函数递归的速度 */
        show_array_recursion(array_addr,leng-1,reserve);
    }
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
        array_addr[i] = rand()%leng+start;
    }
    //printf("%d \n", random_Numbers);
    //return random_Numbers;
}

int selection_problem(int *array_addr,int number,int max_k)
{
    quickSort(array_addr, 0, number-1);
    return array_addr[number-max_k];
}

int search_problem_dichotomy(int *arry, int leng, int item)
{
    int low = 0;
    int hight = leng-1;
    int mid,guess;
    while( low <= hight ){
        mid = (low + hight)/2;
        guess = arry[mid];
        if(guess == item)
            return mid;
        if(guess > item)
            hight = mid -1;
        else
            low = mid+1;
    }
    return 59999;
}

