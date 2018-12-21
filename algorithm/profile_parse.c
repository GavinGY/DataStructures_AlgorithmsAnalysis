#include "profile_parse.h"


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

#define MaxLineSize          (150)
#define ENABLE               (1)
#define DISABLE              (0)

#define tagComment           '#'
#define tagAppL              '<'
#define tagAppR              '>'
#define tagModuleL           '['
#define tagModuleR           ']'
#define tagKey               '='
#define tagValue             '"'

char *appNameTag = NULL;
char ***keyElement = NULL; 


// 配置文件初始化
int profile_init(char *profileName, char *appName)
{
	FILE *cfg_file = NULL;
	int lineNumber = 0;
	char appVaildFlag = 0, moduleVaildFlag = 0;
	char lineContent[MaxLineSize + 2];
	char *lineContentBufer;
	int  lineLength = 0;
	
	InfoPrintf("Profile Name: %s, Target APP: %s \n", profileName, appName);
	cfg_file = fopen(profileName, "r");
	if (cfg_file == NULL){
		ErrorPrintf("can't open '%s' as config file, errno = %d, reason = %s \n", profileName,  errno, strerror(errno));
		return -1;
	} 

	while(fgets(lineContent, sizeof(lineContent), cfg_file) != NULL) {
		lineNumber++;
		lineContentBufer = lineContent;
		lineLength = strlen(lineContent);
		/* judge the line length */
		if(lineLength > MaxLineSize){
			ErrorPrintf("line too long, conf line skipped %s, line %d\n", profileName, lineNumber);
			continue;
		}
		/* eat the whitespace, table symbol, and skip blank line*/
		while((lineNumber > 0) && isspace(lineContentBufer[0])) {
			lineContentBufer++;
			lineLength--;
		}
		if(lineLength == 0)
			continue;
		
		/* see if this is a comment line */
		if (lineContentBufer[0] == tagComment)
			continue;
		
		/* judge APP name, skip invalid line*/
		if(lineContentBufer[0] == tagAppL && (appVaildFlag == DISABLE)){
			char *linePositionTemp,*linePositionTemp1;
			linePositionTemp = lineContentBufer;
			linePositionTemp1 = linePositionTemp+1;
		    while(linePositionTemp[0] != tagAppR)
				linePositionTemp++;
			int lengthTemp = linePositionTemp - linePositionTemp1;
			appNameTag =(char *)malloc(lengthTemp); 
			strncpy(appNameTag, linePositionTemp1, lengthTemp);
			/* Find Target APP name successful */
			if(strcasecmp(appNameTag,appName)==0){
				appVaildFlag = ENABLE;
				continue;
			}
			else{
				free(appNameTag); 
				appNameTag = NULL;   //建议free某个指针之后立刻把这个指针赋值为NULL
				continue;
			}
		}
		else if((lineContentBufer[0] == tagAppL) && appVaildFlag)
			break;
		else if(appVaildFlag == DISABLE)
			continue;
		
		/* judge Module Enable value, skip invalid line*/
		if(lineContentBufer[0] == tagModuleL && (moduleVaildFlag == DISABLE)){
			moduleVaildFlag = ENABLE;
			continue;
		}
		if(moduleVaildFlag)
		{
			
		}
		
		printf("line number: %d , line head: %s", lineNumber, lineContentBufer);
			
	}
	
	if(appNameTag == NULL){
		ErrorPrintf("APP: %s was not found, Please check APP Name for spelling errors (ignore case)\n",appName);
		fclose(cfg_file);
		return -2;
	}
		
	
EXIT:
	fclose(cfg_file);	
	return 1;
}


char *profile_getValue(char *appName, char *moduleName, char *key_Name)
{
	
	
}

int profile_release(void)
{
	if(appNameTag != NULL){
		DebugPrintf("Release appNameTag ...\n");
		free(appNameTag);
		appNameTag = NULL;  //建议free某个指针之后立刻把这个指针赋值为NULL
	}
	if(keyElement != NULL){
		DebugPrintf("Release keyElement ...\n");
		free(keyElement);
		keyElement = NULL;
	}
}


// char *fgets_line(char *s, int n, FILE *stream);

// int getProfile(char* profile){
	// int fileLeng = file_size(profile);
	// printf("profile leng %d \n", fileLeng);
        // char* arry_file = (char*)malloc(fileLeng);
        // FILE* InputFile = fopen(profile, "rb");
        // if( InputFile == NULL ){
		// printf("%s, %s",profile,"config file not exit/n");
		// exit(1);
	// }
	// printf("test: %d \n",getc(InputFile));
        
	// fgets_line(arry_file, fileLeng, InputFile);
	// printf("%s",arry_file);
	
	// while((fgets_line(arry_file, fileLeng, InputFile)) != NULL){
		// if(strlen(arry_file)!=1)//注意这儿是1不是0，因为尽管是空行，它也会读入换行符，strlen(s)=1;
			// printf("%s",arry_file);
        // }

	// for(int i = 0; i < fileLeng; i++){
		// printf("%s",&arry_file[i]);
	// }
	// fclose(InputFile);
	// free(arry_file);
// }

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

