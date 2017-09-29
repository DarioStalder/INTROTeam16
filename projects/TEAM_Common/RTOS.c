/**
 * \file
 * \brief Real Time Operating System (RTOS) main program.
 * \author Erich Styger, erich.styger@hslu.ch
 */

#include "Platform.h"
#if PL_CONFIG_HAS_RTOS
#include "RTOS.h"
#include "FRTOS1.h"
#include "Application.h"

void RTOS_Init(void) {
  /*! \todo Create tasks here */
}

void RTOS_Deinit(void) {
  /* nothing needed for now */
}

#endif /* PL_CONFIG_HAS_RTOS */
