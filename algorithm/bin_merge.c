#include "bin_merge.h"


char *functionModule[4]={"NOR","NAND","NAND_BOOT","NAND_OOB"};

int mergeImage(char *appName)
{
	char *getValue = NULL;
	for(int i=0;i<4;i++){
		getValue = profile_getValue(appName,NULL,functionModule[i]);
		if(getValue == NULL){
			ErrorPrintf("module error \n");
		}
		else if(*getValue == '1')
			DebugPrintf("module [ %s ] function is ENABLE\n",functionModule[i]);
	}
	return 1;
}
