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


/*==========================================*
****************P R I V A T E ***************
*==========================================*/

/*==========================================*
* D E F I N E S *
*==========================================*/
  //Path index
  #define PATH_POSUP 0
  #define PATH_CREATE_POS_RECORD 1



/*==========================================*
* C O N S T A N T S *
*==========================================*/
const BYTE tmsPaths[][50]={"/MTMS/PosUp.ashx",
                 "/MTMS/CreatePosRecord.ashx"};
/*==========================================*
* T Y P E S *
*==========================================*/

/*==========================================*
* V A R I A B L E S *
*==========================================*/

BYTE baExtraParam[50];
BYTE baHeader[200];
/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* F U N C T I O N S *
*==========================================*/
void comm_openGPRS()
{
  GPRS_PARAM_init();
  GPRS_open();
}
void comm_send(BYTE* bpIP,BYTE* bpTxBuff,USHORT usTxBuffLen)
{
  //check/open
  //check/connect
  GPRS_send(baHeader,strlen(baHeader));
}
void comm_ping_TMS()
{
  //local variables 

  //check/establish connection

  comm_openGPRS();
  //build request
    GetSerialNumber(baSerialNumber);
    sprintf(baExtraParam,"SerialNumber:%s\n",baSerialNumber);
    HTTP_BuildHeader(baHeader,tmsPaths[PATH_POSUP],"GET",baExtraParam);
    GPRS_send(baHeader,strlen(baHeader));

  //send request

  

}


