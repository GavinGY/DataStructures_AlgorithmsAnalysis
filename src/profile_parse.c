/**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*
#  > Author  ： Gavin | Zhang GuiYang
#  > Mail    ： gavin.gy.zhang@gmail.com
#  > Date    ： Dec/25/2018
#  > Company ： Foxconn·CNSBG·CPEGBBD·RD
#  > Funciton:  profile parse function
#  > Version :  v1.0 
#  > HowToUse:  -
# *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*/

#include "../include/profile_parse.h"

#define ElementNumber        (2)
#define MaxmoduleNumTempber  (5)
#define MaxLineSize          (150)

#define tagComment           '#'
#define tagAppL              '<'
#define tagAppR              '>'
#define tagModuleL           '['
#define tagModuleR           ']'
#define tagKey               '='
#define tagValue             '"'

char ****keyElement = NULL; 
char *moduleTemp[ElementNumber][MaxmoduleNumTempber];//moduleTemp[0]==>Name; moduleTemp[1] Enable==>Key
int moduleNumTempVaild = 0, moduleNum = 0, appKeyNumber=0, moduleKeyNumber[MaxmoduleNumTempber];


// 配置文件初始化 KEY 和 value 保存
int profileGetKey(char **linepos, char **keyName, char **keyValue)
{
	char *line = *linepos;
	*keyName = line;
	while(line[0] != tagKey){
		line++;
		if(isspace(line[0])) 
			break;
	}
	line[0] = tagSpaceNULL; 
	
	while(line[0] != tagValue)
		line++;
	line++;
	while(isspace(line[0]))
		line++;
	*keyValue = line;
	char *temp = strchr(line, tagValue);
	if(temp == NULL)
		return -3;
	temp[0] = tagSpaceNULL;
	return 1;
}

