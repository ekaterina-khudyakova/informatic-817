#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef struct Arg
{
	int id;
	int fd[2];
	char **file;
	char *rdstring;
	char *wrstring;
}Arg;

void *ToRead(void *arg)
{
	Arg arguments = *((Arg *)arg);
	int size;
	int id = arguments.id;
	
	arguments.fd[(id + 1) % 2] = open(arguments.file[(id + 1) % 2], O_RDONLY);
	if(arguments.fd[(id + 1) % 2] < 0)
	{
		printf("can't get messages\n");
		exit(-1);
	}
	else
	{
		while(1)
		{
			size = read(arguments.fd[(id + 1) % 2], arguments.rdstring, 100);
			printf("%s\n", arguments.rdstring);
		}
	}
}

void *ToWrite(void *arg)
{
	Arg arguments = *((Arg *)arg);
	int size;
	int id = arguments.id;

	arguments.fd[id] = open(arguments.file[id], O_WRONLY);
	if(arguments.fd[id] < 0)
	{
		printf("can't send messages\n");
		exit(-1);
	}
	else
	{
		while(1)
		{
			fgets(arguments.wrstring, 100, stdin);
			size = write(arguments.fd[id], arguments.wrstring, 100);
		}
	}
}

int main(int argc, char *argv[])
{
	Arg arg;
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

	arg.id = id;
	arg.file = file;
	arg.rdstring = rdstring;
	arg.wrstring = wrstring;

	pthread_t thid0, thid1;
	int res;

	res = pthread_create(&thid0, (pthread_attr_t *)NULL, ToRead, &arg);
	if(res != 0)
	{
		printf("can't create thread\n");
		exit(-1);
	}

	res = pthread_create(&thid1, (pthread_attr_t *)NULL, ToWrite, &arg);
	if(res != 0)
	{
		printf("can't create thread\n");
		exit(-1);
	}

	pthread_join(thid0, (void **)NULL);
	pthread_join(thid1, (void **)NULL);
	
	return 0;
}
