
#include "parser.h"


int main()
{
	char cmdBuffer[1024];

	char paramlist[52][50];//maximum 50 parameters
	char cmd[100];
	memset(cmdBuffer, 0, sizeof cmdBuffer);
	while (fgets(cmd, 100, stdin))
	{
		strcat(cmdBuffer, cmd);
	}
	parser(cmdBuffer, paramlist);

	return 0;
}