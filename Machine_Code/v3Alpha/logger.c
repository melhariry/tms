/**
  ******************************************************************************
  * file          logger.c
  * author        
  * component     logger
  *               
  ******************************************************************************
  * copyright POS_GP_TEAM_2016
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusions ------------------------------------*/

/*========================================*
* I N C L U D E S *
*========================================*/
#include "logger.h"

/*==========================================*
****************P U B L I C ****************
*==========================================*/

/*==========================================*
* D E F I N E S *
*==========================================*/
//#define LCDLOG
/*==========================================*
* C O N S T A N T S *
*==========================================*/

/*==========================================*
* T Y P E S *
*==========================================*/

/*==========================================*
* V A R I A B L E S *
*==========================================*/
CTOS_RTC timestamp;
char logBuff[1024];
/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* F U N C T I O N S *
*==========================================*/


int16_t dLogOpen()
{
    fpDebugLog=fopen("debugLog.txt","a");
    if(fpDebugLog==NULL)return -1;
    return 0;
}

int16_t sysLogOpen()
{
    fpSysLog=fopen("/home/ap/pub/MTMS/sysLog.txt","a");
    if(fpSysLog==NULL)return -1;
    return 0;
}

int16_t dLog(char *str)
{
    if(dLogOpen())
        return -1;
    CTOS_RTCGet(&timestamp);
    fprintf(fpDebugLog, "%02d:%02d:%02d %s\n",timestamp.bHour,timestamp.bMinute,timestamp.bSecond,str );
    if(fclose(fpDebugLog)!=0)
        return -1;
}

int16_t sysLog(char *str)
{
    //CTOS_LCDTPrint(str);
    if(sysLogOpen())
        return -1;
    CTOS_RTCGet(&timestamp);
    fprintf(fpSysLog, "%02d:%02d:%02d %s\n",timestamp.bHour,timestamp.bMinute,timestamp.bSecond,str );
    if(fclose(fpSysLog)!=0)
        return -1;
}
int16_t sysLogCall(char *str)
{
    #ifdef LCDLOG
    CTOS_LCDTPrint("\n");
    CTOS_LCDTPrint(str);
    #endif
    strcpy(logBuff,"Call ");
    strcat(logBuff,str);
    return sysLog(logBuff);
}
int16_t sysLogRet(char *str,int32_t value)
{
    sprintf(logBuff,"%s return %d",str,value);
    #ifdef LCDLOG
    CTOS_LCDTPrint("\n");
    CTOS_LCDTPrint(logBuff);
    #endif
    return sysLog(logBuff);
}

