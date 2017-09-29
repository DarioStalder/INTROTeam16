/**
 * \file
 * \brief This is main application file
 * \author (c) 2013 Erich Styger, http://mcuoneclipse.com/
 * \note MIT License (http://opensource.org/licenses/mit-license.html)
 *
 * This module implements the application part of the program.
 */

#include "Platform.h"
#if PL_CONFIG_HAS_RADIO
#include "RNetConf.h"
#if RNET_CONFIG_REMOTE_STDIO
  #include "RStdIO.h"
#endif
#include "Application.h"
#include "RNet_App.h"
#include "Radio.h"
#include "RStack.h"
#include "RApp.h"
#include "FRTOS1.h"
#include "RPHY.h"
#include "Shell.h"
#include "Motor.h"
#include "TmDt1.h"
#if PL_CONFIG_HAS_REMOTE
  #include "Remote.h"
#endif
#if PL_CONFIG_HAS_LCD
  #include "LCD.h"
#endif

static RNWK_ShortAddrType APP_dstAddr = RNWK_ADDR_BROADCAST; /* destination node address */

typedef enum {
  RNETA_NONE,
  RNETA_POWERUP, /* powered up */
  RNETA_TX_RX,
} RNETA_State;

static RNETA_State appState = RNETA_NONE;

RNWK_ShortAddrType RNETA_GetDestAddr(void) {
  return APP_dstAddr;
}

void RNETA_SendSignal(uint8_t signal) {
  uint8_t data[2];

  data[0] = 0; /* group */
  data[1] = signal;
  (void)RAPP_SendPayloadDataBlock(data, sizeof(data), RAPP_MSG_TYPE_LAP_POINT, APP_RNET_ADDR_TIME_SYSTEM, RPHY_PACKET_FLAGS_NONE);
}

uint8_t RNETA_SendIdValuePairMessage(uint8_t msgType, uint16_t id, uint32_t value, RAPP_ShortAddrType addr, RAPP_FlagsType flags) {
  uint8_t dataBuf[6]; /* 2 byte ID followed by 4 byte data */

  if (msgType==RAPP_MSG_TYPE_QUERY_VALUE) { /* only sending query with the ID, no value needed */
    UTIL1_SetValue16LE(id, &dataBuf[0]);
    return RAPP_SendPayloadDataBlock(dataBuf, sizeof(id), msgType, addr, flags);
  } else {
    UTIL1_SetValue16LE(id, &dataBuf[0]);
    UTIL1_SetValue32LE(value, &dataBuf[2]);
    return RAPP_SendPayloadDataBlock(dataBuf, sizeof(dataBuf), msgType, addr, flags);
  }
}

static TickType_t timeA=0, timeB=0, timeC=0;

