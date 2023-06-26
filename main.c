/*===================================CPEG222====================================
 * Program:		main.c
 * Authors: 	Justin Chan
 * Date: 		10/9/2020
 * Description: This program is intended to mimic a "ping pong game". If it is the left player's turn the program cycles through the LEDs
 * from bits 7 to 0 and waits for the BTNR to be pressed or it increments the SSD for the left player.
 * The SSD is also incremented by one if the BTNR is pressed too early. If it is the right player's turn
 * the program cycles through the LEDs form bits 0 to 7 and waits for the BTNL to be pressed or it increments
 * the SSD for the right player. The SSD is also incremented by one if the BTNL is pressed to early.
 * players switch off every two turns (starting with the left). and when one player's score reaches 10 the turns switch off every other turn.
 * a player wins by having 11 points and winning by 2 points.
 *
 * Input: Button press
 * Output: LED is cycled through, the LCD displays "welcome to a new game" in the initial state, "game in play" and who's turn it is in the ReadyToServe state
 * who scored in the score state as is the next turn, finally the LED says game over and who won.
==============================================================================*/

/*------------------ Board system settings. PLEASE DO NOT MODIFY THIS PART ----------*/
#ifndef _SUPPRESS_PLIB_WARNING // suppress the plib warning during compiling
#define _SUPPRESS_PLIB_WARNING
#endif
#pragma config FPLLIDIV = DIV_2 // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20 // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1 // System PLL Output Clock Divider (PLL Divide by 1)
#pragma config FNOSC = PRIPLL   // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = OFF    // Secondary Oscillator Enable (Disabled)
#pragma config POSCMOD = XT     // Primary Oscillator Configuration (XT osc mode)
#pragma config FPBDIV = DIV_8   // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
/*----------------------------------------------------------------------------*/

#include <xc.h>     //Microchip XC processor header which links to the PIC32MX370512L header
#include "config.h" // Basys MX3 configuration header
#include "lcd.h"
#include <stdio.h>
#include "ssd.h"
#include "btn.h"
#include "led.h"

/* -------------------------- Definitions------------------------------------ */
#define SYS_FREQ (80000000L)
int LD_position;
int buttonLockRight = 0;
int buttonLockLeft = 0;
void delay_ms(int ms);
int checkWhoServe(unsigned int score_left, unsigned int score_right);
int GameOverCheck(unsigned int score_left, unsigned int score_right);
unsigned int countLeft = 0;
unsigned int countRight = 0;
int counts = 0;
int highCount = 20000;

/* ----------------------------- Main --------------------------------------- */

