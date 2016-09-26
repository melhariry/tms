/**
  ******************************************************************************
  * file          execute.h
  * author        
  * component     execute
  * description   header file of execute
  *               execute module executes actions on device
  ******************************************************************************
  * copyright POS_GP_TEAM_2016
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusions ------------------------------------*/
#ifndef EXECUTE_H
#define EXECUTE_H


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
USHORT  exeListApps(BYTE* baAppList);
USHORT  exeListFiles(BYTE* baFileList);
//USHORT  exeDeleteFiles(IN BYTE * fileNames[],IN int length,OUT USHORT  deleteResults[]);
USHORT  exeDeleteFiles(IN BYTE * fileNames[],IN int length,OUT BYTE  deleteResults[]);
USHORT  exeSystemTest(BYTE* baTestResult);

#endif