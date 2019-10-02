#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void ToRead(int id, int *fd, char **file, char *rdstring)
{
	int size;
	fd[(id + 1) % 2] = open(file[(id + 1) % 2], O_RDONLY);
	if(fd[(id + 1) % 2] < 0)
	{
		printf("can't get messages\n");
		exit(-1);
	}
	else
	{
		while(1)
		{
			size = read(fd[(id + 1) % 2], rdstring, 100);
			printf("%s\n", rdstring);
		}
	}
}

void ToWrite(int id, int *fd, char **file, char *wrstring)
{
	int size;
	fd[id] = open(file[id], O_WRONLY);
	if(fd[id] < 0)
	{
		printf("can't send messages\n");
		exit(-1);
	}
	else
	{
		while(1)
		{
			fgets(wrstring, 100, stdin);
			size = write(fd[id], wrstring, 100);
		}
	}
}

int main(int argc, char *argv[])
{
	int fd[2], id;
	char **file = (char **)calloc(2, sizeof(char *));
	file[0] = "file0";
	file[1] = "file1";
	char *wrstring = (char *)calloc(100, sizeof(char));
	char *rdstring = (char *)calloc(100, sizeof(char));

	(void)umask(0);

	if(mknod("file0", S_IFIFO | 0666, 0) < 0)
	{
		printf("can't creat file0\n");
//		exit(-1);
	}
	if(mknod("file1", S_IFIFO | 0666, 0) < 0)
	{
		printf("can't creat file1\n");
//		exit(-1);
	}
	
	id = atoi(argv[1]);
	
	pid_t pid = fork();

	if(pid < 0)
	{
		printf("can't creat child\n");
		exit(-1);
	}
	
	if(pid == 0)
		ToRead(id, fd, file, rdstring);
	else
		ToWrite(id, fd, file, wrstring);
	
	return 0;
}
