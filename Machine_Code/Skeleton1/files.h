#ifndef FILES_H
#define FILES_H
/*========================================*
* I N C L U D E S *
*========================================*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctosapi.h>


/* Define Error Code*/
#define d_FILE_EXIST 0x01
#define d_FILE_NO_FILES 0x02
#define d_FILE_DIR_OK 0x03
#define d_FILE_CANCEL 0x04
#define d_FILE_LENGTH_SHORT 0x05




/*========================================*
 *GLOBAL VARIABLES *
 *========================================*/
BYTE key;
USHORT ret;
USHORT usTotal;
BYTE babuff[100];
//BYTE baFileBuff[d_BUFF_SIZE];

/* Define Key Mode Option */
typedef enum
{
  eMain,
  eNewFile, 
  eDelFile,
  eDirFile,
  eWrite,
  eRead,
  eRename,
  eMainPage2,
  eSetAttrib
} emKeyMode;
emKeyMode emKM;
typedef struct
{
    BYTE Filename[15];
    ULONG Filesize;
    BYTE FileAttrib;
    ULONG UID;
} FILETYPE;
USHORT listAllFiles(FILETYPE *stFA,USHORT *pusFileNumber)
{
  //Declare Local Variable //
  CTOS_FILE_ATTRIB stAttrib[256];  //Structure array to retrieve the attribute of file //
  USHORT usFileNo;
  USHORT usRet;
  int i;
  
  //List the attributes of all files //
  usRet = CTOS_FileDirAttrib(stAttrib, &usFileNo);
  
  if (usFileNo == 0) return d_FILE_NO_FILES;
 
  *pusFileNumber=usFileNo;
 
  if (usRet != d_OK) return usRet;

  for (i=0; i<usFileNo; i++)
  {
    strcpy(stFA[i].Filename,stAttrib[i].Filename);
    stFA[i].FileAttrib=stAttrib[i].FileAttrib;
    stFA[i].UID=stAttrib[i].UID;
    usRet=CTOS_FileGetSize(stFA[i].Filename,&stFA[i].Filesize);
  }
  return d_OK;
}
USHORT packingFilesInfo(FILETYPE* stFA,USHORT usFileNo,BYTE *baBuffer,ULONG *ulBuffLen){
    int i,cr=0;
    CTOS_LCDTClearDisplay (); 
    if(sizeof(FILETYPE)*usFileNo>*ulBuffLen) return 1;//F_ERROR_MEM;
        for (i=0; i<usFileNo; i++)
        {
            sprintf(baBuffer+cr,"%cfile[%d][name]=%s&file[%d][size]=%d&file[%d][attrib]=%s",((i==0)?'&':'&'),i,stFA[i].Filename,i,(int )stFA[i].Filesize,i,(stFA[i].FileAttrib==d_FA_PRIVATE?"d_FA_PRIVATE":"d_FA_PUBLIC"));
            CTOS_LCDTPrint(baBuffer);
            cr=strlen(baBuffer);
        }
    return d_OK;//0;//FS_OK;success
}
void printFilesInfo(FILETYPE* stFA,USHORT usFileNo){
    int i=0;
    BYTE baLocalBuff[80],key;
    CTOS_LCDTClearDisplay (); 
    for (i=0; i<usFileNo; i++){
        sprintf(baLocalBuff,"Name: %s\n",stFA[i].Filename);
        CTOS_LCDTPrint(baLocalBuff);
        sprintf(baLocalBuff,"Size: %d\n",(int )stFA[i].Filesize);
        CTOS_LCDTPrint(baLocalBuff);
        switch(stFA[i].FileAttrib){
            case d_FA_PRIVATE:
                CTOS_LCDTPrint("Attrib: d_FA_PRIVATE\n");
                break;
            case d_FA_PUBLIC:
                CTOS_LCDTPrint("Attrib:d_FA_PUBLIC\n");
            break;
        }
        sprintf(baLocalBuff,"UID:%d\n",(int )stFA[i].UID);
        CTOS_LCDTPrint(baLocalBuff);
    }
    CTOS_KBDGet(&key);
}
/* ==================================================================
 * FUNCTION NAME: files_errormsg
 * DESCRIPTION: return error message 
 * RETURN: byte array :error message.
 * NOTES: none.
 * ================================================================ */ 
