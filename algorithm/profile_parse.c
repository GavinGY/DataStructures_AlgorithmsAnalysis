#include "profile_parse.h"

// 参考：https://blog.csdn.net/crazycoder8848/article/details/7097088


#define MAX_VAR_NUM			(50)
#define	MAX_VAR_NAME_LEN	(128)
#define MAX_VAR_VALUE_LEN	(MAX_PATH_LEN)

#define COMMENT_CHARACTER	'#'
#define LINE_SIZE			(768)



char key_Name[MAX_VAR_NUM][MAX_VAR_NAME_LEN + 1];
char key_Value[MAX_VAR_NUM][MAX_VAR_VALUE_LEN + 1];
int g_var_num = 0;

void get_dir_path_of_file(char *file, char *dir_path)
{
	char *temp;
	strncpy(dir_path, file, MAX_PATH_LEN);
	temp = strrchr(dir_path, '/');
	temp[0] = '\0';
}

void remove_trailing_chars(char *path, char c)
{
	size_t len;
	len = strlen(path);
	while (len > 0 && path[len-1] == c)
	path[--len] = '\0';
}

int getkey_Value(char **line, char **key, char **value)
{
	char *linepos;
	char *temp;

	linepos = *line;
	if (!linepos){
		printf("error1 %s\n",linepos);
		return -1;
	}
	
	/* skip whitespace */
	while (isspace(linepos[0]))
		linepos++;
	
	if (linepos[0] == '\0'){
		printf("error2 %s\n",&linepos[0]);
		return -1;
	}
	/* get the key value */
	*key = linepos;
	while (1) {
		linepos++;
		if (linepos[0] == '\0')
		{
			printf("error3 %s\n",&linepos[0]);
			return -1;
		}
		if (isspace(linepos[0]))
			break;
		if (linepos[0] == '=')
			break;
	}

	/* terminate key */
	linepos[0] = '\0';

	while(1){
		linepos++;
		if (linepos[0] == '\0')
		{
			printf("error4 %s\n",&linepos[0]);
			return -1;
		}
		if (isspace(linepos[0]))
			continue;
		if (linepos[0] == '=')
			continue;
		break;
	}	
	
	/* get the value */
	if (linepos[0] == '"'){ 
		linepos++;
	}
	else{
		printf("error5 ASCII code: %d\n",linepos[0]);
		return -1;
	}
	*value = linepos;
	printf("Gavin Debug: %s\n",*value);
	temp = strchr(linepos, '"');;
	if (!temp){
		printf("error6 %s\n",temp);
		return -1;
	}
	temp[0] = '\0';
	return 0;
}

int parse_config_file(char *path_to_config_file)
{
	char line[LINE_SIZE + 2];
	char *bufline;
	char *linepos;
	char *variable;
	char *value;
	char *buf;
	size_t bufsize;
	size_t cur;
	size_t count;
	int lineno;
	int retval = 0;

	FILE *cfg_file = fopen(path_to_config_file, "r");
	if (NULL == cfg_file) {
		Msg_Error("can't open '%s' as config file: %s", path_to_config_file, strerror(errno));
		goto EXIT;
	}

	/* loop through the whole file */
	lineno = 0;
	cur = 0;
	while (NULL != fgets(line, sizeof(line), cfg_file)) {
		lineno++;
		bufline = line;
		count = strlen(line);

		if (count > LINE_SIZE) 
		{
			Msg_Error("line too long, conf line skipped %s, line %d", path_to_config_file, lineno);
			continue;
		}

		/* eat the whitespace */
		while ((count > 0) && isspace(bufline[0])) {
			bufline++;
			count--;
		}
		if (count == 0)
			continue;

		/* see if this is a comment */
		if (bufline[0] == COMMENT_CHARACTER)
			continue;

		memcpy(line, bufline, count);
		line[count] = '\0';

		linepos = line;
		retval = getkey_Value(&linepos, &variable, &value);
		if (retval != 0) {
			Msg_Error("error parsing %s, line %d:%d\n", path_to_config_file, lineno, (int)(linepos-line));
			continue;
		}
		
		if (g_var_num >= MAX_VAR_NUM){
			Msg_Error("too many vars in  %s, line %d:%d\n", path_to_config_file, lineno, (int)(linepos-line));
			continue;
		}

		if (strlen(variable) > MAX_VAR_NAME_LEN){
			Msg_Error("var name to long %s, line %d:%d\n", path_to_config_file, lineno, (int)(linepos-line));
			continue;
		}

		if (strlen(value) > MAX_VAR_VALUE_LEN){
			Msg_Error("value to long %s, line %d:%d\n", path_to_config_file, lineno, (int)(linepos-line));
			continue;
		}

		strncpy(key_Name[g_var_num], variable, sizeof(key_Name[g_var_num]));
		remove_trailing_chars(value, '/');
		strncpy(key_Value[g_var_num], value, sizeof(key_Value[g_var_num]));
		g_var_num++;
		continue;
	}
EXIT:
	fclose(cfg_file);
	return g_var_num;
}

