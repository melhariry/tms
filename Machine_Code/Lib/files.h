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
BYTE babuff[d_BUFF_SIZE];
BYTE baFileBuff[d_BUFF_SIZE];

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
  for (i=0; i<usFileNo; i++){
    //stFA[i]=stAttrib[i];
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
     for (i=0; i<usFileNo; i++){
        sprintf(baBuffer+cr,"Name: %s\nSize: %d\nAttrib: %s\n",stFA[i].Filename,(int )stFA[i].Filesize,
            (stFA[i].FileAttrib==d_FA_PRIVATE?"d_FA_PRIVATE":"d_FA_PUBLIC"));
        cr=strlen(baBuffer);
    }
    return 0;//FS_OK;success
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
USHORT NewFile(BYTE *pbaFileName, BYTE bStorageType,BYTE bAttrib=d_FA_PRIVATE)
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
    ret =CTOS_FileOpenAttrib(pbaFileName , bStorageType , &ulHandle,bAttrib);
    if (ret == d_OK)
        CTOS_FileClose(ulHandle);
    
    return ret;
}   
//=================================================


/* ==========================================================================
 * FUNCTION NAME: DirectoryFile
 * DESCRIPTION: List all created files by the same application
 * RETURN: d_FILE_DIR_OK --> Search files success
 * d_FILE_NO_FILES --> No any files found
 * NOTES: none.
 * ========================================================================== */
USHORT DirectoryFile(USHORT *pusTotal, BYTE *pbaFileBuff)
{
    int i;
    ULONG ulFileSize[256];  //The maximum value is 256
    BYTE baTemp[d_BUFF_SIZE];
    
    memset(baTemp,0x00,sizeof(baTemp));
    
    // List all created files by the same application.
    // pbaFileBuff --> the file names of the files. Each file name occupies 15 bytes
    // ulFileSize  --> the file sizes of the files. it is array of ULONG
    // pusTotal    --> the number of the files created y the same AP
    CTOS_FileDir(pbaFileBuff , ulFileSize , pusTotal);
    
    if (*pusTotal == 0)
        return d_FILE_NO_FILES;
    
    /* Show the files on the LCD screen */
    for (i=1 ; i<=*pusTotal ; i++)
    {
        sprintf(baTemp,"%d.%s  %d", i ,&pbaFileBuff[15*(i-1)],(int )ulFileSize[i-1]);
        CTOS_LCDTPrintXY(1, i+1, baTemp);
    }
    return d_FILE_DIR_OK;
}

/* ==================================================================                                     
 * FUNCTION NAME: DeleteFile                                                                             
 * DESCRIPTION:   Delete a file with specific file name                                                   
 * RETURN:        d_USER_CANCEL  --> Cancel to delete the file                                      
 *                d_FILE_NO_FILES --> No any files by the same application 
 *                d_FS_FILE_NOT_FOUND --> User select wrong file item 
 *                ret --> Return other messages                                                           
 * NOTES:         none.                                                                        
 * ================================================================ */                                   
USHORT DeleteFile(void)
{
    /* Search & list the files by the same appliction */
    if (DirectoryFile(&usTotal, baFileBuff) != d_FILE_DIR_OK)
        return d_FILE_NO_FILES;
    
    /* To select one file */
    CTOS_KBDGet(&key);
    if (key == d_KBD_CANCEL)
        return d_USER_CANCEL;
    
    if ((key - 0x30) > usTotal)
        return d_FS_FILE_NOT_FOUND;
    
    /* Delete selected file */
    return CTOS_FileDelete(&baFileBuff[15*(key-0x31)]);
}                            

/* ==================================================================                                    
 * FUNCTION NAME: FileRead                                                                              
 * DESCRIPTION: Read data from the opened file 
 * RETURN:      d_USER_CANCEL --> Cancel to Read the file
 *              d_FILE_NO_FILES --> No any files by the same application
 *              d_FS_FILE_NOT_FOUND --> User select wrong file item
 *              ret --> Return other messages                                                                
 * NOTES:       none.                                                                             
 * ================================================================ */                                   
