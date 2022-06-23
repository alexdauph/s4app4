#include "main.h"
#include "system_config.h"
#include "system/common/sys_module.h" // SYS function prototypes
#include "driver/spi/src/dynamic/drv_spi_internal.h"
#include "UDP_app.h"
#include "led.h"
#include "ssd.h"
#include "accel.h"
#include "lcd.h"
#include "app_commands.h"
#include "rgbled.h"
#include "packet.h"

MAIN_DATA mainData;

/* Application's LED Task Function */
static unsigned long int counter = 0;
static void LedTask(void)
{
  if (counter++ == 20000)
  {
    LED_ToggleValue(1);
    counter = 0;
  }
}

static bool sw0_old;
static bool sw2_old;
static bool sw3_old;
void ManageSwitches()
{
  bool sw0_new = SWITCH0StateGet();
  bool sw2_new = SWITCH2StateGet();
  bool sw3_new = SWITCH3StateGet();

  if ((sw0_new != sw0_old) && sw0_new)
  {
    strcpy(UDP_Send_Buffer_Ptr, "Bonjour S4\n\r");
    UDP_bytes_to_send = strlen(UDP_Send_Buffer_Ptr);
    UDP_Send_Packet = true;
  }

  if (sw2_new != sw2_old)
  {
    pkt_print = sw2_new;
  }

  if (sw3_new != sw3_old)
  {
    UDP_Drop_Connection = true;
    UDP_Server_ID = sw3_new;
  }

  sw0_old = sw0_new;
  sw2_old = sw2_new;
  sw3_old = sw3_new;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void MAIN_Initialize ( void )

  Remarks:
    See prototype in main.h.
 */

void MAIN_Initialize(void)
{
  /* Place the App state machine in its initial state. */
  mainData.state = MAIN_STATE_INIT;

  mainData.handleUSART0 = DRV_HANDLE_INVALID;

  UDP_Initialize();
  LCD_Init();
  ACL_Init();
  SSD_Init();
  PKT_Init();
  RGBLED_Init();
}

/******************************************************************************
  Function:
    void MAIN_Tasks ( void )

  Remarks:
    See prototype in main.h.
 */

void MAIN_Tasks(void)
{

  /* Check the application's current state. */
  switch (mainData.state)
  {
    /* Application's initial state. */
  case MAIN_STATE_INIT:
  {
    bool appInitialized = true;
    SYS_CONSOLE_MESSAGE("Init\r\n");

    if (mainData.handleUSART0 == DRV_HANDLE_INVALID)
    {
      mainData.handleUSART0 = DRV_USART_Open(MAIN_DRV_USART, DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_NONBLOCKING);
      appInitialized &= (DRV_HANDLE_INVALID != mainData.handleUSART0);
    }

    if (appInitialized)
    {
      mainData.state = MAIN_STATE_SERVICE_TASKS;
    }
    break;
  }

  case MAIN_STATE_SERVICE_TASKS:
  {
    LedTask();
    accel_tasks();
    UDP_Tasks();
    ManageSwitches();
    PKT_Tasks();
    JB1Toggle();
    LED0Toggle();
    //
    break;
  }

    /* The default state should never be executed. */
  default:
  {
    /* TODO: Handle error in application's state machine. */
    break;
  }
  }
}

int main(void)
{

  SYS_Initialize(NULL);
  MAIN_Initialize();
  SYS_INT_Enable();
  SSD_WriteDigitsGrouped(0xFA9B, 0x1);

  while (1)
  {
    SYS_Tasks();
    MAIN_Tasks();
  };

  return 0;
}

/*******************************************************************************
 End of File
 */
