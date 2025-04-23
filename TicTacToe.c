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

/*******************************************************************************************************************//**
 * @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the software delay function provided by the BSP.
 * Only references two other modules including the BSP, IOPORT.
 *
 **********************************************************************************************************************/
int gameboard[9] =
{ 0 }; // 0=empty, 1=Player 1, 2=Player 2
int player = 1; // 1 or 2
int gameWin = 0; // 1= Player 1, 2= Player 2, 3 = Tied
uint32_t delay = 500;
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
ioport_port_pin_t LEDsP2[9] =
{ IOPORT_PORT_06_PIN_03,
  IOPORT_PORT_06_PIN_04,
  IOPORT_PORT_06_PIN_05,
  IOPORT_PORT_06_PIN_11,
  IOPORT_PORT_06_PIN_12,
  IOPORT_PORT_06_PIN_13,
  IOPORT_PORT_06_PIN_14,
  IOPORT_PORT_05_PIN_13,
  IOPORT_PORT_05_PIN_12 };
void FlashLEDs(int win, uint32_t delay_ms);
int checkWinner(int board[9]);
void gameReset();

int checkWinner(int board[9])
{
    for (int i = 0; i <= 6; i += 3)
        // Horizontal win
        if (board[i] != 0 && board[i] == board[i + 1] && board[i + 1] == board[i + 2])
            return board[i];
    for (int i = 0; i < 3; i++)
        // Vertical win
        if (board[i] != 0 && board[i] == board[i + 3] && board[i + 3] == board[i + 6])
            return board[i];
    // Diagonal wins
    if (board[0] != 0 && board[0] == board[4] && board[4] == board[8])
        return board[0];
    if (board[2] != 0 && board[2] == board[4] && board[4] == board[6])
        return board[2];
    // Check whether there's an empty space
    for (int i = 0; i < 9; i++)
        if (board[i] == 0)
            return 0;
    // Tie
    return 3;
}

void FlashLEDs(int win, uint32_t delay_ms)
{
    for (int i = 0; i < 9; i++)
    {
        g_ioport.p_api->pinWrite (LEDsP1[i], IOPORT_LEVEL_LOW);
        g_ioport.p_api->pinWrite (LEDsP2[i], IOPORT_LEVEL_LOW);
    }
    if (win == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                g_ioport.p_api->pinWrite (LEDsP1[j], IOPORT_LEVEL_HIGH);
            }
            R_BSP_SoftwareDelay (delay_ms, BSP_DELAY_UNITS_MILLISECONDS);
            for (int k = 0; k < 9; k++)
            {
                g_ioport.p_api->pinWrite (LEDsP1[k], IOPORT_LEVEL_LOW);
            }
            R_BSP_SoftwareDelay (delay_ms, BSP_DELAY_UNITS_MILLISECONDS);
        }
    }
    else if (win == 2)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                g_ioport.p_api->pinWrite (LEDsP2[j], IOPORT_LEVEL_HIGH);
            }
            R_BSP_SoftwareDelay (delay_ms, BSP_DELAY_UNITS_MILLISECONDS);
            for (int k = 0; k < 9; k++)
            {
                g_ioport.p_api->pinWrite (LEDsP2[k], IOPORT_LEVEL_LOW);
            }
            R_BSP_SoftwareDelay (delay_ms, BSP_DELAY_UNITS_MILLISECONDS);
        }
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                g_ioport.p_api->pinWrite (LEDsP1[j], IOPORT_LEVEL_HIGH);
                g_ioport.p_api->pinWrite (LEDsP2[j], IOPORT_LEVEL_HIGH);
            }
            R_BSP_SoftwareDelay (delay_ms, BSP_DELAY_UNITS_MILLISECONDS);
            for (int k = 0; k < 9; k++)
            {
                g_ioport.p_api->pinWrite (LEDsP1[k], IOPORT_LEVEL_LOW);
                g_ioport.p_api->pinWrite (LEDsP2[k], IOPORT_LEVEL_LOW);
            }
            R_BSP_SoftwareDelay (delay_ms, BSP_DELAY_UNITS_MILLISECONDS);
        }
    }
}
void gameReset()
{
    player = 1;
    gameWin = 0;
    //turn off all the LEDs and reset the gameboard
    for (int i = 0; i < 9; i++)
    {
        g_ioport.p_api->pinWrite (LEDsP1[i], IOPORT_LEVEL_LOW);
        g_ioport.p_api->pinWrite (LEDsP2[i], IOPORT_LEVEL_LOW);
        gameboard[i] = 0;
    }
}
void hal_entry(void)
{
    gameReset ();
    while (1)
    {

        for (int i = 0; i < 9; i++)
        {
            ioport_level_t button_state;
            g_ioport.p_api->pinRead (buttons[i], &button_state);

            // If button pressed and cell not claimed
            if (button_state == IOPORT_LEVEL_LOW && gameboard[i] == 0)
            {
                // Claim cell for current player
                gameboard[i] = player;
                // Light appropriate LED
                if (player == 1)
                {
                    g_ioport.p_api->pinWrite (LEDsP1[i], IOPORT_LEVEL_HIGH);
                }
                else
                {
                    g_ioport.p_api->pinWrite (LEDsP2[i], IOPORT_LEVEL_HIGH);
                }
                gameWin = checkWinner (gameboard);
                if (gameWin == 1 || gameWin == 2 || gameWin == 3)
                {
                    // Flash LEDs 3x
                    FlashLEDs (gameWin, delay);
                    R_BSP_SoftwareDelay (2000, BSP_DELAY_UNITS_MILLISECONDS);
                    gameReset ();
                }
                // Switch player
                player = (player == 1) ? 2 : 1;
                R_BSP_SoftwareDelay (10, BSP_DELAY_UNITS_MILLISECONDS);
            }
        }

    }
}