USHORT FileRead(BYTE *pcaReadString, ULONG *pulLength)
{
    ULONG ulHandle , ulbuffLen;
    
    memset(pcaReadString,0x00,*pulLength);
    
    /* Search & list the files by the same appliction */
    if (DirectoryFile(&usTotal, baFileBuff) != d_FILE_DIR_OK)
        return d_FILE_NO_FILES;
    
    /* To select one file */
    CTOS_KBDGet(&key);
    if (key == d_KBD_CANCEL)
        return d_USER_CANCEL;
    if ((key - 0x30) > usTotal)
        return d_FS_FILE_NOT_FOUND;
    
    /* Open the selected file */
    if (strncmp(&baFileBuff[15*(key-0x31)],"FL",2) == 0)
    {
        /* Reopen a file with specific file name. the file should be created first if not exist */
        ret = CTOS_FileReopen(&baFileBuff[15*(key-0x31)] ,d_STORAGE_FLASH ,&ulHandle);
        if (ret != d_OK)
            return ret;
    }
    else
        return d_FS_FILE_NOT_FOUND;
    
    /* Read data from this opened file */
    ret = CTOS_FileRead(ulHandle ,pcaReadString ,pulLength);
    if (ret != d_OK)
        return ret;
    
    /* Close the opened file */
    CTOS_FileClose(ulHandle);
    return ret;
}                                                                                                      

/* ==================================================================                                    
 * FUNCTION NAME: FileWrite
 * DESCRIPTION:   Write data into the opened file
 * RETURN:        d_USER_CANCEL --> Cancel to wirte the file
 *                d_FILE_NO_FILES   --> No any files by the same application
 *                d_FS_FILE_NOT_FOUND --> User select wrong file item
 *                ret --> Return other messages                                                                
 * NOTES:         none.                                                                        
 * ================================================================ */                                   
USHORT FileWrite(BYTE *pcaString)
{
    ULONG ulHandle,ulPosition;
    
    /* Search & list the files by the same appliction*/
    if (DirectoryFile(&usTotal, baFileBuff) != d_FILE_DIR_OK) 
        return d_FILE_NO_FILES;
    
    /* To select one file */ 
    CTOS_KBDGet(&key);
    if (key == d_KBD_CANCEL) 
        return d_USER_CANCEL;
    if ((key - 0x30) > usTotal) 
        return d_FS_FILE_NOT_FOUND;
    
    /* Open the selected file */
    if (strncmp(&baFileBuff[15*(key-0x31)],"FL",2) == 0)
    { 
        /*Reopen a file with specific file name. the file should be created first if not exist */
        ret = CTOS_FileReopen(&baFileBuff[15*(key-0x31)] ,d_STORAGE_FLASH ,&ulHandle);
        if (ret != d_OK) 
            return ret;
    } else {
        return d_FS_FILE_NOT_FOUND;
    }
    
    /* Move the file pointer to a specific position. 
     * Move backward from the end of the file. */
    ret = CTOS_FileSeek(ulHandle ,0 ,d_SEEK_FROM_EOF);
    if (ret != d_OK) 
        return ret;
    
    CTOS_FileGetPosition(ulHandle ,&ulPosition);
    sprintf(babuff,"BPos=%d",(int )ulPosition);
    CTOS_LCDTPrintXY(1,7,babuff);
    
    /* Write data into this opened file */
    ret = CTOS_FileWrite(ulHandle ,pcaString ,strlen(pcaString));
    if (ret != d_OK) 
        return ret;
    
    CTOS_FileGetPosition(ulHandle ,&ulPosition);
    sprintf(babuff,"FPos=%d",(int )ulPosition);
    CTOS_LCDTPrintXY(9,7,babuff);
    
    /* Close the opened file */
    CTOS_FileClose(ulHandle);
    return ret;
}

/* ==================================================================                                  
 * FUNCTION NAME: FileRename 
 * DESCRIPTION:   Change the name of the file
 * RETURN:        d_USER_CANCEL  --> Cancel to rename the file
 *                d_FILE_NO_FILES   --> No any files by the same application
 *                d_FS_FILE_NOT_FOUND --> User select wrong file item
 *                ret --> Return other messages                                                              
 * NOTES:         none.                                                                             
 * ================================================================ */                                  
