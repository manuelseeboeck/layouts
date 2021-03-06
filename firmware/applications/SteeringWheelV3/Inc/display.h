#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "event_groups.h"
#include "drv_usart.h"

#define DISPLAY_TASK_STACK_SIZE 512

#define DISPLAY_DC_1 0x11
#define DISPLAY_ESC 0x1B
#define DISPLAY_ACK 0x06

#define DISPLAY_TIMEOUT 100

typedef enum DISPLAY_MACRO
{
  DISPLAY_MACRO_Init = 0,

  DISPLAY_MACRO_MenuInv_1,
  DISPLAY_MACRO_MenuInv_2,
  DISPLAY_MACRO_MenuInv_3,
  DISPLAY_MACRO_MenuInv_4,
  DISPLAY_MACRO_MenuInv_5,
  DISPLAY_MACRO_MenuInv_6,

  DISPLAY_MACRO_PageInv_1 = 10,
  DISPLAY_MACRO_PageInv_2,
  DISPLAY_MACRO_PageInv_3,
  DISPLAY_MACRO_PageInv_4,
  DISPLAY_MACRO_PageInv_5,
  DISPLAY_MACRO_PageInv_6,
  DISPLAY_MACRO_PageInv_7,

  DISPLAY_MACRO_PageValue_1 = 18,
  DISPLAY_MACRO_PageValue_2,
  DISPLAY_MACRO_PageValue_3,
  DISPLAY_MACRO_PageValue_4,
  DISPLAY_MACRO_PageValue_5,
  DISPLAY_MACRO_PageValue_6,
  DISPLAY_MACRO_PageValue_7,

  // Main Menu
  DISPLAY_MACRO_Main = 25,

  DISPLAY_MACRO_RacePage = 30,
  DISPLAY_MACRO_Gear_1,
  DISPLAY_MACRO_Gear_2,
  DISPLAY_MACRO_Gear_3,
  DISPLAY_MACRO_Gear_4,
  DISPLAY_MACRO_Gear_5,
  DISPLAY_MACRO_Gear_6,
  DISPLAY_MACRO_Gear_N,
  DISPLAY_MACRO_Inv_TC,
  DISPLAY_MACRO_Inv_BKB,
  DISPLAY_MACRO_Inv_MP,
  DISPLAY_MACRO_Warn_Temp,
  DISPLAY_MACRO_ACC,
  DISPLAY_MACRO_Oil_Cool,
  DISPLAY_MACRO_Oil_Hot,
  DISPLAY_MACRO_Bat_Good,
  DISPLAY_MACRO_Bat_Low,
  DISPLAY_MACRO_TC_On,
  DISPLAY_MACRO_TC_Off,
  DISPLAY_MACRO_CAN_Good,
  DISPLAY_MACRO_CAN_Bad,
  DISPLAY_MACRO_ACC_On,
  DISPLAY_MACRO_ACC_Off,

  DISPLAY_MACRO_ECU = 60,

  DISPLAY_MACRO_ClutchSetup,

  DISPLAY_MACRO_ClutchCalibration = DISPLAY_MACRO_ClutchSetup + 5,
  DISPLAY_MACRO_ClutchNormal,
  DISPLAY_MACRO_ClutchNormalInv_1,
  DISPLAY_MACRO_ClutchNormalInv_2,
  DISPLAY_MACRO_ClutchNormalInv_3,
  DISPLAY_MACRO_ClutchNormalInv_4,
  DISPLAY_MACRO_ClutchNormalInv_5,
  DISPLAY_MACRO_ClutchNormalInv_6,
  DISPLAY_MACRO_ClutchNormalInv_7,
  DISPLAY_MACRO_ClutchNormalInv_8,

  DISPLAY_MACRO_ClutchNormalInv_12,
  DISPLAY_MACRO_ClutchNormalInv_13,
  DISPLAY_MACRO_ClutchNormalInv_14,
  DISPLAY_MACRO_ClutchNormalInv_15,
  DISPLAY_MACRO_ClutchNormalInv_16,
  DISPLAY_MACRO_ClutchNormalInv_17,
  DISPLAY_MACRO_ClutchNormalInv_18,

  DISPLAY_MACRO_ClutchACC = DISPLAY_MACRO_ClutchNormal + 15,

  DISPLAY_MACRO_Gear = 90,

  DISPLAY_MACRO_GearControl = DISPLAY_MACRO_Gear + 4,
  DISPLAY_MACRO_GearACC,

  DISPLAY_MACRO_Power = 100,
  DISPLAY_MACRO_PowerFanSetup = DISPLAY_MACRO_Power + 4,
  DISPLAY_MACRO_PowerLVPD,

  DISPLAY_MACRO_Diagnose = 110,

  DISPLAY_MACRO_DiagnoseTireTemp = DISPLAY_MACRO_Diagnose + 7,
  DISPLAY_MACRO_DiagnoseCarStates,
  DISPLAY_MACRO_DiagnoseTireStates,
  DISPLAY_MACRO_DiagnoseBCM,
  DISPLAY_MACRO_DiagnoseSettings,

} DISPLAY_MACRO_t;

