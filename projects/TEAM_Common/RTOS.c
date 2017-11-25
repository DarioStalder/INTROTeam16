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

#endif /* PL_CONFIG_HAS_RTOS */
