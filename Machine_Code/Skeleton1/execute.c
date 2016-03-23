/**
  ******************************************************************************
  * file          execute.c
  * author        
  * component     execute
  * description   execute implementation
  *               execute module executes actions on device
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
#include "execute.h"
#include "test.h"

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


/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* F U N C T I O N S *
*==========================================*/
USHORT  exeSystemTest(BYTE* baTestResult)
{
    CTOS_LCDTClearDisplay();
            
    CTOS_LCDTPrintXY(1, 1, "Please wait\n system testing is running\n");

  BYTE baBuffer[40],str[10];
  USHORT ret;
    strcpy(baTestResult,"");

    ///////////////////////////////////////////////////Test begins here/////////////////////////////////////////////////////////
    //retrieve system memory info 
    ULONG  ulUsedDiskSize,  ulTotalDiskSize, ulUsedRamSize, ulTotalRamSize;
    ret = CTOS_SystemMemoryStatus (&ulUsedDiskSize ,&ulTotalDiskSize,&ulUsedRamSize,&ulTotalRamSize );
    
    strcpy(baBuffer,"&memory=");
    sprintf(str,"%d",ret);
    strcat(baBuffer,str);
    memset(str,0,sizeof(str));
  //Test failed 
  if(ret == d_OK)
  {
        sprintf(str , ",%lu" , ulTotalDiskSize);
        strcat(baBuffer,str);
        memset(str, 0 , sizeof(str));
        
        sprintf(str , ",%lu" , ulUsedDiskSize);
        strcat(baBuffer,str);
        memset(str, 0 , sizeof(str));
        
         sprintf(str , ",%lu" , ulTotalRamSize);
         strcat(baBuffer,str);
         memset(str, 0 , sizeof(str)); 
         
         sprintf(str , ",%lu" , ulUsedRamSize);
         strcat(baBuffer,str);
         memset(str, 0 , sizeof(str));
          
  }
    
    //write memory test results to file
    strcat(baTestResult , baBuffer);
  memset(baBuffer,0,sizeof baBuffer);
    
  //test Crypto
  ret = CryptoTest();
    strcpy(baBuffer,"&crypto=");
    sprintf(str,"%d",ret);
    strcat(baBuffer,str);
    memset(str,0,sizeof(str));
    
    //write Crypto test result to file
  strcat(baTestResult , baBuffer);
  memset(baBuffer,0,sizeof baBuffer);
  

  //test printer
  ret = PrinterTest();
  strcpy(baBuffer,"&printer=");
    sprintf(str,"%d",ret);
    strcat(baBuffer,str);
    memset(str,0,sizeof(str));
    
  //write printer test result to file 
  strcat(baTestResult , baBuffer);
  memset(baBuffer,0,sizeof baBuffer);
  
    
    //test timer 
  /*ret = TimerTest();
  strcpy(baBuffer,"&timer=");
    sprintf(str,"%d",ret);
    strcat(baBuffer,str);
    memset(str,0,sizeof(str));
    
  //write timer test result to file
  strcat(baTestResult , baBuffer);
  memset(baBuffer,0,sizeof baBuffer);
    */
  //test RTC
  ret = RTCTest();
  strcpy(baBuffer,"&rtc=");
    sprintf(str,"%d",ret);
    strcat(baBuffer,str);
    memset(str,0,sizeof(str));
  
  //write Rtc test result to file
  strcat(baTestResult , baBuffer);
  memset(baBuffer,0,sizeof baBuffer);
 
    
  //test Buzzer
  ret = BuzzerTest();
  strcpy(baBuffer,"&buzzer=");
    sprintf(str,"%d",ret);
    strcat(baBuffer,str);
    memset(str,0,sizeof(str));
    
  //write buzzer test result to file
  strcat(baTestResult , baBuffer);
  memset(baBuffer,0,sizeof baBuffer);
    
    
  //test LED
  ret = LEDTest();
  strcpy(baBuffer,"&led=");
    sprintf(str,"%d",ret);
    strcat(baBuffer,str);
    memset(str,0,sizeof(str));
    
  //write LED test result to file
  strcat(baTestResult , baBuffer);
  memset(baBuffer,0,sizeof baBuffer);
    
  //clear LCD display
  CTOS_LCDTClearDisplay();
    return d_OK;
    
}

