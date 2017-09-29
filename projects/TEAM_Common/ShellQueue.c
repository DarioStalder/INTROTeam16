/**
 * \file
 * \brief Shell Message Queue module.
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This module uses queues for message passing to the Shell.
 */

#include "Platform.h"
#if PL_CONFIG_HAS_SHELL_QUEUE
#include "ShellQueue.h"
#include "FRTOS1.h"

static xQueueHandle SQUEUE_Queue;

#if PL_CONFIG_SQUEUE_SINGLE_CHAR
  #define SQUEUE_LENGTH      48 /* items in queue, that's my buffer size */
  #define SQUEUE_ITEM_SIZE   1  /* each item is a single character */
#else
  #define SQUEUE_LENGTH      5 /* items in queue */
  #define SQUEUE_ITEM_SIZE   sizeof(char_t*) /* each item is a char pointer to a string, allocated in the heap */
#endif

void SQUEUE_SendString(const unsigned char *str) {
  /*! \todo Understand usage of queues */
#if PL_CONFIG_SQUEUE_SINGLE_CHAR
  while(*str!='\0') {
    if (xQueueSendToBack(SQUEUE_Queue, str, 100/portTICK_PERIOD_MS)!=pdPASS) {
      /*for(;;){}*/ /* ups? */ /* loosing character */
    }
    str++;
  }
#else
  unsigned char *ptr;
  size_t bufSize;

  bufSize = UTIL1_strlen(str)+1;
  ptr = pvPortMalloc(bufSize);
  UTIL1_strcpy(ptr, bufSize, str);
  if (xQueueSendToBack(SQUEUE_Queue, &ptr, portMAX_DELAY)!=pdPASS) {
    for(;;){} /* ups? */
  }
#endif
}

#if PL_CONFIG_SQUEUE_SINGLE_CHAR
unsigned char SQUEUE_ReceiveChar(void) {
  /*! \todo Understand functionality */
  unsigned char ch;
  portBASE_TYPE res;

  res = xQueueReceive(SQUEUE_Queue, &ch, 0);
  if (res==errQUEUE_EMPTY) {
    return '\0';
  } else {
    return ch;
  }
}
#else
unsigned char *SQUEUE_ReceiveMessage(void) {
  unsigned char *ptr;
  portBASE_TYPE res;

  res = xQueueReceive(SQUEUE_Queue, &ptr, 0);
  if (res==errQUEUE_EMPTY) {
    return NULL;
  } else {
    return ptr;
  }
}
#endif /* QUEUE_SINGLE_CHAR */

unsigned short SQUEUE_NofElements(void) {
  return (unsigned short)uxQueueMessagesWaiting(SQUEUE_Queue);
}

void SQUEUE_Deinit(void) {
  vQueueDelete(SQUEUE_Queue); /* this will unregister the queue too */
  SQUEUE_Queue = NULL;
}

void SQUEUE_Init(void) {
  SQUEUE_Queue = xQueueCreate(SQUEUE_LENGTH, SQUEUE_ITEM_SIZE);
  if (SQUEUE_Queue==NULL) {
    for(;;){} /* out of memory? */
  }
  vQueueAddToRegistry(SQUEUE_Queue, "ShellQueue");
}
#endif /* PL_CONFIG_HAS_SHELL_QUEUE */