static uint8_t HandleDataRxMessage(RAPP_MSG_Type type, uint8_t size, uint8_t *data, RNWK_ShortAddrType srcAddr, bool *handled, RPHY_PacketDesc *packet) {
#if PL_CONFIG_HAS_SHELL
  uint8_t buf[48];
  CLS1_ConstStdIOTypePtr io = CLS1_GetStdio();
#endif
  uint8_t val;
  
  (void)size;
  (void)packet;
  switch(type) {
#if 0
    case RAPP_MSG_TYPE_LAP_POINT:
      if (size==2) {
        *handled = TRUE;
    #if PL_CONFIG_HAS_SHELL
        UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"Group: ");
        UTIL1_strcatNum8u(buf, sizeof(buf), *(data)); /* group number */
        UTIL1_strcat(buf, sizeof(buf), (unsigned char*)" Event: ");
        UTIL1_chcat(buf, sizeof(buf), *(data+1)); /* event: 'A', 'B', 'C', 'X' or 'T' */
        if (*(data+1)=='A' && timeA==0) {
          timeA = xTaskGetTickCount();
        } else if (*(data+1)=='A' && timeA!=0 && timeB==0 && timeC==0) { /* repeat A */
          UTIL1_strcat(buf, sizeof(buf), (unsigned char*)" repeat A");
        } else if (*(data+1)=='B' && timeA!=0 && timeB==0 && timeC==0) {
          timeB = xTaskGetTickCount();
          UTIL1_strcat(buf, sizeof(buf), (unsigned char*)" timeAB: ");
          UTIL1_strcatNum32u(buf, sizeof(buf), timeB-timeA); /* time */
          UTIL1_strcat(buf, sizeof(buf), (unsigned char*)" ms");
        } else if (*(data+1)=='B' && timeA!=0 && timeB!=0 && timeC==0) { /* repeat B */
          UTIL1_strcat(buf, sizeof(buf), (unsigned char*)" repeat B");
        } else if (*(data+1)=='C' && timeA!=0 && timeB!=0 && timeC==0) {
          timeC = xTaskGetTickCount();
          UTIL1_strcat(buf, sizeof(buf), (unsigned char*)" timeAC: ");
          UTIL1_strcatNum32u(buf, sizeof(buf), timeC-timeA); /* time */
          UTIL1_strcat(buf, sizeof(buf), (unsigned char*)" ms");
        } else if (*(data+1)=='C' && timeA==0 && timeB==0 && timeC==0) { /* repeat C */
          UTIL1_strcat(buf, sizeof(buf), (unsigned char*)" repeat C");
        } else if (*(data+1)=='T') { /* test */
          UTIL1_strcat(buf, sizeof(buf), (unsigned char*)" TEST");
        } else {
          if (*(data+1)=='X') { /* did not finish */
            UTIL1_strcat(buf, sizeof(buf), (unsigned char*)" DNF");
          } else {
            UTIL1_strcat(buf, sizeof(buf), (unsigned char*)" ERROR");
          }
          if (timeA==0) {
            timeB = 5*60*1000;
            timeC = timeB+5*60*1000;
          } else if (timeB==0) {
            timeB = timeA+5*60*1000;
            timeC = timeB+5*60*1000;
          } else { /* DNF for time C */
            timeC = timeB+5*60*1000;
          }
        }
        UTIL1_strcat(buf, sizeof(buf), (unsigned char*)"\r\n");
        CLS1_SendStr(buf, io->stdOut);
        if (timeC!=0) {
          DATEREC date;
          TIMEREC time;

          CLS1_SendStr((unsigned char*)"---------------------------------------\r\n", io->stdOut);
          CLS1_SendStr((unsigned char*)"Date\tTime\tGroup\ttimeAB(ms)\ttimeAC(ms)\r\n", io->stdOut);
          CLS1_SendStr((unsigned char*)"---------------------------------------\r\n", io->stdOut);
          buf[0] = '\0';
          (void)TmDt1_GetDate(&date);
          (void)TmDt1_GetTime(&time);
          (void)TmDt1_AddDateString(buf, sizeof(buf), &date, TmDt1_DEFAULT_DATE_FORMAT_STR);
          UTIL1_chcat(buf, sizeof(buf), '\t');
          (void)TmDt1_AddTimeString(buf, sizeof(buf), &time, TmDt1_DEFAULT_TIME_FORMAT_STR);
          UTIL1_chcat(buf, sizeof(buf), '\t');
          UTIL1_strcatNum8u(buf, sizeof(buf), *(data)); /* group number */
          UTIL1_chcat(buf, sizeof(buf), '\t');
          UTIL1_strcatNum32u(buf, sizeof(buf), timeB-timeA); /* time A to B */
          UTIL1_chcat(buf, sizeof(buf), '\t');
          UTIL1_strcatNum32u(buf, sizeof(buf), timeC-timeA); /* time A to C */
          CLS1_SendStr(buf, io->stdOut);
          CLS1_SendStr((unsigned char*)"\r\n---------------------------------------\r\n", io->stdOut);
          timeA = timeB = timeC = 0;
        }
    #endif
      }
      break;
#endif
    case RAPP_MSG_TYPE_DATA: /* generic data message */
      *handled = TRUE;
      val = *data; /* get data value */
#if PL_CONFIG_HAS_SHELL
      CLS1_SendStr((unsigned char*)"Data: ", io->stdOut);
      CLS1_SendNum8u(val, io->stdOut);
      CLS1_SendStr((unsigned char*)" from addr 0x", io->stdOut);
      buf[0] = '\0';
#if RNWK_SHORT_ADDR_SIZE==1
      UTIL1_strcatNum8Hex(buf, sizeof(buf), srcAddr);
#else
      UTIL1_strcatNum16Hex(buf, sizeof(buf), srcAddr);
#endif
      UTIL1_strcat(buf, sizeof(buf), (unsigned char*)"\r\n");
      CLS1_SendStr(buf, io->stdOut);
#endif /* PL_HAS_SHELL */      
      return ERR_OK;
    default: /*! \todo Handle your own messages here */
      break;
  } /* switch */
  return ERR_OK;
}

