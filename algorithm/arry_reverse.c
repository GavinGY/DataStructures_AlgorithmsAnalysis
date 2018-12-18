#include "arry_reverse.h"
#include <sys/stat.h>
#include <unistd.h>


int arry_reverseA(unsigned int* arry_addr,int leng, FILE* file);
int arry_reverseC(unsigned int* arry_addr,int leng);

int file_size(char* filename)
{
    struct stat statbuf;
    stat(filename,&statbuf);
    int size=statbuf.st_size;

    return size;
}

// 交换bin文件的每4字节 ==> 数据倒序问题：方式A
// 1. 先将文件以4Byte为操作基本单位加载到内存（开辟一个动态数组），然后立马通过fwrite写到新文件并关闭，然后再打开刚刚写的文件
// 2. 以4Byte为基本单位判断数据是否有效，无效不做处理；有效则进行覆盖重写（rb方式打开，fseek偏移文件指针）
// 总结：该方式速度非常慢（i5 64bit 128MB ==> 13s），a.fwrite执行太多次(DDR写入磁盘) b.fseek函数很慢（直接在磁盘操作）
int file_reverse_4ByteA(char* Infile, char* Outfile){
        int file_leng = file_size(Infile);
        printf("file leng: %d \n",file_leng);
        int arry_leng = 0;
        int handle_buffer = sizeof(unsigned int);
        if(file_leng % handle_buffer)
                arry_leng = (file_leng / handle_buffer) + handle_buffer;
        else
                arry_leng = (file_leng / handle_buffer);
        printf("arry leng: %d \n",arry_leng);
        unsigned int* arry_file = (unsigned int*)malloc(handle_buffer * arry_leng);
        FILE* InputFile = fopen(Infile, "rb+");
        FILE* OutputFile = fopen(Outfile, "wb+");
        if( InputFile == NULL ){
                printf("%s, %s",Infile,"not exit/n");
                exit(1);
        }
        int number_read = fread( arry_file, handle_buffer, arry_leng, InputFile);
        printf("file read: %d byte\n", number_read * handle_buffer);
        int number_write = fwrite(arry_file, handle_buffer, arry_leng, OutputFile);
        printf("file write: %d byte\n", number_write * handle_buffer);
        fclose(OutputFile);
        FILE* OutputFile2 = fopen(Outfile, "rb+");
        int invalidNumber = 0;
        //unsigned char* arry_file_addr = (void*)arry_file;
        //printf("print addr: %p data: %X \n", arry_file_addr, *arry_file_addr);
        for(int i = 0; i < arry_leng; i++){
                if(arry_file[i] == 0x00000000 || arry_file[i] == 0xFFFFFFFF){
                        invalidNumber++;
                        //fwrite(&arry_file[i], handle_buffer, 1, OutputFile);
                }
                else{
                        fseek(OutputFile2, i*4, SEEK_SET);
                        arry_reverseA(&arry_file[i], handle_buffer, OutputFile2);
                }
        }
        printf("arry invalid number: %d \n", invalidNumber);
        //int number_write = fwrite(arry_file, handle_buffer, arry_leng, OutputFile);
        //printf("file write: %d byte\n", number_write * handle_buffer);  
	fclose(InputFile);
	fclose(OutputFile2);      
}

// 交换bin文件的每4字节 ==> 数组倒序问题：方式B
// 1. 先将文件以4Byte为操作基本单位加载到内存（开辟一个动态数组）
// 2. 以4Byte为基本单位判断数据是否有效，无效则使用fwrite直接写入原数据到新文件；有效则进行调换重写（rb方式打开，fseek偏移文件指针）
// 总结：该方式速度不够快（i5 64bit 128MB ==> 1.2s) 原因：a.执行fwrite太多次了（该函数需要从ddr copy 到 磁盘）
int file_reverse_4ByteB(char* Infile, char* Outfile){
        int file_leng = file_size(Infile);
        printf("file leng: %d \n",file_leng);
        int arry_leng = 0;
        int handle_buffer = sizeof(unsigned int);
        if(file_leng % handle_buffer)
                arry_leng = (file_leng / handle_buffer) + handle_buffer;
        else
                arry_leng = (file_leng / handle_buffer);
        printf("arry leng: %d \n",arry_leng);
        unsigned int* arry_file = (unsigned int*)malloc(handle_buffer * arry_leng);
        FILE* InputFile = fopen(Infile, "rb+");
        FILE* OutputFile = fopen(Outfile, "wb+");
        if( InputFile == NULL ){
                printf("%s, %s",Infile,"not exit/n");
                exit(1);
        }
        int number_read = fread( arry_file, handle_buffer, arry_leng, InputFile);
        printf("file read: %d byte\n", number_read * handle_buffer);
        int invalidNumber = 0;
        //unsigned char* arry_file_addr = (void*)arry_file;
        //printf("print addr: %p data: %X \n", arry_file_addr, *arry_file_addr);
        for(int i = 0; i < arry_leng; i++){
                if(arry_file[i] == 0x00000000 || arry_file[i] == 0xFFFFFFFF){
                        invalidNumber++;
                        fwrite(&arry_file[i], handle_buffer, 1, OutputFile);
                }
                else{
                        arry_reverseA(&arry_file[i], handle_buffer, OutputFile);
                }
        }
        printf("arry invalid number: %d \n", invalidNumber);
        //int number_write = fwrite(arry_file, handle_buffer, arry_leng, OutputFile);
        //printf("file write: %d byte\n", number_write * handle_buffer);  
        fclose(InputFile);
        fclose(OutputFile);
}

