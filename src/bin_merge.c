/**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*
#  > Author  ： Gavin | Zhang GuiYang
#  > Mail    ： gavin.gy.zhang@gmail.com
#  > Date    ： Dec/25/2018
#  > Company ： Foxconn·CNSBG·CPEGBBD·RD
#  > Funciton:  bin file merge function
#  > Version :  v1.0 
#  > HowToUse:  -
# *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*/

#include "../include/bin_merge.h"

#define	tagAddrValue		'-'

char *getValue = NULL;
char *outputFileName,*outputFileNameBig;
char *functionModule[3]={"NOR","NAND","NAND_BOOT"};
char keyNameAddr[] = "Partition00.Addr",keyNameFile[] = "Partition00.File";

int c2i(char ch);
int hex2dec(char *hex);

int mergeImage2FLASH(char *appName,char* fun)
{
	char *binaryFileFloder = profile_getValue(appName,NULL,"BinaryFileFloder");
	int strlentemp = strlen(binaryFileFloder); //使用 strcat 的前序工作，保存原始字符串长度
	char *partitionNumberChar = (char *)malloc(1);// char partitionNumberChar; 
	int partitionNumber = atoi(profile_getValue(appName,fun,"Partition.Number"));// 调用库函数将字符串(默认当成十进制)转成整数
	int startAddr = 0,fileLeng = 0;
	char *fileName,*fileAddr,*filePath;
	FILE *inputFile = NULL;
	char *inputFile2arry = NULL;
	
	printf("  -> will do merge Image for %s function\n",fun);
	printf("  -> partitionNumber %d\n",partitionNumber);
	outputFileName = strcat(outputFileName,"-");
	outputFileName = strcat(outputFileName,fun);
	outputFileName = strcat(outputFileName,".bin");
	FILE* outputFileNew = fopen(outputFileName, "wb+");
	fclose(outputFileNew);
    FILE* outputFile = fopen(outputFileName, "rb+");

	InfoPrintf("Going to merge image to a full bin file ......\n");
	while(partitionNumber--){
		sprintf(partitionNumberChar,"%d",partitionNumber/10); //使用C语言库函数将整型变量转换成字符（一个字符）(十进制表示) 
		keyNameAddr[9] = keyNameFile[9] = partitionNumberChar[0]; //替换字符串中的第9个字符（这样只支持一个字符）
		sprintf(partitionNumberChar,"%d",partitionNumber%10); 
		keyNameAddr[10] = keyNameFile[10] = partitionNumberChar[0]; 

		DebugPrintf("keyNameAddr: %s ,keyNameFile: %s\n", keyNameAddr, keyNameFile);
		//continue;
		
		/* 读取目标partition的文件名 */
		fileName = profile_getValue(appName,fun,keyNameFile);
		if(fileName[0] == tagSpaceNULL){
			DebugPrintf("value is Blank Space!\n");
			continue;
		}
		filePath = strcat(binaryFileFloder,fileName);
		fileLeng = file_size(filePath);
                if(fileLeng == 0){
                        ErrorPrintf("Cant open file, please check File Floder or file name: %s \n", filePath);
                        goto EXIT;
                }
		printf("  -> file path: %s, file leng: %d \n",filePath, fileLeng);
		inputFile2arry = (char *)malloc(fileLeng);
		inputFile = fopen(filePath, "rb+");
		if(inputFile == NULL){
			ErrorPrintf("Cant open file, please check File Floder or file name !!!\n");
			goto EXIT;
		}
		(binaryFileFloder+strlentemp)[0] = tagSpaceNULL; //使用 strcat 的后序工作，恢复原字符串 binaryFileFloder 的内容
		fread(inputFile2arry, 1, fileLeng, inputFile);
		
	
		fileAddr = profile_getValue(appName,fun,keyNameAddr);
		char *temp = strchr(fileAddr, tagAddrValue);
		if(temp == NULL)
			return -3;
		temp[0] = tagSpaceNULL;
		printf("  -> Addr start:%s; Addr end:%s \n",fileAddr,temp+1);
		startAddr = hex2dec(fileAddr+2);
		printf("  -> Addr start(int):%d\n",startAddr);
		fseek(outputFile, startAddr, SEEK_SET);
		fwrite(inputFile2arry, 1, fileLeng, outputFile);
		
		fclose(inputFile);
		free(inputFile2arry);
		inputFile2arry = NULL;
	}
		
	fclose(outputFile);
	
	if(!strcmp(fun,functionModule[0]))
		goto EXIT;
	
	if(*profile_getValue(appName,fun,"NAND_OOB_Enable") == ENABLE){
		if(!strcmp(profile_getValue(appName,fun,"Endian"),"Big")){
			InfoPrintf("Going to Endian conversion ......\n");
			fseek(outputFile, 0, SEEK_SET);
			strncpy(outputFileNameBig, outputFileName,(strlen(outputFileName)-3)); 
			outputFileNameBig = strcat(outputFileNameBig,"big");
			file_reverse_4ByteC(outputFileName, outputFileNameBig);
		}
		
		InfoPrintf("Going to Add ECC Value to bin file ......\n");
		char **createnfimgArgument = (char **)malloc(17 * sizeof(char *));
		createnfimgArgument[1] = "-b";	createnfimgArgument[2] = profile_getValue(appName,fun,"bch_level");
		createnfimgArgument[3] = "-c";	createnfimgArgument[4] = profile_getValue(appName,fun,"partial_page_size");
		createnfimgArgument[5] = "-j";	createnfimgArgument[6] = profile_getValue(appName,fun,"JFFS2");
		createnfimgArgument[7] = "-m";	createnfimgArgument[8] = profile_getValue(appName,fun,"Field_order");
		createnfimgArgument[9] = "-n";	createnfimgArgument[10] = profile_getValue(appName,fun,"Pages_per_block");
		createnfimgArgument[11] = "-p";	createnfimgArgument[12] = profile_getValue(appName,fun,"Page_size");
		createnfimgArgument[13] = "-r";	createnfimgArgument[14] = profile_getValue(appName,fun,"oob_size");
		createnfimgArgument[15] = "-i";	
		if(!strcmp(profile_getValue(appName,fun,"Endian"),"Big"))
			createnfimgArgument[16] = outputFileNameBig;
		else 
			createnfimgArgument[16] = outputFileName;
		createnfimg(17,createnfimgArgument);	
	}
	
EXIT:
	(binaryFileFloder+strlentemp)[0] = tagSpaceNULL; //使用 strcat 的后序工作，恢复原字符串 binaryFileFloder 的内容
	(outputFileName+strlen(appName))[0] = tagSpaceNULL; //使用 strcat 的后序工作，恢复原字符串 appName 的内容
	return 1;
}

int mergeImage(char *appName)
{
	outputFileName = (char *)malloc(50);
	outputFileNameBig = (char *)malloc(50);
	strcpy(outputFileName, appName);
	for(int i=0;i<3;i++){
		getValue = profile_getValue(appName,NULL,functionModule[i]);
		if(getValue == NULL){
			ErrorPrintf("module error \n");
		}
		else if(*getValue == ENABLE){
			InfoPrintf("********************* module [%s] start *********************\n",functionModule[i]);
			mergeImage2FLASH(appName,functionModule[i]);
		}
	}
	return 1;
}
 

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
