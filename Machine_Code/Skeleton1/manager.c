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
#include "manager.h"

/*==========================================*
****************P R I V A T E ***************
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
BYTE baResponse[3*1024];
BYTE baStr[3*1024];
/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* F U N C T I O N S *
*==========================================*/
USHORT manTest()
{
  USHORT usResult = exeSystemTest(baStr);
  sprintf(baResponse,"cmd=test%s\n",baStr);
  usResult=commSendTms(baResponse);

}

USHORT manAppList()
{
  BYTE key;
  //max 50 apps xD
  USHORT usResult = exeListApps(baStr);
  sprintf(baResponse,"cmd=listApp%s\n",baStr);
  usResult=commSendTms(baResponse);
}

USHORT manFileList()
{
   BYTE key;
  //max 50 apps xD
  USHORT usResult = exeListFiles(baStr);
  sprintf(baResponse,"cmd=listFiles%s\n",baStr);
 // usResult=commSendTms(baResponse); 
}