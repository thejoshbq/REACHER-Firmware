#define SKETCH_NAME "operant_FR.ino" ///< Name of the sketch.
#define VERSION "v1.0.0"             ///< Version of the sketch.

/* ++++++++++++++++++++ INFORMATION ++++++++++++++++++++
  Meta data:
  Josh Boquiren (@thejoshbq on GitHub), Otis Lab

  "Unless the Lord builds the house, those who build it labor in vain.
  Unless the Lord watches over the city, the watchman stays awake in vain."
  Psalm 127:1

  ---------------------------------------------------------------------
  Program notes:
  - An active lever press triggers a cue tone, followed by a trace interval and pump infusion, and will be labeled as "ACTIVE"
  - Presses that occur during the cue tone, trace interval, pump infusion, or timeout period will be labeled as a "TIMEOUT" press
  - All other presses will be denoted as "INACTIVE"
  - Timestamps are adjusted to the start of the program once the program is started (adjusted timestamp = current timestamp - program start time)

  ---------------------------------------------------------------------
  Defaults:
  - ratio, 1 reward:1 active press
  - trace interval length, 0ms (time between tone and infusion)
  - timeout period length, 20000ms (time from cue tone end)
  - cue tone length, 1600ms
  - infusion length, 2000ms
  - active lever, right-hand lever
  - laser pulse duration, 30000ms

  ---------------------------------------------------------------------
  Current pin configuration:
  - Pin 2, trigger for frame timestamp input signals
  - Pin 3 (PWM capable pin), conditioned stimulus speaker (denoted as "cs") and speaker for linked/unlinked jingle
  - Pin 4, pump
  - Pin 5, lick circuit
  - Pin 6, laser
  - Pin 9, trigger for imaging program start and stop
  - Pin 10, right-hand lever
  - Pin 13, left-hand lever

  ---------------------------------------------------------------------
  Sections:
  - Section 1: Definitions and configurations
  - Section 2: setup(), loop(), and sendSetupJSON()
  - Section 3: Serial commands library for this module
  - Section 4: Main program structure

  ++++++++++++++++++++ INFORMATION ++++++++++++++++++++ */

// =======================================================
// ====================== SECTION 1 ======================
// =======================================================

// Libraries
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "Device.h"
#include "Laser.h"
#include "Laser_Utils.h"
#include "Lever.h"
#include "Lever_Utils.h"
#include "Cue.h"
#include "Cue_Utils.h"
#include "Pump.h"
#include "Pump_Utils.h"
#include "LickCircuit.h"
#include "LickCircuit_Utils.h"
#include "Utils.h"
#include "Program_Utils.h"

// Pin definitions
const byte RH_LEVER_PIN = 10;        ///< Right-hand lever pin.
const byte LH_LEVER_PIN = 13;        ///< Left-hand lever pin.
const byte CS_PIN = 3;               ///< Conditioned stimulus speaker pin (PWM capable).
const byte PUMP_PIN = 4;             ///< Pump pin.
const byte IMAGING_TRIGGER = 9;      ///< Imaging trigger pin.
const byte TIMESTAMP_TRIGGER = 2;    ///< Frame timestamp trigger pin.
const byte LICK_CIRCUIT_PIN = 5;     ///< Lick circuit pin.
const byte LASER_PIN = 6;            ///< Laser pin.

// Class instantiations for components
Lever leverRH(RH_LEVER_PIN);         ///< Right-hand lever object.
Lever leverLH(LH_LEVER_PIN);         ///< Left-hand lever object.
Lever* activeLever = &leverRH;       ///< Pointer to the active lever (default: RH).
Lever* inactiveLever = &leverLH;     ///< Pointer to the inactive lever (default: LH).
Cue cs(CS_PIN);                      ///< Cue speaker object.
Pump pump(PUMP_PIN);                 ///< Pump object.
LickCircuit lickCircuit(LICK_CIRCUIT_PIN); ///< Lick circuit object.
Laser laser(LASER_PIN);              ///< Laser object.

// Global Boolean variables
bool setupFinished = false;          ///< Indicates if setup is complete.
bool programIsRunning = false;       ///< Indicates if the program is running.
bool linkedToGUI = false;            ///< Indicates if connected to the GUI.
bool collectFrames = false;          ///< Indicates if frame signals are collected.
volatile bool frameSignalReceived = false; ///< Indicates if a frame signal was received.

