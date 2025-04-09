# REACHER Suite Operant Conditioning Projects

[![Documentation](https://img.shields.io/badge/Documentation-grey?style=for-the-badge)](https://otis-lab-musc.github.io/REACHER-Firmware/)

*Written by*: Joshua Boquiren ([*@thejoshbq*](https://github.com/thejoshbq/thejoshbq/))

## Overview

This repository contains four operant conditioning paradigms implemented as Arduino sketches: `operant_FR.ino`, `omission.ino`, `operant_PR.ino`, and `operant_VI.ino`, all of which are compatible with the [REACHER Suite](https://github.com/Otis-Lab-MUSC/REACHER-Suite/). Each sketch controls hardware components (levers, cues, pumps, lasers, etc.) via serial commands from the REACHER Suite Python framework, collects real-time data, and ensures data integrity through immediate serial logging.


---

## System Requirements

- **Hardware**: Arduino microcontroller (e.g., Arduino UNO)
- **Firmware Dependencies**: Arduino libraries (`Arduino.h`, `SoftwareSerial.h`, `ArduinoJson.h`)
- **Pin Configuration** (consistent across projects):
  - Pin 2: Frame timestamp trigger (input)
  - Pin 3: Cue speaker (PWM capable)
  - Pin 4: Pump
  - Pin 5: Lick circuit
  - Pin 6: Laser
  - Pin 9: Imaging trigger (output)
  - Pin 10: Right-hand lever
  - Pin 13: Left-hand lever
- **Connectivity**: USB connection to a host computer running the REACHER Suite Python framework

## Project Downloads

| Project          | Paradigm Description                                      | Download Link                                                                 |
|------------------|----------------------------------------------------------|--------------------------------------------------------------------------------|
| Operant FR       | Fixed-ratio: Rewards after a set number of presses       | [![Download](https://img.shields.io/badge/Download-blue?style=for-the-badge)](https://github.com/Otis-Lab-MUSC/REACHER-Firmware/releases/download/v1.0.1/operant_FR.zip) |
| Omission         | Contingency-based: Rewards withheld under conditions     | [![Download](https://img.shields.io/badge/Download-blue?style=for-the-badge)](https://github.com/Otis-Lab-MUSC/REACHER-Firmware/releases/download/v1.0.1/omission.zip) |
| Operant PR       | Progressive-ratio: Press requirement increases           | [![Download](https://img.shields.io/badge/Download-blue?style=for-the-badge)](https://github.com/Otis-Lab-MUSC/REACHER-Firmware/releases/download/v1.0.1/operant_PR.zip) |
| Operant VI       | Variable-interval: Reward available once per interval    | [![Download](https://img.shields.io/badge/Download-blue?style=for-the-badge)](https://github.com/Otis-Lab-MUSC/REACHER-Firmware/releases/download/v1.0.1/operant_VI.zip) |
| All Projects     | Combined archive of all four paradigms                   | [![Download](https://img.shields.io/badge/Download-blue?style=for-the-badge)](https://github.com/Otis-Lab-MUSC/REACHER-Firmware/releases/download/v1.0.1/operant_suite.zip) |

## Project Details

### 1. Operant Fixed Ratio (operant_FR.ino)
#### Paradigm
- **Fixed-Ratio**: Triggers rewards (cue tone, pump infusion) after a set number of active lever presses (default ratio: 1:1).
- **Press Types**: Labels presses as "ACTIVE" (triggers reward), "TIMEOUT" (during timeout period), or "INACTIVE" (non-active lever or no reward condition).

#### Key Features
- Configurable fixed ratio via serial command (`SET_RATIO:`).
- Timeout period after reward delivery prevents immediate re-triggering.
- Real-time logging of lever presses and infusions.

#### Default Settings
- Fixed ratio: 1
- Trace interval: 0ms
- Timeout period: 20,000ms
- Cue duration: 1,600ms
- Infusion duration: 2,000ms
- Laser duration: 30,000ms

### 2. Omission (omission.ino)
#### Paradigm
- **Contingency-Based**: Rewards are delivered after a cue unless a specific condition (e.g., licking during the cue) occurs, in which case the reward is omitted.
- **Press Types**: "ACTIVE" (eligible for reward), "OMISSION" (reward withheld due to lick), "INACTIVE" (non-active lever).

#### Key Features
- Lick detection during cue period triggers omission.
- Laser stimulation with configurable frequency and mode (e.g., "BURST").
- Adjustable trace and timeout intervals.

#### Default Settings
- Cue duration: 1,600ms
- Trace interval: 0ms
- Timeout period: 20,000ms
- Infusion duration: 2,000ms
- Laser frequency: 20Hz, burst mode

### 3. Operant Progressive Ratio (operant_PR.ino)
#### Paradigm
- **Progressive-Ratio**: Number of lever presses required for a reward increases after each delivery (e.g., 1, 3, 5... with increment of 2).
- **Press Types**: "ACTIVE" (triggers reward), "TIMEOUT" (during cue/timeout), "INACTIVE" (non-active lever).

#### Key Features
- Progressive ratio increment configurable (`SET_PRATIO:`).
- Periodic laser stimulation independent of presses.
- Timeout period post-reward.

#### Default Settings
- Fixed ratio: 1 (initial)
- Progressive ratio increment: 2
- Trace interval: 0ms
- Timeout period: 20,000ms
- Cue duration: 1,600ms
- Infusion duration: 2,000ms
- Laser duration: 3,000ms

### 4. Operant Variable Interval (operant_VI.ino)
#### Paradigm
- **Variable-Interval**: Reward available once randomly within each interval (default: 15s); only one active press per interval is rewarded.
- **Press Types**: "ACTIVE" (triggers reward once per interval), "INACTIVE" (all other presses).

#### Key Features
- Random reward availability within a configurable interval (`SET_VARIABLE_INTERVAL:`).
- Cue and pump linked to active presses.
- Interval resets after each cycle.

#### Default Settings
- Variable interval: 15,000ms
- Trace interval: 0ms
- Timeout period: 20,000ms
- Cue duration: 1,600ms
- Infusion duration: 2,000ms
- Laser duration: 3,000ms

## Shared Features Across Projects

- **Hardware Control**:
  - Manages levers, cues, pumps, lasers, lick circuits, and imaging triggers.
  - Dynamic configuration via serial commands (e.g., `ARM_CS`, `SET_FREQUENCY_CS:8000`).
- **Serial Communication**:
  - String-based commands and data transmission at 115200 baud.
- **Data Collection**:
  - Logs events (e.g., lever presses, infusions) with timestamps adjusted to program start (`differenceFromStartTime`).
- **Modularity**:
  - Uses C++ classes (`Device`, `Cue`, `Laser`, `Lever`, `LickCircuit`, `Pump`) for extensibility.

## Usage Example

1. Upload the desired sketch to your Arduino.
2. Connect the Arduino to a computer running the REACHER Suite Python framework.
3. Send serial commands via the dashboard:
    ```bash
    LINK              # Establishes connection
    ARM_LEVER_RH      # Arms right-hand lever
    ACTIVE_LEVER_RH   # Sets right-hand lever as active (if applicable)
    START-PROGRAM     # Begins the experiment
4. Monitor serial output for events (e.g., `"RH_LEVER,ACTIVE_PRESS,100,150"`, `"PUMP,INFUSION,200,2200"`).

## Notable Functions (Shared Across Projects)

- **`setup()`**: Initializes pins, sets up hardware objects, and starts serial communication.
- **`loop()`**: Runs the main program (PROGRAM()) and monitors serial commands.
- **`sendSetupJSON()`**: Sends configuration data as JSON over serial when the program starts.
- **`monitorSerialCommands()`**: Processes incoming commands using a command-handler table.
- **`PROGRAM()`**: Orchestrates hardware monitoring and data logging when linked to the GUI.

## Data Integrity Measures

- **Real-Time Serial Logging**:
    - Events are sent immediately over serial with adjusted timestamps, ensuring no data loss if the connection drops.
    - Example: `"RH_LEVER,ACTIVE_PRESS,100,150"`.
- **Debouncing**:
    - Lever inputs: 100ms debounce delay.
    - Lick circuit: 25ms debounce delay.
- **Interrupt-Driven Frame Capture**:
    - Frame timestamps captured via `frameSignalISR` and logged instantly.
- **Ping Mechanism**:
    - Periodic ping (every 10-30s, varies by project) verifies serial connectivity.

## Additional Notes

- **Version**: All projects are at v1.0.1.
- **Warning**: These implementations are in testing and not fully verified.
- **Documentation**: Full Doxygen-generated documentation is available at the link above, covering classes, functions, and source code.

## Getting Started

1. Clone the repository or download the desired project(s) from the table above.
2. Install required Arduino libraries via the Library Manager.
3. Upload the `.ino` file to your Arduino.
4. Launch the REACHER Suite Dashboard (download [here](https://github.com/Otis-Lab-MUSC/REACHER-Suite/)) and connect to the Arduino.
5. Configure and start your experiment using the provided serial commands.

This suite provides a robust foundation for operant conditioning research, integrating seamlessly with the REACHER ecosystem.

<br><br>
<div align="center">
  <h2>Copyright & License</h2>
  <p>© 2025 Otis Lab. All rights reserved.</p>
  <p>For more information, please contact the author at <a href="mailto:thejoshbq@proton.me"><i>thejoshbq@proton.me</i></a>
</div>

<div align="center", style="padding: 10px; background-color: #333; color: white;">
    <p><i>"The heavens declare the glory of God, and the sky above proclaims his handiwork."</i>
    <p>Psalm 19:1</p>
</div>
