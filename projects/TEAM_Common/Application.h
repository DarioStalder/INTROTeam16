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

void APP_HandleEvent(void (*callback)(EVNT_Handle), bool clearEvent);

// DZ Header declaration of the HARD FAULT functions;
void (*f)(void);
void call_null_pointer_function(void); /* will execute code at address zero */
void write_to_rom(void); /* tries to write to address zero */

//DZ new function included blinky, App sTart


#endif /* SOURCES_FS2016_COMMON_APPLICATION_H_ */