// 配置文件初始化
int profile_init(char *profileName, char *appNameInput)
{
	FILE *cfg_file = NULL;
	int lineNumber = 0,lineLength = 0,lineNumberHandle = 0,lineNumberTemp = 0,lineNumberTemp1 = 0;
	int validLineStart = 0,validLineEnd = 0,save_i = 0,save_j = 0,save_k = 0,moduleNumTemp = 0;
	char appVaildFlag = DISABLE, moduleVaildFlag = DISABLE, moduleFlag = DISABLE, firstInit = ENABLE,lineContent[MaxLineSize + 2];
	char *lineContentBufer,*lineElement,*keyName,*keyValue,*appNameTemp = NULL;

	InfoPrintf("Profile Name: %s, Target APP: %s \n", profileName, appNameInput);
	cfg_file = fopen(profileName, "r");
	if(cfg_file == NULL)
		goto EXIT1;
	if(appNameInput == NULL)
		goto EXIT2;

AGAIN:
	while(fgets(lineContent, sizeof(lineContent), cfg_file) != NULL) {
		lineNumber++;
		lineNumberHandle++;
		
		/* Initialization complete, Skip invalid line. */
		if(firstInit == DISABLE){
			if(lineNumber < validLineStart)
				continue; /* skip invalid line ahead of target appName. */
			else if(lineNumber > validLineEnd)
				break; /* over read from profile */
		} 
		
		lineContentBufer = lineContent;
		lineLength = strlen(lineContent);
		/* judge the line length */
		if(lineLength > MaxLineSize){
			ErrorPrintf("line too long, can't over %d char, profile line skipped %s, line:%d\n", MaxLineSize, profileName, lineNumber);
			continue;
		}
		
		/* eat the whitespace, table symbol, and skip blank line*/
		while((lineLength > 0) && isspace(lineContentBufer[0])) {
			lineContentBufer++;
			lineLength--;
		}
		if(lineLength == 0){
			lineNumberHandle--;
			continue;
		}
			
		
		/* see if this is a comment line */
		if(lineContentBufer[0] == tagComment){
			lineNumberHandle--;
			continue;
		}
		
		/* judge APP name and save to temp variable, skip invalid line*/			
		if(appVaildFlag == DISABLE){
			/* skip invalid line ahead of target appName. */
			if(lineContentBufer[0] == tagAppL){ 
				lineContentBufer++;			 
				while(isspace(lineContentBufer[0]))
					lineContentBufer++;
				lineElement = lineContentBufer;
				while(lineContentBufer[0] != tagAppR){
					lineContentBufer++;
					if(isspace(lineContentBufer[0])) // skip blank Character behind of appName.
						break;
				}
				lineContentBufer[0] = tagSpaceNULL; /* terminate target Element(str) */
				if(!strcasecmp(lineElement,appNameInput)){   /* Find Target APP name successful */
					appNameTemp  =(char *)malloc(strlen(lineElement));
					strncpy(appNameTemp, lineElement, strlen(lineElement));
					appVaildFlag = ENABLE; 
					validLineStart = lineNumber + 1;
					lineNumberTemp1 = lineNumberHandle;
					continue;
				}
				else
					continue;
			}
			else
				continue;
		}/* over first read from profile, once find next APP name. */
		else if((lineContentBufer[0] == tagAppL) && (appVaildFlag == ENABLE)){ 
			moduleKeyNumber[moduleNumTemp-1] = lineNumberHandle - lineNumberTemp -2;
			validLineEnd = lineNumber - 1;
			if(moduleNumTemp >= MaxmoduleNumTempber)
				goto EXIT4;
			moduleNum = moduleNumTemp;
			/* 申请前三级指针变量的内存空间，用于保存Key */
			keyElement = (char ****)malloc((moduleNumTempVaild+1) * sizeof(char***));// 多一个用于存放APP的全局KEY元素
			for(int i=0; i<=moduleNumTempVaild; i++)
				keyElement[i] = (char ***)malloc(ElementNumber * sizeof(char**));
			for(int i=0; i<ElementNumber; i++)
				keyElement[0][i] = (char **)malloc((1 + moduleNum + appKeyNumber) * sizeof(char*));
			int k=0;
			for(int i=1; i<=moduleNumTempVaild; i++){
				for(int j=0; j<ElementNumber; j++){
					while(*moduleTemp[1][k]==DISABLE)
						k++;
					keyElement[i][j] = (char **)malloc(moduleKeyNumber[k] * sizeof(char*));
				}
				k++;
			}
			/* 申请第四级指针变量的内存空间，保存APP基本信息(appName, moduleName, moduleEnable) */
			keyElement[0][0][0] = (char *)malloc(strlen(appNameTemp));
			strncpy(keyElement[0][0][0], appNameTemp, strlen(appNameTemp));
			for(int i=1;i <=moduleNum;i++){
				keyElement[0][0][i] = (char *)malloc(strlen(moduleTemp[0][i-1]));
				strcpy(keyElement[0][0][i], moduleTemp[0][i-1]);
				keyElement[0][1][i] = (char *)malloc(strlen(moduleTemp[1][i-1]));
				strcpy(keyElement[0][1][i], moduleTemp[1][i-1]);
			}
			/* 变量重新初始化 */		
			firstInit = DISABLE;
			moduleNumTemp = 0;
			moduleVaildFlag = ENABLE;
			lineNumber = 0;
			moduleNumTempVaild = 0;
			fclose(cfg_file);
			cfg_file = fopen(profileName, "r");
			goto AGAIN;
		}

		/* judge Module Enable value, skip invalid line*/
		if(lineContentBufer[0] == tagModuleL){ 
			if(firstInit == ENABLE){
				lineContentBufer++;		
				while(isspace(lineContentBufer[0]))
					lineContentBufer++;
				lineElement = lineContentBufer;	
				while(lineContentBufer[0] != tagModuleR){
					lineContentBufer++;
					if(isspace(lineContentBufer[0])) // skip blank Character behind of Module Name.
						break;
				}
				lineContentBufer[0] = tagSpaceNULL;
				moduleTemp[0][moduleNumTemp] = (char *)malloc(strlen(lineElement));
				strncpy(moduleTemp[0][moduleNumTemp], lineElement, strlen(lineElement));
				//DebugPrintf("moduleName:%s\n",moduleTemp[0][moduleNumTemp]);
				if(appKeyNumber == 0)
					appKeyNumber = lineNumberHandle - lineNumberTemp1 - 1;
				if(lineNumberTemp)
					moduleKeyNumber[moduleNumTemp-1] = lineNumberHandle - lineNumberTemp -2;
				lineNumberTemp = lineNumberHandle;
			}
			moduleFlag = ENABLE;
			continue;
		}	
		if(moduleFlag == ENABLE){
			if(firstInit == ENABLE){
				while(lineContentBufer[0] != tagValue)
					lineContentBufer++;	
				lineContentBufer++;			
				while(isspace(lineContentBufer[0]))
					lineContentBufer++;
				moduleTemp[1][moduleNumTemp] = (char *)malloc(1);
				strncpy(moduleTemp[1][moduleNumTemp], &lineContentBufer[0], 1);
			}
			if(*moduleTemp[1][moduleNumTemp] == ENABLE){	
				moduleVaildFlag = ENABLE;
				moduleNumTempVaild++;
			}
			else
				moduleVaildFlag = DISABLE;
			moduleNumTemp++;
			moduleFlag = DISABLE;
		}
		
		if(moduleVaildFlag == DISABLE)
			continue;
		
		if(firstInit == ENABLE)
			continue;
		
		/* ------------------------------ The work of the second processing ---------------------------------- */
		
		int returnNum = profileGetKey(&lineContentBufer, &keyName, &keyValue);
		if(returnNum == -3)
			goto EXIT3;
		
		if(!strcasecmp(keyName,"Enable")){
			save_j++;
			save_k=0;
			continue;
		}
		
		/* save APP top keyName and keyValue */
		if(save_j == 0){
			keyElement[0][0][1 + moduleNum + save_i] = (char *)malloc(strlen(keyName));
			strcpy(keyElement[0][0][1 + moduleNum + save_i], keyName); 
			keyElement[0][1][1 + moduleNum + save_i] = (char *)malloc(strlen(keyValue)+50);
			//上面加50的的目的是为了方便使用strcat进行字符串拼接；因为此处经常是一个文件夹路径字符串需要拼接具体文件名字符串，根据库函数strcat函数实现特点，会将连接字符串copy到目标字符串后面
			strcpy(keyElement[0][1][1 + moduleNum + save_i], keyValue); 
			save_i++;
			continue;
		}
		
		/* save each module keyName and keyValue */
		keyElement[save_j][0][save_k] = (char *)malloc(strlen(keyName));
		strcpy(keyElement[save_j][0][save_k], keyName);
		keyElement[save_j][1][save_k] = (char *)malloc(strlen(keyValue));
		strcpy(keyElement[save_j][1][save_k], keyValue); 
		save_k++;
	}
	
	/* can't find valid APP name on profile */
	if(appNameTemp  == NULL)
		goto EXIT2;
	if(!validLineEnd)
		goto EXIT3;

	fclose(cfg_file);	
	return 1;

EXIT1:	
	ErrorPrintf("can't open '%s' as config file, errno = %d, reason = %s \n", profileName,  errno, strerror(errno));
	return -1;
EXIT2:
	ErrorPrintf("APP: %s was not found, Please check APP Name define in profile or check spelling(ignore case)\n",appNameInput);
	fclose(cfg_file);
	return -2;
EXIT3:
	ErrorPrintf("Profile format is not correct, please check line: %d\n",lineNumber);
	fclose(cfg_file);
	return -3;
EXIT4:
	ErrorPrintf("Profile format is not correct, Each APP supports up to %d modules\n",MaxmoduleNumTempber);
	fclose(cfg_file);
	return -4;
}


