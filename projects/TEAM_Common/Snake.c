/**
 * \file
 * \brief Snake game
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This module implements a classic Nokia phone game: the Snake game.
 *    Based on: https://bitbucket.org/hewerthomn/snake-duino-iii/src/303e1c2a016eb0746b2a7ad1176b16ee78f0177b/SnakeDuinoIII.ino?at=master
 */
/*
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* <phk@FreeBSD.ORG> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return Poul-Henning Kamp
* ----------------------------------------------------------------------------
*/
#include "Platform.h"

#if PL_CONFIG_HAS_SNAKE_GAME
#include "PDC1.h"
#include "WAIT1.h"
#include "GDisp1.h"
#include "FDisp1.h"
#include "GFONT1.h"
#include "Snake.h"
#include "UTIL1.h"
#include "FRTOS1.h"
#include "Event.h"
#include "LCD.h"
#if configUSE_SEGGER_SYSTEM_VIEWER_HOOKS
  #include "SYS1.h"
#endif

/* frame size */
#define MAX_WIDTH  GDisp1_GetWidth()
#define MAX_HEIGHT GDisp1_GetHeight()

/* defaults */
#define SNAKE_LEN   10 /* initial snake length */
#define SNAKE_SPEED 100 /* initial delay in ms */

static int snakeLen = SNAKE_LEN; /* actual snake length */
static int point    = 0, points = 10;
static int level    = 0, time   = SNAKE_SPEED;
static GDisp1_PixelDim xFood = 0, yFood  = 0; /* location of food */

typedef enum {
  SNAKE_DIRECTION_LEFT,
  SNAKE_DIRECTION_RIGHT,
  SNAKE_DIRECTION_UP,
  SNAKE_DIRECTION_DOWN,
} SNAKE_Direction_e;
static SNAKE_Direction_e SnakeCurrentDirection;

#define SNAKE_MAX_LEN   32 /* maximum length of snake */

/* vector containing the coordinates of the individual parts */
/* of the snake {cols[0], row[0]}, corresponding to the head */
static GDisp1_PixelDim snakeCols[SNAKE_MAX_LEN];

/* Vector containing the coordinates of the individual parts */
/* of the snake {cols [snake_lenght], row [snake_lenght]} correspond to the tail */
static GDisp1_PixelDim snakeRow[SNAKE_MAX_LEN];

#if 1
static xTaskHandle gameTaskHandle;
static xQueueHandle SNAKE_Queue;
#define SNAKE_QUEUE_LENGTH     5
#define SNAKE_QUEUE_ITEM_SIZE  sizeof(SNAKE_Event)

bool SNAKE_GameIsRunning(void) {
  eTaskState taskState;

  if (gameTaskHandle==NULL) {
    return FALSE;
  }
  taskState = eTaskGetState(gameTaskHandle);
  return (taskState!=eSuspended); /* only if task is not suspended */
}

void SNAKE_SendEvent(SNAKE_Event event) {
  if (event==SNAKE_SIDE_BOTTOM_EVENT || event==SNAKE_SIDE_TOP_EVENT || event==SNAKE_ENTER_EVENT) {
    event = SNAKE_START_STOP_EVENT;
  } else if (event==SNAKE_RESUME_GAME) {
    vTaskResume(gameTaskHandle);
    return;
  } else if (event==SNAKE_SUSPEND_GAME) {
    vTaskSuspend(gameTaskHandle);
    return;
  }
  if (SNAKE_GameIsRunning()) { /* only if task is not suspended */
#if configUSE_SEGGER_SYSTEM_VIEWER_HOOKS
    uint8_t buf[32];

    UTIL1_strcpy(buf, sizeof(buf), "SNAKE_SendEvent: ");
    UTIL1_strcatNum32u(buf, sizeof(buf), event);
    UTIL1_strcat(buf, sizeof(buf), "\r\n");
    SYS1_Print(buf);
#endif
    if (xQueueSendToBack(SNAKE_Queue, &event, portMAX_DELAY)!=pdPASS) {
      for(;;){} /* ups? */
    }
  }
}

