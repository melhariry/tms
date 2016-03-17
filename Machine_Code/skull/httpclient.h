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
* D E F I N E S *
*========================================*/
BYTE baSerialNumber[40];
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
	//sprintf(Header,"%s %s HTTP/1.1\nContent-Type: application/x-www-form-urlencoded\nAccept: */*\n%s\n\n",Method,Path,ExtraPar);
	sprintf(Header,"%s %s HTTP/1.1\nContent-Type: application/x-www-form-urlencoded\nAccept: */*\nAccept-Encoding: gzip, deflate\nAccept-Language: en-US,en;q=0.8,ar;q=0.6,es;q=0.4\n%s\n\n",Method,Path,ExtraPar);
	return 0;
}
USHORT HTTP_BuildMessage(OUT BYTE* Message,IN BYTE* Header,IN BYTE* MSG_Content){
	memset(Message,0,sizeof Message);
	
	sprintf(Message,"%s%s",Header,MSG_Content);
	
	return 0;
}
USHORT HTTP_GetReply(OUT BYTE* Reply,IN BYTE* Response){
	memset(Reply,0,sizeof Reply);
	USHORT i;
	//BYTE Header[200];
	for(i=0;i<strlen(Response)-4;++i){
	 	if(Response[i]=='\r'&&Response[i+2]=='\r'&&Response[i+1]=='\n'&&Response[i+3]=='\n'){
	 		strcpy(Reply,Response+i+4);	
	 	}
	 		
	 }
	
	return 0;
}

#endif