char *profile_getValue(char *appName, char *moduleName, char *key_Name)
{
	if(keyElement == NULL){
		ErrorPrintf("keyElement is NULL!\n");
		return 0;
	}
	int value_i=1,temp_i=0;
	
	/* check appName */
	if(strcasecmp(keyElement[0][0][0],appName)){//  *keyElement[0][0][0] != *appName
		ErrorPrintf("APP call Failed !!! The loading APP is: %s , please check the APP name of you input.\n",keyElement[0][0][0]);
		return NULL;
	}
	
	/* find APP key name and return value, moduleName and Enable status */
	if(moduleName == NULL){
		for(int i = 1;i <= moduleNum + appKeyNumber;i++){
			if(!strcasecmp(keyElement[0][0][i],key_Name))
				return keyElement[0][1][i];
		}
		ErrorPrintf("can't find this APP Key name !!! please check of you input.\n");
		return NULL;
	}
	
	/* check moduleName and count the array number */
	for(int i = 1;i <= moduleNum;i++){
		if(strcasecmp(keyElement[0][0][i],moduleName)){
			if(*keyElement[0][1][i] == ENABLE)
				value_i++;
		}
		else{
			if(*keyElement[0][1][i] != ENABLE){
				ErrorPrintf("This module function is DISABLE, if you want to using, please ENABLE it on profile.\n");
				return NULL;
			}
			else{ 
				/* find module key name and return value */
				for(int j = 0;j < moduleKeyNumber[i-1];j++){
					if(!strcasecmp(keyElement[value_i][0][j],key_Name))
						return keyElement[value_i][1][j];
				}
				ErrorPrintf("Key Name Find Failed !!! please check the key name of you input.\n");
				return NULL;
			}
		}
	}
	ErrorPrintf("moduleNum call Failed !!! please check the module name of you input.\n");
	return NULL;
}

