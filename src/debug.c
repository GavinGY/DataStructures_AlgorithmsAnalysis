/**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*
#  > Author  ： Gavin | Zhang GuiYang
#  > Mail    ： gavin.gy.zhang@gmail.com
#  > Date    ： Dec/25/2018
#  > Company ： Foxconn·CNSBG·CPEGBBD·RD
#  > Funciton:  debug config 
#  > Version :  v1.0 
#  > HowToUse:  -
# *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*/

#include "../include/debug.h"

int execution_timeDebug(char *label,int (*function)(char *),char *a)
{
    clock_t start, finish;
    double  duration;
    start = clock();
    int re = function(a);
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    InfoPrintf("Function %s Execution Time is: %f seconds\n",label ,duration);
    return re;
}

void my_trace(const char *cmd, ...)
{
	printf("%s %s ", __DATE__, __TIME__);
	va_list args;       //定义一个va_list类型的变量，用来储存单个参数
	va_start(args,cmd); //使args指向可变参数的第一个参数
	vprintf(cmd,args);  //必须用vprintf等带V的
	va_end(args);       //结束可变参数的获取
}

