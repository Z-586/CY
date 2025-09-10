/**************************************************************************//**
 * @file     system_CMS32F030.c
 * @brief    CMSIS Cortex-M0 Device Peripheral Access Layer Source File
 *           for the Kiano CMS32F030 Series
 * @version  V2.10
 * @date     20. July 2011
 *
 * @note
 * Copyright (C) 2010-2011 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#include <stdint.h>
#include "CMS32F030.h"
#include "system_CMS32F030.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
/* ToDo: initialize SystemCoreClock with the system core clock frequency value
         achieved after system intitialization.
         This means system core clock frequency after call to SystemInit()    */
uint32_t SystemCoreClock = HSI_22M_VAL;  /*!< System Clock Frequency (Core Clock)*/
uint32_t SystemAPBClock  = HSI_22M_VAL;  /*!< System APB Clock Frequency (APB Clock)*/
uint32_t CyclesPerUs = (HSI_22M_VAL / 1000000); /* Cycles per micro second */


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
/* ToDo: add code to calculate the system frequency based upon the current
         register settings.
         This function can be used to retrieve the system core clock frequeny
         after user changed register sittings.                                */
    uint32_t pllmul = 0;
  switch (SYSCON->CLKSEL)
  {
  case 0:
      if(SYSCON->CLKCON & 0x01) //22.1184M
      {
          SystemCoreClock = HSI_22M_VAL;
      }else     //8M
      {
          SystemCoreClock = HSI_8M_VAL;
      }
    break;

  case 1:
      if((SYSCON->CLKCON >> 5) & 0x01) //HS
      {
          SystemCoreClock = HSE_H_VAL;
      }else //32.768K
      {
          SystemCoreClock = HSE_L_VAL;
      }
    break;
  case 2:   //10K
      SystemCoreClock = RC_10K_VAL;
    break;
  case 3:
      if((SYSCON->PLLCON >> 1) & 0x01)  //XTOSC
      {
          if((SYSCON->CLKCON >> 5) & 0x01)  //HSE_H
          {
              SystemCoreClock = HSE_H_VAL;
          }else     //HSL
          {
              SystemCoreClock = HSE_L_VAL;
          }
      }else //IHRC
      {
          if(SYSCON->CLKCON & 0x01) //22.1184M
          {
              SystemCoreClock = HSI_22M_VAL;
          }else     //8M
          {
              SystemCoreClock = HSI_8M_VAL;
          }
      }

      //PLL config
      switch((SYSCON->PLLCON >> 2) & 0x03)
      {
          case 00 :
          break;

          case 0x01 :
              SystemCoreClock /= 2;
          break;

          case 0x02 :
              SystemCoreClock /= 4;
          break;

          case 0x03 :
              SystemCoreClock /= 8;
          break;
      }


      pllmul = ((SYSCON->PLLCON >> 14) & 0x3F) + 2;

      if((SYSCON->PLLCON >> 5) & 0x01)  //SEL_N = 1
      {
          pllmul /= ( ( (SYSCON->PLLCON >> 8) & 0x3F) +2);
      }

      switch((SYSCON->PLLCON >> 6) & 0x03)
      {
          case 0x00 :
              break;

          case 0x01 :
              pllmul /= 2;
              break;

          case 0x02 :
              pllmul /= 4;
              break;

          case 0x03 :
              pllmul /= 8;
              break;
      }
      SystemCoreClock *= pllmul;
    break;
  }
  if(SYSCON->AHBCKDIV != 0)
  SystemCoreClock   /= (2 * SYSCON->AHBCKDIV);
  if(SYSCON->APBCKDIV != 0)
  SystemAPBClock    /= (2 * SYSCON->APBCKDIV);
  else
  SystemAPBClock = SystemCoreClock;
  CyclesPerUs = SystemCoreClock / 1000000;
}


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{
/* ToDo: add code to initialize the system
         do not use global variables because this function is called before
         reaching pre-main. RW section maybe overwritten afterwards.          */
  SystemCoreClockUpdate ();
}