#define DISPLAY_MENU_MAIN_NR_ENTRIES 6
#define DISPLAY_MENU_CLUTCH_NR_ENTRIES 4

typedef enum DISPLAY_NEW_DATA_EVENT
{
  DISPLAY_EVENT_UPDATE = 0x01,
  DISPLAY_EVENT_BUTTON_PRESSED =          DISPLAY_EVENT_UPDATE << 1,

} DISPLAY_NEW_DATA_EVENT_t;

#define DISPLAY_EVENT_ALL  (DISPLAY_EVENT_UPDATE |\
                            DISPLAY_EVENT_BUTTON_PRESSED |\
                            DISPLAY_EVENT_NEW_DATA_RACEPAGE |\
                            DISPLAY_EVENT_NEW_DATA_GEAR_CONTROL |\
                            DISPLAY_EVENT_NEW_DATA_GEAR_ACC |\
                            DISPLAY_EVENT_NEW_DATA_CLUTCH_NORMAL |\
                            DISPLAY_EVENT_NEW_DATA_CLUTCH_ACC\
                                    )

typedef struct DISPLAY_Clutch_Normal
{
  uint8_t clutch_points;
  uint8_t clutch_tolerance;
  uint16_t c_sens_min;
  uint16_t c_sens_max;

} DISPLAY_Clutch_Normal_t;

typedef struct DISPLAY_ClutchACC
{
  uint8_t acc_clutch_k1;
  uint8_t acc_clutch_k2;
  uint8_t acc_clutch_k3;
  uint8_t acc_clutch_p1;
  uint8_t acc_clutch_p2;

} DISPLAY_ClutchACC_t;

typedef struct DISPLAY_GearControl
{
  uint8_t g_min_shift_delay; // *10[ms]
  uint8_t g_up_holdtime; // *10[ms]
  uint8_t g_dn_holdtime; // *10[ms]
  uint8_t g_n_holdtime; // *1[ms]

} DISPLAY_GearControl_t;

typedef struct DISPLAY_GearACC
{
  uint8_t g_acc_max_wspin;
  uint8_t g_acc_min_speed;
  uint8_t g_acc_shift_rpm_1; // RPM/100
  uint8_t g_acc_shift_rpm_2; // RPM/100
  uint8_t g_acc_shift_rpm_3; // RPM/100

} DISPLAY_GearACC_t;

typedef struct DISPLAY_Racepage
{
  uint8_t traction; // traction control level: 0 - 11
  uint8_t map; // mapping: 1 - 2
  float rev;
  float ath;
  float speed;
  uint8_t gear;
  int16_t toil;
  int16_t twat;

} DISPLAY_Racepage_t;

typedef struct DISPLAY_POWER_FAN
{
  uint8_t fan_off_temp;
  uint8_t fan_on_temp;
  uint8_t fan_off_rpm;
  uint8_t fan_on_rpm;

} DISPLAY_POWER_FAN_t;

typedef struct DISPLAY_POWER_CURRENT
{
  uint16_t enable_bitfield;
  uint8_t threshold_value[16];

} DISPLAY_POWER_CURRENT_t;

typedef struct DISPLAY_BUTTON
{
  uint8_t minus;
  uint8_t plus;
  uint8_t enter;

} DISPLAY_BUTTON_t;

typedef struct DISPLAY_Config
{
  UART_Config_t *uart_config;
  TaskHandle_t task_handle;
  TimerHandle_t update_timer;

  uint8_t menu_position;
  uint8_t current_menu;

  uint8_t edit_mode;

} DISPLAY_Config_t;

extern DISPLAY_Racepage_t DISPLAY_DATA_Racepage;
extern DISPLAY_Clutch_Normal_t DISPLAY_DATA_ClutchNormal;
extern DISPLAY_GearControl_t DISPLAY_DATA_GearControl;
extern DISPLAY_GearACC_t DISPLAY_DATA_GearACC;
extern DISPLAY_ClutchACC_t DISPLAY_DATA_ClutchACC;
extern DISPLAY_POWER_FAN_t DISPLAY_DATA_PowerFan;
extern DISPLAY_POWER_CURRENT_t DISPLAY_DATA_PowerCurrent;
extern DISPLAY_BUTTON_t DISPLAY_DATA_Buttons;

extern EventGroupHandle_t DISPLAY_NewDataEventHandle;

HAL_StatusTypeDef DISPLAY_Init(DISPLAY_Config_t *config);

#endif