char *get_config_var(char *var_name)
{
	for(int i = 0; i < g_var_num; i++){
		if(strcasecmp(key_Name[i],var_name)==0){
			return key_Value[i];
		}	
	}
	Msg_Error("get %s failed", var_name);
	return NULL;
}

void print_all_vars()
{
	int i;
	Msg_Error("g_var_num == %d \n", g_var_num);
	for(i = 0; i < g_var_num; i++){
		printf("%s = %s\n", key_Name[i], key_Value[i]);
	}
}

#define ElementNumber        (2)
#define MaxModuleNumber      (5)
#define MaxLineSize          (150)
#define ENABLE               '1'
#define DISABLE              '0'

#define tagComment           '#'
#define tagAppL              '<'
#define tagAppR              '>'
#define tagModuleL           '['
#define tagModuleR           ']'
#define tagKey               '='
#define tagValue             '"'
#define tagSpaceNULL         '\0'

char ****keyElement = NULL; 
char *moduleTemp[2][MaxModuleNumber];//moduleTemp[0]==>Name; moduleTemp[1] Enable==>Key
int moduleNumVaild = 0, moduleNum = 0, appKeyNumber=0, moduleKeyNumber[MaxModuleNumber];


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
	
	// while(line[0] != tagValue){
		// line++;
		// if(isspace(line[0])) 
			// break;
	// }
	// line[0] = tagSpaceNULL; 
	
	return 1;
}

// 配置文件初始化
int profile_init(char *profileName, char *appNameInput)
{
	FILE *cfg_file = NULL;
	int lineNumber = 0,lineLength = 0,lineNumberHandle = 0,lineNumberTemp = 0,lineNumberTemp1 = 0,validLineStart,validLineEnd;
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
		
		/* judge APP name, skip invalid line*/			
		if(appVaildFlag == DISABLE){
			/* skip invalid line ahead of target appName. */
			if(lineContentBufer[0] == tagAppL){ 
				lineContentBufer++;			 
				while(isspace(lineContentBufer[0]))
					lineContentBufer++;
				lineElement = lineContentBufer;
				while(lineContentBufer[0] != tagAppR){
					lineContentBufer++;
					// if(lineContentBufer[0] == tagSpaceNULL) // skip blank Character behind of appName.
						// goto EXIT3;
					if(isspace(lineContentBufer[0])) // skip blank Character behind of appName.
						break;
				}
				lineContentBufer[0] = tagSpaceNULL; /* terminate target Element(str) */
				if(strcasecmp(lineElement,appNameInput)==0){   /* Find Target APP name successful */
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
			moduleKeyNumber[moduleNum-1] = lineNumberHandle - lineNumberTemp -2;
			validLineEnd = lineNumber - 1;
			
			for(int i=0;i < moduleNum;i++)
				DebugPrintf("moduleName:%s, moduleEanble:%s, moduleKeyNumber:%d\n",moduleTemp[0][i],moduleTemp[1][i],moduleKeyNumber[i]);
			DebugPrintf("APP Name:%s, APP Key Number:%d \n",appNameTemp,appKeyNumber);
			DebugPrintf("moduleNumVaild: %d\n",moduleNumVaild);
			/* 申请前三级指针变量的内存空间，用于保存Key */
			keyElement = (char ****)malloc((moduleNumVaild+1) * sizeof(char***));// 多一个用于存放APP的全局KEY元素
			for(int i=0; i<=moduleNumVaild; i++)
				keyElement[i] = (char ***)malloc(ElementNumber * sizeof(char**));
			for(int i=0; i<ElementNumber; i++)
				keyElement[0][i] = (char **)malloc((1+moduleNum+1+appKeyNumber) * sizeof(char*));
			int k=0;
			for(int i=1; i<=moduleNumVaild; i++){
				for(int j=0; j<ElementNumber; j++){
					while(*moduleTemp[1][k]==DISABLE)
						k++;
					keyElement[i][j] = (char **)malloc(moduleKeyNumber[k] * sizeof(char*));
					DebugPrintf("malloc(moduleKeyNumber[k],%d\n",moduleKeyNumber[k]);
				}
				k++;
			}
			/* 申请第四级指针变量的内存空间，保存APP基本信息(appName, moduleName, moduleEnable) */
			char *appNameLable="appName";
			keyElement[0][0][0] = (char *)malloc(strlen(appNameLable));
			strncpy(keyElement[0][0][0], appNameLable, strlen(appNameLable));
			keyElement[0][1][0] = (char *)malloc(strlen(appNameTemp));
			strncpy(keyElement[0][1][0], appNameTemp, strlen(appNameTemp));
			for(int i=1;i <=moduleNum;i++){
				keyElement[0][0][i] = (char *)malloc(strlen(moduleTemp[0][i-1]));
				strncpy(keyElement[0][0][i], moduleTemp[0][i-1], strlen(moduleTemp[0][i-1]));
				keyElement[0][1][i] = (char *)malloc(strlen(moduleTemp[1][i-1]));
				strncpy(keyElement[0][1][i], moduleTemp[1][i-1], strlen(moduleTemp[1][i-1]));
			}
			/* 变量重新初始化 */		
			firstInit = DISABLE;
			lineNumber = 0;
			moduleNumVaild = 0;
			if(moduleNum >= MaxModuleNumber)
				goto EXIT4;
			moduleNum = 0;
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
				moduleTemp[0][moduleNum] = (char *)malloc(strlen(lineElement));
				strncpy(moduleTemp[0][moduleNum], lineElement, strlen(lineElement));
				//DebugPrintf("moduleName:%s\n",moduleTemp[0][moduleNum]);
				if(appKeyNumber == 0)
					appKeyNumber = lineNumberHandle - lineNumberTemp1 - 1;
				if(lineNumberTemp)
					moduleKeyNumber[moduleNum-1] = lineNumberHandle - lineNumberTemp -2;
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
				moduleTemp[1][moduleNum] = (char *)malloc(1);
				strncpy(moduleTemp[1][moduleNum], &lineContentBufer[0], 1);
				//DebugPrintf("moduleEanble:%s\n",moduleTemp[1][moduleNum]);
				DebugPrintf("module number:%d\n",moduleNum);
			}
			if(*moduleTemp[1][moduleNum] == ENABLE){	
				moduleVaildFlag = ENABLE;
				moduleNumVaild++;
			}
			else
				moduleVaildFlag = DISABLE;
			moduleNum++;
			moduleFlag = DISABLE;
		}
		if(moduleVaildFlag == DISABLE)
			continue;
		
		if(firstInit == ENABLE)
			continue;
		
		/* The work of the second processing */
		
		int returnNum = profileGetKey(&lineContentBufer, &keyName, &keyValue);
		if(returnNum == -3)
			goto EXIT3;
		//printf("line number: %d , line head: %s", lineNumber, lineContentBufer);
		printf("keyName:%s, keyValue:%s\n", keyName, keyValue);
		// if(keyValue[0] == tagSpaceNULL)
			// InfoPrintf("keyValue NULL\n");
	
	}
	
	/* can't find valid APP name on profile */
	if(appNameTemp  == NULL)
		goto EXIT2;


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
	ErrorPrintf("Profile format is not correct, Each APP supports up to %d modules\n",MaxModuleNumber);
	fclose(cfg_file);
	return -4;
}


