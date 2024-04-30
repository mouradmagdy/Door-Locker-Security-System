# Door Locker Security System
## Overview
This project implements a door locking system with password authentication using two ATmega32 Microcontrollers. The system is designed based on a layered architecture model, comprising two main components:
- HMI_ECU (Human Machine Interface): Responsible for user interaction via a 2x16 LCD and a 4x4 keypad.
- Control_ECU: Handles all system processing, including password checking, door locking/unlocking, and activating the system alarm.

## System Operation
### Step 1: Create a System Password
The system prompts the user to enter a password consisting of 5 numbers via the HMI_ECU.
The entered password is confirmed by re-entering it.
If the two passwords match, they are stored in the EEPROM, and the system proceeds to the next step. Otherwise, the process is repeated.
### Step 2: Main Options
The LCD always displays the main system options.
### Step 3: Open Door
- The user enters the password to unlock the door.
- If the entered password matches the stored one, the door is unlocked, rotated for 15 seconds clockwise, then held for 3 seconds before locking again by rotating anticlockwise for 15 seconds.
### Step 4: Change Password
The user can change the system password by entering the current password and then setting a new one.
### Step 5: Error Handling
- If the entered password is incorrect in the previous steps, the user is prompted to re-enter the password.
- After three consecutive failed attempts, the system activates a buzzer for 1 minute, displays an error message on the LCD, and locks the system for inputs.
  
### Drivers Used in the Project:
#### GPIO Driver
Handles General Purpose Input/Output (GPIO) pins of the microcontroller.
#### LCD Driver
Controls the Liquid Crystal Display (LCD) for displaying messages.
#### Keypad Driver
Manages input from the keypad.
#### DC_Motor Driver
Controls the DC motor for locking and unlocking the door.
#### EEPROM Driver
Manages communication with the external Electrically Erasable Programmable Read-Only Memory (EEPROM).
#### I2C Driver
Facilitates communication via the Inter-Integrated Circuit (I2C) protocol.
#### UART Driver
Manages communication via the Universal Asynchronous Receiver-Transmitter (UART) protocol.
#### Timer Driver
Manages timer functionality for precise timing operations.
### Buzzer Driver
Controls the buzzer for generating audible alerts.
