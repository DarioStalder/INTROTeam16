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
#include "Application.h"


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
	FRTOS1_xTaskCreate(BlinkyTask, "Blinky1", configMINIMAL_STACK_SIZE+50,NULL,1, &taskHndl);
}

void RTOS_Deinit(void) {
  /* nothing needed for now */
}

#endif /* PL_CONFIG_HAS_RTOS */
