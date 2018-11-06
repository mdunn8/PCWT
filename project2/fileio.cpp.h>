#include <stdio.h>
#include <stdlib.h>
#include <string.h>



FILE* infile;


int main() {
	
	char buffer[500];
	
	char in_path[200];
	//char out_path[200];
	const char *homedir;
	homedir = getenv("HOME");
	//if (homedir != NULL)
	//	homedir = getpwuid(getuid())->pw_dir;
	// get the actual home directory for the current user
	strcpy(in_path, homedir);
	//strcpy(out_path, homedir);
	// now add on the common part of the path
	strcat(in_path,"/temp/coursein/p2-in.txt");
	//strcat(out_path,"/temp/courseout/p2-out.txt");

	infile = fopen(in_path,"rb");
	if (infile==NULL) {
		printf("Input file could not be opened. Stopping.\n");
		return 0;
	}


	//outfile = fopen(out_path,"wb");
	//if (outfile==NULL) {
	//	printf("Output file could not be opened. Stopping.\n");
	//	return 0;
	//}

	
	for (int i = 0; i < 200; ++i)
	{
		printf("%c",(char)in_path[i]);
	}
	

	
	fgets(&buffer, 500, infile);

	for (int i = 0; i < 500; ++i)
	{
		printf("%c",buffer[i] );
	}

	
	fclose(infile);

	
	return(0);
}