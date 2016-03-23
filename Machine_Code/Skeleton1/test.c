
/*========================================*
* I N C L U D E S *
*========================================*/
#include <string.h>
#include <ctosapi.h>
#include "util.h"
#include "test.h"

/*==========================================*
* D E F I N E S *
*==========================================*/
#define d_BUFF_SIZE 128 //Buffer Size

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



