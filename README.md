![IMG_4175_Original](https://github.com/user-attachments/assets/5d4d4aaa-40df-489f-b11a-4c5f16f7c2b9)


Segment Hunter: Joystick-Controlled Arduino Game

Segment Hunter is a standalone, interactive arcade game built on the Arduino Uno platform. 
This project was developed as the final assignment for a "Mechatronics" course to demonstrate the integration of input devices, processing logic, and multi-module display outputs.
The game challenges players to navigate a character toward a target within a strict time limit, utilizing real-time feedback from an LCD and a 7-segment LED.


Gameplay

Objective: Move your character (o) to the target location (X) before the timer hits zero.


Controls: Use the 2-axis joystick to move freely across the 16x2 LCD screen.


Time Limit: You have 9 seconds per round, displayed as a live countdown on the 7-segment LED.

Scoring: Successfully reaching the target displays an "OK!" message and resets the game with a new random target.



Hardware Specifications

Component,Function
Arduino Uno,Central processing unit.
16x2 LCD,"Displays the game field, player, and target."
1-Digit 7-Segment LED,Displays the 9-second countdown.
Analog Joystick,Provides X and Y axis user input.
