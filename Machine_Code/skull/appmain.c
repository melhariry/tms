/**
  ******************************************************************************
  * file          appmain.c
  * author        
  * component     main
  * description   the entery point of the application
  *               
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
#include <stdarg.h>
/** These two files are necessary for calling CTOS API **/
#include <ctosapi.h>

/*========================================*
* I N C L U D E S *
*========================================*/
#include "GPRS.h"
#include "files.h"
#include "httpclient.h"
#include "util.h"
#include "parser.h"
#include "common.h"
#include "communication.h"








/*==========================================*
* D E F I N E S *
*==========================================*/
	
/*==========================================*
* C O N S T A N T S *
*==========================================*/
	//BYTE bIPaddress[]="197.34.110.15";

/*==========================================*
* T Y P E S *
*==========================================*/

/*========================================*
* G L O B A L     V A R I A B L E S *
*========================================*/
	struct command cmdList[40];
	USHORT cmdLen;
	USHORT usCmdstatus;

	/*BYTE baHeader[200];
	BYTE baExtraParam[200];
	BYTE baMessage[300];
	BYTE baResponse[100];
	BYTE baPayload[100];
	BYTE baReply[400];
	*/
	/*BYTE baTXBuffer[1024];
	BYTE baRXBuffer[1024];
	//BYTE baSerialNumber[40];
	USHORT usBufferLen=1024;
	*/

/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* F U N C T I O N S *
*==========================================*/

void gprs_session(){
	GPRS_PARAM_init();
	CTOS_LCDTPrint("\ninit Done");	

	GPRS_open();
	CTOS_LCDTPrint("\nopen Done");	
}
void hello()
{
/*	int i,j;
	BYTE key2;
	gprs_session();
	GetSerialNumber(baSerialNumber);
	sprintf(baExtraParam,"SerialNumber:%s\n",baSerialNumber);
    HTTP_BuildHeader(baHeader,"/MTMS/PosUp.ashx","GET",baExtraParam);
	//GPRS_connect(bIPaddress,80);
	GPRS_send(baHeader,strlen(baHeader));
	CTOS_LCDTPrint(baHeader);
	CTOS_KBDGet(&key);
	usBufferLen=1024;
	GPRS_recieve(baRXBuffer,&usBufferLen);
	HTTP_GetMessage(baMessage,baRXBuffer);
	
	
	parser(baMessage,cmdList,&cmdLen);
	memset(baResponse,0,sizeof baResponse);
	for(i=0;i<cmdLen;++i){
		usCmdstatus=execute(cmdList[i].paramlist,cmdList[i].paramLen);
		sprintf(baResponse,"%s\n&%s=%d",baResponse,cmdList[i].paramlist[0],usCmdstatus);
        for(j=0;j<cmdList[i].paramLen;++j){
	        CTOS_LCDTPrint(cmdList[i].paramlist[j]);
	        CTOS_LCDTPrint(" ");
        }
        CTOS_LCDTPrint("\n");
    }
   
    */
    /*sprintf(baExtraParam,"SerialNumber:%s\nContent-Length: %d",baSerialNumber,strlen(baResponse));
    HTTP_BuildHeader(baHeader,"/MTMS/CreatePosRecord.ashx","POST",baExtraParam);
    HTTP_BuildMessage(baMessage,baHeader,baResponse);
    CTOS_KBDGet(&key2);
	GPRS_connect(bIPaddress,80);

	GPRS_send(baMessage,strlen(baMessage));
	CTOS_LCDTPrint(baMessage);
	CTOS_KBDGet(&key);
	*/

	

	GPRS_disconnect();
	GPRS_close();
	//CTOS_LCDTPrint(baRXBuffer);

}
/*==========================================*
* E N T E R Y  P O I N T *
*==========================================*/
int main(int argc,char *argv[])
{
	int i=0,j;
    BYTE key;  
    
    strcpy(bIPaddress,"197.34.110.15");

	// TODO: Add your program here //
	CTOS_LCDTClearDisplay();
	hello();
	/*parser("delete_files f1,f2,f3;\ndelete_files f4,f5,f6;CreatePosRecord;list_apps;list_files;",cmdList,&cmdLen);
    for(i=0;i<cmdLen;++i){
        for(j=0;j<cmdList[i].paramLen;++j){
        CTOS_LCDTPrint(cmdList[i].paramlist[j]);
        CTOS_LCDTPrint(" ");
        }
        CTOS_LCDTPrint("\n");
    }*/
	
    
	 
	 
	
	//CTOS_LCDTPrint(baReply);	




    CTOS_KBDGet(&key);
	exit(0);
}