int main(void)
{
    /*-------------------- Port and State Initialization -------------------------*/

    LCD_Init();
    SSD_Init();
    BTN_Init();
    LED_Init();

    DDPCONbits.JTAGEN = 0;

    enum states
    {
        initial,
        ReadyToServe,
        FlyingLeft,
        FlyingRight,
        HitOrDieLeft,
        HitOrDieRight,
        score,
        gameOver
    } state;
    state = initial;

    while (1)
    {

        /*-------------------- Main logic and actions start --------------------------*/

        switch (state)
        {
        case initial:
            countLeft = 0;
            countRight = 0;
            highCount = 10;
            LCD_WriteStringAtPos("Welcome to  ", 0, 2);
            LCD_WriteStringAtPos("a new game     ", 1, 2);
            LED_SetGroupValue(0);
            SSD_WriteDigits(countRight % 10, countRight / 10, countLeft % 10, countLeft / 10, 0, 0, 0, 0);
            break;

        case ReadyToServe:
            delay_ms(25);
            if (GameOverCheck(countLeft, countRight) == 1 || GameOverCheck(countLeft, countRight) == 2)
            {
                state = gameOver;
            }

            if (checkWhoServe(countLeft, countRight) == 0x80)
            {
                LCD_WriteStringAtPos("Game in Play   ", 0, 2);
                LCD_WriteStringAtPos("Serve = Left   ", 1, 2);
                LD_position = 0x80;
            }
            else if (checkWhoServe(countLeft, countRight) == 0x01)
            {
                LCD_WriteStringAtPos("Game in Play   ", 0, 2);
                LCD_WriteStringAtPos("Serve = Right   ", 1, 2);
                LD_position = 0x01;
            }

            LED_SetGroupValue(checkWhoServe(countRight, countLeft));
            break;

        case FlyingLeft:
            counts++;
            if (counts == 20000)
            {
                counts = 0;
                LD_position <<= 1;
                LED_SetGroupValue(LD_position);

                if (LD_position == 0x80)
                {
                    delay_ms(100);
                    state = HitOrDieLeft;
                }
            }
            break;

        case FlyingRight:
            counts++;
            if (counts == 20000)
            {
                counts = 0;
                LD_position >>= 1;
                LED_SetGroupValue(LD_position);

                if (LD_position == 0x01)
                {
                    delay_ms(100);
                    state = HitOrDieRight;
                }
            }
            break;

        case HitOrDieLeft:
            if (highCount > 0)
            {
                highCount = 20000;
                countRight++;
                LCD_WriteStringAtPos("Right Scored   ", 0, 2);
                state = score;
            }
            highCount--;
            break;

        case HitOrDieRight:
            if (highCount > 0)
            {

                highCount = 20000;
                countLeft++;
                LCD_WriteStringAtPos("Left Scored   ", 0, 2);
                state = score;
            }
            highCount--;
            break;

        case score:
            LED_SetGroupValue(0);

            if (GameOverCheck(countLeft, countRight) == 1 || GameOverCheck(countLeft, countRight) == 2)
            {
                state = gameOver;
            }

            if (checkWhoServe(countRight, countLeft) == 0x80)
            {

                LCD_WriteStringAtPos("Serve = Left  ", 1, 2);
                SSD_WriteDigits(countRight % 10, countRight / 10, countLeft % 10, countLeft / 10, 0, 0, 0, 0);
                LD_position = 0x80;
                delay_ms(1000);
                state = ReadyToServe;
            }
            else
            {

                LCD_WriteStringAtPos("Serve = Right   ", 1, 2);
                SSD_WriteDigits(countRight % 10, countRight / 10, countLeft % 10, countLeft / 10, 0, 0, 0, 0);
                LD_position = 0x01;
                delay_ms(1000);
                state = ReadyToServe;
            }
            break;

        case gameOver:
            LED_SetGroupValue(0);
            if (countLeft > countRight)
            {
                LCD_WriteStringAtPos("Game Over     ", 0, 2);
                LCD_WriteStringAtPos("Congrats Left   ", 1, 2);
            }
            if (countLeft < countRight)
            {
                LCD_WriteStringAtPos("Game Over     ", 0, 2);
                LCD_WriteStringAtPos("Congrats Right    ", 1, 2);
            }
            break;
        }
        if (BTN_GetValue('R') && !buttonLockRight)
        {
            buttonLockRight = 1;

            if (state == initial)
            {
                state = ReadyToServe;
            }
            else if (state == ReadyToServe)
            {
                if ((checkWhoServe(countRight, countLeft) == 0x01))
                {
                    state = FlyingLeft;
                }
            }

            else if (state == FlyingRight)
            {

                countLeft++;
                LCD_WriteStringAtPos("Left Scored   ", 0, 2);
                state = score;
            }

            else if (state == HitOrDieRight)
            {

                state = FlyingLeft;
            }
            else if (state == gameOver)
            {
                state = initial;
            }
        }

        else if (buttonLockRight && !BTN_GetValue('R'))
        {
            delay_ms(20);
            buttonLockRight = 0;
        }

        if (BTN_GetValue('L') && !buttonLockLeft)
        {
            buttonLockLeft = 1;

            if (state == initial)
            {
                state = ReadyToServe;
            }

            else if (state == ReadyToServe)
            {
                if ((checkWhoServe(countRight, countLeft) == 0x80))
                {
                    state = FlyingRight;
                }
            }
            else if (state == FlyingLeft)
            {
                LCD_WriteStringAtPos("Right Scored   ", 0, 2);
                countRight++;
                state = score;
            }
            else if (state == HitOrDieLeft)
            {
                state = FlyingRight;
            }
            else if (state == gameOver)
            {
                state = initial;
            }
        }
        else if (buttonLockLeft && !BTN_GetValue('L'))
        {
            delay_ms(20);
            buttonLockLeft = 0;
        }
    }
    delay_ms(25);

    /*--------------------- Action and logic end ---------------------------------*/
}
/* -----------------------------------------------------------------------------
**	delay_ms
**	Parameters:
**		ms - amount of milliseconds to delay (based on 80 MHz SSCLK)
**	Return Value:
**		none
**	Description:
**		Create a delay by counting up to counter variable
** -------------------------------------------------------------------------- */
void delay_ms(int ms)
{
    int i, counter;
    for (counter = 0; counter < ms; counter++)
    {
        for (i = 0; i < 1426; i++)
        {
        } // software delay 1 millisec
    }
}

int checkWhoServe(unsigned int score_left, unsigned int score_right)
{
    int turns = score_left + score_right;

    if (score_left >= 10 && score_right >= 10)
    {
        if (((turns) % 2) == 0)
        {

            return 0x01;
        }

        else if (((turns) % 2) == 1)
        {

            return 0x80;
        }
    }
    else
    {
        if (turns % 4 == 0 || turns % 4 == 1)
        {

            return 0x80;
        }
        else if (turns % 4 == 2 || turns % 4 == 3)
        {

            return 0x01;
        }
    }
}

int GameOverCheck(unsigned int score_left, unsigned int score_right)
{

    if ((score_left > score_right + 1) && score_left >= 11)
    {
        return 1;
    }
    else if ((score_right > score_left + 1) && score_right >= 11)
    {

        return 2;
    }
    return 0;
}