SNAKE_Event SNAKE_ReceiveEvent(bool blocking) {
  portBASE_TYPE res;
  SNAKE_Event event;

  if (blocking) {
    res = xQueueReceive(SNAKE_Queue, &event, portMAX_DELAY);
  } else {
    res = xQueueReceive(SNAKE_Queue, &event, 0);
  }
  if (res==errQUEUE_EMPTY) {
    return SNAKE_NO_EVENT;
  }
#if configUSE_SEGGER_SYSTEM_VIEWER_HOOKS
  {
    uint8_t buf[32];

    UTIL1_strcpy(buf, sizeof(buf), "SNAKE_ReceiveEvent: ");
    UTIL1_strcatNum32u(buf, sizeof(buf), event);
    UTIL1_strcat(buf, sizeof(buf), "\r\n");
    SYS1_Print(buf);
  }
#endif
  return event;
}
#endif

static void waitAnyButton(void) {
  (void)SNAKE_ReceiveEvent(TRUE); /* blocking wait */
}

static int random(int min, int max) {
  TickType_t cnt;
  
  cnt = xTaskGetTickCount();
  cnt &= 0xff; /* reduce to 8 bits */
  if (max>64) {
    cnt >>= 1;
  } else {
    cnt >>= 2;
  }
  if (cnt<min) {
    cnt = min;
  }
  if (cnt>max) {
    cnt = max/2;
  }
  return cnt;
}

static void showPause(void) {
  FDisp1_PixelDim x, y;
  FDisp1_PixelDim charHeight, totalHeight;
  GFONT_Callbacks *font = GFONT1_GetFont();
  uint8_t buf[16];

  FDisp1_GetFontHeight(font, &charHeight, &totalHeight);
  GDisp1_Clear();
  
  x = (GDisp1_GetWidth()-FDisp1_GetStringWidth((unsigned char*)"Pause", font, NULL))/2; /* center text */
  y = 0;
  FDisp1_WriteString((unsigned char*)"Pause", GDisp1_COLOR_BLACK, &x, &y, font);

  x = 0;
  y += totalHeight;
  FDisp1_WriteString((unsigned char*)"Level: ", GDisp1_COLOR_BLACK, &x, &y, font);
  UTIL1_Num16sToStr(buf, sizeof(buf), level);
  FDisp1_WriteString(buf, GDisp1_COLOR_BLACK, &x, &y, font);

  x = 0;
  y += totalHeight;
  FDisp1_WriteString((unsigned char*)"Points: ", GDisp1_COLOR_BLACK, &x, &y, font);
  UTIL1_Num16sToStr(buf, sizeof(buf), point-1);
  FDisp1_WriteString(buf, GDisp1_COLOR_BLACK, &x, &y, font);
  
  x = (GDisp1_GetWidth()-FDisp1_GetStringWidth((unsigned char*)"(Press Button)", font, NULL))/2; /* center text */
  y += totalHeight;
  FDisp1_WriteString((unsigned char*)"(Press Button)", GDisp1_COLOR_BLACK, &x, &y, font);

  GDisp1_UpdateFull();
  
  waitAnyButton();
  GDisp1_Clear();
  GDisp1_UpdateFull();
}

