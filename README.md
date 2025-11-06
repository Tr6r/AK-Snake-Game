## Snake Gam built on AK-base-kit

![alt text](assets/game.jpg){ width=300px }

<div align="center">
  <img src="assets/gamedemo.mp4" alt="Snake Game Demo" />
</div>

This game was built on top of AK-mOS (a lightweight RTOS) together with a small OLED GUI system. The main purpose was to experiment with an event-driven task model — using tasks, message/events, and non-blocking flows — while still keeping the whole project simple and small enough to run on a low-power MCU.

## Introduction
This is a minimalist Snake Game project running on STM32L151 with a 128×64 OLED display, 1 piezo buzzer and 3 physical push buttons for input control.
<div align="center">
  <img src="assets/image.png" alt="Ak base kit" />
</div>
The game is designed to be fully standalone on the MCU (no PC connection needed). It supports multiple maps, multiple difficulty levels, persistent user config, stored highscores and in-game animations.

- This repository demonstrates:
- low-cost embedded game logic on an ultra-low-power MCU
- simple scene/state management (Menu → Map Select → Gameplay → Game Over)
- saving persistent user configuration + highscores via internal Flash
- hardware UI feedback via buzzer events
- auto screen saver: enters idle animation after 5 seconds of no input (prevents OLED burn-in and looks cool)

## Features

- Game rendered on LCD Oled 1.3"(128x64)
- Controlled by 3 physical buttons (Left, Right, Mode)
- Simple sound effects using a passive buzzer (click, score, game over, ...)
- Game config + high score are stored in STM32L15 EEPROOM
- Data integrity guaranteed with uint16 checksum
- Smooth animations for all movements
- Snake, food and walls are drawn with bitmap icons (more lively than plain pixels)
- User-adjustable mode, difficulty and buzzer on/off
- Built-in screen saver: if no user action for 5 seconds, system switches into idle screen
