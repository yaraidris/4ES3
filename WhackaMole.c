/***********************************************************************************************************************
 * Copyright [2015-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : hal_entry.c
 * Description  : This is a very simple example application that blinks all LEDs on a board.
 ***********************************************************************************************************************/

#include "hal_data.h"
int gameboard[9] =
{ 0 };
ioport_port_pin_t buttons[9] =
{ IOPORT_PORT_02_PIN_00,
  IOPORT_PORT_02_PIN_01,
  IOPORT_PORT_02_PIN_03,
  IOPORT_PORT_03_PIN_03,
  IOPORT_PORT_04_PIN_00,
  IOPORT_PORT_03_PIN_08,
  IOPORT_PORT_03_PIN_09,
  IOPORT_PORT_03_PIN_10,
  IOPORT_PORT_03_PIN_11 };
ioport_port_pin_t LEDsP1[9] =
{ IOPORT_PORT_06_PIN_00,
  IOPORT_PORT_06_PIN_01,
  IOPORT_PORT_06_PIN_02,
  IOPORT_PORT_06_PIN_08,
  IOPORT_PORT_06_PIN_09,
  IOPORT_PORT_06_PIN_10,
  IOPORT_PORT_05_PIN_06,
  IOPORT_PORT_05_PIN_07,
  IOPORT_PORT_05_PIN_11 };

static int score = 0;
static int lives = 3;
static int mole_index = -1;
static int mole_duration_ms = 2000;
void LEDsOff(void);
void FlashLEDs(int times, uint32_t delay_ms);
void LEDsScore(void);

void LEDsOff(void)
{
    for (int i = 0; i < 9; i++)
    {
        g_ioport.p_api->pinWrite (LEDsP1[i], IOPORT_LEVEL_LOW);
    }
}

void FlashLEDs(int times, uint32_t delay_ms)
{
    for (int i = 0; i < times; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            g_ioport.p_api->pinWrite (LEDsP1[j], IOPORT_LEVEL_HIGH);
        }
        R_BSP_SoftwareDelay (delay_ms, BSP_DELAY_UNITS_MILLISECONDS);
        LEDsOff ();
        R_BSP_SoftwareDelay (delay_ms, BSP_DELAY_UNITS_MILLISECONDS);
    }
}

void LEDsScore(void)
{
// Turn on one LED per point scored (max 9)
    for (int i = 0; i < score && i < 9; i++)
    {
        g_ioport.p_api->pinWrite (LEDsP1[i], IOPORT_LEVEL_HIGH);
    }
    R_BSP_SoftwareDelay (2000, BSP_DELAY_UNITS_MILLISECONDS); // Show for 2 seconds
    LEDsOff();
}

void gameReset(void)
{
    score = 0;
    lives = 3;
    mole_duration_ms = 2000;
    LEDsOff();
}

void hal_entry(void)
{
    srand (1);
    gameReset ();

    while (1)
    {
        if (lives == 0)
        {
            FlashLEDs (3, 300);
            LEDsScore ();
            R_BSP_SoftwareDelay (1000, BSP_DELAY_UNITS_MILLISECONDS);
            gameReset ();
            continue;
        }

        mole_index = rand () % 9;
        g_ioport.p_api->pinWrite (LEDsP1[mole_index], IOPORT_LEVEL_HIGH);

        bool hit = false;
        int elapsed = 0;
        const int check_interval = 50;

        while (elapsed < mole_duration_ms)
        {
            ioport_level_t level;
            g_ioport.p_api->pinRead (buttons[mole_index], &level);
            if (level == IOPORT_LEVEL_LOW)
            {
                hit = true;
                break;
            }
            R_BSP_SoftwareDelay (check_interval, BSP_DELAY_UNITS_MILLISECONDS);
            elapsed += check_interval;
        }

        g_ioport.p_api->pinWrite (LEDsP1[mole_index], IOPORT_LEVEL_LOW);

        if (hit)
        {
            score++;
            if (mole_duration_ms > 600)
                mole_duration_ms -= 100;
        }
        else
        {
            lives--;
        }

        R_BSP_SoftwareDelay (300, BSP_DELAY_UNITS_MILLISECONDS);
    }
}