static void resetGame(void) {
  int i;
  FDisp1_PixelDim x, y;
  FDisp1_PixelDim charHeight, totalHeight;
  GFONT_Callbacks *font = GFONT1_GetFont();
  
  GDisp1_Clear();
  FDisp1_GetFontHeight(font, &charHeight, &totalHeight);
  
  x = (GDisp1_GetWidth()-FDisp1_GetStringWidth((unsigned char*)"Ready?", font, NULL))/2; /* center text */
  y = totalHeight;
  FDisp1_WriteString((unsigned char*)"Ready?", GDisp1_COLOR_BLACK, &x, &y, font);
  
  x = (GDisp1_GetWidth()-FDisp1_GetStringWidth((unsigned char*)"(Press Button)", font, NULL))/2; /* center text */
  y += totalHeight;
  FDisp1_WriteString((unsigned char*)"(Press Button)", GDisp1_COLOR_BLACK, &x, &y, font);
  GDisp1_UpdateFull();
  waitAnyButton();
  
  GDisp1_Clear();
  FDisp1_GetFontHeight(font, &charHeight, &totalHeight);
  x = (GDisp1_GetWidth()-FDisp1_GetStringWidth((unsigned char*)"Go!", font, NULL))/2; /* center text */
  y = GDisp1_GetHeight()/2 - totalHeight/2;
  FDisp1_WriteString((unsigned char*)"Go!", GDisp1_COLOR_BLACK, &x, &y, font);
  GDisp1_UpdateFull();
  WAIT1_WaitOSms(1000);
  GDisp1_Clear();
  GDisp1_UpdateFull();
  
  snakeLen = SNAKE_LEN;
  for(i=0; i<snakeLen && i<SNAKE_MAX_LEN; i++) {
    snakeCols[i] = snakeLen+2-i; /* start off two pixels from the boarder */
    snakeRow[i]  = (MAX_HEIGHT/2);
  }
  /* initial food position */
  xFood = (GDisp1_GetWidth()/2);
  yFood = (GDisp1_GetHeight()/2);
 
  level  = 0;  
  point  = 0;
  points = SNAKE_LEN;
  time   = SNAKE_SPEED;
 
  SnakeCurrentDirection = SNAKE_DIRECTION_RIGHT;
}

static void moveSnake(void) {
  int i, dr, dc;

  /* move snake */
  for(i=snakeLen; i>1; i--) { /* extend snake by one position */
    snakeRow[i]  = snakeRow[i-1];
    snakeCols[i] = snakeCols[i-1];
  }
  /* change snake head based on direction */
  switch(SnakeCurrentDirection) {
    case SNAKE_DIRECTION_UP:    { dr = -1; dc =  0;} break;
    case SNAKE_DIRECTION_RIGHT: { dr =  0; dc =  1;} break;
    case SNAKE_DIRECTION_DOWN:  { dr =  1; dc =  0;} break;
    case SNAKE_DIRECTION_LEFT:  { dr =  0; dc = -1;} break;
  }
  snakeRow[0]  += dr;
  snakeCols[0] += dc;
  snakeRow[1]  += dr;
  snakeCols[1] += dc;
}

static void drawSnake(void) {
  int i;

  /* draw snake */
  GDisp1_Clear();
  GDisp1_DrawBox(0, 0, MAX_WIDTH, MAX_HEIGHT, 1, GDisp1_COLOR_BLACK); /* draw border around display */
  GDisp1_DrawCircle(snakeCols[1], snakeRow[1], 1, GDisp1_COLOR_BLACK); /* draw head */
  for(i=0; i<snakeLen; i++) { /* draw from head to tail */
    GDisp1_PutPixel(snakeCols[i], snakeRow[i], GDisp1_COLOR_BLACK);
  }
  GDisp1_DrawFilledBox(xFood, yFood, 3, 3, GDisp1_COLOR_BLACK); /* draw food */
  GDisp1_UpdateFull();
}

static void eatFood(void) {
  /* increase the point and snake length */
  point++;
  snakeLen += 2;
  if (snakeLen>SNAKE_MAX_LEN-1) {
    snakeLen = SNAKE_MAX_LEN-1;
  }
  /* new random coordinates for food */
  xFood = random(1, MAX_WIDTH-3);
  yFood = random(1, MAX_HEIGHT-3);
  drawSnake();  
}

static void upLevel(void) {
  level++;
  point   = 1;
  points += 10;
  if(level > 1) {
    time -= 10;
    if (time<10) {
      time = 10;
    }
  }
}

