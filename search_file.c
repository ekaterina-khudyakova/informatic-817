#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void find(DIR *dir, char *dirname, char *file, int counter, char **results, int *i)
{
	printf("start, %d\n", counter);
	if(counter == 0)
	{
		printf("finish\n");
		exit(0);
	}

	struct dirent *current;
	struct stat statbuf;
	char path[300];

	while(current = readdir(dir))
	{
		if(strcmp(file, current->d_name) == 0)
		{
			sprintf(path, "%s/%s", dirname, current->d_name);
			strcpy(results[*i], path);
			printf("find file #%d\n", *i);
			*i = *i + 1;
		}
			
		stat(current->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode) && (strcmp(".", current->d_name) != 0) && (strcmp("..", current->d_name) != 0) && (strcmp(".git", current->d_name) != 0))
		{
			DIR *nextdir = opendir(current->d_name);
			find(nextdir, current->d_name, file, counter - 1, results, i);
		}
	}
	
}

int main(int argc, char **argv)
{

	char **fullpath = (char **)calloc(10, sizeof(char *));
	int i, j;
	for(i = 0; i < 10; i++)
		fullpath[i] = (char *)calloc(300, sizeof(char));
	
	DIR *dir = opendir(argv[2]);
	if(dir == NULL)
		printf("can't open directory\n");

	int limit = atoi(argv[3]);
	i = 0;
	find(dir, argv[2], argv[1], limit, fullpath, &i);

	for(j = 0; j < i; j++)
		printf("%s\n", fullpath[j]);

	closedir(dir);
	
	return 0;
}
