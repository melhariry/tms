#ifndef TEST_H
#define TEST_H

/*========================================*
* I N C L U D E S *
*========================================*/
#include <string.h>
#include <ctosapi.h>

/*==========================================*
* D E F I N E S *
*==========================================*/

/* ==================================================================
* FUNCTION NAME: CryptoTest
* DESCRIPTION: Test Crypto
* RETURN: USHORT -> Crypto Test result code
* FALSE-> Crypto Test Fail
* NOTES: none.
* ================================================================ */
USHORT CryptoTest(void);
/* ==================================================================
* FUNCTION NAME: PrinterTest
* DESCRIPTION: Test printer status
* RETURN: USHORT -> Printer Test result code
* NOTES: none.
* ================================================================ */
USHORT PrinterTest(void);
/* ==================================================================
* FUNCTION NAME: TimerTest
* DESCRIPTION:
* RETURN: USHORT timer test error code.
* NOTES: none.
* ================================================================ */
USHORT TimerTest(void);
/* ==================================================================
* FUNCTION NAME: RTC Test
* DESCRIPTION: Use this function to read the date and the time settings of the Real
Time Clock
* RETURN: USHORT RTC test error code.
* NOTES: none.
* ================================================================= */
USHORT RTCTest(void);
/* ==================================================================
* FUNCTION NAME: Buzzer Test
* DESCRIPTION:
* RETURN: USHORT -> buzzer test error code.
* NOTES: none.
* ================================================================ */
USHORT BuzzerTest(void);
/* ==================================================================
* FUNCTION NAME: LED Test
* DESCRIPTION:
* RETURN: USHORT LED test error code.
* NOTES: none.
* ================================================================ */
USHORT LEDTest(void);


#endif