BYTE * files_errormsg(USHORT ret)
{
    switch(ret)
    {
        case d_OK:
        case d_USER_CANCEL:
        case d_FILE_DIR_OK:
            break;
        case d_FILE_EXIST:
            //return "File Exist ";
            return "File Exist";
            break;
        case d_FILE_LENGTH_SHORT:
            return "Length<20bytes";
            break;
        case d_FILE_NO_FILES:
            return "No Any Files";
            break;
        case d_FS_INVALID_PARAMETER:
            return "Invalid Parameter";
            break;
        case d_FS_FHT_FULL:
            return "FHT Full";
            break;
        case d_FS_FILE_ALREADY_OPENED:
            return "Already Opened";
            break;
        case d_FS_FILE_NOT_OPENED:
            return "Not Opened";
            break;
        case d_FS_FILE_NOT_FOUND:
            return "File Not Found";
            break;
        case d_FS_FILE_INVALID_HANDLE:
            return "Invalid handle";
            break;
        case d_FS_DATA_FULL:
            return "Data Full";
            break;
        case d_FS_NOT_INITIALED:
            return "Not Initialed";
            break;
        case d_FS_CHECKSUM_ERROR:
            return "Checksum Error";
            break;
        case d_FS_FILE_ALREADY_EXISTED:
            return "Already Existed";
            break;
        case d_FS_NOT_OWNER:
            return "Not Owner";
            break;
        default:
            return "Other Error !!";
    }
}

/*==================================================================      
 * FUNCTION NAME: NewFile                                                 
 * DESCRIPTION:   Create a new file in the Flash.  (*NOT* support SRAM type)
 * RETURN:        d_FILE_EXIST --> File already is existed   
 *                ret --> Return other messages                             
 * NOTES:         none.                                              
 * ================================================================ */     
USHORT NewFile(BYTE *pbaFileName, ULONG *pulHandle,BYTE bStorageType,BYTE bAttrib)
{
    ULONG ulFileSize = 0, ulHandle;
    
    /* Get the file size with specific file name.
     * If Get file size > 0, the file is already existed      */
    ret = CTOS_FileGetSize(pbaFileName,&ulFileSize);
    
    if (ulFileSize > 0)
        return d_FILE_EXIST;
    
    if ((ret != d_OK) && (ret != d_FS_FILE_NOT_FOUND))
        return ret;
    
    /* Open a file and return a number called a file handle.
     * If the specified file name does not exist , it will be created first. */
    //ret =CTOS_FileOpen(pbaFileName , bStorageType , &ulHandle);
    ret =CTOS_FileOpenAttrib(pbaFileName , bStorageType , pulHandle,bAttrib);
    if (ret == d_OK)
        CTOS_FileClose(*pulHandle);
    
    return ret;
}   
//=================================================
USHORT DeleteFile(BYTE *pbaFileName)
{
    ULONG ulFileSize;
    ret = CTOS_FileGetSize(pbaFileName,&ulFileSize);
    if(ulFileSize > 0)
        /* Delete selected file */
        return CTOS_FileDelete(pbaFileName);
    else
        return d_FS_FILE_NOT_FOUND;
}  
//=========================================
/*USHORT FileWrite(BYTE *pbaString,ULONG ulHandle){
    ret=CTOS_FileWrite(ulHandle,pbaString,strlen(pbaString));
    return ret;
}*/
USHORT FileWrite(BYTE *pcaString,BYTE *baFileName)
{
    ULONG ulHandle,ulPosition;
    
    /* Search & list the files by the same appliction*/
    
    
    /* Open the selected file */
    ret = CTOS_FileReopen(baFileName ,d_STORAGE_FLASH ,&ulHandle);
        
    /* Move the file pointer to a specific position. 
     * Move backward from the end of the file. */
    ret = CTOS_FileSeek(ulHandle ,0 ,d_SEEK_FROM_EOF);
    if (ret != d_OK) 
        return ret;
    
    CTOS_FileGetPosition(ulHandle ,&ulPosition);
    sprintf(babuff,"BPos=%d",(int )ulPosition);
    //CTOS_LCDTPrintXY(1,7,babuff);
    
    /* Write data into this opened file */
    ret = CTOS_FileWrite(ulHandle ,pcaString ,strlen(pcaString));
    if (ret != d_OK) 
        return ret;
    
    CTOS_FileGetPosition(ulHandle ,&ulPosition);
    sprintf(babuff,"FPos=%d",(int )ulPosition);
    //CTOS_LCDTPrintXY(9,7,babuff);
    
    /* Close the opened file */
    CTOS_FileClose(ulHandle);
    return ret;
}
USHORT FileRead(BYTE *baFileName,BYTE *pcaReadString, ULONG *pulLength)
{
    ULONG ulHandle , ulbuffLen;
    
    memset(pcaReadString,0x00,*pulLength);
    
    /* Search & list the files by the same appliction */
   

        /* Reopen a file with specific file name. the file should be created first if not exist */
    ret = CTOS_FileReopen(baFileName ,d_STORAGE_FLASH ,&ulHandle);
    if (ret != d_OK)
        return ret;
    
    /* Read data from this opened file */
    ret = CTOS_FileRead(ulHandle ,pcaReadString ,pulLength);
    if (ret != d_OK)
        return ret;
    
    /* Close the opened file */
    CTOS_FileClose(ulHandle);
    return ret;
}                

