#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

void Split(char* string, char* delimiters, int* count, char** words)
{
	int i = 0;
	char* buf = (char*)calloc(100, sizeof(char));

	buf = strtok(string, delimiters);

	while(buf != NULL && i < *count)
	{
		strncpy(words[i], buf, strlen(buf));
//		printf("%d: %s\n", i, words[i]);
		i++;
		buf = strtok(NULL, delimiters);
	}
	words[*count] = NULL;
//	free(buf);
}

int NumberOfWords(char *string, char *delimiters)
{
	int number = 0;
	int i;
	char *pointer = NULL;

	for(i = 0; i < 2; ++i)
	{
		pointer = strchr(string, delimiters[i]);
		while(pointer != NULL)
		{
			++number;
			pointer = strchr(pointer + 1, delimiters[i]);
		}
	}
	return number;
}

int main()
{
	FILE *file = fopen("./useless.txt", "r");
	if(file == NULL)
	{
		printf("can't open file :(\n");
		return -1;
	}
	int progNumber;
	int i;

	char *buf = (char *)calloc(100, sizeof(char));
	char delimiters[] = {' ', '\n'};

	progNumber = atoi(fgets(buf, 100, file));
//	printf("progNumber=%d\n", progNumber);
	

	for(i = 0; i < progNumber; ++i)
	{
		fgets(buf, 100, file);
//		printf("%s\n", buf);

		pid_t pid = fork();
		if(pid == 0)
		{
			int argNumber = NumberOfWords(buf, delimiters);
//			printf("argNumber=%d\n", argNumber);

			int j;
			char **arguments = (char **)calloc(argNumber, sizeof(char *));
			for(j = 0; j < argNumber; ++j)
				arguments[j] = (char *)calloc(10, sizeof(char));
			int time = 0;

			Split(buf, delimiters, &argNumber, arguments);
//			for(j = 0; arguments[j]; j++)
//				printf("'%s'\n", arguments[j]);

			time = atoi(arguments[0]);
			sleep(time);
			execvp(arguments[1], arguments + 1);
			printf("error\n");
			exit(1);
		}
	}

//	free(buf);
	fclose(file);
	return 0;
}

