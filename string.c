#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void GenerateString(int n, char* string)
{
	string[0] = 'a';
	int i = 1;
	while(i < (1<<n) - 1)
	{
		char* buf = (char*)calloc(i + 1, sizeof(char));
		strncpy(buf, string, i);
		string[i] = string[(i - 1) / 2] + 1;
		strncpy(string + i + 1, buf, i);
		string[i * 2 + 1] = '\0';
//		printf("%s\n", string);
		free(buf);
		i = i * 2 + 1;
	}
	string[(1<<n) - 1] = '\0';
}

int main()
{
	int n;
	char* string;
	int i;

	scanf("%d", &n);
	string = (char*)calloc((1<<n) - 1, sizeof(char));
	GenerateString(n, string);
	printf("%s\n", string);
	free(string);
	
	return 0;
}
