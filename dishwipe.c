#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

int * get_times(char *fname)
{
	FILE *file = fopen(fname, "r");
	if(file == NULL)
	{
		printf("error: can't open file %s\n", fname);
		exit(-1);
	}

	int *dishes = (int *)calloc(5, sizeof(int));
	char buf[300];
	while(fgets(buf, 300, file))
	{
		int type = atoi(buf);
		int time = atoi(strstr(buf, ":") + 1);
		dishes[type] = time;
	}
	return dishes;
}

typedef struct
{
	long type;
	int dish_type;
}Message;

int main(int argc, char **argv)
{
	char fname[] = "wipe_times";
	int *wipe_times = get_times(fname);

	key_t key = ftok("ftok_file", 0);
	if(key < 0)
	{
		printf("can't generate key\n");
		exit(-2);
	}

	int msgid = msgget(key, 0666 | IPC_CREAT);
	if(msgid < 0)
	{
		printf("can't get q message\n");
		exit(-1);
	}

	Message message;

	while(1)
	{
		msgrcv(msgid, &message, sizeof(message.dish_type), 0, 0);
	//	printf("message %ld\n", message.type);
		if(message.type == 0)
			break;
		printf("get dish\n");

		sleep(wipe_times[message.dish_type]);
		printf("%d wiped\n", message.dish_type);
	}
	
	free(wipe_times);

	return 0;
}
