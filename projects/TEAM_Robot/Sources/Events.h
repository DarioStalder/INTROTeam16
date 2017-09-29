/* ###################################################################
**     Filename    : Events.h
**     Project     : INTRO_Robot_Master
**     Processor   : MK22FX512VLQ12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-09-20, 21:03, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "CLS1.h"
#include "MCUC1.h"
#include "WAIT1.h"
#include "CS1.h"
#include "HF1.h"
#include "XF1.h"
#include "KIN1.h"
#include "UTIL1.h"
#include "LEDPin1.h"
#include "BitIoLdd1.h"
#include "LEDPin2.h"
#include "BitIoLdd2.h"
#include "SW1.h"
#include "BitIoLdd3.h"
#include "AS1.h"
#include "ASerialLdd3.h"
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "BUZ1.h"
#include "BitIoLdd4.h"
#include "RTT1.h"
#include "TMOUT1.h"
#include "USB1.h"
#include "CDC1.h"
#include "Tx1.h"
#include "Rx1.h"
#include "IFsh1.h"
#include "IntFlashLdd1.h"
#include "USB0.h"
#include "ADC_Bat.h"
#include "TmDt1.h"



#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MK22FN1M0LQ12]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMIINT(void);



/*
** ===================================================================
**     Event       :  QuadInt_OnInterrupt (module Events)
**
**     Component   :  QuadInt [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void QuadInt_OnInterrupt(void);

/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI1_OnInterrupt(void);

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
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
