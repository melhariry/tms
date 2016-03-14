#ifndef  PARSER_H
#define  PARSER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void put_files(char files_ist[][50], int listLen)
{
	for (int i = 1; i < listLen; i++)
	{
		//TODO put file with name files_list[i]
	}
}

void delete_files(char files_list[][50], int listLen)
{
	for (int i = 1; i < listLen; i++)
	{
		//TODO delete file with name files_list[i]
	}
}

void put_apps(char apps_ist[][50], int listLen)
{
	for (int i = 1; i < listLen; i++)
	{
		//TODO put app with name apps_list[i]
	}
}

void delete_apps(char apps_list[][50], int listLen)
{
	for (int i = 1; i < listLen; i++)
	{
		//TODO delete app with name apps_list[i]
	}
}

void update_apps(char apps_list[][50], int listLen)
{
	for (int i = 1; i < listLen; i++)
	{
		//TODO update app with name apps_list[i]
	}
}

void list_apps()
{
	//TODO return list of apps and change void XD
}

void list_files()
{
	//TODO return list of files names
}

void system_test()
{
	//TODO call sysyem test and send results to server
}

int strtoken(char *outstr, char *str, char *token)
{
	int i = 0;
	//printf("strtoken:%s\n",str);
	while (i<strlen(str))
	{
		int j = 0;
		for (j = 0; j<strlen(token); ++j)
		{
			if (str[i] == token[j])
			{

				strncpy(outstr, str, i + 1);
				outstr[i] = '\0';
				return i + 1;
			}
		}
		i++;
		//outstr[i]=str[i];

	}
	return i;

}
void execute(char paramlist[][50], int paramLen)
{
	char cmdname[50];

	if (strcmp(paramlist[0], "delete_file") == 0)
	{
		delete_files(paramlist, paramLen);
	}
	else if (strcmp(paramlist[0], "delete_app") == 0)
	{
		delete_apps(paramlist,paramLen);
	}
	else if (strcmp(paramlist[0], "put_file") == 0)
	{
		put_files(paramlist, paramLen);
	}
	else if (strcmp(paramlist[0], "put_app") == 0)
	{
		put_apps(paramlist, paramLen);
	}
	else if (strcmp(paramlist[0], "update_app") == 0)
	{
		update_apps(paramlist, paramLen);
	}
	else if (strcmp(paramlist[0], "test") == 0)
	{
		system_test();
	}
	else if (strcmp(paramlist[0], "list_apps") == 0)
	{
		list_apps();
	}
	else if (strcmp(paramlist[0], "list_files") == 0)
	{
		list_files();
	}

}
void parser(char *cmdBuffer, char   paramlist[][50])
{
	char cmdstr[100];
	int cr = 0;
	char cmd[100];
	cr = strtoken(cmdstr, cmdBuffer + cr, ";");


	while (cr<strlen(cmdBuffer))
	{
		//printf("cr=%d\n",cr);
		printf("%s\n", cmdstr);
		char tmpstr[100];
		strcpy(tmpstr, cmdstr);
		char *token = strtok(tmpstr, " ,\n\r");

		int paramLen = 0;
		while (token)
		{
			strcpy(paramlist[paramLen++], token);
			token = strtok(NULL, " ,");
		}
		execute(paramlist, paramLen);
		cr += strtoken(cmdstr, cmdBuffer + cr, ";");

	}
}





#endif 
