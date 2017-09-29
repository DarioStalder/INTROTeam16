/**
 * \file
 * \brief Common platform configuration interface.
 * \author Erich Styger, erich.styger@hslu.ch
 */

#ifndef SOURCES_INTRO_COMMON_PLATFORM_H_
#define SOURCES_INTRO_COMMON_PLATFORM_H_

/* include shared header files */
#include "PE_Types.h" /* common Processor Expert types: bool, NULL, ... */
#include "Cpu.h" /* for PEcfg_RoboV2/PEcfg_RoboV1 */

#include "Platform_Local.h" /* local project configuration, present in each project (FRDM, Robot, Remote, ...) */

/* check local platform configuration */
#if PL_LOCAL_CONFIG_BOARD_IS_ROBO
  #define PL_CONFIG_BOARD_IS_FRDM     (0)
  #define PL_CONFIG_BOARD_IS_REMOTE   (0)
  #define PL_CONFIG_BOARD_IS_ROBO     (1)
  #if defined(PEcfg_RoboV2)
    #define PL_CONFIG_BOARD_IS_ROBO_V1  (0)
    #define PL_CONFIG_BOARD_IS_ROBO_V2  (1)
  #else
    #define PL_CONFIG_BOARD_IS_ROBO_V1  (1)
    #define PL_CONFIG_BOARD_IS_ROBO_V2  (0)
  #endif
#elif PL_LOCAL_CONFIG_BOARD_IS_FRDM
  #define PL_CONFIG_BOARD_IS_FRDM     (1)
  #define PL_CONFIG_BOARD_IS_REMOTE   (0)
  #define PL_CONFIG_BOARD_IS_ROBO     (0)
  #define PL_CONFIG_BOARD_IS_ROBO_V1  (0)
  #define PL_CONFIG_BOARD_IS_ROBO_V2  (0)
#elif PL_LOCAL_CONFIG_BOARD_IS_REMOTE
  #define PL_CONFIG_BOARD_IS_FRDM     (0)
  #define PL_CONFIG_BOARD_IS_REMOTE   (1)
  #define PL_CONFIG_BOARD_IS_ROBO     (0)
  #define PL_CONFIG_BOARD_IS_ROBO_V1  (0)
  #define PL_CONFIG_BOARD_IS_ROBO_V2  (0)
#else
  #error "One board type has to be defined in Platform_Local.h!"
#endif

#define PL_CONFIG_HAS_SUMO     (0 && PL_LOCAL_CONFIG_BOARD_IS_ROBO)

/* configuration from local config */
#define PL_CONFIG_NOF_LEDS      PL_LOCAL_CONFIG_NOF_LEDS /* number of LEDs */
#define PL_CONFIG_NOF_KEYS      PL_LOCAL_CONFIG_NOF_KEYS /* number of keys */
#define PL_CONFIG_KEY_1_ISR     PL_LOCAL_CONFIG_KEY_1_ISR /* if key is using interrupt */
#define PL_CONFIG_KEY_2_ISR     PL_LOCAL_CONFIG_KEY_2_ISR /* if key is using interrupt */
#define PL_CONFIG_KEY_3_ISR     PL_LOCAL_CONFIG_KEY_3_ISR /* if key is using interrupt */
#define PL_CONFIG_KEY_4_ISR     PL_LOCAL_CONFIG_KEY_4_ISR /* if key is using interrupt */
#define PL_CONFIG_KEY_5_ISR     PL_LOCAL_CONFIG_KEY_5_ISR /* if key is using interrupt */
#define PL_CONFIG_KEY_6_ISR     PL_LOCAL_CONFIG_KEY_6_ISR /* if key is using interrupt */
#define PL_CONFIG_KEY_7_ISR     PL_LOCAL_CONFIG_KEY_7_ISR /* if key is using interrupt */
#define PL_CONFIG_HAS_KBI       (PL_CONFIG_KEY_1_ISR||PL_CONFIG_KEY_2_ISR||PL_CONFIG_KEY_3_ISR||PL_CONFIG_KEY_4_ISR||PL_CONFIG_KEY_5_ISR||PL_CONFIG_KEY_6_ISR||PL_CONFIG_KEY_7_ISR)

