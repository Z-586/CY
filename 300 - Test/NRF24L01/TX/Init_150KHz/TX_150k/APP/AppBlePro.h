/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup app_gpiote GPIOTE Handler
 * @{
 * @ingroup app_common
 *
 * @brief GPIOTE handler module.
 *
 * @details The GPIOTE handler allows several modules ("users") to share the GPIOTE interrupt,
 *          each user defining a set of pins able to generate events to the user.
 *          When a GPIOTE interrupt occurs, the GPIOTE interrupt handler will call the event handler
 *          of each user for which at least one of the pins generated an event.
 *
 *          The GPIOTE users are responsible for configuring all their corresponding pins, except
 *          the SENSE field, which should be initialized to GPIO_PIN_CNF_SENSE_Disabled.
 *          The SENSE field will be updated by the GPIOTE module when it is enabled or disabled,
 *          and also while it is enabled.
 *
 *          The module specifies on which pins events should be generated if the pin(s) goes
 *          from low->high or high->low or both directions.
 *
 * @note    Even if the application is using the @ref app_scheduler, the GPIOTE event handlers will
 *          be called directly from the GPIOTE interrupt handler.
 *
 * @warning If multiple users registers for the same pins the behavior for those pins are undefined.
 */

#ifndef APP_BLE_PRO_H__
#define APP_BLE_PRO_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef signed   char   int8;
typedef unsigned char   uint8;
typedef signed   short  int16;
typedef unsigned short  uint16;
typedef signed   long   int32;
typedef unsigned long   uint32;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

struct TX_parameter {
	uint32 Tx_U_PFC;                //  
	uint32 TX_hall;
	uint32 TX_Power;
	int32  TX_T1;	
	uint8  led_stat;
	uint8  efficiency;
	uint8  system_stat1;
	uint8  system_stat2;
	uint8  power_on;
//	uint8  charging_led;
	uint8  hall_err_cnt;
	float  fan_duty;
	uint32 fan_speed;
	uint32 calc_speed;
//	uint8  Ble_ID[6];
};


struct RX_parameter {
	uint32 	RX_I;
	uint32 	RX_Uout;
	uint32 	RX_Uin;
	int32 	RX_T1;
	uint32  RX_MCU_STATUS;
	uint32  RX_Power;
	uint8 	efficiency;
	uint8 	Receive_data[128];	
	uint8   receive_size;
};


#endif // APP_BLE_PRO_H__

/** @} */
