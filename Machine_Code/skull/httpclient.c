/**
  ******************************************************************************
  * file          httpclient.c
  * author        
  * component     httpclient
  * description   implementation
  *               client that build http requests 
  ******************************************************************************
  * copyright POS_GP_TEAM_2016
  *
  ******************************************************************************
  */

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
#include "httpclient.h"


/*==========================================*
***************P R I V A T E ****************
*==========================================*/

/*==========================================*
* D E F I N E S *
*==========================================*/
	
/*==========================================*
* C O N S T A N T S *
*==========================================*/

/*==========================================*
* T Y P E S *
*==========================================*/

/*==========================================*
* V A R I A B L E S *
*==========================================*/

/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* F U N C T I O N S *
*==========================================*/

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