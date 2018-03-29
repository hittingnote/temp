#include <stdio.h>


int main(int argc, char *argv[])
{
	char c;
	while(scanf("%c", &c) != EOF)
	{
		if(c == '\n')
			printf("%c", '\r');
		printf("%c", c);
	}

	return 0;
}