static const RAPP_MsgHandler handlerTable[] = 
{
#if RNET_CONFIG_REMOTE_STDIO
  RSTDIO_HandleStdioRxMessage,
#endif
  HandleDataRxMessage,
#if PL_CONFIG_HAS_REMOTE
  REMOTE_HandleRemoteRxMessage,
#endif
#if PL_CONFIG_HAS_LCD
  LCD_HandleRemoteRxMessage,
#endif
  NULL /* sentinel */
};

static void RadioPowerUp(void) {
  /* need to ensure that we wait 100 ms after power-on of the transceiver */
  portTickType xTime;
  
  xTime = FRTOS1_xTaskGetTickCount();
  if (xTime<(100/portTICK_PERIOD_MS)) {
    /* not powered for 100 ms: wait until we can access the radio transceiver */
    xTime = (100/portTICK_PERIOD_MS)-xTime; /* remaining ticks to wait */
    FRTOS1_vTaskDelay(xTime);
  }
  (void)RNET1_PowerUp();
}

static void Process(void) {
  for(;;) {
    switch(appState) {
    case RNETA_NONE:
      appState = RNETA_POWERUP;
      continue;
      
    case RNETA_POWERUP:
      RadioPowerUp();
      appState = RNETA_TX_RX;
      break;
      
    case RNETA_TX_RX:
      (void)RNET1_Process();
      break;
  
    default:
      break;
    } /* switch */
    break; /* break for loop */
  } /* for */
}

static void Init(void) {
#if PL_CONFIG_BOARD_IS_FRDM
  RAPP_SetThisNodeAddr(APP_RNET_ADDR_TIME_SYSTEM); /* time keeping system */
#elif PL_CONFIG_BOARD_IS_ROBO /*! \todo */
  RAPP_SetThisNodeAddr(APP_RNET_ADDR_ROBOT); /* robot */
#elif PL_CONFIG_BOARD_IS_REMOTE /*! \todo */
  RAPP_SetThisNodeAddr(APP_RNET_ADDR_REMOTE); /* remote controller/LCD system */
#else
  if (RAPP_SetThisNodeAddr(RNWK_ADDR_BROADCAST)!=ERR_OK) { /* set a default address */
    //APP_DebugPrint((unsigned char*)"ERR: Failed setting node address\r\n");
  }
#endif
}

static void RadioTask(void *pvParameters) {
  (void)pvParameters; /* not used */
  Init(); /* initialize address */
  appState = RNETA_NONE; /* set state machine state */
  configASSERT(portTICK_PERIOD_MS<=2); /* otherwise  vTaskDelay() below will not delay and starve lower prio tasks */
  for(;;) {
    Process(); /* process state machine and radio in/out queues */
    vTaskDelay(2/portTICK_PERIOD_MS); /* \todo This will only work properly if having a <= 2ms tick period */
  }
}

void RNETA_Deinit(void) {
  RNET1_Deinit();
}

