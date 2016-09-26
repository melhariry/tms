#ifndef  PARSER_H
#define  PARSER_H
/*========================================*
* I N C L U D E S *
*========================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*========================================*
* I N C L U D E S *
*========================================*/
#include "httpclient.h"
#include "GPRS.h"


struct command
{
	BYTE paramlist[10][50];
	USHORT paramLen;
};

USHORT create_pos_record()
{
    USHORT ret;
    ULONG  ulUsedDiskSize,  ulTotalDiskSize, ulUsedRamSize, ulTotalRamSize;
    ret = CTOS_SystemMemoryStatus (&ulUsedDiskSize ,&ulTotalDiskSize,&ulUsedRamSize,&ulTotalRamSize );
    memset(baResponse,0,sizeof baResponse);
    sprintf(baResponse,"Vendor=Castles&TotalDiskCapacity=%d&TotalRamSize=%d",ulTotalDiskSize,ulTotalRamSize);

    sprintf(baExtraParam,"SerialNumber:%s\nContent-Length: %d",baSerialNumber,strlen(baResponse));
    HTTP_BuildHeader(baHeader,"/MTMS/CreatePosRecord.ashx","POST",baExtraParam);

    HTTP_BuildRequest(baMessage,baHeader,baResponse);
    GPRS_connect(bIPaddress,80);

	GPRS_send(baMessage,strlen(baMessage));
	CTOS_LCDTClearDisplay();
	CTOS_LCDTPrint(baMessage);
	CTOS_KBDGet(&key);
	return ret;
    //Vendor,TotalDiskCapacity,TotalRamSize


}
USHORT put_files(char files_ist[][50], int listLen)
{
    int i;
	for (i = 1; i < listLen; i++)
	{
		//TODO put file with name files_list[i]
	}
}

USHORT delete_files(char files_list[][50], int listLen)
{
    int i;
	for (i = 1; i < listLen; i++)
	{
		//TODO delete file with name files_list[i]
	}
}

USHORT put_apps(char apps_ist[][50], int listLen)
{
    int i;
	for (i = 1; i < listLen; i++)
	{
		//TODO put app with name apps_list[i]
	}
}

USHORT delete_apps(char apps_list[][50], int listLen)
{
    int i;
	for (i = 1; i < listLen; i++)
	{
		//TODO delete app with name apps_list[i]
	}
}

USHORT update_apps(char apps_list[][50], int listLen)
{
    int i;
	for (i = 1; i < listLen; i++)
	{
		//TODO update app with name apps_list[i]
	}
}

USHORT list_apps()
{
	//TODO return list of apps and change USHORT XD
}

USHORT list_files()
{
	//TODO return list of files names
}

USHORT system_test()
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
USHORT execute(char paramlist[][50], int paramLen)
{
	char cmdname[50];

	if (strcmp(paramlist[0], "delete_file") == 0)
	{
		return delete_files(paramlist, paramLen);
	}
	else if (strcmp(paramlist[0], "delete_app") == 0)
	{
		return delete_apps(paramlist,paramLen);
	}
	else if (strcmp(paramlist[0], "put_file") == 0)
	{
		return put_files(paramlist, paramLen);
	}
	else if (strcmp(paramlist[0], "put_app") == 0)
	{
		return put_apps(paramlist, paramLen);
	}
	else if (strcmp(paramlist[0], "update_app") == 0)
	{
		return update_apps(paramlist, paramLen);
	}
	else if (strcmp(paramlist[0], "test") == 0)
	{
		return system_test();
	}
	else if (strcmp(paramlist[0], "list_apps") == 0)
	{
		return list_apps();
	}
	else if (strcmp(paramlist[0], "list_files") == 0)
	{
		return list_files();
	}
	else if (strcmp(paramlist[0], "CreatePosRecord") == 0)
	{
		return create_pos_record();
	}
	else if (strcmp(paramlist[0], "eoc") == 0)
	{
		return 0xFF;
	}
	//return true;

}
void parser2(IN char *cmdBuffer,OUT char paramlist[][50],OUT int* paramLen)
{
	char *token = strtok(cmdBuffer, " ,");

		*paramLen = 0;
		while (token)
		{
			strcpy(paramlist[(*paramLen)++], token);
			token = strtok(NULL, " ,");
		}
		

	
}
void parser(IN BYTE *cmdBuffer,OUT struct command cmdlist[],OUT USHORT* cmdLen)
{
	char cmdstr[100];
	BYTE kk;
	int cr = 0;
	char cmd[100];
	cr = strtoken(cmdstr, cmdBuffer + cr, ";");
	
	*cmdLen=0;
	USHORT usCmdLen=0;
	while (cr<strlen(cmdBuffer))
	{
		char tmpstr[100];
		strcpy(tmpstr, cmdstr);
		char *token = strtok(tmpstr, " ,\n\r");

		cmdlist[usCmdLen].paramLen=0;


		while (token)
		{
			strcpy(cmdlist[usCmdLen].paramlist[(cmdlist[usCmdLen].paramLen)++], token);

			token = strtok(NULL, " ,");
		}
		//execute(paramlist, paramLen);
		usCmdLen++;
		*cmdLen=usCmdLen;
		cr += strtoken(cmdstr, cmdBuffer + cr, ";");

	}
}





#endif 
