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

}

void RTOS_Deinit(void) {
  /* nothing needed for now */
}


// AB hier Sumo Wettkamp Erweiterung


#if HAS_SUMO_FIGHT

//direct function declarition without h-file
void SUMO_StartStopSumo(void);
void SUMO_StartSumo(void);
void SUMO_StopSumo(void);
bool SUMO_IsRunningSumo(void);
void SUMO_Init(void);
void SUMO_Deinit(void);
void setState_FIGHT(void* param);

typedef enum {
		  SUMO_STATE_IDLE,
		  SUMO_STATE_WAIT5MS,
		  SUMO_STATE_FIGHT,
		  SUMO_STATE_DRIVING,
		  SUMO_STATE_CHECK_OBSTACLE,
		  SUMO_STATE_CHECK_LINE,
		  SUMO_STATE_STOP
		} SUMO_State_t;


static SUMO_State_t sumoState = SUMO_STATE_IDLE;
static TaskHandle_t sumoTaskHndl;
static bool TRG_setFlag;
TRG_TriggerKind TRG_WAIT5MS;

/* direct task notification bits */
#define SUMO_START_SUMO (1<<0)  /* start sumo mode */
#define SUMO_STOP_SUMO  (1<<1)  /* stop stop sumo */

setState_FIGHT(void* param){
	(void)param;
	sumoState = SUMO_STATE_FIGHT;
}

bool SUMO_IsRunningSumo(void) {
  return sumoState=SUMO_STATE_DRIVING;
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
		switch (sumoState) {

		case SUMO_STATE_IDLE:
			if ((notifcationValue & SUMO_START_SUMO)) {
				sumoState = SUMO_STATE_WAIT5MS;
				break; /* handle next state */
			}
			return;

		case SUMO_STATE_WAIT5MS:
			if (notifcationValue & SUMO_STOP_SUMO) {
				if (!TRG_setFlag) {
					TRG_setFlag = TRUE;
					TRG_SetTrigger(TRG_WAIT5MS, 5000 / TRG_TICKS_MS,
							(TRG_Callback) setState_FIGHT, NULL);
				}
				break; /* handle next state */
			}
			return;

		case SUMO_STATE_FIGHT:
			if (notifcationValue & SUMO_STOP_SUMO) {
				DRV_SetSpeed(1000, 1000);		// maybe change to 8000 8000
				DRV_SetMode(DRV_MODE_SPEED);
				sumoState = SUMO_STATE_CHECK_OBSTACLE;
				break; /* handle next state */
			}
			return;

		case SUMO_STATE_CHECK_OBSTACLE:
			if (notifcationValue & SUMO_STOP_SUMO) {
				if (DIST_NearFrontObstacle(200))
				{
					DRV_SetMode(DRV_MODE_SPEED);
					DRV_SetSpeed(8000, 8000);
				} else if (DIST_NearRearObstacle(200))
				{
					TURN_Turn(TURN_LEFT180, NULL);
					DRV_SetMode(DRV_MODE_SPEED);
					DRV_SetSpeed(8000, 8000);
				} else if (DIST_NearRightObstacle(200))
				{
					DRV_SetMode(DRV_MODE_SPEED);
					TURN_Turn(TURN_RIGHT90, NULL);
					DRV_SetSpeed(8000, 8000);
				} else if (DIST_NearLeftObstacle(200))
				{
					DRV_SetMode(DRV_MODE_SPEED);
					TURN_Turn(TURN_LEFT90, NULL);
					DRV_SetSpeed(8000, 8000);
				} else
				{
					DRV_SetMode(DRV_MODE_SPEED);
					DRV_SetSpeed(1000, 1000);
				break; /* handle next state */
			}
			return;

		case:

		default: /* should not happen? */
		return;
	}
	/* switch */
} /* for */
}

static void SumoTask(void* param) {
  sumoState = SUMO_STATE_IDLE;
  for(;;) {
    if (notifcationValue&SUMO_STOP_SUMO) {
       sumoState = SUMO_STATE_STOP;
    SumoRun();

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void SUMO_Init(void) {
  if (xTaskCreate(SumoTask, "Sumo", 500/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+2, &sumoTaskHndl) != pdPASS) {
    for(;;){} /* error case only, stay here! */
  }
}








#endif

#endif /* PL_CONFIG_HAS_RTOS */
