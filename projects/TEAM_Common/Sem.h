/**
 * \file
 * \brief Semaphore usage
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * Module demonstrating semaphores.
 */

#ifndef __SEM_H_
#define __SEM_H_

#include "Platform.h" /* interface to the platform */
#if PL_CONFIG_HAS_SEMAPHORE
/*! \brief Initialization of the module */
void SEM_Init(void);

/*! \brief Deinitialization of the module */
void SEM_Deinit(void);
#endif /* PL_CONFIG_HAS_SEMAPHORE */

#endif /* __SEM_H_ */