USHORT FileRename(void)
{
    BYTE tmpKey;
    BYTE baNewName[d_BUFF_SIZE];
    
    memset(baNewName,0x00,sizeof(baNewName));
    memset(baFileBuff,0x00,sizeof(baFileBuff));
    
    /* Search & list the files by the same appliction */
    if (DirectoryFile(&usTotal, baFileBuff) != d_FILE_DIR_OK)
        return d_FILE_NO_FILES;
    
    /* To select one file */
    CTOS_KBDGet(&key);
    if (key == d_KBD_CANCEL)
        return d_USER_CANCEL;
    if ((key - 0x30) > usTotal)
        return d_FS_FILE_NOT_FOUND;
    
    if (strncmp(&baFileBuff[15*(key-0x31)],"FL",2) == 0)
    {
        sprintf(baNewName,"%.*s_1.txt",4,&baFileBuff[15*(key-0x31)]);
        CTOS_LCDTPrintXY(1,8,baNewName);
        CTOS_KBDGet(&tmpKey);
    }
    else
        return d_FS_FILE_NOT_FOUND;
    
    /* Change the name of selected file */
    ret = CTOS_FileRename(&baFileBuff[15*(key-0x31)] , baNewName);
    return ret;
}

/* ==================================================================                                  
 * FUNCTION NAME: FileCut                                                                               
 * DESCRIPTION:   Cut off the file content such that the file length is the specific value                
 * RETURN:        d_FILE_LENGTH_SHORT --> The file data length is short                               
 *                d_USER_CANCEL --> Cancel to cut off the file                                      
 *                d_FILE_NO_FILES   --> No any files by the same application                                
 *                d_FS_FILE_NOT_FOUND --> User select wrong file item 
 *                ret --> Return other messages                                                          
 * NOTES:         none.                                                                          
 * ================================================================ */                                  
USHORT FileCut(void)
{
    USHORT usret;
    ULONG ulFileSize;
    
    /* Search & list the files by the same appliction */
    if (DirectoryFile(&usTotal, baFileBuff) != d_FILE_DIR_OK)
        return d_FILE_NO_FILES;
    
    /* To select one file */
    CTOS_KBDGet(&key);
    if (key == d_KBD_CANCEL)
        return d_USER_CANCEL;
    if ((key - 0x30) > usTotal)
        return d_FS_FILE_NOT_FOUND;
    
    /* Get the file size with specific file name */
    CTOS_FileGetSize(&baFileBuff[15*(key-0x31)],&ulFileSize);
    
    /* Check whether this file size is greater than 20 bytes */
    if (ulFileSize <= 20)
        return d_FILE_LENGTH_SHORT;
    
    /* Cut off the file size to be specific value.
     * The length 20 bytes is the final size of the file.  */
    usret = CTOS_FileCut(&baFileBuff[15*(key-0x31)], 20);
    
    if (DirectoryFile(&usTotal, baFileBuff) != d_FILE_DIR_OK) //Re-get file lists
        return d_FILE_NO_FILES;
    return usret;
}

/* ==========================================================================
 * FUNCTION NAME: FileDirAttrib
 * DESCRIPTION:   List the attributes of all files
 * RETURN:        d_FILE_NO_FILES	--> No any files by the same application
 *		  ret		        --> Return Other messages
 * NOTES:         none.
 * ========================================================================== */
USHORT FileDirAttrib(void)
{
  //Declare Local Variable //
  CTOS_FILE_ATTRIB stAttrib[256];  //Structure array to retrieve the attribute of file //
  USHORT usFileNo;
  int i;
  
  //List the attributes of all files //
  ret = CTOS_FileDirAttrib(stAttrib, &usFileNo);
  if (usFileNo == 0) return d_FILE_NO_FILES;
  
  if (ret != d_OK) return ret;
  
  //Print the filename, the filesize, the attribute, the UID of all files //
  CTOS_PrinterPutString("      Attribute Directory     \n");

  for (i=0; i<usFileNo; i++){
    sprintf(babuff,"No.%d %s",i+1,stAttrib[i].Filename);
    CTOS_PrinterPutString(babuff);
    sprintf(babuff,"Size %d",(int )stAttrib[i].Filesize);
    //CTOS_PrinterPutString(babuff);
    switch(stAttrib[i].FileAttrib){
      case d_FA_PRIVATE:
	CTOS_PrinterPutString("Attrib   d_FA_PRIVATE");
	break;
      case d_FA_PUBLIC:
	CTOS_PrinterPutString("Attrib   d_FA_PUBLIC");
	break;
    }
    sprintf(babuff,"UID %d\n",(int )stAttrib[i].UID);
    CTOS_PrinterPutString(babuff);
  }
  CTOS_PrinterFline(d_LINE_DOT*10);
}