char *profile_getValue(char *appName, char *moduleName, char *key_Name)
{
	
	
}

int profile_getALL(void)
{
	if(keyElement == NULL){
		ErrorPrintf("keyElement is NULL!\n");
		return 0;
	}
	for(int i=0;i <= 4;i++)
		DebugPrintf("%s, %s\n",keyElement[0][0][i],keyElement[0][1][i]);
}

void FreeGrid(char**** p)
{
    if(*p != NULL)
    {
        if(**p != NULL)
        {
			 if(***p != NULL)
			{
				free(***p);
				***p = NULL;
			}
            free(**p);
            **p = NULL;
        }
        free(*p);
        *p = NULL;
    }
    free(p);
    p = NULL;
}

void FreeGrid2(char ****p,int m,int n,int t1,int *t2,char *arg[2][MaxModuleNumber])
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
		DebugPrintf("Release OK!\n");
    }
}


int profile_release(void)
{
	// if(keyElement != NULL){
		// DebugPrintf("Release keyElement ...\n");
		// free(keyElement);
		// keyElement = NULL; //建议free某个指针之后立刻把这个指针赋值为NULL
	// }
	//FreeGrid(keyElement);
	FreeGrid2(keyElement,moduleNumVaild,ElementNumber,(moduleNum + appKeyNumber + 2), moduleKeyNumber, moduleTemp);
    keyElement = NULL;
}




////基于标准库 fgets 函数修改
// char* fgets_line(char *s, int n, FILE *stream)
// {
	// register int c;
	// register char *cs;
	// cs=s;
	// while(--n > 0 && (c = getc(stream)) != EOF){
		// if ((*cs++ = c) =='\n'){
			// break;
		// }
	// }
	// *cs = '\0';
	// return (c == EOF && cs == s) ? NULL : s ;
// }

