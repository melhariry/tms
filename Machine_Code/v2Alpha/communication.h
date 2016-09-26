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
#ifndef COMMUNICATION_H
#define COMMUNICATION_H


/*========================================*
* I N C L U D E S *
*========================================*/
#include "common.h"



/*==========================================*
****************P U B L I C ****************
*==========================================*/

/*==========================================*
* D E F I N E S *
*==========================================*/
	//Path index
  #define PATH_POSUP 0
  #define PATH_CREATE_POS_RECORD 1
  #define PATH_SUBMIT_COMMAND_RESULT 2
  #define PATH_REQUEST_COMMAND_PARAMETERS 3

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
USHORT commInit(void);
USHORT commClose(void);
USHORT commSendTms(IN BYTE *baMessage,IN USHORT usPathIndex);
USHORT commSendFile(IN BYTE * baFilePath,IN BYTE * baFileName);
USHORT commRecieveFile(IN BYTE * baFilePath,IN BYTE * baFileName);
#endif