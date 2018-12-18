#include "profile_parse.h"



int getProfile(char* profile){
	int fileLeng = file_size(profile);
        char* arry_file = (char*)malloc(fileLeng);
        FILE* InputFile = fopen(profile, "rb");
        if( InputFile == NULL ){
		printf("%s, %s",profile,"config file not exit/n");
		exit(1);
	}
        fread(arry_file, 1, fileLeng, InputFile);
	// fscanf(InputFile, "%s", arry_file);
	for(int i = 0; i < fileLeng; i++){
		printf("%d",arry_file[i]);
	}
	fclose(InputFile);
	free(arry_file);
}