void RNETA_Init(void) {
  RNET1_Init(); /* initialize stack */
  if (RAPP_SetMessageHandlerTable(handlerTable)!=ERR_OK) { /* assign application message handler */
    //APP_DebugPrint((unsigned char*)"ERR: failed setting message handler!\r\n");
    for(;;) {} /* error */
  }
  if (xTaskCreate(
        RadioTask,  /* pointer to the task */
        "Radio", /* task name for kernel awareness debugging */
        600/sizeof(StackType_t), /* task stack size */
        (void*)NULL, /* optional task startup argument */
        tskIDLE_PRIORITY+3,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS) {
    /*lint -e527 */
    for(;;){}; /* error! probably out of memory */
    /*lint +e527 */
  }
}

#if PL_CONFIG_HAS_SHELL
static uint8_t PrintStatus(const CLS1_StdIOType *io) {
  uint8_t buf[32];
  
  CLS1_SendStatusStr((unsigned char*)"app", (unsigned char*)"\r\n", io->stdOut);
  
  UTIL1_strcpy(buf, sizeof(buf), (unsigned char*)"0x");
#if RNWK_SHORT_ADDR_SIZE==1
  UTIL1_strcatNum8Hex(buf, sizeof(buf), APP_dstAddr);
#else
  UTIL1_strcatNum16Hex(buf, sizeof(buf), APP_dstAddr);
#endif
  UTIL1_strcat(buf, sizeof(buf), (unsigned char*)"\r\n");
  CLS1_SendStatusStr((unsigned char*)"  dest addr", buf, io->stdOut);
  
  return ERR_OK;
}

static void PrintHelp(const CLS1_StdIOType *io) {
  CLS1_SendHelpStr((unsigned char*)"app", (unsigned char*)"Group of application commands\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  help", (unsigned char*)"Shows radio help or status\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  saddr 0x<addr>", (unsigned char*)"Set source node address\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  daddr 0x<addr>", (unsigned char*)"Set destination node address\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  send val <val>", (unsigned char*)"Set a value to the destination node\r\n", io->stdOut);
#if RNET_CONFIG_REMOTE_STDIO
  CLS1_SendHelpStr((unsigned char*)"  send (in/out/err)", (unsigned char*)"Send a string to stdio using the wireless transceiver\r\n", io->stdOut);
#endif
  CLS1_SendHelpStr((unsigned char*)"  reset labtime", (unsigned char*)"reset lab time\r\n", io->stdOut);
}

uint8_t RNETA_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io) {
  uint8_t res = ERR_OK;
  const uint8_t *p;
  uint16_t val16;
  uint8_t val8;

  if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"app help")==0) {
    PrintHelp(io);
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"app status")==0) {
    *handled = TRUE;
    return PrintStatus(io);
  } else if (UTIL1_strncmp((char*)cmd, (char*)"app saddr", sizeof("app saddr")-1)==0) {
    p = cmd + sizeof("app saddr")-1;
    *handled = TRUE;
    if (UTIL1_ScanHex16uNumber(&p, &val16)==ERR_OK) {
      (void)RNWK_SetThisNodeAddr((RNWK_ShortAddrType)val16);
    } else {
      CLS1_SendStr((unsigned char*)"ERR: wrong address\r\n", io->stdErr);
      return ERR_FAILED;
    }
  } else if (UTIL1_strncmp((char*)cmd, (char*)"app send val", sizeof("app send val")-1)==0) {
    p = cmd + sizeof("app send val")-1;
    *handled = TRUE;
    if (UTIL1_ScanDecimal8uNumber(&p, &val8)==ERR_OK) {
      (void)RAPP_SendPayloadDataBlock(&val8, sizeof(val8), RAPP_MSG_TYPE_DATA, APP_dstAddr, RPHY_PACKET_FLAGS_NONE); /* only send low byte */
    } else {
      CLS1_SendStr((unsigned char*)"ERR: wrong number format\r\n", io->stdErr);
      return ERR_FAILED;
    }
  } else if (UTIL1_strncmp((char*)cmd, (char*)"app daddr", sizeof("app daddr")-1)==0) {
    p = cmd + sizeof("app daddr")-1;
    *handled = TRUE;
    if (UTIL1_ScanHex16uNumber(&p, &val16)==ERR_OK) {
      APP_dstAddr = val16;
    } else {
      CLS1_SendStr((unsigned char*)"ERR: wrong address\r\n", io->stdErr);
      return ERR_FAILED;
    }
#if RNET_CONFIG_REMOTE_STDIO
  } else if (UTIL1_strncmp((char*)cmd, (char*)"app send", sizeof("app send")-1)==0) {
    unsigned char buf[32];
    RSTDIO_QueueType queue;
    
    if (UTIL1_strncmp((char*)cmd, (char*)"app send in", sizeof("app send in")-1)==0) {
      queue = RSTDIO_QUEUE_TX_IN;
      cmd += sizeof("app send in");
    } else if (UTIL1_strncmp((char*)cmd, (char*)"app send out", sizeof("app send out")-1)==0) {
      queue = RSTDIO_QUEUE_TX_OUT;      
      cmd += sizeof("app send out");
    } else if (UTIL1_strncmp((char*)cmd, (char*)"app send err", sizeof("app send err")-1)==0) {
      queue = RSTDIO_QUEUE_TX_ERR;      
      cmd += sizeof("app send err");
    } else {
      return ERR_OK; /* not handled */
    }
    UTIL1_strcpy(buf, sizeof(buf), cmd);
    UTIL1_chcat(buf, sizeof(buf), '\n');
    buf[sizeof(buf)-2] = '\n'; /* have a '\n' in any case */
    if (RSTDIO_SendToTxStdio(queue, buf, UTIL1_strlen((char*)buf))!=ERR_OK) {
      CLS1_SendStr((unsigned char*)"failed!\r\n", io->stdErr);
    }
    *handled = TRUE;
#endif
  } else if (UTIL1_strcmp((char*)cmd, (char*)"reset labtime")==0) {
    timeA=0, timeB=0, timeC=0;
    *handled = TRUE;
  }
  return res;
}
#endif /* PL_CONFIG_HAS_SHELL */

#endif /* PL_CONFIG_HAS_RADIO */