// Global variables
uint32_t baudrate = 115200;          ///< Baud rate for serial communication.
uint32_t differenceFromStartTime;    ///< Offset from program start time (ms).
uint32_t traceIntervalLength = 0;    ///< Trace interval between cue and infusion (ms).
uint32_t timeoutIntervalLength = 20000; ///< Timeout period after cue end (ms).
uint32_t timeoutIntervalStart;       ///< Start timestamp of timeout interval (ms).
uint32_t timeoutIntervalEnd;         ///< End timestamp of timeout interval (ms).
uint32_t previousPing = 0;           ///< Last ping timestamp (ms).
const uint32_t pingInterval = 30000; ///< Ping interval (ms).
volatile uint32_t frameSignalTimestamp = 0; ///< Frame signal timestamp (ms).
int32_t fRatio = 1;                  ///< Fixed ratio for reward delivery.
int32_t pressCount = 0;              ///< Counter for lever presses.

// =======================================================
// ====================== SECTION 2 ======================
// =======================================================

/**
 * @brief Initializes the Arduino and configures pins and devices.
 * 
 * Sets up pin modes, attaches interrupts, initializes devices, and starts serial communication.
 */
void setup() {
    // 2P setup
    pinMode(IMAGING_TRIGGER, OUTPUT);
    pinMode(TIMESTAMP_TRIGGER, INPUT);
    attachInterrupt(digitalPinToInterrupt(TIMESTAMP_TRIGGER), frameSignalISR, RISING);

    // Laser setup
    pinMode(laser.getPin(), OUTPUT);
    laser.disarm();
    laser.setDuration(30);  // 30 seconds -> 30000 ms
    laser.setFrequency(20); // 20 Hz

    // RH lever setup
    pinMode(leverRH.getPin(), INPUT_PULLUP);
    leverRH.disarm();
    leverRH.setOrientation("RH");

    // LH lever setup
    pinMode(leverLH.getPin(), INPUT_PULLUP);
    leverLH.disarm();
    leverLH.setOrientation("LH");

    // CS setup
    pinMode(cs.getPin(), OUTPUT);
    cs.disarm();

    // Pump setup
    pinMode(pump.getPin(), OUTPUT);
    pump.disarm();

    // Lick circuit setup
    pinMode(lickCircuit.getPin(), INPUT);
    lickCircuit.disarm();

    // Serial connection
    Serial.begin(baudrate);
    delay(2000); // Delay to avoid buffer overload
    Serial.println(SKETCH_NAME);
    setupFinished = true;
}

/**
 * @brief Main loop to run the program and monitor serial commands.
 */
void loop() {
    PROGRAM();
    monitorSerialCommands();
}

/**
 * @brief Sends setup configuration as JSON to the serial monitor.
 * 
 * Outputs key configuration parameters for debugging or GUI integration.
 */
void sendSetupJSON() {
    StaticJsonDocument<200> doc;
    doc["DOC"] = SKETCH_NAME;
    doc["VERSION"] = VERSION;

    doc["TRACE INTERVAL LENGTH"] = traceIntervalLength;
    doc["TIMEOUT INTERVAL LENGTH"] = timeoutIntervalLength;
    doc["DELTA START TIME"] = differenceFromStartTime;
    doc["BAUDRATE"] = baudrate;

    doc["CS DURATION"] = cs.getDuration();
    doc["CS FREQUENCY"] = cs.getFrequency();
    doc["PUMP INFUSION LENGTH"] = pump.getInfusionDuration();
    doc["LASER STIM LENGTH"] = laser.getDuration();
    doc["LASER STIM FREQUENCY"] = laser.getFrequency();
    doc["LASER STIM MODE"] = laser.getStimMode();

    serializeJson(doc, Serial);
    Serial.println('\n');
}

// =======================================================
// ====================== SECTION 3 ======================
// =======================================================

#define COMMAND_BUFFER_SIZE 32 ///< Size of the command buffer.
char commandBuffer[COMMAND_BUFFER_SIZE]; ///< Buffer for incoming serial commands.

/**
 * @brief Extracts a numeric parameter from a command string.
 * 
 * @param cmd Command string (e.g., "SET_RATIO:5").
 * @param prefix Prefix to match (e.g., "SET_RATIO:").
 * @return Extracted value as a long integer, or 0 if not found.
 */
