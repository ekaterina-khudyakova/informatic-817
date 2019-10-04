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

int get_dishes(FILE *file, int *type, int *count)
{
	char buf[300];
	while(fgets(buf, 300, file))
	{
		*type = atoi(buf);
		*count = atoi(strstr(buf, ":") + 1);
		return 1;
	}
	return 0;
}

typedef struct
{
	long type;
	int dish_type;
}Message;

int main(int argc, char **argv)
{
	int table_size = atoi(argv[1]);
	char fname[] = "wash_times";
	int *wash_times = get_times(fname);

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

	struct msqid_ds mes;
	Message message;
	int type, count;

	FILE *file = fopen("dishes", "r");
	if(file == NULL)
	{
		printf("can't open dishes file\n");
		exit(-1);
	}

	while(get_dishes(file, &type, &count))
	{
		int i;
		for(i = 0; i < count; ++i)
		{
			sleep(wash_times[type]);
			printf("#%d: %d washed\n", i, type);

			msgctl(msgid, IPC_STAT, &mes);
			while(mes.msg_qnum >= table_size);			

			message.type = 1;
			message.dish_type = type;
			msgsnd(msgid, &message, sizeof(message.dish_type), 0);

			printf("#%d: %d put on table\n", i, type);
		}
	}

	message.type = 0;
	message.dish_type = 0;
	msgsnd(msgid, &message, sizeof(message.dish_type), 0);

	printf("finish washing\n");

	free(wash_times);

	return 0;
}
