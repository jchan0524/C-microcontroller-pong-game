# Pong Game

This is a C program that implements a simplified version of the "Pong" Game. The program uses various input and output devices, such as LEDs, buttons an LCD display, and a seven-segment display (SSD). The objective of the game is to mimic a ping pong game between two players

# Program Details

- **Program Name:** main.c
- **Author:** Justin Chan
- Date: 10/2020

# Description

The program simulates a ping pong game where two players take turns. If it is the left player's turn, the program cycles through the LEDs from bits 7 to 0 and waits for the "BTNR" button to be pressed. Alternatively, it increments the SSD for the left player if the button is pressed too early. The same logic applies to the right player's turn, but the LEDs cycle through bits 0 to 7, and the "BTNL" button is used
The players switch off ecery two turns, starting with the left player. When one player's score reaches 10, the turns switch off every other turn. A player wins by having 11 points and winning by a margin of 2 points.

# Input/Output

**Input:** Button press (BTNR and BTNL buttons)

**Output:**

- Cycling of LEDs to represent the active player's turn.
- LCD displays different messqages based on the game state:
  - "Welcome to a new game" in the initial state.
  - "Game in play" and whose turn it is in the ReadyToServe state.
  - "Serve = Left" or "Serve = Right" when it's time to serve.
  - "Game Over" and the winning player's name in the gameOver state.
- SSD displays the scores of both players.

# Files Needed

1. `main.c`: The main source code file containing the implementation of the Pong game logic.
2. `lcd.h`: A header file that provides the necessary functions and definitions for interfacing with the LCD display.
3. `lcd.c`: The source code file that contains the implementation of the LCD display functions.
4. `ssd.h`: A header file that provides the necessary functions and definitions for interfacing with the seven-segment display.
5. `ssd.c`: The source code file that contains the implementation of the seven-segment display functions.

Make sure to include these files in your project and compile them togethter to run the Pong game on your microcontroller board.

# Board System Settings

The program includes system settings for the board and uses the Microchip XC processor header for the PIC32MX370512L microcontroller. The configuration settings for the board are as follows:

- PLL Input Divider: DIV_2
- PLL Multiplier: MUL_20
- System PLL Output Clock Divider: DIV_1
- Oscillator Selection Bits: Primary OSC w/PLL
- Secondary Oscillator: Disabled
- Primary Oscillator Configuration: XT osc mode
- Peripheral Clock Divisor: Pb_Clk is Sys_Clk/8

# Functions

The program includes several functions to handle specific tasks:

1. `delay_ms(int ms)`: Creates a delay by counting up to a counter variable.
2. `checkWhoServe(unsigned int score_left, unsigned int score_right)`: Determines which player serves based on the scores of both players and the game state.
3. `GameOverCheck(unsigned int score_left, unsigned int score_right)`: Checks if the game is over based on the scores of both players and the winning conditions.

# Main Logic

The program's main logic is implementing using a state machine with different states representing various game stages, such as initial, ReadyToServer, FlyingLeft, FlyingRight, HitOrDieLeft, HitOrDieRight, score, and gameOver. The state machine is inside an infinite loop that continuously checks the input buttons and updates the game state accordingly. The LEDs, LCD display, and SSD are updated based on the current state.

# Usage

To use the program, ensure that the necessary hardware components are properly connected to the microcontroller board. Compile and run the program on the microcontroller, and the game will start. Follow the on-screen instructions and press the appropriate buttons to play the game.
Note: For more detailed information, refer to the comments within the main.c source code file.
