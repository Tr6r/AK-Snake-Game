## Snake Gam built on AK-base-kit


<div align="center">
  <img src="assets/game.jpg" width="400" />
</div>

https://github.com/user-attachments/assets/8ab2e111-32e4-478b-8cb4-f2843a4841f3

- The gameplay is inspired by the classic Snake game on old Nokia phones. The logic stays very simple — a moving player, basic direction control, and item pickups — making it a good fit for testing how an event-driven system handles periodic updates and input without blocking.
  
- This game was built on top of AK-OS. The main purpose was to experiment with an event-driven pattern — using tasks, message, events, and non-blocking flows — while still keeping the whole project simple and small enough to run on a MCU (STM32l151).

## I.Introduction
This is a minimalist Snake Game project running on STM32L151 with a 128×64 OLED display, 1 piezo buzzer and 3 physical push buttons for input control.
<div align="center">
<img src="assets/image.png" width="400" />
</div>
The game is designed to be fully standalone on the MCU (no PC connection needed). It supports multiple maps, multiple difficulty levels, persistent user config, stored highscores and in-game animations.

- This repository demonstrates:
- low-cost embedded game logic on an ultra-low-power MCU
- simple scene/state management (Menu → Map Select → Gameplay → Game Over)
- saving persistent user configuration + highscores via internal Flash
- hardware UI feedback via buzzer events
- auto screen saver: enters idle animation after 5 seconds of no input (prevents OLED burn-in and looks cool)

## II.Features

- Game rendered on LCD Oled 1.3"(128x64)
- Controlled by 3 physical buttons (Left, Right, Mode)
- Simple sound effects using a passive buzzerz
# 5. Protocol Specification
| Field | Size (bytes) | Value / Range | Description |
|-------|--------------:|---------------|-------------|
| SOP   | 1             | 0xEF          | Start of packet |
| LEN   | 1             | 0..254        | Number of DATA bytes |
| DATA  | LEN           | —             | Payload (CMD + payload) |
| FCS   | 1             | XOR           | LEN ^ data[0] ^ data[1] ... (simple XOR) |
