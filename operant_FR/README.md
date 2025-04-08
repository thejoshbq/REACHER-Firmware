# Operant Fixed Ratio (operant_FR.ino) Documentation

[![Documentation](https://img.shields.io/badge/Documentation-grey?style=for-the-badge)](https://otis-lab-musc.github.io/REACHER-Operant-FR/) [![Documentation](https://img.shields.io/badge/Download-blue?style=for-the-badge)](https://github.com/Otis-Lab-MUSC/REACHER-Operant-FR/releases/download/v1.0.1/operant_FR.zip) 

## Overview

The `operant_FR.ino` sketch is a core component of the REACHER Suite Arduino framework, implementing a fixed-ratio reinforcement paradigm for head-fixed rodent experiments. Designed for the REACHER Suite (Rodent Experiment Application Controls and Handling Ecosystem for Research), this sketch controls hardware components (e.g., levers, cues, pumps) via serial commands from the Python framework, collects real-time data, and ensures robust data integrity through immediate serial logging.

## System Requirements

- **Hardware**: Arduino microcontroller (e.g., Arduino UNO)
- **Firmware Dependencies**: Arduino libraries (`Arduino.h`, `SoftwareSerial.h`, `ArduinoJson.h`)
- **Pin Configuration**:
  - Pin 2: Frame timestamp trigger (input)
  - Pin 3: Cue speaker (PWM capable)
  - Pin 4: Pump
  - Pin 5: Lick circuit
  - Pin 6: Laser
  - Pin 9: Imaging trigger (output)
  - Pin 10: Right-hand lever
  - Pin 13: Left-hand lever
- **Connectivity**: USB connection to a host computer running the REACHER Suite Python framework

## Key Features

- **Fixed-Ratio Paradigm**:
    - Triggers rewards (cue tone, pump infusion) after a set number of active lever presses (default ratio: 1:1).
    - Labels presses as "ACTIVE", "TIMEOUT", or "INACTIVE" based on timing conditions.
- **Hardware Control**:
    - Manages levers, cues, pumps, lasers, lick circuits, and imaging triggers.
    - Supports dynamic configuration via serial commands (e.g., "ARM_LEVER_RH", "SET_FREQUENCY_CS:8000").
- **Serial Communication**:
    - Uses string-based commands and data transmission at 115200 baud for debugging and interoperability with Python.
- **Data Collection**:
    - Logs events (e.g., lever presses, infusions) with adjusted timestamps in real time over serial.
- **Modularity**:
    - Integrates with a suite of C++ classes (Lever, Cue, Pump, etc.) for extensibility.

## Usage Example

1. Upload the sketch to your Arduino.
2. Connect the Arduino to a computer running the REACHER Suite Python framework.
3. Send serial commands via the dashboard:
    ```bash
    // Example commands:
    
    LINK              // Establishes connection
    ARM_LEVER_RH      // Arms right-hand lever
    ACTIVE_LEVER_RH   // Sets right-hand lever as active
    START-PROGRAM     // Begins the experiment
    ```
4. Monitor serial output for events (e.g., `"RH_LEVER,ACTIVE_PRESS,<start>,<end>"`).

## Notable Functions

- `setup()`: Initializes pins, sets up hardware objects, and starts serial communication.
- `loop()`: Runs the main program (`PROGRAM()`) and monitors serial commands.
- `sendSetupJSON()`: Sends configuration data as JSON over serial when the program starts.
- `monitorSerialCommands()`: Processes incoming commands (e.g., `"START-PROGRAM"`, `"SET_RATIO:5"`) using a command-handler table.
- `PROGRAM()`: Orchestrates hardware monitoring and data logging when linked to the GUI.

## Data Integrity Measures

- **Real-Time Serial Logging**:
    - Events (e.g., lever presses, infusions) are immediately sent over serial with timestamps adjusted to program start (`differenceFromStartTime`), ensuring no data loss even if the connection drops.
    - Example: `"RH_LEVER,ACTIVE_PRESS,100,150"`.
- **Debouncing**:
    - Lever and lick circuit inputs use a debounce delay (100ms for levers, 25ms for licks) to filter noise and ensure accurate event detection.
- **Interrupt-Driven Frame Capture**:
    - Frame timestamps are captured via an interrupt service routine (`frameSignalISR`), logged instantly when `collectFrames` is true.
- **Ping Mechanism**:
    - Sends a periodic ping (every 30s) to verify serial connectivity, maintaining session integrity.

## Additional Notes

- **Default Settings**:
    - Fixed ratio: 1
    - Trace interval: 0ms
    - Timeout period: 20,000ms
    - Cue duration: 1,600ms
    - Infusion duration: 2,000ms
    - Laser duration: 30,000ms
- **Program Logic**:
    - Active presses trigger rewards only when outside timeout periods, with configurable ratios.

This sketch integrates seamlessly with the REACHER Suite Python framework, providing a reliable foundation for operant conditioning experiments.




