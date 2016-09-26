/**
  ******************************************************************************
  * file          util.h
  * author        
  * component     utilites
  * description   
  *               
  *               
  ******************************************************************************
  * copyright POS_GP_TEAM_2016
  *
  ******************************************************************************
  */
#ifndef UTIL_H
#define UTIL_H
/*========================================*
* I N C L U D E S *
*========================================*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctosapi.h>

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
void split(IN BYTE* baStr,BYTE* baDelimiters,OUT BYTE *tokens[],USHORT* usNoToken);
void unpack(OUT char* pStr, IN BYTE* pData, IN USHORT Len, IN char* pSpilt);
USHORT GetSerialNumber(OUT BYTE * SerialNumber);
USHORT GetGPRSIP(OUT BYTE * GPRSIP);
BYTE ith(BYTE hex_digit);
BYTE IntToStr(BYTE* buf, DWORD v);
void util_strncpy(BYTE *dest,const BYTE *source,ULONG ulLen);

#endif