#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <dirent.h>

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
	int sockfd = *((int *)arg);
	char recvline[1000];
	int n;

	while(1)
	{	
		if((n = recvfrom(sockfd, recvline, 1000, 0, (struct sockaddr *)NULL, NULL)) < 0)
		{
			printf("can't get messages\n");
			perror(NULL);
			close(sockfd);
			exit(1);
		}
		else
			printf("%s\n", recvline);
	}
}

/*void *ToWrite(void *arg)
{
	int sockfd = *((int *)arg);
	char sendline[1000];
	int n;

	while(1)
	{
		fgets(sendline, 1000, stdin);

		if(sendto(sockfd, sendline, strlen(sendline) + 1, 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		{
			printf("can't send messages\n");
			perror(NULL);
			close(sockfd);
			exit(1);
		}
} */

int main(int argc, char **argv)
{
	int sockfd;
	char sendline[1000];
	struct sockaddr_in servaddr, cliaddr;

	if(argc != 2)
	{
		printf("usage: a.out <IP>\n");
		exit(1);
	}

	if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror(NULL);
		exit(1);
	}
	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(0);
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51000);
	
	if(inet_aton(argv[1], &servaddr.sin_addr) == 0)
	{
		printf("invalide IP\n");
		close(sockfd);
		exit(1);
	}

	pthread_t thid;
	int res;

	res = pthread_create(&thid, (pthread_attr_t *)NULL, ToRead, &sockfd);
	if(res != 0)
	{
		printf("can't create thread\n");
		exit(-1);
	}

	while(1)
	{
		fgets(sendline, 1000, stdin);

		if(sendto(sockfd, sendline, strlen(sendline) + 1, 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		{
			printf("can't send messages\n");
			perror(NULL);
			close(sockfd);
			exit(1);
		}
	}

	pthread_join(thid, (void **)NULL);
	
	return 0;
}
