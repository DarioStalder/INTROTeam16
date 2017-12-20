/**
 * \file
 * \brief Real Time Operating System (RTOS) main program.
 * \author Erich Styger, erich.styger@hslu.ch
 */

#include "Platform.h"
#include "LED.h"
#if PL_CONFIG_HAS_RTOS
#include "RTOS.h"
#include "FRTOS1.h"

//DZ hinzugefügt
#include "Application.h"
#include "WAIT1.h"
#include  "KeyDebounce.h"
#include "drive.h"
#include "Trigger.h"
#include "Turn.h"


void RTOS_Init(void) {
  /*! \todo Create tasks here */

	xTaskHandle taskHndl;
//	xTaskCreate(BlinkyTask, "Blinky", 200/sizeof(StackType_t), NULL, 1,&taskHndl);
//#if (PL_LOCAL_CONFIG_BOARD_IS_ROBO)
//	xTaskCreate(BlinkyTask2, "Blinky2", 200/sizeof(StackType_t), NULL, 1,&taskHndl);
//#endif
// 	xTaskCreate(AppTask, "App", 200/sizeof(StackType_t), NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(Key_Task, "Key", configMINIMAL_STACK_SIZE + 100, (void *) NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(Event_Task, "Event", configMINIMAL_STACK_SIZE + 100, (void *) NULL, tskIDLE_PRIORITY, NULL);

	//SUMO_Init();



}

void RTOS_Deinit(void) {
  /* nothing needed for now */
}


// AB hier Sumo Wettkamp Erweiterung


#if 0

//direct function declarition without h-file
void SUMO_StartStopSumo(void);
void SUMO_StartSumo(void);
void SUMO_StopSumo(void);
bool SUMO_IsRunningSumo(void);
void SUMO_Init(void);
void SUMO_Deinit(void);

typedef enum {
  SUMO_STATE_IDLE,
  SUMO_STATE_START_DRIVING,
  SUMO_STATE_DRIVING,
  SUMO_STATE_TURN,
  SUMO_STATE_DETECT,
} SUMO_State_t;


static SUMO_State_t sumoState = SUMO_STATE_IDLE;
static TaskHandle_t sumoTaskHndl;
static bool TRG_setFlag;
TRG_TriggerKind TRG_WAIT5MS;

/* direct task notification bits */
#define SUMO_START_SUMO (1<<0)  /* start sumo mode */
#define SUMO_STOP_SUMO  (1<<1)  /* stop stop sumo */


bool SUMO_IsRunningSumo(void) {
  return sumoState==SUMO_STATE_DRIVING;
}

void SUMO_StartSumo(void) {
  (void)xTaskNotify(sumoTaskHndl, SUMO_START_SUMO, eSetBits);
}

void SUMO_StopSumo(void) {
  (void)xTaskNotify(sumoTaskHndl, SUMO_STOP_SUMO, eSetBits);
}

void SUMO_StartStopSumo(void) {
  if (SUMO_IsRunningSumo()) {
    (void)xTaskNotify(sumoTaskHndl, SUMO_STOP_SUMO, eSetBits);
  } else {
    (void)xTaskNotify(sumoTaskHndl, SUMO_START_SUMO, eSetBits);
  }
}

static void SumoRun(void) {
  uint32_t notifcationValue;
  /*! \todo extend as needed */

  (void)xTaskNotifyWait(0UL, SUMO_START_SUMO|SUMO_STOP_SUMO, &notifcationValue, 0); /* check flags */
  for(;;) { /* breaks */
    switch(sumoState) {
      case SUMO_STATE_IDLE:
        if ((notifcationValue&SUMO_START_SUMO)) {
          sumoState = SUMO_STATE_START_DRIVING;
          break; /* handle next state */
        }
        return;

      case SUMO_STATE_START_DRIVING:
        DRV_SetSpeed(1000, 1000);
        DRV_SetMode(DRV_MODE_SPEED);
        sumoState = SUMO_STATE_TURN;
        break; /* handle next state */


      case SUMO_STATE_TURN:
			if(DIST_NearFrontObstacle(100)){		// front obstracle
				DRV_SetMode(DRV_MODE_SPEED);
				DRV_SetSpeed(2000,2000);
			}else if(DIST_NearRearObstacle(100)){	// rear obstracle
				TURN_Turn(TURN_LEFT180, 0);
				DRV_SetMode(DRV_MODE_SPEED);
				DRV_SetSpeed(1000,1000);
			}else if(DIST_NearRightObstacle(100)){	// right obstracle
				DRV_SetMode(DRV_MODE_SPEED);
				TURN_Turn(TURN_RIGHT90, 0);
			}else if(DIST_NearLeftObstacle(100)){	//left obstracle
				DRV_SetMode(DRV_MODE_SPEED);
				TURN_Turn(TURN_LEFT90, 0);
			}else{
				DRV_SetMode(DRV_MODE_SPEED);
				DRV_SetSpeed(4000,4000);					// nothing detected
			}
			sumoState = SUMO_STATE_DRIVING;
			break;

      case SUMO_STATE_DRIVING:
        if (notifcationValue&SUMO_STOP_SUMO) {
           DRV_SetMode(DRV_MODE_STOP);
           sumoState = SUMO_STATE_IDLE;
           break; /* handle next state */
        }
        return;

      default: /* should not happen? */
        return;
    } /* switch */
  } /* for */
}

static void SumoTask(void* param) {
  sumoState = SUMO_STATE_IDLE;
  for(;;) {
    SumoRun();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void SUMO_Init(void) {
  if (xTaskCreate(SumoTask, "Sumo", 500/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+2, &sumoTaskHndl) != pdPASS) {
    for(;;){} /* error case only, stay here! */
  }
}

#endif /* Sumo Fight */
#endif /* PL_CONFIG_HAS_RTOS */
