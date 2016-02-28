/** 
**	A Template for developing new terminal application
**/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
/** These two files are necessary for calling CTOS API **/
#include <ctosapi.h>
#include "GPRS.h"
#include "files.h"
#include "httpclient.h"
#include "util.h"


// constants
BYTE bIPaddress[]="197.34.106.209";



/** 
** The main entry of the terminal application 
**/
BYTE baHeader[200];
BYTE baExtraParam[200];
BYTE baMessage[300];
BYTE baResponse[400];
BYTE baReply[400];
BYTE baTXBuffer[1024];
BYTE baRXBuffer[1024];
BYTE baSerialNumber[40];
USHORT usBufferLen=1024;



void gprs_session(){
	GPRS_PARAM_init();
	CTOS_LCDTPrint("\ninit Done");	

	GPRS_open();
	CTOS_LCDTPrint("\nopen Done");	
}
void hello()
{
	gprs_session();
	GetSerialNumber(baSerialNumber);
	sprintf(baExtraParam,"SerialNumber:%s",baSerialNumber);
    HTTP_BuildHeader(baHeader,"/MTMS/PosUp.ashx","GET",baExtraParam);
    HTTP_BuildMessage(baMessage,baHeader,"");
	GPRS_connect(bIPaddress,80);
	GPRS_send(baMessage,strlen(baMessage));
	usBufferLen=1024;
	GPRS_recieve(baRXBuffer,&usBufferLen);
	HTTP_GetReply(baReply,baRXBuffer);
	CTOS_LCDTPrint(baReply);
	//CTOS_LCDTPrint(baRXBuffer);

}
int main(int argc,char *argv[])
{
	BYTE key;  
	// TODO: Add your program here //
	CTOS_LCDTClearDisplay();
	hello();
	///HTTP_GetReply(baReply,"abc 101 OK\r\ncontent:10\r\n\r\nMSG\r\n");
	 
	 
	 
	
	CTOS_LCDTPrint(baReply);	




    CTOS_KBDGet(&key);
	exit(0);
}
