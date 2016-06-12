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
#include <errno.h>
#include <sys/stat.h>
#include <curl/curl.h>
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
  


/*==========================================*
* C O N S T A N T S *
*==========================================*/
const BYTE tmsPaths[][50]={"/MTMS/PosUp.ashx",
                 "/MTMS/CreatePosRecord.ashx",
                 "/MTMS/SubmitCommandResult.ashx",
                  "/MTMS/RequestCommandParameters.ashx",};
/*==========================================*
* T Y P E S *
*==========================================*/

/*==========================================*
* V A R I A B L E S *
*==========================================*/

CURL *curl;
CURLcode res;
USHORT Retry;

BYTE baTmsUrl[50]="193.227.38.138";
//BYTE ftpUser[100]="Administrator";
//BYTE ftpPass[100]="Elgp2.com";
BYTE ftpUser[100]="MTMS_FTP";
BYTE ftpPass[100]="MobiWire1234";


USHORT usTmsPort=80;
//BYTE baTmsUrl[50]="41.47.137.50";
//BYTE ftpUser[100]="MTMS_FTP"; //moneer
//BYTE ftpUser[100]="MTMS_USER";

//BYTE ftpPass[100]="1234";


BYTE commbuff[512];



/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* F U N C T I O N S *
*==========================================*/
USHORT ftpUpload(BYTE *baSrcPath,BYTE *baFileName);

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void **userp)
{
    BYTE key;
    size_t realsize = size * nmemb;
    if(realsize<=1)return realsize;
    BYTE *mem = (BYTE *)*userp;
    mem =realloc(mem,  realsize + 1);
    memset(mem,0,sizeof mem);
    if(mem == NULL)
    {
        /* out of memory! */ 
        
        return 0;
    }
     memcpy(mem, contents, realsize);
    mem[realsize] = 0;
    *userp=mem;
   
    return realsize;
}
USHORT commConfig(IN char *cpHostName,IN char *cpFTPUserName,IN char *cpFTPPass)
{
    strcpy(baTmsUrl,cpHostName);
    //usTmsPort=u16HostPort;IN uint16_t u16HostPort
    strcpy(ftpUser,cpFTPUserName);
    strcpy(ftpPass,cpFTPPass);
}
USHORT commInit()
{
  gprsOpen();  
  gprsConnect(baTmsUrl,usTmsPort);  
  GetSerialNumber(baSerialNumber);
  Retry=2;
}
USHORT commClose()
{
    gprsDisconnect();
    gprsClose();
}
USHORT commSendTms(INOUT BYTE *baMessage,IN USHORT usPathIndex)
{
    //local variables
    BYTE key;
    USHORT usret;
    enuGprsStateType gprsState;
    BYTE *baResponse;
    gprsState = gprsGetState();
    USHORT try=0;
  
    //memory allocation 
    baResponse=(BYTE*)malloc(2);
   
    //check GPRS connection
    if(gprsState!=GPRS_ESTABLISHED)
    {
        usret=gprsConnect(baTmsUrl,usTmsPort);
        if(usret!=d_OK)return usret;
    }

    //cURL starts
    curl_global_init(CURL_GLOBAL_ALL);
    /* get a curl handle */ 
    curl = curl_easy_init();
    if(curl) 
    {
        //local for curl : header
        struct curl_slist *params = NULL;
        
        //POST
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */
        sprintf(commbuff,"http://%s/%s",baTmsUrl,tmsPaths[usPathIndex]); 
        curl_easy_setopt(curl, CURLOPT_URL, commbuff);
        /* Now specify the POST data */ 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,(char*)baMessage);

        //HEADER
        /* Add a custom header */ 
        sprintf(commbuff,"SerialNumber:%s",baSerialNumber);
        params = curl_slist_append(params, commbuff);
        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, params);

        //Time to wait response
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT,1);//lower than 1 byte/sec
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME,50);

        //curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS,55000);


        //Response
        /* send all data to this function  */ 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */ 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&baResponse);
        


        /* Perform the request, res will get the return code */ 
        if(gprsState!=GPRS_ESTABLISHED)
        {
            usret=gprsConnect(baTmsUrl,usTmsPort);
            if(usret!=d_OK)return usret;
        }
        do
        {
            res = curl_easy_perform(curl);
            
            
            /* Check for errors */ 
            if(res != CURLE_OK){
              //fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
                sprintf(commbuff,"Error: commSendTms: curl_easy_perform(): %s\n",curl_easy_strerror(res));
                sysLog(commbuff);
                strcpy(baMessage,"0");

            }

            else
            {
                
                strcpy(baMessage,baResponse);
                break;
            }
            ++try;
        }while(try<Retry);
        /* always cleanup */ 
        
        curl_easy_cleanup(curl);        
    }
    curl_global_cleanup();
   
    //sprintf(commbuff,"Event: commSendTms :Terminated cleanly with%04x\n",usret);
    //sysLog(commbuff);
    sysLogRet("commSendTms",res);
    return res;
}