int32_t extractParam(const char* cmd, const char* prefix) {
    size_t prefixLen = strlen(prefix);
    if (strncmp(cmd, prefix, prefixLen) == 0) {
        const char* paramStart = cmd + prefixLen;
        return atol(paramStart);
    }
    return 0;
}

/**
 * @brief Handles the "LINK" command to connect to the GUI.
 * @param cmd Command string.
 */
void handleLink(const char* cmd) {
    connectionJingle("LINK", cs, linkedToGUI);
}

/**
 * @brief Handles the "UNLINK" command to disconnect from the GUI.
 * @param cmd Command string.
 */
void handleUnlink(const char* cmd) {
    connectionJingle("UNLINK", cs, linkedToGUI);
}

/**
 * @brief Handles the "START-PROGRAM" command to begin the program.
 * @param cmd Command string.
 */
void handleStartProgram(const char* cmd) {
    startProgram(IMAGING_TRIGGER);
    sendSetupJSON();
    programIsRunning = true;
}

/**
 * @brief Handles the "END-PROGRAM" command to stop the program.
 * @param cmd Command string.
 */
void handleEndProgram(const char* cmd) {
    endProgram(IMAGING_TRIGGER);
    programIsRunning = false;
    delay(1000);
}

/**
 * @brief Handles the "SET_RATIO:" command to set the fixed ratio.
 * @param cmd Command string with parameter (e.g., "SET_RATIO:5").
 */
void handleSetRatio(const char* cmd) {
    int32_t value = extractParam(cmd, "SET_RATIO:");
    fRatio = value;
}

/**
 * @brief Handles the "SET_TIMEOUT_PERIOD_LENGTH:" command to set timeout length.
 * @param cmd Command string with parameter.
 */
void handleSetTimeoutPeriodLength(const char* cmd) {
    int32_t value = extractParam(cmd, "SET_TIMEOUT_PERIOD_LENGTH:");
    timeoutIntervalLength = value;
}

/**
 * @brief Handles the "ARM_FRAME" command to enable frame collection.
 * @param cmd Command string.
 */
void handleArmFrame(const char* cmd) {
    collectFrames = true;
}

/**
 * @brief Handles the "DISARM_FRAME" command to disable frame collection.
 * @param cmd Command string.
 */
void handleDisarmFrame(const char* cmd) {
    collectFrames = false;
}

/**
 * @brief Handles the "ARM_LEVER_RH" command to arm the right-hand lever.
 * @param cmd Command string.
 */
void handleArmLeverRH(const char* cmd) {
    leverRH.arm();
}

/**
 * @brief Handles the "DISARM_LEVER_RH" command to disarm the right-hand lever.
 * @param cmd Command string.
 */
void handleDisarmLeverRH(const char* cmd) {
    leverRH.disarm();
}

/**
 * @brief Handles the "ACTIVE_LEVER_RH" command to set RH as active lever.
 * @param cmd Command string.
 */
void handleActiveLeverRH(const char* cmd) {
    activeLever = &leverRH;
    inactiveLever = &leverLH;
    Serial.print(F("ACTIVE LEVER: "));
    Serial.println(activeLever->getOrientation());
}

/**
 * @brief Handles the "ARM_LEVER_LH" command to arm the left-hand lever.
 * @param cmd Command string.
 */
void handleArmLeverLH(const char* cmd) {
    leverLH.arm();
}

/**
 * @brief Handles the "DISARM_LEVER_LH" command to disarm the left-hand lever.
 * @param cmd Command string.
 */
void handleDisarmLeverLH(const char* cmd) {
    leverLH.disarm();
}

/**
 * @brief Handles the "ACTIVE_LEVER_LH" command to set LH as active lever.
 * @param cmd Command string.
 */
void handleActiveLeverLH(const char* cmd) {
    activeLever = &leverLH;
    inactiveLever = &leverRH;
    Serial.print(F("ACTIVE LEVER: "));
    Serial.println(activeLever->getOrientation());
}

/**
 * @brief Handles the "ARM_CS" command to arm the cue speaker.
 * @param cmd Command string.
 */
void handleArmCS(const char* cmd) {
    cs.arm();
}

/**
 * @brief Handles the "DISARM_CS" command to disarm the cue speaker.
 * @param cmd Command string.
 */
