#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "time_functions.h"
//#include "setpath_defs.h"
#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#endif

FILE* infile;
FILE* outfile;

#define bufferSize 1000
#define bufferCount 10

pthread_t thread1, thread2;

sem_t s;
sem_t n;
sem_t e;

void buffer_ptr_init();

char buffer[bufferCount][bufferSize];
char *buffer_ptr[bufferCount];
int buffer_index = 0;

char pro_buff[bufferSize], con_buff[bufferSize] ;

void* producer(void* ptr);
void* consumer(void* ptr);

void produce();
void consume();

void append();
void take();

int countlines();
int lineCount;

int x=0; int y=0;
	
int main(int argc, char** argv) {

////////////////////////////////////////////////////////////
#ifdef _WIN32  //Windows
	char in_path[] = "C:\\temp\\coursein\\p2-in.txt";
	char out_path[] = "C:\\temp\\courseout\\p2-out.txt";
#else //Linux
	// For Linux, the "home" directory actually contains the current userid
	// so we have to get that name from the system and put it in front of the path.
	// Many Linux shells will only expand the "~" if it is used from the command line
	char in_path[200];
	char out_path[200];
	const char *homedir;
	homedir = getenv("HOME");
	if (homedir != NULL)
		homedir = getpwuid(getuid())->pw_dir;
	// get the actual home directory for the current user
	strcpy(in_path, homedir);
	strcpy(out_path, homedir);
	// now add on the common part of the path
	strcat(in_path,"/temp/coursein/p2-in.txt");
	strcat(out_path,"/temp/courseout/p2-out.txt");
#endif
	infile = fopen(in_path,"rb");
	if (infile==NULL) {
		printf("Input file could not be opened. Stopping.\n");
		return 0;
	}

	for (int i = 0; i < 200; ++i)
	{
		printf("%c",(char)in_path[i]);
	}


	outfile = fopen(out_path,"wb");
	if (outfile==NULL) {
		printf("Output file could not be opened. Stopping.\n");
		return 0;
	}
// Now you can use the names, infile and outfile in your fopen, input and output statements

	//setpath();

    lineCount = countlines();
    infile = fopen(in_path,"rb");

    printf("infile line count: %d\n", lineCount);
    
	buffer_ptr_init();
	
	sem_init(&s, 0, 1);
	sem_init(&n, 0, 0);
	sem_init(&e, 0, 10);

	//pthread_t producer, consumer;
	pthread_create( &thread1, NULL, producer, NULL);
	pthread_create( &thread2, NULL, consumer, NULL);

	start_timing();
	/**** TIMED PORTION OF CODE ****/
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	
	fclose(infile);
    fclose(outfile);

	stop_timing();
	printf("The time from 'ftime' is %10.3f seconds\n",get_wall_clock_diff());
	printf("The time from 'CPUtime' is %10.3f seconds\n",get_CPU_time_diff());

	return 0;
}

void* producer(void* ptr) {
	printf("Producer thread starting...\n");
	int toRead = lineCount;
	printf("File to copy contains %d lines\n",toRead);
	while(toRead > 0) {
		printf("To produce: %d\n",toRead);
		produce();
		//printf("Produced\n");
		sem_wait(&e);
		sem_wait(&s);
		//printf("Appending\n");
		append();
		sem_post(&s);
		sem_post(&n);
		toRead--;
		//printf("Read %d lines\n",toRead);
	}

	printf("Producer thread exiting...\n");

	pthread_exit(0);
}

void* consumer(void* ptr) {
	int writtencount = 0;
	printf("Consumer thread staring...\n");
	int toWrite = lineCount;
	printf("Consumer needs to write %d lines\n",toWrite);
	fseek(outfile, 0, SEEK_SET);
	while(toWrite > 0) {
		sem_wait(&n);
		sem_wait(&s);
		//printf("Taking\n");
		take();
		sem_post(&s);
		sem_post(&e);
		printf("To consume: %d\n",toWrite);
		consume();	
		toWrite--;
		writtencount++;
		//printf("Wrote %d lines\n",toWrite);
	}
	printf("Consumer thread exiting after writing %d lines...\n",writtencount);

	pthread_exit(0);
}

void produce() {

	//fgets(pro_buff, bufferSize, infile);

/* Debug
	printf("pro_buff contains:\n");
	for (int i = 0; i < bufferSize; ++i)
	{
		printf("%c",pro_buff[i]);
	}
	printf("\n");
*/
	fread(&pro_buff, bufferSize, 1, infile);
}

void consume() {
	//fputs(con_buff, outfile);
	fwrite(&con_buff, bufferSize, 1, outfile);
}

void append() {
	printf("Writing to buffer slot %d\n",x);
	for(int i=0; i<bufferSize; i++) {
		buffer[x][i] = pro_buff[i];
	}
	x++;
	x = x%10;
}

void take() {
	printf("Reading from buffer slot %d\n",y);
	for(int i=0; i<bufferSize; i++) {
		con_buff[i] = buffer[y][i];
	}
	y++;
	y=y%10;
	//printf("y=%d\n",y);
}

void buffer_ptr_init() {
	for(int i=0; i<bufferCount; i++) {
		buffer_ptr[i] = &buffer[i][0];
		printf("Ptr to buffer %d: %p\n",i,buffer_ptr[i]);
	}
}

int countlines() { 
  int ch=0;
  int lines=0;

  if (infile == NULL) {
  	printf("Line count failed\n");
    	return 0;
  }

  lines++;
  while ((ch = fgetc(infile)) != EOF)
    {
      if (ch == '\n')
    lines++;
    }
  fclose(infile);
  return lines;
}

//#include "setpath_fn.h"