USHORT commSendFile(IN BYTE *baSrcPath,IN BYTE * baFileName)
{
    //gprsConnect(baTmsUrl,21);
    enuGprsStateType gprsState;
    USHORT usret;
    if(gprsState!=GPRS_ESTABLISHED)
    {
        usret=gprsConnect(baTmsUrl,21);
        if(usret!=d_OK)return usret;
    }
    usret=ftpUpload(baSrcPath,baFileName);
    //sprintf(commbuff,"Event: commSendFile :Terminated cleanly with%04x\n",usret);
    //sysLog(commbuff);
    sysLogRet("commSendFile",usret);
    return usret;
    
}


USHORT ftpUpload(BYTE *baSrcPath,BYTE *baDstPath)
{
   
    CURL *curl;
    CURLcode res;
    LONG codep;
    struct stat file_info;
    double speed_upload, total_time;
    FILE *fd;
    
    fd = fopen(baSrcPath, "rb"); /* open file to upload */ 
    if(!fd) {
        return 1; /* can't continue */ 
    }

    /* to get the file size */ 
    if(fstat(fileno(fd), &file_info) != 0) {
        return 1; /* can't continue */ 
    }
    sprintf(commbuff,"ftp://%s:%s@%s/Terminals/Castles/%s/%s",ftpUser,ftpPass,baTmsUrl,baSerialNumber,baDstPath);//serial number folder



    curl_global_init(CURL_GLOBAL_ALL);//could be called once
    curl = curl_easy_init();
    if(curl) {
        /* upload to this place */ 
        curl_easy_setopt(curl, CURLOPT_URL,commbuff);
        /* tell it to "upload" to the URL */ 
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* set where to read from (on Windows you need to use READFUNCTION too) */ 
        curl_easy_setopt(curl, CURLOPT_READDATA, fd);

        /* and give the size of the upload (optional) */ 
        //curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
        //                 (curl_off_t)file_info.st_size);

        /* enable verbose for easier tracing */ 
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);
        
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &codep);
        //CTOS_LCDTPrint("state=");
        //utilPrintI(codep);
        //CTOS_LCDTPrint("\n");
        /* Check for errors */ 
        if(res != CURLE_OK) {
          
            sprintf(commbuff, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));

        }
        else {
            /* now extract transfer info */ 
            curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

            //sprintf(commbuff, "Speed: %.3f bytes/sec during %.3f seconds\n",
            //speed_upload, total_time);
            ////CTOS_LCDTPrint(commbuff);
        }
    /* always cleanup */ 
    curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return codep;

}

