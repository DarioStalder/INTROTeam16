/* ###################################################################
**     Filename    : main.c
**     Project     : INTRO_Remote_Master
**     Processor   : MK20DX128VFT5
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-09-20, 21:05, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "MCUC1.h"
#include "LEDPin1.h"
#include "BitIoLdd17.h"
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "WAIT1.h"
#include "UTIL1.h"
#include "KIN1.h"
#include "XF1.h"
#include "IFsh1.h"
#include "IntFlashLdd1.h"
#include "CLS1.h"
#include "AS1.h"
#include "ASerialLdd1.h"
#include "RTT1.h"
#include "SW1.h"
#include "BitIoLdd10.h"
#include "SW2.h"
#include "BitIoLdd11.h"
#include "SW3.h"
#include "BitIoLdd12.h"
#include "SW4.h"
#include "BitIoLdd13.h"
#include "SW5.h"
#include "BitIoLdd14.h"
#include "SW6.h"
#include "BitIoLdd15.h"
#include "SW7.h"
#include "BitIoLdd16.h"
#include "PTA.h"
#include "PTB.h"
#include "PTD.h"
#include "USB0.h"
#include "USB1.h"
#include "CDC1.h"
#include "Tx1.h"
#include "Rx1.h"
#include "TmDt1.h"
#include "TMOUT1.h"
#include "HF1.h"
#include "CS1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "Application.h"
#include "LED.h"
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */

int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  APP_Start();

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

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
