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


#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>

/*========================================*
* I N C L U D E S *
*========================================*/
#include "execute.h"
#include "test.h"
#include "util.h"

#include "files.h"
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
extern  int alphasort();
int app_select(struct direct   *entry)
{
  if ((strcmp(entry->d_name, ".") == 0) ||(strcmp(entry->d_name, "..") == 0))
    return (FALSE);
  else
  if ((strcmp(entry->d_name, "pub") == 0))
    return (FALSE);
  return (TRUE);
}
int file_select(struct direct   *entry)
{
  if ((strcmp(entry->d_name, ".") == 0) ||(strcmp(entry->d_name, "..") == 0))
    return (FALSE);
  else
  return (TRUE);
}
void ls(BYTE *pathname,BYTE* baNames,BOOL app)
{
    int count,i;
    struct direct **files;
    int file_select();
    int app_select();
    if(app)
      count = scandir(pathname, &files, app_select, alphasort);
    else
      count = scandir(pathname, &files, file_select, alphasort);
    memset(baNames,0,sizeof baNames);

    for (i=1;i<count+1;++i){
    
      //sprintf(baBuffer,"%s\n",files[i-1]->d_name);
      strcat(baNames,files[i-1]->d_name);
      strcat(baNames,"\n");
    }
 
    
  
  
}
USHORT  exeListApps(BYTE* baAppList)
{
  BYTE key;
  CTOS_stCAPInfo appInfo;
  BYTE appNames[1000];
  BYTE *tokens[50];
  int i;
  USHORT index;
  USHORT usNoTokens ;
  BYTE baTemp[100];
  ls("/home/ap/",appNames,TRUE);
  split(appNames, "\n", tokens,&usNoTokens);
  memset(baAppList,0,sizeof baAppList);
  for(i=0;i<usNoTokens;++i)
  {
    sscanf(tokens[i],"%d",&index);
    CTOS_APGet(index,&appInfo);
    memset(baTemp,0,sizeof baTemp);
    sprintf(baTemp,"&name=%s&version=%x%x&com=%s",appInfo.baName,appInfo.baVersion[0],appInfo.baVersion[1],appInfo.baCompany);
    strcpy(baAppList,baTemp);

  }
  return 0;
}

USHORT  exeListFiles(BYTE* baFileList)
{

  CTOS_FILE_ATTRIB stAttrib[256];  //Structure array to retrieve the attribute of file //
  USHORT usFileNo;
  USHORT usRet;
  int i;
  BYTE baTemp[100];
  ULONG hamada;
  NewFile("private",&hamada,d_STORAGE_FLASH,d_FA_PRIVATE);
  //List the attributes of all files //
  usRet = CTOS_FileDirAttrib(stAttrib, &usFileNo);
  memset(baFileList,0, sizeof baFileList);
  for (i=0; i<usFileNo; i++)
  {
    memset(baTemp,0,sizeof baTemp);
    CTOS_FileGetSize(stAttrib[i].Filename,&stAttrib[i].Filesize);
    sprintf(baTemp,"&name=%s\n&attribute=%d\n&size=%ld\n",stAttrib[i].Filename,stAttrib[i].FileAttrib,stAttrib[i].Filesize);
    strcpy(baFileList,baTemp);
  }
  
  return 0;
}





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

