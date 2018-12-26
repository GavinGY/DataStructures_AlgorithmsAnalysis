#include "bin_merge.h"

#define	tagAddrValue		'-'

char *getValue = NULL;
char *functionModule[4]={"NOR","NAND","NAND_BOOT","NAND_OOB"};

/*
 * 将字符转换为数值
 * */
int c2i(char ch)
{
        // 如果是数字，则用数字的ASCII码减去48, 如果ch = '2' ,则 '2' - 48 = 2
        if(isdigit(ch))
                return ch - 48;
 
        // 如果是字母，但不是A~F,a~f则返回
        if( ch < 'A' || (ch > 'F' && ch < 'a') || ch > 'z' )
                return -1;
 
        // 如果是大写字母，则用数字的ASCII码减去55, 如果ch = 'A' ,则 'A' - 55 = 10
        // 如果是小写字母，则用数字的ASCII码减去87, 如果ch = 'a' ,则 'a' - 87 = 10
        if(isalpha(ch))
                return isupper(ch) ? ch - 55 : ch - 87;
 
        return -1;
}

/*
 * 功能：将十六进制字符串转换为整型(int)数值
 * */
int hex2dec(char *hex)
{
        int len;
        int num = 0;
        int temp;
        int bits;
        int i;
        
        // 此例中 hex = "1de" 长度为3, hex是main函数传递的
        len = strlen(hex);
 
        for (i=0, temp=0; i<len; i++, temp=0)
        {
                // 第一次：i=0, *(hex + i) = *(hex + 0) = '1', 即temp = 1
                // 第二次：i=1, *(hex + i) = *(hex + 1) = 'd', 即temp = 13
                // 第三次：i=2, *(hex + i) = *(hex + 2) = 'd', 即temp = 14
                temp = c2i( *(hex + i) );
                // 总共3位，一个16进制位用 4 bit保存
                // 第一次：'1'为最高位，所以temp左移 (len - i -1) * 4 = 2 * 4 = 8 位
                // 第二次：'d'为次高位，所以temp左移 (len - i -1) * 4 = 1 * 4 = 4 位
                // 第三次：'e'为最低位，所以temp左移 (len - i -1) * 4 = 0 * 4 = 0 位
                bits = (len - i - 1) * 4;
                temp = temp << bits;
 
                // 此处也可以用 num += temp;进行累加
                num = num | temp;
        }
 
        // 返回结果
        return num;
}

int mergeImage2FLASH(char *appName,char* fun)
{
	DebugPrintf("will do merge Image for %s function\n",fun);
	getValue = profile_getValue(appName,fun,"Partition0.Addr");
	if(getValue != NULL){
		if(getValue[0] != tagSpaceNULL){
			DebugPrintf("Get value is: %s\n",getValue);
			char *temp = strchr(getValue, tagAddrValue);
			if(temp == NULL)
				return -3;
			temp[0] = tagSpaceNULL;
			printf("Addr start:%s; Addr end:%s \n",getValue,temp+1);
			for(int i=0;i<strlen(temp+1);i++){
				printf("addr end value %i:%c \n",i,(temp+1)[i]);
			}
			char *hello="002624";
			printf("addr end change to int:%d\n",atoi(hello));
			printf("addr end change to int:%d\n",hex2dec(hello));
		}
		else
			ErrorPrintf("value is Blank Space!\n");
	}
	else
		ErrorPrintf("value get failed!\n");
	
	return 1;
}



int mergeImage(char *appName)
{
	for(int i=0;i<4;i++){
		getValue = profile_getValue(appName,NULL,functionModule[i]);
		if(getValue == NULL){
			ErrorPrintf("module error \n");
		}
		else if(*getValue == ENABLE){
			DebugPrintf("module [ %s ] function is ENABLE\n",functionModule[i]);
			mergeImage2FLASH(appName,functionModule[i]);
		}
	}
	return 1;
}
 
