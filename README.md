# Segment Hunter

![IMG_4175_Original](https://github.com/user-attachments/assets/5d4d4aaa-40df-489f-b11a-4c5f16f7c2b9)

**Segment Hunter** is a joystick-controlled Arduino game built as a standalone interactive project.
It was developed as a final assignment for a **Mechatronics** course to demonstrate the integration of input devices, game logic, and display modules.

## Overview

In this game, the player moves a character across a **16x2 LCD** and tries to reach a randomly placed target before time runs out.
A **7-segment LED** shows the countdown in real time, creating a simple but fast-paced arcade-style experience.

## Gameplay

### Objective
Move your character (`o`) to the target (`X`) before the timer reaches zero.

### Controls
Use the **2-axis joystick** to move freely across the **16x2 LCD** screen.

### Time limit
Each round lasts **9 seconds**. The remaining time is displayed on the **1-digit 7-segment LED**.

### Result
- If you reach the target in time, the game displays **`OK!`**.
- The game then resets and places a new random target.

## Hardware specifications

| Component | Function |
|---|---|
| Arduino Uno | Main controller for the game |
| 16x2 LCD | Displays the game field, player, and target |
| 1-digit 7-segment LED | Displays the countdown timer |
| Analog joystick | Provides X-axis and Y-axis player input |

## Repository purpose

This repository contains the original source code for **Segment Hunter**.

## Suggested future additions

This README can be expanded later with:

- Circuit diagram
- Pin assignments
- Build and upload instructions
- Required libraries
- Demo video or screenshots
- License information