#endif

#ifdef NOCTOS_API
//=============================================================================================================================
//
// File System Functions
//
//=============================================================================================================================

// Storage Type
#define d_STORAGE_FLASH                 0
#define d_STORAGE_SRAM                  1

// Attribute
#define d_FA_PRIVATE                    0
#define d_FA_READ                       1       //  Read by other APs
#define d_FA_WRITE                      2       //  Written by other APs
#define d_FA_PUBLIC                     0xFF

#define d_SEEK_FROM_BEGINNING           0
#define d_SEEK_FROM_CURRENT             1
#define d_SEEK_FROM_EOF                 2

//For the whole useless blocks in the file system.
#define d_FORMAT_TYPE_0                 0

//For the last ## blocks, the flash FAT size assigned by this AP, in the file system.
#define d_FORMAT_TYPE_1                 1

typedef struct
{
    BYTE Filename[15];
    ULONG Filesize;
    BYTE FileAttrib;
    ULONG UID;
} CTOS_FILE_ATTRIB;

typedef struct
{
    BYTE  Filename[15];
    ULONG Filesize;
    BYTE  FileStorageType;
    BYTE  RFU[4];
} CTOS_FILE_INFO;   

USHORT CTOS_FileOpen(BYTE* caFileName, BYTE bStorageType, ULONG* pulFileHandle);
USHORT CTOS_FileOpenAttrib(BYTE* caFileName, BYTE bStorageType, ULONG* pulFileHandle ,BYTE bAttrib); //<VK>2011.5.3
USHORT CTOS_FileClose(ULONG ulFileHandle);
USHORT CTOS_FileDelete(BYTE* caFileName);
USHORT CTOS_FileGetSize(BYTE* caFileName, ULONG* pulFileSize);
USHORT CTOS_FileSeek(ULONG ulFileHandle, ULONG ulOffset, BYTE bOrigin);
USHORT CTOS_FileRead(ULONG ulFileHandle, BYTE* baBuffer, ULONG* pulActualLength);
USHORT CTOS_FileWrite(ULONG ulFileHandle, BYTE* baBuffer, ULONG ulBufferLength);
USHORT CTOS_FileDir(BYTE *baFileBuf, ULONG *pulFileSize, USHORT *pusLen);
USHORT CTOS_FileCut (BYTE* caFileName, ULONG ulFileLen);
USHORT CTOS_FileRename (BYTE* caFileName, BYTE* caFileNewName);
USHORT CTOS_FileGetPosition(ULONG ulFileHandle, ULONG *pulPosition);
USHORT CTOS_FileReopen(BYTE* caFileName, BYTE bStorageType, ULONG* pulFileHandle);
USHORT CTOS_FileSetAttrib(BYTE* caFileName, BYTE bAttrib);
USHORT CTOS_FileDirAttrib(CTOS_FILE_ATTRIB *pstFA, USHORT *pusFileNumber);
USHORT CTOS_FileDirA(CTOS_FILE_INFO *pstInfo , USHORT *pusLen);
USHORT CTOS_FileGetAttrib(BYTE* caFileName, CTOS_FILE_ATTRIB *pstFA);
USHORT CTOS_FileFormat(BYTE bType);
USHORT CTOS_FileGetFreeSpace(BYTE bStorgeType, ULONG* pulFreeSize);
USHORT CTOS_SystemMemoryStatus(ULONG* ulUsedDiskSize ,ULONG* ulTotalDiskSize ,ULONG* ulUsedRamSize ,ULONG* ulTotalRamSize);
#endif