/* driver configuration: first entry (0 or 1) disables or enables the driver. Using the _DISABLED define the local configuration can disable it too */
/* general features */
#define PL_CONFIG_HAS_LEDS              (1 && !defined(PL_LOCAL_CONFIG_HAS_LEDS_DISABLED)) /* LED driver */
#define PL_CONFIG_HAS_EVENTS            (1 && !defined(PL_LOCAL_CONFIG_HAS_EVENTS_DISABLED)) /* event driver */
#define PL_CONFIG_HAS_TIMER             (1 && !defined(PL_LOCAL_CONFIG_HAS_TIMER_DISABLED)) /* timer interrupts */
#define PL_CONFIG_HAS_KEYS              (1 && !defined(PL_LOCAL_CONFIG_HAS_KEYS_DISABLED)) /* support for keys */
#define PL_CONFIG_HAS_TRIGGER           (1 && !defined(PL_LOCAL_CONFIG_HAS_TRIGGER_DISABLED)) /* support for triggers */
#define PL_CONFIG_HAS_DEBOUNCE          (1 && !defined(PL_LOCAL_CONFIG_HAS_DEBOUNCE_DISABLED)) /* support for debouncing */
#define PL_CONFIG_HAS_RTOS              (1 && !defined(PL_LOCAL_CONFIG_HAS_RTOS_DISABLED)) /* RTOS support */
#define PL_CONFIG_HAS_SHELL             (1 && !defined(PL_LOCAL_CONFIG_HAS_SHELL_DISABLED)) /* shell support disabled for now */
#define PL_CONFIG_HAS_SEGGER_RTT        (1 && !defined(PL_LOCAL_CONFIG_HAS_SEGGER_RTT_DISABLED) && PL_CONFIG_HAS_SHELL) /* using RTT with shell */
#define PL_CONFIG_HAS_SHELL_QUEUE       (1 && !defined(PL_LOCAL_CONFIG_HAS_SHELL_QUEUE_DISABLED) && PL_CONFIG_HAS_SHELL) /* enable shell queueing */
#define PL_CONFIG_SQUEUE_SINGLE_CHAR    (1 && !defined(PL_LOCAL_CONFIG_HAS_SQUEUE_SINGLE_CHAR_DISABLED) && PL_CONFIG_HAS_SHELL_QUEUE) /* using single character shell queue */
#define PL_CONFIG_HAS_SEMAPHORE         (1 && !defined(PL_LOCAL_CONFIG_HAS_SEMAPHORE_DISABLED)) /* semaphore tests */
#define PL_CONFIG_HAS_CONFIG_NVM        (1 && !defined(PL_LOCAL_CONFIG_HAS_CONFIG_NVM_DISABLED))
#define PL_CONFIG_HAS_RADIO             (1 && !defined(PL_LOCAL_CONFIG_HAS_RADIO_DISABLED))
#define PL_CONFIG_HAS_USB_CDC           (1 && !defined(PL_LOCAL_CONFIG_HAS_USB_CDC_DISABLED))

/* remote controller specific features */
#define PL_CONFIG_HAS_LCD               (1 && !defined(PL_LOCAL_CONFIG_HAS_LCD_DISABLED))
#define PL_CONFIG_HAS_LCD_MENU          (1 && !defined(PL_LOCAL_CONFIG_HAS_LCD_MENU_DISABLED) && PL_CONFIG_HAS_LCD)
#define PL_CONFIG_HAS_REMOTE            (1 && !defined(PL_LOCAL_CONFIG_HAS_REMOTE_DISABLED))
#define PL_CONFIG_CONTROL_SENDER        (1 && !defined(PL_LOCAL_CONFIG_HAS_CONTROL_SENDER_DISABLED))
#define PL_CONFIG_HAS_JOYSTICK          (1 && !defined(PL_LOCAL_CONFIG_HAS_JOYSTICK_DISABLED) && PL_CONFIG_BOARD_IS_FRDM)