void handleDisarmCS(const char* cmd) {
    cs.disarm();
}

/**
 * @brief Handles the "SET_FREQUENCY_CS:" command to set cue frequency.
 * @param cmd Command string with parameter.
 */
void handleSetFrequencyCS(const char* cmd) {
    int32_t frequency = extractParam(cmd, "SET_FREQUENCY_CS:");
    cs.setFrequency(frequency);
}

/**
 * @brief Handles the "SET_DURATION_CS:" command to set cue duration.
 * @param cmd Command string with parameter.
 */
void handleSetDurationCS(const char* cmd) {
    int32_t duration = extractParam(cmd, "SET_DURATION_CS:");
    cs.setDuration(duration);
}

/**
 * @brief Handles the "ARM_PUMP" command to arm the pump.
 * @param cmd Command string.
 */
void handleArmPump(const char* cmd) {
    pump.arm();
}

/**
 * @brief Handles the "DISARM_PUMP" command to disarm the pump.
 * @param cmd Command string.
 */
void handleDisarmPump(const char* cmd) {
    pump.disarm();
}

/**
 * @brief Handles the "SET_TRACE_INTERVAL:" command to set trace interval.
 * @param cmd Command string with parameter.
 */
void handleSetTraceInterval(const char* cmd) {
    int32_t value = extractParam(cmd, "SET_TRACE_INTERVAL:");
    traceIntervalLength = value;
}

/**
 * @brief Handles the "PUMP_TEST_ON" command to turn the pump on for testing.
 * @param cmd Command string.
 */
void handlePumpTestOn(const char* cmd) {
    pump.on();
}

/**
 * @brief Handles the "PUMP_TEST_OFF" command to turn the pump off for testing.
 * @param cmd Command string.
 */
void handlePumpTestOff(const char* cmd) {
    pump.off();
}

/**
 * @brief Handles the "ARM_LASER" command to arm the laser.
 * @param cmd Command string.
 */
void handleArmLaser(const char* cmd) {
    laser.arm();
}

/**
 * @brief Handles the "DISARM_LASER" command to disarm the laser.
 * @param cmd Command string.
 */
void handleDisarmLaser(const char* cmd) {
    laser.disarm();
}

/**
 * @brief Handles the "LASER_TEST_ON" command to turn the laser on for testing.
 * @param cmd Command string.
 */
void handleLaserTestOn(const char* cmd) {
    laser.on();
}

/**
 * @brief Handles the "LASER_TEST_OFF" command to turn the laser off for testing.
 * @param cmd Command string.
 */
void handleLaserTestOff(const char* cmd) {
    laser.off();
}

/**
 * @brief Handles the "LASER_STIM_MODE_CYCLE" command to set laser to CYCLE mode.
 * @param cmd Command string.
 */
void handleLaserStimModeCycle(const char* cmd) {
    laser.setStimMode(CYCLE);
}

/**
 * @brief Handles the "LASER_STIM_MODE_ACTIVE-PRESS" command to set laser mode.
 * @param cmd Command string.
 */
void handleLaserStimModeActivePress(const char* cmd) {
    laser.setStimMode(ACTIVE_PRESS);
}

/**
 * @brief Handles the "LASER_DURATION:" command to set laser duration.
 * @param cmd Command string with parameter.
 */
void handleLaserDuration(const char* cmd) {
    int32_t duration = extractParam(cmd, "LASER_DURATION:");
    laser.setDuration(duration);
}

/**
 * @brief Handles the "LASER_FREQUENCY:" command to set laser frequency.
 * @param cmd Command string with parameter.
 */
void handleLaserFrequency(const char* cmd) {
    int32_t frequency = extractParam(cmd, "LASER_FREQUENCY:");
    laser.setFrequency(frequency);
}

/**
 * @brief Handles the "ARM_LICK_CIRCUIT" command to arm the lick circuit.
 * @param cmd Command string.
 */
void handleArmLickCircuit(const char* cmd) {
    lickCircuit.arm();
}

/**
 * @brief Handles the "DISARM_LICK_CIRCUIT" command to disarm the lick circuit.
 * @param cmd Command string.
 */
void handleDisarmLickCircuit(const char* cmd) {
    lickCircuit.disarm();
}

