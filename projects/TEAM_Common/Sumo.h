/*
 * Sumo.h
 *
 *  Created on: 16.05.2017
 *      Author: Erich Styger Local
 */

#ifndef SOURCES_INTRO_COMMON_SUMO_C_
#define SOURCES_INTRO_COMMON_SUMO_C_

#include "Platform.h"

#if PL_CONFIG_HAS_SHELL
  #include "CLS1.h"

  /*!
   * \brief Module command line parser
   * \param cmd Pointer to command string to be parsed
   * \param handled Set to TRUE if command has handled by parser
   * \param io Shell standard I/O handler
   * \return Error code, ERR_OK if everything was ok
   */
  uint8_t SUMO_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);
#endif

void SUMO_StartStopSumo(void);
void SUMO_StartSumo(void);
void SUMO_StopSumo(void);
bool SUMO_IsRunningSumo(void);

void SUMO_Init(void);

void SUMO_Deinit(void);

#endif /* SOURCES_INTRO_COMMON_SUMO_C_ */
