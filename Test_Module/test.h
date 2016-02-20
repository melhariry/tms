#ifndef TEST_H
#define TEST_H

/*========================================*
* I N C L U D E S *
*========================================*/
#include <string.h>
#include <ctosapi.h>
#include "files.h"
/*==========================================*
* D E F I N E S *
*==========================================*/
#define d_BUFF_SIZE 128 //Buffer Size
/* ==================================================================
 * FUNCTION NAME: test_errormsg
 * DESCRIPTION: return error message 
 * RETURN: byte array :error message.
 * NOTES: none.
 * ================================================================ */ 
BYTE * test_errormsg(USHORT ret)
{
    switch(ret)
    {
        
        case d_SYSTEM_INVALID_PARA:
            return "System Invalid Parameter";
        case d_SYSTEM_HALT_FAULT:
            return "System HALT FAULT";
        case d_SYSTEM_SYS_PARA_ABSENT:
            return "System SYS Parameter Absent";
        case d_CRYPTO_INVALID_PARA:
            return "CRYPTO Invalid Parameter";
            break;
        case d_CRYPTO_HAL_FAULT:
            return "CRYPTO HAL Fault";
            break;
        case d_CRYPTO_RNG_TIMEOUT:
            return "CRYPTO RNG TimeOut";
            break;
        case d_TIMER_INVALID_PARA:
            return "Timer Invalid Parameter";
            break;
        case d_TIMER_HAL_FAULT:
            return "Timer HAL Fault";
            break;
        case d_RTC_INVALID_PARA:
            return "RTC Invalid Parameter";
            break;
        case d_RTC_HAL_FAULT:
            return "RTC HAL Fault";
            break;
        case d_PRINTER_HEAD_OVERHEAT:
            return "Printer HEAD OverHeat";
            break;
        case d_PRINTER_PAPER_OUT:
            return "Printer is out of paper";
            break;
        case d_BUZZER_INVALID_PARA:
            return "Buzzer Invalid Parameter";
            break;
        case d_BUZZER_HAL_FAULT:
            return "Buzzer HAL Fault";
            break;
        case d_LED_INVALID_PARA:
            return "LED Invalid Parameter";
            break;
        case d_LED_HAL_FAULT:
            return "LED HAL Fault";
            break;
        case d_BKLIT_NOT_SUPPORT:
            return "BKLIT Not Supported";
            break;
        default:
            return "Other Error !!";
    }
}
/* ==================================================================
* FUNCTION NAME: ith
* DESCRIPTION:
* RETURN: 1 Byte
* NOTES: none.
* ================================================================ */
BYTE ith(BYTE hex_digit)
{
	hex_digit &= 0x0F;
	if(hex_digit > 9)
	{
	return hex_digit - 10 + 'A';
	}
	return hex_digit + '0';
}
/* =================================================================
* FUNCTION NAME: IntToStr
* DESCRIPTION: Integer transform string
* RETURN: 1 Byte
* NOTES: none.
* ================================================================ */
BYTE IntToStr(BYTE* buf, DWORD v)
{
	DWORD upp;
	DWORD i;
	upp = 0;
	i = v;
	while(i)
	{
	i /= 10;
	upp++;
	}
	if(!upp)
	{
	upp = 1;
	}
	for(i = 0; i < upp; i++)
	{
	buf[upp-1-i] = (v % 10) + '0';
	v /= 10;
	}
	return upp;
}
/* ==================================================================
* FUNCTION NAME: unpack
* DESCRIPTION:
* RETURN: none.
* NOTES: none.
* ================================================================ */
void unpack(OUT char* pStr, IN BYTE* pData, IN USHORT Len, IN char* pSpilt)
{
	USHORT i;
	USHORT offset;
	BYTE s_len;
	s_len = strlen(pSpilt);
	for(i = 0; i < Len; ++i)
	{
	offset = (s_len + 2) * i;
	pStr[offset] = ith(pData[i] >> 4);
	pStr[offset+1] = ith(pData[i]);
	if(i < Len - 1)
	{
	memcpy(&pStr[offset+2], pSpilt, s_len);
	}
	}
	pStr[(s_len + 2) * i] = 0;
}
/* ==================================================================
* FUNCTION NAME: CryptoTest
* DESCRIPTION: Test Crypto
* RETURN: USHORT -> Crypto Test result code
* FALSE-> Crypto Test Fail
* NOTES: none.
* ================================================================ */
USHORT CryptoTest(void)
{
	//Declare Local Variable //
	USHORT ret;
	BYTE key;
	BYTE rng[8];
	BYTE temp[d_BUFF_SIZE];
	memset(temp, 0x00, 128);
	ret = CTOS_RNG(rng);
	return ret;
}
/* ==================================================================
* FUNCTION NAME: PrinterTest
* DESCRIPTION: Test printer status
* RETURN: USHORT -> Printer Test result code
* NOTES: none.
* ================================================================ */
USHORT PrinterTest(void)
{
	//Declare Local Variable //
	USHORT ret;
	ret = CTOS_PrinterStatus();
	return ret;
}
/* ==================================================================
* FUNCTION NAME: TimerTest
* DESCRIPTION:
* RETURN: USHORT timer test error code.
* NOTES: none.
* ================================================================ */
USHORT TimerTest(void)
{
	//Declare Local Variable //
	USHORT ret;
	DWORD start, end, distance;
	BYTE key;
	BYTE upp;
	BYTE const TimerID[] = {TIMER_ID_1, TIMER_ID_2, TIMER_ID_3, TIMER_ID_4};
	BYTE str[d_BUFF_SIZE];
	int i;
	//The system timer tick is only incremented at set periods //
	start = CTOS_TickGet();
	CTOS_Delay(1000);
	end = CTOS_TickGet();
	distance = end - start;
	if(distance < 99 || distance > 101)
	{
	goto timer_fail;
	}
	memcpy(str, "Timer ", 16);
	for(i = 0; i < 4; ++i)
	{
	str[5] = '1' + TimerID[i];
	//Set an alarm flag that will be set to d_YES after the time configured by the
	//specified time //
	ret = CTOS_TimeOutSet(TimerID[i], 500);
	if(ret != d_OK)
	{
	goto timer_fail;
	}
	start = CTOS_TickGet();
	//Check the specified timer is timeout or not //
	while(CTOS_TimeOutCheck(TimerID[i]) != d_YES);
	end = CTOS_TickGet();
	distance = end - start;
	if(distance < 499 || distance > 501)
	{
	memset(str, ' ', 16);
	str[16] = 0;
	upp = 0;
	upp += IntToStr(&str[upp], distance);
	goto timer_fail;
	}
	CTOS_Delay(500);
	}
	return ret;
	timer_fail:
		return ret;
}
/* ==================================================================
* FUNCTION NAME: RTC Test
* DESCRIPTION: Use this function to read the date and the time settings of the Real
Time Clock
* RETURN: USHORT RTC test error code.
* NOTES: none.
* ================================================================= */
USHORT RTCTest(void)
{
	//Declare Local Variable //
	USHORT ret;
	CTOS_RTC stRTC;
	BYTE upp;
	BYTE key;
	BYTE str[17];
	//Get date & time //
	ret = CTOS_RTCGet(&stRTC);
	if(ret != d_OK)
	{
	goto rtc_fail;
	}
	memset(str, ' ', 16);
	str[16] = 0;
	upp = 0;
	upp += IntToStr(&str[upp], stRTC.bYear) + 1;
	upp += IntToStr(&str[upp], stRTC.bMonth) + 1;
	upp += IntToStr(&str[upp], stRTC.bDay) + 1;
	upp += IntToStr(&str[upp], stRTC.bHour) + 1;
	upp += IntToStr(&str[upp], stRTC.bMinute) + 1;
	upp += IntToStr(&str[upp], stRTC.bSecond) + 1;
	upp = 0;
	upp += IntToStr(&str[upp], stRTC.bDoW) + 1;
	str[upp] = 0;
	return ret;
	rtc_fail:
	return ret;
}
/* ==================================================================
* FUNCTION NAME: Buzzer Test
* DESCRIPTION:
* RETURN: USHORT -> buzzer test error code.
* NOTES: none.
* ================================================================ */
USHORT BuzzerTest(void)
{
	//Declare Local Variable //
	USHORT ret;
	BYTE key;
	//Generates a simple tone on the system speaker //
	ret = CTOS_Beep();
	if(ret != d_OK)
	{
	goto buuzer_fail;
	}
	CTOS_Delay(500);
	//Generates a simple sound from the system speaker //
	ret = CTOS_Sound(1500, 100);
	if(ret != d_OK)
	{
	goto buuzer_fail;
	}
	return ret;
	buuzer_fail:
	return ret;
}
/* ==================================================================
* FUNCTION NAME: LED Test
* DESCRIPTION:
* RETURN: USHORT LED test error code.
* NOTES: none.
* ================================================================ */
USHORT LEDTest(void)
{
	//Declare Local Variable //
	USHORT ret;
	BYTE key;
	BYTE const Led[] = {d_LED1, d_LED1, d_LED2, d_LED2, d_LED3, d_LED3};
	BYTE const OnOff[] = {d_ON, d_OFF, d_ON, d_OFF, d_ON, d_OFF};
	BYTE str[d_BUFF_SIZE];
	int i;
	str[16] = 0;
	memcpy(str, "LED1 ", 16);
	for(i = 0; i < 6; ++i)
	{
	//Turn on/off specific LED //
	ret = CTOS_LEDSet(Led[i], OnOff[i]);
	str[3] = '1' + Led[i];
	if(OnOff[i]) memcpy(&str[5], "ON ", 3);
	else memcpy(&str[5], "OFF", 3);
	if(ret != d_OK) goto led_fail;
	CTOS_Delay(500);
	}
	return ret;
	led_fail:	
	return ret;
}