static void checkButtons(void) {
  /*! \todo Check implementation of handling push button inputs */
  SNAKE_Event event;
  GDisp1_PixelDim xSnake, ySnake; /* snake face */

  event = SNAKE_ReceiveEvent(FALSE);
  /* get head position */
  xSnake = snakeCols[0];
  ySnake = snakeRow[0];
  /* LEFT */
  if(event==SNAKE_LEFT_EVENT && SnakeCurrentDirection!=SNAKE_DIRECTION_RIGHT) {
    if((xSnake > 0 || xSnake <= GDisp1_GetWidth() - xSnake)) {
      SnakeCurrentDirection = SNAKE_DIRECTION_LEFT;
    }
    return;
  }
  /* RIGHT */
  if(event==SNAKE_RIGHT_EVENT && SnakeCurrentDirection!=SNAKE_DIRECTION_LEFT) {
    if((xSnake > 0 || xSnake <= GDisp1_GetWidth() - xSnake)) {
      SnakeCurrentDirection = SNAKE_DIRECTION_RIGHT;
    }
    return;
  }
  /* UP */
  if(event==SNAKE_UP_EVENT && SnakeCurrentDirection!=SNAKE_DIRECTION_DOWN) {
    if((ySnake > 0 || ySnake <= GDisp1_GetHeight() - ySnake)) {
      SnakeCurrentDirection = SNAKE_DIRECTION_UP;
    }
    return;
  }
  /* DOWN */
  if(event==SNAKE_DOWN_EVENT && SnakeCurrentDirection!=SNAKE_DIRECTION_UP) {
    if((ySnake > 0 || ySnake <= GDisp1_GetHeight() - ySnake)) {
      SnakeCurrentDirection = SNAKE_DIRECTION_DOWN;
    }
    return;
  }
  /* START/PAUSE */
  if(event==SNAKE_START_STOP_EVENT) {
    showPause();
  }
}

static void gameover(void) {
  FDisp1_PixelDim x, y;
  FDisp1_PixelDim charHeight, totalHeight;
  GFONT_Callbacks *font = GFONT1_GetFont();
  uint8_t buf[16];

  GDisp1_Clear();
  FDisp1_GetFontHeight(font, &charHeight, &totalHeight);

  x = (GDisp1_GetWidth()-FDisp1_GetStringWidth((unsigned char*)"End of Game", font, NULL))/2; /* center text */
  y = 0;
  FDisp1_WriteString((unsigned char*)"End of Game", GDisp1_COLOR_BLACK, &x, &y, font);

  x = 0;
  y += totalHeight;
  FDisp1_WriteString((unsigned char*)"Level: ", GDisp1_COLOR_BLACK, &x, &y, font);
  UTIL1_Num16sToStr(buf, sizeof(buf), level);
  FDisp1_WriteString(buf, GDisp1_COLOR_BLACK, &x, &y, font);

  x = 0;
  y += totalHeight;
  FDisp1_WriteString((unsigned char*)"Points: ", GDisp1_COLOR_BLACK, &x, &y, font);
  UTIL1_Num16sToStr(buf, sizeof(buf), point-1);
  FDisp1_WriteString(buf, GDisp1_COLOR_BLACK, &x, &y, font);

  x = (GDisp1_GetWidth()-FDisp1_GetStringWidth((unsigned char*)"(Press Button)", font, NULL))/2; /* center text */
  y += totalHeight;
  FDisp1_WriteString((unsigned char*)"(Press Button)", GDisp1_COLOR_BLACK, &x, &y, font);
  GDisp1_UpdateFull();
  WAIT1_WaitOSms(4000);
  waitAnyButton();
 
#if 1 /*! \todo check implementation */
  LCD_SetEvent(LCD_REFRESH);
  vTaskSuspend(NULL); /* pause task until resumed again */
#endif
  resetGame();  
}

