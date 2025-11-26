## Snake Gam built on AK-base-kit


<div align="center">
  <img src="assets/game.jpg" width="400" />
</div>

https://github.com/user-attachments/assets/8ab2e111-32e4-478b-8cb4-f2843a4841f3



This game was built on top of AK-OS (a lightweight RTOS) together with a small OLED system. The main purpose was to experiment with an event-driven task model — using tasks, message/events, and non-blocking flows — while still keeping the whole project simple and small enough to run on a low-power MCU.

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
- Simple sound effects using a passive buzzer (click, score, game over, ...)
- Game config + high score are stored in STM32L15 EEPROOM
- Data integrity guaranteed with uint16 checksum
- Smooth animations for all movements
- Snake, food and walls are drawn with bitmap icons (more lively than plain pixels)
- User-adjustable mode, difficulty and buzzer on/off
- Built-in screen saver: if no user action for 5 seconds, system switches into idle screen

## III.Flow Charts
The project logic is organized into two main flows: **Settings** and **Gameplay**.  
Below diagrams illustrate the transitions between screens, tasks, and user interactions:

<div align="center" style="display: flex; justify-content: center; gap: 20px;">
  <img src="assets/setting_flowchart.png" alt="Settings Flowchart" width="400"/>
  <img src="assets/gameplay_flowchart.png" alt="Gameplay Flowchart" width="400"/>
</div>

## IV.Class
![alt text](assets/class_diagram.png)

## V.Task
Each task runs independently and has its own private stack. Tasks that handle messages require a message queue
- Event Handling: Tasks respond to events by executing actions in response to messages.
- Synchronization: Tasks ensure events are processed in order, avoiding conflicts between concurrent actions.
- Control Flow Management: Task priorities determine execution order for multiple events.
- Logic Separation: Tasks allow modular design, keeping event handling logic clear and readable.
- Task Hierarchy: Task levels define message-processing priority in the system queue.

| Task ID             | Priority Level       | Task Function       |
|--------------------|-------------------|------------------|
| AC_TASK_SYSTEM_ID   | TASK_PRI_LEVEL_2   | task_system       |
| AC_TASK_DBG_ID      | TASK_PRI_LEVEL_4   | task_dbg          |
| AC_TASK_DISPLAY_ID  | TASK_PRI_LEVEL_4   | task_display      |
| AC_TASK_LIFE_ID     | TASK_PRI_LEVEL_6   | task_life         |
| AC_TASK_IDLE_ID     | TASK_PRI_LEVEL_6   | task_idle         |

## Task Life
<img width="600" alt="task_life" src="https://github.com/user-attachments/assets/ea980801-739b-4de4-b9da-cf41b6d42b80" />

## Task Idle
<img width="600" alt="task_idle" src="https://github.com/user-attachments/assets/bdfbb0c6-74f9-4aec-af3c-9e9185683bf0" />

## Task Display
Task screen manage all screens in this application including:
- Start up screen
- Menu screen
- Map screen
- Game play screen
- Setting screen
- Idle screen

## VI.EEPROM Storage
Game settings and high scores are persistently stored in the STM32L151 flash(0X0800 0000)
## Stored data:
<img width="300" alt="gameconfig" src="https://github.com/user-attachments/assets/2b198618-d949-4e06-827d-f2671cc8fe7b" />

## VII. OTA Firmware Update using UART Shell + External Flash
This document describes the updated STM32 firmware update (FWU) architecture, where the update is now performed in the Application layer. Using the sys_irq_shell interface to receive data, the task_fw module to write firmware chunks into external flash.

# 1. Architecture
| Region             | Address Range              | Description                          |
|--------------------|----------------------------|--------------------------------------|
| Bootloader         | 0x08000000 – 0x08001FFF    | 8 KB region, protected, runs on boot |
| BSF                | 0x08002000 – 0x08002FFF    | 4 KB, stores boot flags & headers    |
| Application start  | 0x08003000                 | Main firmware                        |
| Ext_Flash start    | 0x80000                    | Storage new firmware                 |

# 2. Firmware Update Flow (MCU)
### 2.1. Receive CMD and Diasble Shell Stage
![transfer-firmware](assets/images/mcu_fwu_1.png)

### 2.2. Meta Stage
![transfer-firmware](assets/images/mcu_fwu_2.png)

### 2.3. Transfer Firmware Stage
![transfer-firmware](assets/images/mcu_fwu_3.png)

### 2.4. Final Checksum Stage
![transfer-firmware](assets/images/mcu_fwu_4.png)

# 3. Bootloader Flow (MCU)

### 3.1. Boot Entry & Boot Flag Check
![boot-entry](assets/images/boot_mcu_1.png)

### 3.2. External → Internal Flash Update Flow
![boot-entry](assets/images/boot_mcu_2.png)
![boot-entry](assets/images/boot_mcu_3.png)

### 3.3. After Flashing (Checksum & Reset)
![boot-entry](assets/images/boot_mcu_4.png)

# 4. Host-side Flow (PC)
### 4.1. Initialization (UART Open) and Send FWSET Command
![boot-entry](assets/images/host_fwu_1.png)

### 4.2. Send Metadata (Header)
![boot-entry](assets/images/host_fwu_2.png)

### 4.3. Transfer Firmware (Chunk Loop)
![boot-entry](assets/images/host_fwu_3.png)

# V. Protocol Specification
| Field | Size (bytes) | Value / Range | Description |
|-------|--------------:|---------------|-------------|
| SOP   | 1             | 0xEF          | Start of packet |
| LEN   | 1             | 0..254        | Number of DATA bytes |
| DATA  | LEN           | —             | Payload (CMD + payload) |
| FCS   | 1             | XOR           | LEN ^ data[0] ^ data[1] ... (simple XOR) |
