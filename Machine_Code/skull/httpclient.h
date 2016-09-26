/**
  ******************************************************************************
  * file          httpclient.h
  * author        
  * component     httpclient
  * description   header file of httpclient
  *               client that build http requests 
  ******************************************************************************
  * copyright POS_GP_TEAM_2016
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusions ------------------------------------*/
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
* I N C L U D E S *
*========================================*/
//#include "common.h"




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
* F U N C T I O N S *
*==========================================*/
USHORT HTTP_BuildHeader(OUT BYTE* Header,IN BYTE* Path,IN BYTE* Method,IN BYTE* ExtraPar);
USHORT HTTP_BuildRequest(OUT BYTE* Request,IN BYTE* Header,IN BYTE* Message);
USHORT HTTP_GetMessage(OUT BYTE* Message,IN BYTE* Request);


#endif