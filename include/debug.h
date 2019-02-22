/**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*
#  > Author  ： Gavin | Zhang GuiYang
#  > Mail    ： gavin.gy.zhang@gmail.com
#  > Date    ： Dec/25/2018
#  > Company ： Foxconn·CNSBG·CPEGBBD·RD
#  > Funciton:  debug config 
#  > Version :  v1.0 
#  > HowToUse:  -
# *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*/
#ifndef _DEBUG_H_
#define _DEBUG_H_

// 参考：
// C语言如何封装printf函数？ https://www.zhihu.com/question/51102741
// 变参标准函数的重新封装，如printf https://blog.csdn.net/coder_weisong/article/details/10285291
// C/C++关于printf的宏定义封装 https://blog.csdn.net/Poetry_And_Distance/article/details/81007604  Get √
// 用printf做彩色日志记录 及宏定义printf https://blog.csdn.net/centnetHY/article/details/80089343
// 总结几种log打印printf函数的宏定义 https://blog.csdn.net/yiziweiyang/article/details/52181984

// 字颜色: 30--39	字背景颜色: 40--49	通用格式控制: 0-8  
// 30: 黑           40: 黑              0: 重置所有属性
// 31: 红           41: 红              1: 高亮/加粗
// 32: 绿           42: 绿              2: 暗淡	
// 33: 黄           43: 黄              	
// 34: 蓝           44: 蓝   	        4: 下划线
// 35: 紫           45: 紫   	        5: 闪烁
// 36: 深绿         46: 深绿            7: 反转
// 37：白色         47：白色            8: 隐藏


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#define __DEBUG__

#define DBG_PRINT (printf("%s:%u %s:%s:\t", __FILE__, __LINE__, __DATE__, __TIME__), printf)

#define ESC_START     "\033["
#define ESC_END       "\033[0m"
#define COLOR_FATAL   "31;40;5m"
#define COLOR_ALERT   "31;40;1m"
#define COLOR_CRIT    "31;40;1m"
#define COLOR_ERROR   "31;40;1m"
#define COLOR_ERROR2  "31;1m"
#define COLOR_WARN    "33;40;1m"
#define COLOR_WARN2    "33;1m"
#define COLOR_NOTICE  "34;40;1m"
#define COLOR_INFO    "32;40;1m"
#define COLOR_INFO2    "32;1m"
#define COLOR_DEBUG   "36;40;1m"
#define COLOR_TRACE   "37;40;1m"

#define Msg_Info(format, args...) (printf( ESC_START COLOR_INFO "[INFO]-[%s]-[%s]-[%d]:" format ESC_END, __FILE__, __FUNCTION__ , __LINE__, ##args))

#define Msg_Debug(format, args...) (printf( ESC_START COLOR_DEBUG "[DEBUG]-[%s]-[%s]-[%d]:" format ESC_END, __FILE__, __FUNCTION__ , __LINE__, ##args))

#define Msg_Warn(format, args...) (printf( ESC_START COLOR_WARN "[WARN]-[%s]-[%s]-[%d]:" format ESC_END, __FILE__, __FUNCTION__ , __LINE__, ##args))

#define Msg_Error(format, args...) (printf( ESC_START COLOR_ERROR2 "[ERROR]-[%s]-[%s]-[%d]:" format ESC_END, __FILE__, __FUNCTION__ , __LINE__, ##args))


#define ErrorPrintf(format, args...) (printf( ESC_START COLOR_ERROR2 "[Error] " format ESC_END, ##args))

#define InfoPrintf(format, args...) (printf( ESC_START COLOR_INFO2 "[Info] " format ESC_END, ##args))


#ifdef __DEBUG__

#define DebugPrintf(format, args...) (printf( ESC_START COLOR_WARN2 "[Debug] " format ESC_END, ##args))

#else
	
#define DebugPrintf(format, args...)

#endif

#define Error( Str )        FatalError( Str )
#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )


void my_trace(const char *cmd, ...);
int execution_timeDebug(char *label,int (*function)(char *),char *a);

#endif