/* ==================================================================
* FUNCTION NAME: SystemTest
* DESCRIPTION: test system utilities and create a .txt file with the results to be sent to server 
* RETURN: error code
* NOTES: call this method to perform systemTest command.
* ================================================================ */
USHORT SystemTest(BYTE *baFileName)
{
	CTOS_LCDTClearDisplay();
            
    CTOS_LCDTPrintXY(1, 1, "Please wait system testing is running\n");

	BYTE baBuffer[250],str[50];
	USHORT ret;
	ULONG ulHandle;
	strcpy(baFileName,"test_result.txt");
	ret=DeleteFile(baFileName);
	ret=NewFile(baFileName,&ulHandle,0,d_FA_PUBLIC);//flash
	if(ret!=d_OK)
    {
        CTOS_LCDTClearDisplay();
        return ret;
    }
	
    //retrieve system memory info 
     ULONG * ulUsedDiskSize, * ulTotalDiskSize, *ulUsedRamSize, *ulTotalRamSize;
    ret = CTOS_SystemMemoryStatus (ulUsedDiskSize ,ulTotalDiskSize,ulUsedRamSize,ulTotalRamSize );
	//Test failed 
	if(ret != d_OK)
	{
		
		strcpy(baBuffer,"Retrieve System Memory INFO FAILED. Error msg: ");	
		strcat(baBuffer,test_errormsg(ret))	;
		strcat(baBuffer,"\n");
	}
	else
	{
		strcpy(baBuffer,"System memory Info:\n");
        
        strcat(baBuffer,"total disk space size: ");
        sprintf(str , "%lu\n" , *ulTotalDiskSize);
        strcat(baBuffer,str);
        memset(str, 0 , sizeof(str));
        
        strcat(baBuffer,"used disk space size: ");
        sprintf(str , "%lu\n" , *ulUsedDiskSize);
        strcat(baBuffer,str);
        memset(str, 0 , sizeof(str));
        
         strcat(baBuffer,"total Ram space size: ");
         sprintf(str , "%lu\n" , *ulTotalRamSize);
         strcat(baBuffer,str);
         memset(str, 0 , sizeof(str)); 
        
         strcat(baBuffer,"used Ram space size: ");
         sprintf(str , "%lu\n" , *ulUsedRamSize);
         strcat(baBuffer,str);
         memset(str, 0 , sizeof(str)); 	
	}
	ret=FileWrite(baBuffer,"test_result.txt");
	memset(baBuffer,0,sizeof baBuffer);
	sprintf(baBuffer,"\nMemory test write=%04x\n",ret);
	CTOS_LCDTPrint(baBuffer);
    
	//test Crypto
	ret = CryptoTest();
	//Test failed 
	if(ret != d_OK)
	{
		
		strcpy(baBuffer,"CRYPTO TEST FAILED. Error msg: ");	
		strcat(baBuffer,test_errormsg(ret))	;
		strcat(baBuffer,"\n");


	}
	else
	{
		strcpy(baBuffer,"CRYPTO TEST PASSED\n");	
	}
	//write: 
	//ret=FileWrite(baBuffer,ulHandle);
	ret=FileWrite(baBuffer,"test_result.txt");
	memset(baBuffer,0,sizeof baBuffer);
	sprintf(baBuffer,"\ncrypto write=%04x\n",ret);
	CTOS_LCDTPrint(baBuffer);
	

	//test printer
	ret = PrinterTest();
	//Test failed 
	if(ret != d_OK)
	{
		strcpy(baBuffer,"PRINTER TEST FAILED. Error msg: ");	
		strcat(baBuffer,test_errormsg(ret));
		strcat(baBuffer,"\n");
	}
	else
	{
		strcpy(baBuffer,"PRINTER TEST PASSED\n");
	}
	//ret=FileWrite(baBuffer,ulHandle);
	ret=FileWrite(baBuffer,"test_result.txt");
	memset(baBuffer,0,sizeof baBuffer);
	sprintf(baBuffer,"\nret write=%04x\n",ret);
	CTOS_LCDTPrint(baBuffer);
	//test timer 
	ret = TimerTest();
	//Test failed 
	if(ret != d_OK)
	{
		strcpy(baBuffer,"TIMER TEST FAILED. Error msg: ");	
		strcat(baBuffer,test_errormsg(ret));
		strcat(baBuffer,"\n");
	}
	else
	{
		strcpy(baBuffer,"Timer TEST PASSED\n");
	}
	//ret=FileWrite(baBuffer,ulHandle);
	ret=FileWrite(baBuffer,"test_result.txt");
	memset(baBuffer,0,sizeof baBuffer);
	sprintf(baBuffer,"\nret write=%04x\n",ret);
	CTOS_LCDTPrint(baBuffer);
	//test RTC
	ret = RTCTest();
	//Test failed 
	if(ret != d_OK)
	{
		strcpy(baBuffer,"RTC TEST FAILED. Error msg: ");	
		strcat(baBuffer,test_errormsg(ret));	
		strcat(baBuffer,"\n");
	}
	else
	{
		strcpy(baBuffer,"RTC TEST PASSED\n");
	}
	//ret=FileWrite(baBuffer,ulHandle);
	ret=FileWrite(baBuffer,"test_result.txt");
	memset(baBuffer,0,sizeof baBuffer);
	sprintf(baBuffer,"\nret write=%04x\n",ret);
	CTOS_LCDTPrint(baBuffer);
	//test Buzzer
	ret = BuzzerTest();
	//Test failed 
	if(ret != d_OK)
	{
		strcpy(baBuffer,"BUZZER TEST FAILED. Error msg: ");	
		strcat(baBuffer,test_errormsg(ret))	;
	}
	else
	{
		strcpy(baBuffer,"BUZZER TEST PASSED\n");
	}
	//ret=FileWrite(baBuffer,ulHandle);
	ret=FileWrite(baBuffer,"test_result.txt");
	memset(baBuffer,0,sizeof baBuffer);
	sprintf(baBuffer,"\nret write=%04x\n",ret);
	CTOS_LCDTPrint(baBuffer);
	//test LED
	ret = LEDTest();
	//Test failed 
	if(ret != d_OK)
	{
		strcpy(baBuffer,"LED TEST FAILED. Error msg: ");	
		strcat(baBuffer,test_errormsg(ret));
		strcat(baBuffer,"\n");	
	}
	else
	{
		strcpy(baBuffer,"LED TEST PASSED\n");
	}
	//ret=FileWrite(baBuffer,ulHandle);
	ret=FileWrite(baBuffer,"test_result.txt");
	memset(baBuffer,0,sizeof baBuffer);
	sprintf(baBuffer,"\nret write=%04x\n",ret);
	CTOS_LCDTPrint(baBuffer);
	//close file
	CTOS_FileClose(ulHandle);
	//clear LCD display
	CTOS_LCDTClearDisplay();
    return d_OK;
}

#endif