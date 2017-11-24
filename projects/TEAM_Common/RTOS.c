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

static void AppTask(void *pvParameters){
	for (;;) {
#if PL_CONFIG_HAS_DEBOUNCE
#else
    KEY_Scan(); /* scan keys and set events */
#endif
    WAIT1_WaitOSms(50);
    EVNT_HandleEvent(APP_EventHandler, TRUE);
    vTaskDelay(pdMS_TO_TICKS(10));
  }

	}

static void BlinkyTask(void *pvParameters){
for (;;) {
	vTaskDelay (500/ portTICK_PERIOD_MS);
LED1_Neg();
}
}
static void BlinkyTask2(void *pvParameters){
for (;;) {
LED2_Neg();
vTaskDelay (500/ portTICK_PERIOD_MS);
}
}



void RTOS_Init(void) {
  /*! \todo Create tasks here */
	xTaskHandle taskHndl;
	/*xTaskCreate(BlinkyTask, "Blinky", 200/sizeof(StackType_t), NULL, 1,&taskHndl);
#if (PL_LOCAL_CONFIG_BOARD_IS_ROBO)
	xTaskCreate(BlinkyTask2, "Blinky2", 200/sizeof(StackType_t), NULL, 1,&taskHndl);*/
//#endif
/*if (xTaskCreate(AppTask, "App", 200/sizeof(StackType_t), NULL, tskIDLE_PRIORITY, NULL)!=pdPASS) {
	    for(;;) {}  error?
	  }*/

}

void RTOS_Deinit(void) {
  /* nothing needed for now */
}

#endif /* PL_CONFIG_HAS_RTOS */
