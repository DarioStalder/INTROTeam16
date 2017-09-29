/**
 * \file
 * \brief Main application interface
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This provides the main application entry point.
 */

#ifndef SOURCES_FS2016_COMMON_APPLICATION_H_
#define SOURCES_FS2016_COMMON_APPLICATION_H_

#include "Platform.h"

#if PL_CONFIG_HAS_EVENTS
#include "Event.h"

void APP_EventHandler(EVNT_Handle event);
#endif

void APP_Start(void);

#endif /* SOURCES_FS2016_COMMON_APPLICATION_H_ */
