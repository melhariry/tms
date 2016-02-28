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

/*========================================*
* D E F I N E S *
*========================================*/

/*========================================*
* Machine utilities 
*========================================*/
USHORT GetSerialNumber(OUT BYTE * SerialNumber)
{
	BYTE packedSN[17];
	USHORT ret=CTOS_GetSerialNumber(packedSN);
	if(ret!=d_OK)
		return ret;
	unpack(SerialNumber,packedSN,8,"");
	return d_OK;
}

USHORT GetGPRSIP(OUT BYTE * GPRSIP)
{
	BYTE baLocalIP[5];
	USHORT ret=CTOS_TCP_GPRSGetIP(baLocalIP);
	if(ret!=d_OK)
		return ret;
	sprintf(GPRSIP,"%d.%d.%d.%d",baLocalIP[0],baLocalIP[1],baLocalIP[2],baLocalIP[3]);
	return d_OK;
}




/*========================================*
* string utilities 
*========================================*/

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

#endif