/* robot specific features: */
#define PL_CONFIG_HAS_BUZZER            (1 && !defined(PL_LOCAL_CONFIG_HAS_BUZZER_DISABLED) && PL_CONFIG_BOARD_IS_ROBO) /* support for buzzer */
#define PL_CONFIG_HAS_BLUETOOTH         (1 && !defined(PL_LOCAL_CONFIG_HAS_BLUETOOTH_DISABLED) && PL_CONFIG_BOARD_IS_ROBO)
#define PL_CONFIG_HAS_MOTOR             (1 && !defined(PL_LOCAL_CONFIG_HAS_MOTOR_DISABLED) && PL_CONFIG_BOARD_IS_ROBO)
#define PL_CONFIG_HAS_QUADRATURE        (1 && !defined(PL_LOCAL_CONFIG_HAS_QUADRATURE_DISABLED) && PL_CONFIG_HAS_MOTOR)
#define PL_CONFIG_HAS_MOTOR_TACHO       (1 && !defined(PL_LOCAL_CONFIG_HAS_MOTOR_TACHO_DISABLED) && PL_CONFIG_HAS_QUADRATURE)
#define PL_CONFIG_HAS_MCP4728           (1 && !defined(PL_LOCAL_CONFIG_HAS_MPC4728_DISABLED) && PL_CONFIG_BOARD_IS_ROBO && PL_CONFIG_BOARD_IS_ROBO_V1) /* only for V1 robot */
#define PL_CONFIG_HAS_QUAD_CALIBRATION  (1 && !defined(PL_LOCAL_CONFIG_HAS_QUAD_CALIBRATION_DISABLED) && PL_CONFIG_HAS_MCP4728)
#define PL_CONFIG_HAS_PID               (1 && !defined(PL_LOCAL_CONFIG_HAS_PID_DISABLED) && PL_CONFIG_HAS_QUADRATURE)
#define PL_CONFIG_HAS_DRIVE             (1 && !defined(PL_LOCAL_CONFIG_HAS_DRIVE_DISABLED) && PL_CONFIG_HAS_PID)
#define PL_CONFIG_HAS_REFLECTANCE       (1 && !defined(PL_LOCAL_CONFIG_HAS_REFLECTANCE_DISABLED) && PL_CONFIG_BOARD_IS_ROBO)
#define PL_CONFIG_HAS_LINE_FOLLOW       (1 && !defined(PL_LOCAL_CONFIG_HAS_LINE_FOLLOW_DISABLED)/* && PL_CONFIG_HAS_DRIVE*/)
#define PL_CONFIG_HAS_TURN              (1 && !defined(PL_LOCAL_CONFIG_HAS_TURN_DISABLED) && PL_CONFIG_HAS_QUADRATURE)
#define PL_CONFIG_HAS_LINE_MAZE         (1 && !defined(PL_LOCAL_CONFIG_HAS_LINE_MAZE_DISABLED) && PL_CONFIG_HAS_LINE_FOLLOW)
#define PL_HAS_DISTANCE_SENSOR          (1 && !defined(PL_LOCAL_CONFIG_HAS_DISTANCE_DISABLED) && PL_CONFIG_BOARD_IS_ROBO)
#define PL_HAS_TOF_SENSOR               (1 && !defined(PL_LOCAL_CONFIG_HAS_TOF_SENSOR_DISABLED) && PL_HAS_DISTANCE_SENSOR)
#define PL_HAS_SIDE_DISTANCE            (0)
#define PL_HAS_FRONT_DISTANCE           (0)

#define PL_CONFIG_HAS_BATTERY_ADC       (1 && !defined(PL_LOCAL_CONFIG_HAS_BATTERY_ADC_DISABLED) && PL_CONFIG_BOARD_IS_ROBO)

#define PL_CONFIG_HAS_SNAKE_GAME        (1 && !defined(PL_LOCAL_CONFIG_HAS_SNAKE_GAME_DISABLED) && PL_CONFIG_HAS_LCD)

/*!
 * \brief Driver de-initialization
 */
void PL_Deinit(void);

/*!
 * \brief  driver initialization
 */
void PL_Init(void);

#endif /* SOURCES_INTRO_COMMON_PLATFORM_H_ */
