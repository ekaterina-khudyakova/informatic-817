#include <stdio.h>
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
	free(buf);
}

int main()
{
	char* string = (char*)calloc(100, sizeof(char));
	char delimiters[8] = {' ', ',', ';', ':', '.', '	', '_', '-'};
	int count = 1;
	char** words;
	
	char* pointer = 0;
	int i;

	fgets(string, 100, stdin);
	

	for(i = 0; i < 8; i++)
	{
		pointer = strchr(string, delimiters[i]);
		while(pointer != NULL)
		{
			count++;
			pointer = strchr(pointer + 1, delimiters[i]);
		}
	}

	words = (char**)calloc(count, sizeof(char*));
	for(i = 0; i < count; i++)
		words[i] = (char*)calloc(100, sizeof(char));
	
	Split(string, delimiters, &count, words);

	for(i = 0; i < count; i++)
		printf("%s\n", words[i]);

	for(i = 0; i < count; i++)
		free(words[i]);
	free(words);
	free(string);

	return 0;
}