USHORT ftpDownload(BYTE *baSrcPath,BYTE *baDstPath)
{
    CURL *curl;
    CURLcode res;
    LONG codep;
    struct stat file_info;
    double speed_upload, total_time;
    FILE *fd;
   
    fd = fopen(baDstPath, "wb"); /* open file to upload */ 
    if(!fd) {
        return 1; /* can't continue */ 
    }

   
    sprintf(commbuff,"ftp://%s:%s@%s/%s",ftpUser,ftpPass,baTmsUrl,baSrcPath);

    curl_global_init(CURL_GLOBAL_ALL);//could be called once
    curl = curl_easy_init();
    if(curl) {
        /* upload to this place */ 
        curl_easy_setopt(curl, CURLOPT_URL,commbuff);
        
        
       
         curl_easy_setopt(curl, CURLOPT_WRITEDATA, fd);

        /* and give the size of the upload (optional) */ 
        //curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
        //                 (curl_off_t)file_info.st_size);

        /* enable verbose for easier tracing */ 
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);
        
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &codep);
        //CTOS_LCDTPrint("xxstate=");
        //utilPrintI(codep);
        //CTOS_LCDTPrint("\n");
        /* Check for errors */ 
        if(res != CURLE_OK) {
          
            
            sprintf(commbuff,"Error: ftpDownload: curl_easy_perform(): %s\n",curl_easy_strerror(res));
            sysLog(commbuff);
            

        }
        
    /* always cleanup */ 
    curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    fclose(fd);
    return codep;

}

USHORT commRecieveFile(BYTE *baSrcPath,BYTE *baDstPath)
{
    USHORT usret;
    BYTE srcPath[100];
    //gprsConnect(baTmsUrl,21);
    enuGprsStateType gprsState;
   if(gprsState!=GPRS_ESTABLISHED)
    {
        usret=gprsConnect(baTmsUrl,21);
        if(usret!=d_OK)return usret;
    }

    sprintf(srcPath,"Terminals/Castles/%s/%s",baSerialNumber,baSrcPath);

    usret=ftpDownload(srcPath,baDstPath);
    //sprintf(commbuff,"Event: commRecieveFile :Terminated cleanly with%04x\n",usret);
    //sysLog(commbuff);
    sysLogRet("commRecieveFile",usret);
    return usret;
    
}
USHORT commRecieveRoot(BYTE *baSrcPath,BYTE *baDstPath)
{
    USHORT usret;
    enuGprsStateType gprsState;
    gprsConnect(baTmsUrl,21);
    //sprintf(srcPath,"%s",baSrcPath);
    if(gprsState!=GPRS_ESTABLISHED)
    {
        usret=gprsConnect(baTmsUrl,21);
        if(usret!=d_OK)return usret;
    }
    usret=ftpDownload(baSrcPath,baDstPath);
    //sprintf(commbuff,"Event: commRecieveRoot :Terminated cleanly with%04x\n",usret);
    //sysLog(commbuff);
    sysLogRet("commRecieveRoot",usret);
    return usret;
    
}
USHORT commRecieveCAP(BYTE *baSrcPath,BYTE *baDstPath)
{
    enuGprsStateType gprsState;
    USHORT usret;
    BYTE srcPath[100];

    //gprsConnect(baTmsUrl,21);
    //sprintf(cmdline,"wget -O %s/%s ftp://%s:%s@%s/Terminals/Castles/%s > stdout.txt 2> stderr.txt",baFilePath,baFileName,ftpUser,ftpPass,baTmsUrl,baFileName);
    //sysret=system(cmdline);
    //return sysret;
    if(gprsState!=GPRS_ESTABLISHED)
    {
        usret=gprsConnect(baTmsUrl,21);
        if(usret!=d_OK)return usret;
    }
    sprintf(srcPath,"Apps/Castles/%s",baSrcPath);

    usret=ftpDownload(srcPath,baDstPath);
    //sprintf(commbuff,"Event: commRecieveCAP :Terminated cleanly with%04x\n",usret);
    //sysLog(commbuff);
    sysLogRet("commRecieveCAP",usret);
    return usret;
    
}

USHORT wgetftpDownload(BYTE* baHost,BYTE* baUser,BYTE* baPass,BYTE* baPath,BYTE* baFileName)
{
  USHORT sysret;
  BYTE cmdline[300];
  sprintf(cmdline,"wget -O /home/ap/pub/%s ftp://%s:%s@%s/%s/%s > stdout.txt 2> stderr.txt",baFileName,baUser,baPass,baHost,baPath,baFileName);
  sysret=system(cmdline);
  return sysret;
}

