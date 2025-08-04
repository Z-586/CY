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

#ifndef MAIN_H__
#define MAIN_H__

#include "stdlib.h"
//单位 ： mm
#define X_Max 40
#define X_Min -40

#define Y_Max 40
#define Y_Min -40

#define Z_Max 50
#define Z_Min 0

#define One_Run_Lenth 90 	//一圈90cm
#define One_Run_Pulse 4000 	//一圈pulse 8000

#endif // MAIN_H__

/** @} */
