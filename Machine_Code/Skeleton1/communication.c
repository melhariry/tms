/**
  ******************************************************************************
  * file          communication.c
  * author        
  * component     communication
  * description   implementation of communication module
  *               communication module provides the Terminal abstraction layer of 
  *               communication with TMS 
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
#include "communication.h"
#include "util.h"
#include "GPRS.h"
#include "httpclient.h"

/*==========================================*
****************P R I V A T E ***************
*==========================================*/

/*==========================================*
* D E F I N E S *
*==========================================*/
  //Path index
  #define PATH_POSUP 0
  #define PATH_CREATE_POS_RECORD 1
  #define PATH_SUBMIT_COMMAND_RESULT 2



  #define SIZE_HTTP_BUFF 1224
  #define SIZE_HTTP_MESSAGE 200
  #define SIZE_HTTP_HEADER 100
  #define SIZE_HTTP_PARAM 50

/*==========================================*
* C O N S T A N T S *
*==========================================*/
const BYTE tmsPaths[][50]={"/MTMS/PosUp.ashx",
                 "/MTMS/CreatePosRecord.ashx",
                 "/MTMS/SubmitCommandResult.ashx" };
/*==========================================*
* T Y P E S *
*==========================================*/

/*==========================================*
* V A R I A B L E S *
*==========================================*/

BYTE baHttpParam[SIZE_HTTP_PARAM];
BYTE baHttpHeader[SIZE_HTTP_HEADER];
BYTE baHttpMessage[SIZE_HTTP_MESSAGE];
BYTE baHttpBuff[SIZE_HTTP_BUFF];
USHORT usHttpBuffLen=SIZE_HTTP_BUFF;

BYTE baTmsUrl[]="197.34.102.84";


USHORT usTmsPort=80;
/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* F U N C T I O N S *
*==========================================*/

void commPingTms()
{
  //local variables 

  //check/establish connection
    gprsOpen();
    gprsConnect(baTmsUrl,usTmsPort);
  //build request
    GetSerialNumber(baSerialNumber);
    sprintf(baHttpParam,"SerialNumber:%s\n",baSerialNumber);
    HTTP_BuildHeader(baHttpHeader,tmsPaths[PATH_POSUP],"GET",baHttpParam);
  //send request
    gprsSend(baHttpHeader,strlen(baHttpHeader));
  //wait response
    usHttpBuffLen=SIZE_HTTP_BUFF;
    gprsRecieve(baHttpBuff,&usHttpBuffLen);
    HTTP_GetMessage(baHttpMessage,baHttpBuff);
  //
    CTOS_LCDTClearDisplay();
            
    CTOS_LCDTPrint(baHttpMessage);
    CTOS_Delay(2000);
    gprsDisconnect();

}
USHORT commSendTms(BYTE *baMessage)
{
    gprsOpen();
    gprsConnect(baTmsUrl,usTmsPort);
    sprintf(baHttpParam,"SerialNumber:%s\nContent-Length:%d",baSerialNumber,strlen(baMessage));
    HTTP_BuildHeader(baHttpHeader,tmsPaths[PATH_POSUP],"POST",baHttpParam);
    HTTP_BuildRequest(baHttpBuff,baHttpHeader,baMessage);
    gprsSend(baHttpBuff,strlen(baHttpBuff));
    usHttpBuffLen=SIZE_HTTP_BUFF;
    memset(baHttpBuff,0,sizeof baHttpBuff);
    gprsRecieve(baHttpBuff,&usHttpBuffLen);
    HTTP_GetMessage(baHttpMessage,baHttpBuff);
    if(strcmp(baHttpMessage,"ACK"))
      return 0xFF;//error
    return 0;//ack
    
}


