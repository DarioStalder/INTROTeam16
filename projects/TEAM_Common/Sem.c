/**
 * \file
 * \brief Semaphore usage
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * Module using semaphores.
 */

/**
 * \file
 * \brief Semaphore usage
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * Module using semaphores.
 */

#include "Platform.h" /* interface to the platform */
#if PL_CONFIG_HAS_SEMAPHORE
#include "FRTOS1.h"
#include "Sem.h"
#include "LED.h"

static xSemaphoreHandle sem = NULL;

static void vSlaveTask(void *pvParameters) {
  /*! \todo Implement functionality */
}

static void vMasterTask(void *pvParameters) {
  /*! \todo send semaphore from master task to slave task */
}

void SEM_Deinit(void) {
}

/*! \brief Initializes module */
void SEM_Init(void) {
}
#endif /* PL_CONFIG_HAS_SEMAPHORE */
