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
int print_array2(char *label,unsigned int *array_addr,int leng);
int print_array3(char *label,char *array_addr,int leng);


int main(int argc, char *argv[])
{
	// if( argc < 3 ){
        // printf("usage: %s %s/n", argv[0], "infile outfile");
        // exit(1);
    	// }

	/*
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
	*/
	
    /* 测试for循环和函数递归的速度 */
    // execution_time("show_array",show_array, NULL, n, 0);
    // execution_time("show_array_recursion",show_array_recursion, NULL, n, 0);

    	/* 选择问题的算法实现 */
    	/*
	int target_num = 6;
    	int num_value = execution_time("selection_problem", selection_problem, data_show2, n, target_num);
    	printf("选择问题 - 选出第 %d 个最大数为：%d （从 1 开始）\n",target_num, num_value);
    	#ifdef PRINT
    	print_array("选择问题 - 正向排序后的数组为：",data_show2,n);
    	#endif
	*/

    	/* 查找问题的算法实现：二分法 */ // 使用有序数组或者先对数组进行排序 
    	/*
	int item = execution_time("search_problem_dichotomy", search_problem_dichotomy, data_show2, n, num_value);
   	 printf("查找问题 - 查找目标 %d 的数组位置为：第 %d 个（从 0 开始）\n", num_value, item);
    	#ifdef PRINT
    	print_array("查找问题 - 正向排序后的数组为：",data_show2,n);
    	#endif
	*/
	
	/* 基本数据结构：链表操作 */
	//pNode linkList=linkList_creat(5);
	/*
	pNode linkList=linkList_creat_arr(data, n);
	linkList_traverse(linkList);
	linkList_get(linkList,3);
	printf("insert和add 第 11 和 12 节点，modify 第3个节点：\n");
	linkList_insert(linkList,11, 11111);
	linkList_insert(linkList,12, 22222);
	linkList_modify(linkList, 3, 33333);
	linkList_traverse(linkList);
	linkList_get(linkList,3);
	linkList_find(linkList,11111);
	printf("删除第 5 个节点： \n");
	linkList_deleteNode(linkList, 5);
	linkList_traverse(linkList);
	linkList_deleteList(linkList);
	linkList_traverse(linkList);
	*/
	
	/* CPU架构大小端判断 */
	// 联合是一个在同一个存储空间里存储不同类型数据的数据类型。
	// 这些存储区的地址都是一样的，联合里不同存储区的内存是重叠的，修改了任何一个其他的会受影响。
	/*
	union{
		short i;
		char a[2];
	}u;
	u.a[0] = 0x11;
	u.a[1] = 0x22;
	printf ("大小端判断：0x%x\n", u.i);  //0x2211 为小端  0x1122 为大端
	*/

	/* 数组逆序问题，文件操作（二进制）指定字节倒序==>大小端转换问题==>数组逆序问题 */
	// clock_t start, finish;
	// double  duration;
	/*	
	start = clock();
	file_reverse_4ByteA(argv[1], "aa.bin");
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("Function Execution Time is: %f seconds\n",duration);
        start = clock();
        file_reverse_4ByteB(argv[1], "bb.bin");
        finish = clock();
        duration = (double)(finish - start) / CLOCKS_PER_SEC;
        printf("Function Execution Time is: %f seconds\n",duration);
        start = clock();
        file_reverse_4ByteC(argv[1], argv[2]);
	int checksum1 = checkSum(argv[1]);
	int checksum2 = checkSum(argv[2]);
	printf("file %s Check sum: %d \nfile %s Check sum: %d \n", argv[1], checksum1, argv[2], checksum2);
	if(checksum1 == checksum2)
		printf("file check sum OK, they are the same!\n");
	else
		printf("file check sum FAILED !!!\n");
        finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
        printf("Function Execution Time is: %f seconds\n",duration);
	*/
	
	/* 利用sprintf实现hex到ascii的转换 */
	/*
	char* arry_addr=(char*)malloc(sizeof(char) * 16);
	char szTemp[4]={0};
	FILE* InputFile = fopen(argv[1], "rb+");
	FILE* OutputFile = fopen("OutputFile.bin", "wb+");
	if( InputFile == NULL ){
        	printf("%s, %s",argv[1],"not exit/n");
        	exit(1);
    	}   
	int number_read = fread( arry_file, sizeof(unsigned int), 16, InputFile);
	printf("文本操作 - 二进制文本读取的数据个数：%d 个\n",number_read);
	// 指针地址打印和输入，以及直接赋值操作
	printf("测试 数组地址（指向第一个byte）: %p 地址长度 %ld Byte \n", arry_file ,sizeof(arry_file));
	unsigned char* new;     // 整形直接赋值给指针变量的方式举例：unsigned char* new = (void*)0xFFFFFFFF
	new = (void*)arry_file; //scanf("%p",&new);  
	new++;
	printf("测试 数组第一个单位数据（4Byte）%X \n", *arry_file);
	printf("测试 地址 %p 的数据（1Byte）%X \n", new, *new); 
	print_array2("文本操作 - 二进制文本读取的数据内容 1：", arry_file, 16);
	// 大小端转换和文件写入
	for (int j = 0; j<16; j++)
        	sprintf(&arry_addr[j],"%d",(char)arry_file[j]);
	print_array3("文本操作 - 二进制文本读取的数据内容 2：", arry_addr, 16);
	//printf("%X",arry_file[0]);
	sprintf(szTemp,"%d",arry_file[0]);  //把整型变量保持到字符串中 ,sprintf 二进制?
	printf("%s\n",szTemp);
	printf("%d %d %d %d %d\n",szTemp[0],szTemp[1],szTemp[2],szTemp[3],szTemp[4]);
	printf("%X",atoi(szTemp));
	unsigned int arry_file_1 = atoi(szTemp);
	fwrite(&arry_file_1, sizeof(unsigned int), 1, OutputFile);
	*/
	
	/* 配置文件解析问题 Profile Parse */ 
	// 实现获取配置文件中的指定数据，Get ==> APP.Module.KEY.Value
        clock_t start, finish;
        double  duration;      
        start = clock();
        getProfile(argv[1]);
        finish = clock();
        duration = (double)(finish - start) / CLOCKS_PER_SEC;
        printf("Function Execution Time is: %f seconds\n",duration);


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

int print_array2(char *label,unsigned int *array_addr,int leng)
{
    printf("%s", label);
    for (int j = 0; j<leng; j++)
        printf("|%08X| ", array_addr[j]);
    printf("\n");
    return 0;
}

int print_array3(char *label,char *array_addr,int leng)
{
    printf("%s", label);
    for (int j = 0; j<leng; j++)
        printf("|%X| ", array_addr[j]);
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

