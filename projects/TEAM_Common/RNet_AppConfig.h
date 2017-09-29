/**
 * \file
 * \brief This is a configuration file for the RNet stack
 * \author (c) 2013 Erich Styger, http://mcuoneclipse.com/
 * \note MIT License (http://opensource.org/licenses/mit-license.html)
 *
 * Here the stack can be configured using macros.
 */

#ifndef __RNET_APP_CONFIG__
#define __RNET_APP_CONFIG__

#include "Platform.h"
#if PL_CONFIG_HAS_RADIO
/*! type ID's for application messages */
typedef enum {
  RAPP_MSG_TYPE_STDIN = 0x00,
  RAPP_MSG_TYPE_STDOUT = 0x01,
  RAPP_MSG_TYPE_STDERR = 0x02,
  RAPP_MSG_TYPE_ACCEL = 0x03,
  RAPP_MSG_TYPE_DATA = 0x04,
  RAPP_MSG_TYPE_JOYSTICK_XY = 0x05,
  RAPP_MSG_TYPE_JOYSTICK_BTN = 0x54, /* Joystick button message (data is one byte: 'A', 'B', ... 'F' and 'K') */
  RAPP_MSG_TYPE_REQUEST_SET_VALUE = 0x55,       /* id16:val32, request to set a value for id: 16bit ID followed by 32bit value */
  RAPP_MSG_TYPE_NOTIFY_VALUE = 0x56,            /* id16:val32, notification about a value: 16bit ID followed by 32bit value */
  RAPP_MSG_TYPE_QUERY_VALUE = 0x57,             /* id16, request to query for a value: data ID is a 16bit ID */
  RAPP_MSG_TYPE_QUERY_VALUE_RESPONSE = 0x58,    /* id16:val32, response for RAPP_MSG_TYPE_QUERY_VALUE request: 16bit ID followed by 32bit value */
  RAPP_MSG_TYPE_LAP_POINT = 0xAC,
  /* \todo extend with your own messages */
} RAPP_MSG_Type;

/* IDs for RAPP_MSG_TYPE_REQUEST_SET_VALUE, RAPP_MSG_TYPE_NOTIFY_VALUE, RAPP_MSG_TYPE_QUERY_VALUE and RAPP_MSG_TYPE_QUERY_VALUE_RESPONSE */
typedef enum {
  RAPP_MSG_TYPE_DATA_ID_NONE = 0,           /* dummy ID */
  RAPP_MSG_TYPE_DATA_ID_TOF_VALUES = 4,     /* ToF values: four 8bit values */
  RAPP_MSG_TYPE_DATA_ID_BATTERY_V = 7,      /* Battery voltage */
  RAPP_MSG_TYPE_DATA_ID_PID_FW_SPEED = 8,   /* PID forward speed */
  RAPP_MSG_TYPE_DATA_ID_START_STOP = 9,     /* start/stop robot */
  /*! \todo extend as needed */
} RAPP_MSG_DateIDType;

#endif /* PL_CONFIG_HAS_RADIO */

#endif /* __RNET_APP_CONFIG__ */
