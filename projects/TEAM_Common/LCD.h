/**
 * \file
 * \brief Interface to handle the LCD display
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This is the interface to the Nokia LCD Display.
 */

#ifndef SOURCES_LCD_H_
#define SOURCES_LCD_H_

#include "Platform.h"
#if PL_CONFIG_HAS_LCD

#if PL_CONFIG_HAS_RADIO
#include "RApp.h"
#include "RNWK.h"
#include "RPHY.h"

uint8_t LCD_HandleRemoteRxMessage(RAPP_MSG_Type type, uint8_t size, uint8_t *data, RNWK_ShortAddrType srcAddr, bool *handled, RPHY_PacketDesc *packet);
#endif

#if 1
typedef enum {
  /* button press events */
  LCD_BTN_LEFT,
  LCD_BTN_RIGHT,
  LCD_BTN_UP,
  LCD_BTN_DOWN,
  LCD_BTN_CENTER,
  LCD_SIDE_BTN_UP,
  LCD_SIDE_BTN_DOWN,
  /* button release events */
  LCD_BTN_LEFT_RELEASE,
  LCD_BTN_RIGHT_RELEASE,
  LCD_BTN_UP_RELEASE,
  LCD_BTN_DOWN_RELEASE,
  LCD_BTN_CENTER_RELEASE,
  LCD_SIDE_BTN_UP_RELEASE,
  LCD_SIDE_BTN_DOWN_RELEASE,
  /* special events */
  LCD_REFRESH,
} LCD_BTN_Events;

/*!
 * \brief Sets an LCD button event.
 * \param event Event to be set.
 */
void LCD_SetEvent(LCD_BTN_Events event);
#endif

/*!
 * \brief Driver de-initialization
 */
void LCD_Deinit(void);

/*!
 * \brief Driver initialization
 */
void LCD_Init(void);

#endif /* PL_CONFIG_HAS_LCD */

#endif /* SOURCES_LCD_H_ */
