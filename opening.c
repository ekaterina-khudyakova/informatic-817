#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv)
{	
	DIR *dir = opendir(argv[1]);
	if(dir == NULL)
	{
		printf("can't open directory\n");
		perror("ERROR 1 ");
		return -1;
	}

	struct dirent *current;
	struct stat statbuf;

	while(current = readdir(dir))
	{
		printf("here we are\n");
		stat(current->d_name, &statbuf);

		printf("%s is dir %d\n", current->d_name, S_ISDIR(statbuf.st_mode));

		if(S_ISDIR(statbuf.st_mode) && (strcmp(".", current->d_name) != 0) && (strcmp("..", current->d_name) != 0) && (strcmp(".git", current->d_name) != 0))
		{
				dir = opendir(current->d_name);
				if(dir == NULL)
				{
					printf("can't open directory %s\n", current->d_name);
					perror("ERROR 2 ");
					exit(-1);
				}
				printf("*** %s\n", current->d_name);
		}
	}

	return 0;
}