// 交换bin文件的每4字节 ==> 数据倒序问题：方式C
// 1. 先将文件以4Byte为操作基本单位加载到内存（开辟一个动态数组）
// 2. 以4Byte为基本单位判断数据是否有效，无效则跳过；有效则进行调换重写（交换）
// 总结：该方式速度快（i5 64bit 128MB ==> 0.3s) 原因：a.执行fwrite只需要1次
int file_reverse_4ByteC(char* Infile, char* Outfile){
        int file_leng = file_size(Infile);
        printf("file leng: %d \n",file_leng);
        int arry_leng = 0;
        int handle_buffer = sizeof(unsigned int);
        if(file_leng % handle_buffer)
                arry_leng = (file_leng / handle_buffer) + handle_buffer;
        else
                arry_leng = (file_leng / handle_buffer);
        printf("arry leng: %d \n",arry_leng);
        unsigned int* arry_file = (unsigned int*)malloc(handle_buffer * arry_leng);
        FILE* InputFile = fopen(Infile, "rb+");
        FILE* OutputFile = fopen(Outfile, "wb+");
        if( InputFile == NULL ){
                printf("%s, %s",Infile,"not exit/n");
                exit(1);
        }
        int number_read = fread( arry_file, handle_buffer, arry_leng, InputFile);
        printf("file read: %d byte\n", number_read * handle_buffer);
        int invalidNumber = 0;
        //unsigned char* arry_file_addr = (void*)arry_file;
        //printf("print addr: %p data: %X \n", arry_file_addr, *arry_file_addr);
        for(int i = 0; i < arry_leng; i++){
                if(arry_file[i] == 0x00000000 || arry_file[i] == 0xFFFFFFFF){
                        invalidNumber++;
                }
                else{
                        arry_reverseC(&arry_file[i], handle_buffer);
                }
        }
        printf("arry invalid number: %d \n", invalidNumber);
        int number_write = fwrite(arry_file, handle_buffer, arry_leng, OutputFile);
        printf("file write: %d byte\n", number_write * handle_buffer);
        fclose(InputFile);
        fclose(OutputFile);
}


int arry_reverseA(unsigned int* arry_addr,int leng, FILE* file)
{
        unsigned char* arry_addrByte = (void*)arry_addr;
        //unsigned int dataTemp = *arry_addr;
        fwrite(arry_addrByte+3, 1, 1, file);
        fwrite(arry_addrByte+2, 1, 1, file);
        fwrite(arry_addrByte+1, 1, 1, file);
        fwrite(arry_addrByte, 1, 1, file);
}

int arry_reverseC(unsigned int* arry_addr,int leng)
{
        unsigned char* arry_addrByte = (void*)arry_addr;
        unsigned int dataTemp = *arry_addr;
	unsigned char* arry_addrByteTemp = (void*)&dataTemp; 
        for (int i = 0; i < leng; ++i)
		arry_addrByte[i] = arry_addrByteTemp[leng-1-i];
}

int checkSum(char* File)
{
        int file_leng = file_size(File);
        int arry_leng = 0;
        int handle_buffer = sizeof(unsigned int);
        if(file_leng % handle_buffer)
                arry_leng = (file_leng / handle_buffer) + handle_buffer;
        else
                arry_leng = (file_leng / handle_buffer);
        unsigned int* arry_file = (unsigned int*)malloc(handle_buffer * arry_leng);
        FILE* InputFile = fopen(File, "rb+");
        if( InputFile == NULL ){
                printf("%s, %s",File,"not exit/n");
                exit(1);
        }
        int number_read = fread( arry_file, handle_buffer, arry_leng, InputFile);
	unsigned char* arry_addrByte = (void*)arry_file;
	int checksum = 0;
	for(int i = 0; i < arry_leng; i++)
		checksum = checksum + arry_addrByte[i];
	fclose(InputFile);
	return checksum;
}
