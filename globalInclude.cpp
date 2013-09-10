#include "globalInclude.h"
#include "errno.h"

int frameWidth, frameHeight;
int GridRows = 6, GridCols = 6;

void programPause()
{
	printf("press enter key to continue\n");
	while(1)
	{
		char c = getchar();
		if(c != '\n')
			sleep(120);
		else
			break;
	}
}

void printUsedTime(time_t startTime)
{
	time_t endTime = time(0);
	int totalUsed = endTime - startTime;
	printf("%d\n", totalUsed);
}

int countWords(const char* filename)
{
	char cmdline[500];
	sprintf(cmdline, "cat %s | wc -w", filename);
	printf("cmdline: %s\n", cmdline);
	FILE* pipe = popen(cmdline, "r");
	if(!pipe)
	{
		perror("pipe is null!\n");
		printf("%s\n", strerror(errno));
		exit(1);
	}
	char buffer[100];
	fgets(buffer, sizeof(buffer), pipe);
	if(pclose(pipe) == -1)
	{
		perror("close pipe failed!\n");
		exit(1);
	}
	printf("buffer: %s\n", buffer);
	int n = atoi(buffer);
	return n;
}

int countLines(const char* filename)
{
	char cmdline[500];
	sprintf(cmdline, "cat %s | wc -l", filename);
	FILE* pipe = popen(cmdline, "r");
	if(!pipe)
	{
		perror("pipe is null!\n");
		exit(1);
	}
	char buffer[100];
	fgets(buffer, sizeof(buffer), pipe);
	if(pclose(pipe) == -1)
	{
		perror("close pipe failed!\n");
		printf("%s\n", strerror(errno));
		exit(1);
	}
	int n = atoi(buffer);
	return n;
}