/* ==========================================================================
 * FUNCTION NAME: FileSetAttrib
 * DESCRIPTION:   Set the attribute of the file
 * RETURN:        d_FILE_CANCEL		--> Cancel to cut off the file
 *		  d_FILE_NO_FILES	--> No any files by the same application
 *		  ret		        --> Return Other messages
 * NOTES:         none.
 * ========================================================================== */
USHORT FileSetAttrib(void)
{                                                                                 
  //Declare Local Variable //                                                                           
  BYTE AttribKey;                                                                                       
                                                                                                        
   //Search & list the files by the same appliction //                                                  
  if (DirectoryFile(&usTotal, baFileBuff) != d_FILE_DIR_OK) return d_FILE_NO_FILES;                     
                                                                                                        
  //Select and set the file //                                                                          
  CTOS_KBDGet(&key);                                                                                    
  if ((key - 0x30) > usTotal) return d_USER_CANCEL;                                                     
                                                                                                        
  show_screen(8);                                                                                       
  //Select and set the attribute //                                                                     
  CTOS_KBDGet(&AttribKey);                                                                              
                                                                                                        
  if (AttribKey == d_KBD_1)
  {                                                                                                     
    //Set d_FA_PRIVATE attribute in the file. d_FA_PRIVATE, access only by owner //                       
    ret = CTOS_FileSetAttrib(&baFileBuff[15*(key-0x31)],d_FA_PRIVATE);                                  
  }
  else if (AttribKey == d_KBD_2)
  {                                                                      
    //Set d_FA_PUBLIC attribute in the file. d_FA_PUBLIC, both read and write access are free for all //
    ret = CTOS_FileSetAttrib(&baFileBuff[15*(key-0x31)],d_FA_PUBLIC);                                   
  }else return d_USER_CANCEL;                                                                           
                                                                                                        
  return ret;                                                                                           
}

/* ==========================================================================
 * FUNCTION NAME: FileGetAttrib
 * DESCRIPTION:   Get the attribute of the file
 * RETURN:        d_FILE_CANCEL		--> Cancel to cut off the file
 *		  d_FILE_NO_FILES	--> No any files by the same application
 *		  ret		        --> Return Other messages
 * NOTES:         none.
 * ========================================================================== */
USHORT FileGetAttrib(void)
{
  //Declare Local Variable //
  BYTE AttribKey;
  CTOS_FILE_ATTRIB stAttrib; //Structure to retrieve the attribute of file //
  
  CTOS_LCDTPrint("\fc\fr  GET ATTRIB  \fn"); 
  
  //Search & list the files by the same appliction //
  if (DirectoryFile(&usTotal, baFileBuff) != d_FILE_DIR_OK) return d_FILE_NO_FILES;
  
  //Select to get the attribute of the file //
  CTOS_KBDGet(&key);
  if ((key - 0x30) > usTotal) return d_FILE_CANCEL;
  
  //Get the attribute of the file //
  ret = CTOS_FileGetAttrib(&baFileBuff[15*(key-0x31)],&stAttrib);
  if (ret == d_OK){
    CTOS_LCDTPrint("\fc\fr  GET ATTRIB  \fn");
    CTOS_LCDTPrintXY(1,2,stAttrib.Filename);
    sprintf(babuff,"Size:   %d",(int )stAttrib.Filesize);
    CTOS_LCDTPrintXY(1,3,babuff);
    if (stAttrib.FileAttrib == d_FA_PRIVATE) sprintf(babuff,"Attrib: Privte");
    else if (stAttrib.FileAttrib == d_FA_PUBLIC) sprintf(babuff,"Attrib: Public");
    CTOS_LCDTPrintXY(1,4,babuff);
    sprintf(babuff,"UID:    %d",(int )stAttrib.UID);
    CTOS_LCDTPrintXY(1,5,babuff);
  }
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