typedef void (*CommandHandler)(const char*); ///< Function pointer type for command handlers.

struct Command {
    const char* prefix;     ///< Command prefix to match.
    CommandHandler handler; ///< Handler function for the command.
};

/**
 * @brief Array of supported commands and their handlers.
 */
Command commands[] = {
    {"LINK", handleLink},
    {"UNLINK", handleUnlink},
    {"START-PROGRAM", handleStartProgram},
    {"END-PROGRAM", handleEndProgram},
    {"SET_RATIO:", handleSetRatio},
    {"SET_TIMEOUT_PERIOD_LENGTH:", handleSetTimeoutPeriodLength},
    {"ARM_FRAME", handleArmFrame},
    {"DISARM_FRAME", handleDisarmFrame},
    {"ARM_LEVER_RH", handleArmLeverRH},
    {"DISARM_LEVER_RH", handleDisarmLeverRH},
    {"ACTIVE_LEVER_RH", handleActiveLeverRH},
    {"ARM_LEVER_LH", handleArmLeverLH},
    {"DISARM_LEVER_LH", handleDisarmLeverLH},
    {"ACTIVE_LEVER_LH", handleActiveLeverLH},
    {"ARM_CS", handleArmCS},
    {"DISARM_CS", handleDisarmCS},
    {"SET_FREQUENCY_CS:", handleSetFrequencyCS},
    {"SET_DURATION_CS:", handleSetDurationCS},
    {"ARM_PUMP", handleArmPump},
    {"DISARM_PUMP", handleDisarmPump},
    {"SET_TRACE_INTERVAL:", handleSetTraceInterval},
    {"PUMP_TEST_ON", handlePumpTestOn},
    {"PUMP_TEST_OFF", handlePumpTestOff},
    {"ARM_LASER", handleArmLaser},
    {"DISARM_LASER", handleDisarmLaser},
    {"LASER_TEST_ON", handleLaserTestOn},
    {"LASER_TEST_OFF", handleLaserTestOff},
    {"LASER_STIM_MODE_CYCLE", handleLaserStimModeCycle},
    {"LASER_STIM_MODE_ACTIVE-PRESS", handleLaserStimModeActivePress},
    {"LASER_DURATION:", handleLaserDuration},
    {"LASER_FREQUENCY:", handleLaserFrequency},
    {"ARM_LICK_CIRCUIT", handleArmLickCircuit},
    {"DISARM_LICK_CIRCUIT", handleDisarmLickCircuit},
};

/**
 * @brief Monitors and processes incoming serial commands.
 * 
 * Reads commands from the serial buffer and executes the corresponding handler.
 */
void monitorSerialCommands() {
    if (setupFinished && Serial.available() > 0) {
        size_t bytesRead = Serial.readBytesUntil('\n', commandBuffer, COMMAND_BUFFER_SIZE - 1);
        commandBuffer[bytesRead] = '\0'; // Null-terminate the string
        bool commandHandled = false;
        for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
            const Command& cmd = commands[i];
            size_t prefixLen = strlen(cmd.prefix);
            if (cmd.prefix[prefixLen - 1] == ':') {
                if (strncmp(commandBuffer, cmd.prefix, prefixLen) == 0) {
                    cmd.handler(commandBuffer);
                    commandHandled = true;
                    break;
                }
            } else {
                if (strcmp(commandBuffer, cmd.prefix) == 0) {
                    cmd.handler(commandBuffer);
                    commandHandled = true;
                    break;
                }
            }
        }
        if (!commandHandled) {
            Serial.print(F(">>> Command ["));
            Serial.print(commandBuffer);
            Serial.println(F("] is invalid."));
        }
        delay(50); // Short delay for command processing
    }
}

// =======================================================
// ====================== SECTION 4 ======================
// =======================================================

/**
 * @brief Main program function to manage device interactions.
 * 
 * Runs the core logic for lever pressing, licking, laser stimulation, and frame handling
 * when connected to the GUI.
 */
void PROGRAM() {
    if (linkedToGUI) {
        monitorPressing(programIsRunning, activeLever, &cs, &pump, &laser);
        monitorPressing(programIsRunning, inactiveLever, nullptr, nullptr, nullptr);
        monitorLicking(lickCircuit);
        manageStim(laser);
        handleFrameSignal();
        pingDevice(previousPing, pingInterval);
    }
}