static void snake(void) {
  int i;
  GDisp1_PixelDim xSnake, ySnake; /* snake face */
  
  xSnake = snakeCols[0];
  ySnake = snakeRow[0];
  if(point == 0 || point >= points) {
    upLevel();
  }  
  checkButtons();
  /* the snake has eaten the food (right or left) */
  for(i=0; i < 3; i++) { /* check 3 pixels around */
    /* control the snake's head (x) with x-coordinates of the food */
    if((xSnake+1 == xFood) || (xSnake == xFood+1))  {
      /* control the snake's head (y) with y-coordinates of the food */
      if((ySnake == yFood) || (ySnake+1 == yFood) || (ySnake == yFood+1)) {
        eatFood();
      }
    }
    /* the snake has eaten the food (from above or from below) */
    if((ySnake == yFood) || (ySnake == yFood+i)) {
      if((xSnake == xFood) || (xSnake+i == xFood) || (xSnake == xFood+i)) {
        eatFood();
      }
    }    
  }
  if(SnakeCurrentDirection==SNAKE_DIRECTION_LEFT) {
    /* snake touches the left wall */
    if(xSnake == 1) {
      gameover();
    }
    if(xSnake > 1) {
      moveSnake();
      drawSnake();
    }
  }
  if(SnakeCurrentDirection==SNAKE_DIRECTION_RIGHT) {
    /* snake touches the top wall */
    if(xSnake == MAX_WIDTH-1) {
      gameover();
    }
    if(xSnake < MAX_WIDTH-1) {
      moveSnake();
      drawSnake();
    }
  }
  if(SnakeCurrentDirection==SNAKE_DIRECTION_UP) {
    /* snake touches the above wall */
    if(ySnake == 1) {
      gameover();
    }
    if(ySnake > 1) {
      moveSnake();
      drawSnake();
    }
  }
  if(SnakeCurrentDirection==SNAKE_DIRECTION_DOWN) {
    /* snake touches the ground */
    if(ySnake == MAX_HEIGHT-1) {
      gameover();
    }
    if(ySnake  < MAX_HEIGHT-1) {
      moveSnake();
      drawSnake();
    }
  }
}

static void intro(void) {
  FDisp1_PixelDim x, y;
  FDisp1_PixelDim charHeight, totalHeight;
  GFONT_Callbacks *font = GFONT1_GetFont();
  
  GDisp1_Clear();
  FDisp1_GetFontHeight(font, &charHeight, &totalHeight);
  x = (GDisp1_GetWidth()-FDisp1_GetStringWidth((unsigned char*)"Snake Game", font, NULL))/2; /* center text */
  y = totalHeight;
  FDisp1_WriteString((unsigned char*)"Snake Game", GDisp1_COLOR_BLACK, &x, &y, font);
  y += totalHeight;
  x = (GDisp1_GetWidth()-FDisp1_GetStringWidth((unsigned char*)"McuOnEclipse", font, NULL))/2; /* center text */
  FDisp1_WriteString((unsigned char*)"McuOnEclipse", GDisp1_COLOR_BLACK, &x, &y, font);
  GDisp1_UpdateFull();
  WAIT1_WaitOSms(2000);
}

static void SnakeTask(void *pvParameters) {
  intro();
  resetGame();
  drawSnake();
  for(;;) {
    snake();
    vTaskDelay(pdMS_TO_TICKS(time));
  }
}

void SNAKE_Deinit(void) {
  /* nothing to do? */
}

void SNAKE_Init(void) {
  /*! \todo check implementation */
#if 1
  SNAKE_Queue = xQueueCreate(SNAKE_QUEUE_LENGTH, SNAKE_QUEUE_ITEM_SIZE);
  if (SNAKE_Queue==NULL) {
    for(;;){} /* out of memory? */
  }
  vQueueAddToRegistry(SNAKE_Queue, "SnakeQueue");
  if (xTaskCreate(
      SnakeTask,  /* pointer to the task */
        "Snake", /* task name for kernel awareness debugging */
        500/sizeof(StackType_t), /* task stack size */
        (void*)NULL, /* optional task startup argument */
        tskIDLE_PRIORITY,  /* initial priority */
        &gameTaskHandle /* optional task handle to create */
      ) != pdPASS) {
    /*lint -e527 */
    for(;;){}; /* error! probably out of memory */
    /*lint +e527 */
  }
  vTaskSuspend(gameTaskHandle);
#endif
}
#endif /* PL_HAS_SNAKE_GAME */
