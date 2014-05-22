/* ###################################################################
 **     Filename    : ProcessorExpert.c
 **     Project     : ProcessorExpert
 **     Processor   : MKL25Z128VLK4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2014-05-12, 09:30, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file ProcessorExpert.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup ProcessorExpert_module ProcessorExpert module documentation
 **  @{
 */
/* MODULE ProcessorExpert */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Led_Green.h"
#include "LEDpin2.h"
#include "BitIoLdd2.h"
#include "Led_Yellow.h"
#include "LEDpin3.h"
#include "BitIoLdd3.h"
#include "CC_VREG_EN.h"
#include "BitIoLdd4.h"
#include "CC_RESET.h"
#include "BitIoLdd5.h"
#include "CC_CSN.h"
#include "BitIoLdd6.h"
#include "CC_CLK.h"
#include "BitIoLdd7.h"
#include "CC_FIFO.h"
#include "BitIoLdd8.h"
#include "CC_CCA.h"
#include "BitIoLdd9.h"
#include "CC_SFD.h"
#include "BitIoLdd10.h"
#include "CC_FIFOP.h"
#include "ExtIntLdd1.h"
#include "INT_SysTick.h"
#include "EE24LC256.h"
#include "MPL_RST.h"
#include "BitIoLdd12.h"
#include "MPL_SHDN.h"
#include "BitIoLdd13.h"
#include "WAIT1.h"
#include "GI2C1.h"
#include "I2C0.h"
#include "USBD.h"
#include "USB0.h"
#include "CDC1.h"
#include "Tx1.h"
#include "Rx1.h"
#include "CS1.h"
#include "UTIL1.h"
#include "WP.h"
#include "BitIoLdd1.h"
#include "INT_LPTimer.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "mpl115a2.h"
#include <stdio.h>

#define USE_USB_CDC 0
#define EEPROM_ADDR 0x00

void SysTick_IRQHandler(void) {}
void LPTMR_IRQHandler(void) {}

#if USE_USB_CDC
static uint8_t cdc_buffer[USBD_DATA_BUFF_SIZE];
static uint8_t in_buffer[USBD_DATA_BUFF_SIZE];

unsigned char buf[16];

static void
CDC_Run(void)
{
  int i, cnt = 0;
  uint32_t val = 0;
  
  (void) CDC1_SendString(
      (unsigned char*) "Hello world from the KL25Z with USB CDC\r\n");
  for (;;)
    {
      while (CDC1_App_Task(cdc_buffer, sizeof(cdc_buffer)) == ERR_BUSOFF)
        {
          /* device not enumerated */
          Led_Yellow_Neg();
          Led_Green_Off();
          WAIT1_Waitms(10);
        }
      Led_Yellow_Off();
      Led_Green_Neg();
      if (CDC1_GetCharsInRxBuf() != 0)
        {
          i = 0;
          while (i < sizeof(in_buffer) - 1
              && CDC1_GetChar(&in_buffer[i]) == ERR_OK)
            {
              i++;
            }
          in_buffer[i] = '\0';
          (void) CDC1_SendString((unsigned char*) "echo: ");
          (void) CDC1_SendString(in_buffer);
          UTIL1_strcpy(buf, sizeof(buf), (unsigned char*) " val: ");
          UTIL1_strcatNum32u(buf, sizeof(buf), val);
          UTIL1_strcat(buf, sizeof(buf), (unsigned char*) "\r\n");
          (void) CDC1_SendString(buf);
          val++;
        }
      else
        {
          WAIT1_Waitms(10);
          cnt++;
          if ((cnt % 1024) == 0)
            { /* from time to time, write some text */
              (void) CDC1_SendString(
                  (unsigned char*) "Type some text and it will echo.\r\n");
            }
        }
    }
}
#endif /* USE_USB_CDC */

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int
main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
  uint8_t res;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
  //mpl115a2_init();
  res = EE24LC256_Test();
  if (res != ERR_OK)
    {
      for (;;) {} /* failure */
    }
#if USE_USB_CDC
  (void) CDC1_SendString((unsigned char*) "EEPROM OK*\r\n");
  (void) EE24LC256_WriteByte(EEPROM_ADDR, 0x0A);
  (void) CDC1_SendString((unsigned char*) "Writing 0x0A to EEPROM...*\r\n");
  WAIT1_Waitms(500);
  (void) EE24LC256_ReadByte(EEPROM_ADDR, &value);
  UTIL1_strcpy(buf, sizeof(buf), (unsigned char*) "Read back EEPROM value: ");
  UTIL1_strcatNum8u(buf, sizeof(buf), value);
  UTIL1_strcat(buf, sizeof(buf), (unsigned char*) "\r\n");
  (void) CDC1_SendString(buf);
  
  CDC_Run();
#endif /* USE_USB_CDC */
  for(;;)
    {
      Led_Green_Neg();
      Led_Yellow_Neg();
      WAIT1_Waitms(500);
    }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END ProcessorExpert */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.3 [05.08]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