int profile_getALL(void)
{
	if(keyElement == NULL){
		ErrorPrintf("keyElement is NULL!\n");
		return 0;
	}
	
	/* get APP basic info */
	for(int i=0;i < (1+ moduleNum + appKeyNumber);i++)
		printf("%s, %s\n",keyElement[0][0][i],keyElement[0][1][i]);
	
	/* get module key info */
	int l=0;
	for(int i=1;i <= moduleNumTempVaild;i++){
		while(*moduleTemp[1][l]==DISABLE)
			l++;
		for(int k = 0;k < moduleKeyNumber[l];k++)
			printf("%s - %s\n",keyElement[i][0][k],keyElement[i][1][k]);
		l++;
	}
	
	return 1;
}

void FreeGrid2(char ****p,int m,int n,int t1,int *t2,char *arg[ElementNumber][MaxmoduleNumTempber])
{
    if(p != NULL){
		if(p[0] != NULL){
			for(int i = 0;i < n;i++){
				if(p[0][i] != NULL){
					for(int j = 0;j < t1;j++){
						if(p[0][i][j] != NULL){
							free(p[0][i][j]);
							p[0][i][j] = NULL;
						}
					}
					free(p[0][i]);
					p[0][i] = NULL;
				}
			}
			free(p[0]);
			p[0] = NULL;
		}
		
		int l=0;
		for(int i = 1;i <= m;i++){
			if(p[i] != NULL){
				for(int j = 0;j < n;j++){
					if(p[i][j] != NULL){
						while(*arg[1][l]==DISABLE)
							l++;
						for(int k = 0;k < t2[l];k++){
							if(p[i][j][k] != NULL){
								free(p[i][j][k]);
								p[i][j][k] = NULL;
							}
						}
						free(p[i][j]);
						p[i][j] = NULL;
					}
				}
				l++;
				free(p[i]);
				p[i] = NULL;
			}
		}
        free(p);
        p = NULL;
		DebugPrintf("Release1 OK!\n");
    }
	
	DebugPrintf("Release arg: ");
	for(int i=0;i<ElementNumber;i++){
		for(int j=0;j<MaxmoduleNumTempber;j++){
			if(arg[i][j] != NULL){
				free(arg[i][j]);
				arg[i][j] = NULL;
				//printf("arg[%d][%d],",i,j);
			}
		}
	}
	DebugPrintf("OK!\n");
}


int profile_release(void)
{
	FreeGrid2(keyElement,moduleNumTempVaild,ElementNumber,(1 + moduleNum + appKeyNumber), moduleKeyNumber, moduleTemp);
    keyElement = NULL;
}

