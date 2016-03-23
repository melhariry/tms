#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H
/*========================================*
* I N C L U D E S *
*========================================*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctosapi.h>

/*========================================*
* I N C L U D E S *
*========================================*/
#include "common.h"

/*========================================*
* D E F I N E S *
*========================================*/



/*==========================================*
* P U B L I C *
*==========================================*/
BYTE baHeader[200];
BYTE baExtraParam[200];
BYTE baMessage[300];
BYTE baResponse[100];
BYTE baPayload[100];
BYTE baReply[400];

/*========================================*
* Functions
*========================================*/
USHORT HTTP_BuildHeader(OUT BYTE* Header,IN BYTE* Path,IN BYTE* Method,IN BYTE* ExtraPar){
	memset(Header,0,sizeof Header);
	sprintf(Header,"%s %s HTTP/1.1\nContent-Type: application/x-www-form-urlencoded\nAccept: */*\nAccept-Encoding: gzip, deflate\nAccept-Language: en-US,en;q=0.8,ar;q=0.6,es;q=0.4\n%s\n\n",Method,Path,ExtraPar);
	return 0;
}
USHORT HTTP_BuildRequest(OUT BYTE* Request,IN BYTE* Header,IN BYTE* Message){
	memset(Request,0,sizeof Request);
	sprintf(Request,"%s%s",Header,Message);
	return 0;
}
USHORT HTTP_GetMessage(OUT BYTE* Message,IN BYTE* Request){
	memset(Message,0,sizeof Message);
	USHORT i;
	for(i=0;i<strlen(Request)-4;++i)
	{
	 	if(Request[i]=='\r'&&Request[i+2]=='\r'&&Request[i+1]=='\n'&&Request[i+3]=='\n')
	 		strcpy(Message,Request+i+4);	
	}
	
	return 0;
}

#endif