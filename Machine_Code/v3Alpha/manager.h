/**
  ******************************************************************************
  * file          communication.h
  * author        
  * component     communication
  * description   header file of communication
  *               communication module provides the Terminal abstraction layer of 
  *               communication with TMS 
  ******************************************************************************
  * copyright POS_GP_TEAM_2016
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusions ------------------------------------*/
#ifndef MANAGER_H
#define MANAGER_H


/*========================================*
* I N C L U D E S *
*========================================*/
#include "common.h"
#include "communication.h"
#include "execute.h"


/*==========================================*
****************P U B L I C ****************
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
* P R O T O T Y P E S *
*==========================================*/
BYTE manLoadConfig(void);
USHORT manUp(void);
USHORT manTest(void);
USHORT manAppList(void);
USHORT manFileList(void);
USHORT manDeleteFile(void);
USHORT manPushFile(void);
USHORT manPullFile(void);
USHORT manRecieveFile(void);
USHORT manOpenFTP(void);
USHORT manUpdateApp(void);
USHORT manCreatePosRecord(void);

#endif