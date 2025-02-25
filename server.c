#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int sockfd;
	int clilen, n;
	char line[1000];
	unsigned int clientIP[100];
	unsigned short clientPort[100];
	int i, number = 0;

	struct sockaddr_in servaddr, cliaddr;

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51000);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror(NULL);
		exit(1);
	}

	if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	while(1)
	{
		clilen = sizeof(cliaddr);
		if((n = recvfrom(sockfd, line, 999, 0, (struct sockaddr *)&cliaddr, &clilen)) < 0)
		{
			perror(NULL);
			close(sockfd);
			exit(1);
		}
		printf("%s\n", line);

		if(strcmp(line, "new client\n") == 0)
		{
			clientIP[number] = cliaddr.sin_addr.s_addr;
			clientPort[number] = cliaddr.sin_port;
			++number;
			strcpy(line, "success\n");
			if(sendto(sockfd, line, strlen(line), 0, (struct sockaddr *)&cliaddr, clilen) < 0)
			{
				perror(NULL);
				close(sockfd);
				exit(1);
			}
		}
		else
		{
			for(i = 0; i < number; ++i)
			{
				clientIP[i] = cliaddr.sin_addr.s_addr;
				clientPort[i] = cliaddr.sin_port;
				if(sendto(sockfd, line, strlen(line), 0, (struct sockaddr *)&cliaddr, clilen) < 0)
				{
					perror(NULL);
					close(sockfd);
					exit(1);
				}
				
			}

		}
	}
	return 0;
}
