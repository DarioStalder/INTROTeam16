/**
 * \file
 * \brief LED driver
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This is a generic module to drive one or multiple LEDs.
 */

#include "Platform.h"
#if PL_CONFIG_HAS_LEDS
#include "LED.h"

void LED_Deinit(void) {
  LED1_Off();
  LED2_Off();
  LED3_Off();
#if PL_CONFIG_NOF_LEDS>=4
  #error "only 3 LEDs supported"
#endif
}

void LED_Init(void) {
  LED1_Off();
  LED2_Off();
  LED3_Off();
}

#endif /* PL_CONFIG_HAS_LEDS */
