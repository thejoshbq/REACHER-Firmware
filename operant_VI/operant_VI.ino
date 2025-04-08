#define SKETCH_NAME "operant_VI.ino" ///< Name of the sketch.
#define VERSION "v1.0.0"             ///< Version of the sketch.

/* ++++++++++++++++++++ INFORMATION ++++++++++++++++++++
  Meta data:
  Josh Boquiren (@thejoshbq on GitHub), Otis Lab

  "Unless the Lord builds the house, those who build it labor in vain.
  Unless the Lord watches over the city, the watchman stays awake in vain."
  Psalm 127:1

  ---------------------------------------------------------------------
  Program notes:
  - An active press becomes available once randomly within each variable interval and will be labeled as "ACTIVE"
  - Once an active press occurs, the reward becomes unavailable for the remainder of that interval
  - All other presses will be denoted as "INACTIVE"
  - Timestamps are adjusted to the start of the program once the program is started (adjusted timestamp = current timestamp - program start time)

  ---------------------------------------------------------------------
  Defaults:
  - ratio, 1 reward:15 seconds
  - trace interval length, 0ms (time between tone and infusion)
  - timeout period length, 20000ms (time from cue tone end)
  - cue tone length, 1600ms
  - infusion length, 2000ms
  - active lever, right-hand lever
  - laser pulse duration, 3000ms

  ---------------------------------------------------------------------
  Current pin configuration:
  - Pin 10, right-hand lever
  - Pin 13, left-hand lever
  - Pin 4, pump
  - Pin 3 (PWM capable pin), conditioned stimulus speaker (denoted as "cs") and speaker for linked/unlinked jingle
  - Pin 2, trigger for frame timestamp input signals
  - Pin 9, trigger for imaging program start and stop
  - Pin 5, lick circuit
  - Pin 6, laser

  ---------------------------------------------------------------------
  Sections:
  - Section 1: Definitions and configurations
  - Section 2: setup() and loop()
  - Section 3: Utility functions
  - Section 4: Looped functions
  - Section 5: Interrupt functions
  - Section 6: Main program

  ++++++++++++++++++++ INFORMATION ++++++++++++++++++++ */

/**
 * @warning This implementation is in testing and not fully verified.
 */

// =======================================================
// ====================== SECTION 1 ======================
// =======================================================

// Libraries
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "Device.h"
#include "Laser.h"
#include "Lever.h"
#include "Cue.h"
#include "Pump.h"
#include "LickCircuit.h"

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
volatile bool frameSignalReceived = false; ///< Indicates if a frame signal was received.

// Global variables
uint32_t baudrate = 115200;          ///< Baud rate for serial communication.
uint32_t differenceFromStartTime;    ///< Offset from program start time (ms).
uint32_t traceIntervalLength = 0;    ///< Trace interval between cue and infusion (ms).
uint32_t timeoutIntervalLength = 20000; ///< Timeout period after cue end (ms).
uint32_t timeoutIntervalStart;       ///< Start timestamp of timeout interval (ms).
uint32_t timeoutIntervalEnd;         ///< End timestamp of timeout interval (ms).
uint32_t previousPing = 0;           ///< Last ping timestamp (ms).
const uint32_t pingInterval = 10000; ///< Ping interval (ms).
volatile uint32_t frameSignalTimestamp = 0; ///< Frame signal timestamp (ms).
uint32_t variableInterval = 15000;   ///< Variable interval duration (ms).

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

// =======================================================
// ====================== SECTION 3 ======================
// =======================================================

/**
 * @brief Logs lever press and release data to the serial monitor.
 * 
 * Records press and release timestamps, and infusion data if applicable, adjusted by program start time.
 * 
 * @param lever Reference to a pointer to the Lever object being monitored.
 * @param pump Pointer to the Pump object (optional).
 */
void pressingDataEntry(Lever*& lever, Pump* pump) {
    String pressEntry;
    String infusionEntry;
    lever->setReleaseTimestamp(millis()); // Set press release timestamp
    pressEntry = lever->getOrientation() + "_LEVER,";
    pressEntry += lever->getPressType() + "_PRESS,";
    if (differenceFromStartTime) {
        pressEntry += String(lever->getPressTimestamp() - differenceFromStartTime) + ",";
        pressEntry += String(lever->getReleaseTimestamp() - differenceFromStartTime);
    } else {
        pressEntry += String(lever->getPressTimestamp()) + ",";
        pressEntry += String(lever->getReleaseTimestamp());
    }
    Serial.println(pressEntry); // Send press data to serial connection
    if (pump && pump->isArmed() && lever->getPressType() == "ACTIVE") {
        infusionEntry = "PUMP,INFUSION,";
        infusionEntry += differenceFromStartTime ? String(pump->getInfusionStartTimestamp() - differenceFromStartTime) : String(pump->getInfusionStartTimestamp());
        infusionEntry += ",";
        infusionEntry += differenceFromStartTime ? String(pump->getInfusionEndTimestamp() - differenceFromStartTime) : String(pump->getInfusionEndTimestamp());
        Serial.println(infusionEntry);
    }
}

/**
 * @brief Defines the type of lever press based on variable interval logic.
 * 
 * Labels the press as "ACTIVE" if it occurs within the random interval and no prior active press
 * has occurred, otherwise "INACTIVE" or "NO CONDITION".
 * 
 * @param programRunning Boolean indicating if the program is running.
 * @param lever Reference to a pointer to the Lever object being pressed.
 * @param cue Pointer to the Cue object (optional).
 * @param pump Pointer to the Pump object (optional).
 */
void definePressActivity(bool programRunning, Lever*& lever, Cue* cue, Pump* pump) {
    int32_t timestamp = millis();
    if (lever == activeLever && !lever->getActivePressOccurred() && 
        timestamp >= lever->getIntervalStartTime() + lever->getRandomInterval() && 
        timestamp < lever->getIntervalStartTime() + 15000 && cue->isArmed()) {
        lever->setPressType("ACTIVE");
        lever->setActivePressOccurred(true);
        if (cue->isArmed()) {
            cue->setOnTimestamp(timestamp);
            cue->setOffTimestamp(timestamp);
        }
        if (pump->isArmed()) {
            pump->setInfusionPeriod(cue->getOffTimestamp(), traceIntervalLength);
        }
    } else if (!cue->isArmed()) {
        lever->setPressType("NO CONDITION");
    } else {
        lever->setPressType("INACTIVE");
    }
}

/**
 * @brief Starts the program and triggers imaging.
 * 
 * Signals the start of the program and sets the initial time offset.
 * 
 * @param pin The digital pin to trigger imaging.
 */
void startProgram(byte pin) {
    Serial.println("STARTING PROGRAM");
    digitalWrite(pin, HIGH);            // Trigger imaging start
    delay(50);                          // Ensure data transmission
    digitalWrite(pin, LOW);             // Finish trigger
    differenceFromStartTime = millis(); // Set program start offset
}

/**
 * @brief Ends the program and signals imaging stop.
 * 
 * @param pin The digital pin to trigger imaging end.
 */
void endProgram(byte pin) {
    Serial.println("ENDING PROGRAM");
    digitalWrite(pin, HIGH); // Trigger imaging end
    delay(50);
    digitalWrite(pin, LOW);
}

/**
 * @brief Sets the trace interval length.
 * 
 * Adjusts the delay between cue tone and infusion.
 * 
 * @param length Duration in milliseconds for the trace interval.
 */
void setTraceInterval(uint32_t length) {
    traceIntervalLength = length;
    Serial.println("SET TRACE INTERVAL TO " + String(traceIntervalLength));
}

/**
 * @brief Plays a jingle to indicate GUI connection status.
 * 
 * Plays an ascending tone for "LINK" and descending for "UNLINK".
 * 
 * @param connected String indicating "LINK" or "UNLINK".
 * @param cue Cue object to play the tone.
 */
void connectionJingle(String connected, Cue cue) {
    if (connected == "LINK") {
        linkedToGUI = true;
        static int pitch = 500; // Starting tone frequency
        for (int i = 0; i < 3; i++) {
            tone(cue.getPin(), pitch, 100);
            delay(100);
            noTone(cue.getPin());
            pitch += 500; // Increase frequency
        }
        pitch = 500;                     // Reset frequency
        Serial.println("Linked to GUI"); // Output status
    } else if (connected == "UNLINK") {
        linkedToGUI = false;
        static int pitch = 1500; // Starting tone frequency
        for (int i = 0; i < 3; i++) {
            tone(cue.getPin(), pitch, 100);
            delay(100);
            noTone(cue.getPin());
            pitch -= 500; // Decrease frequency
        }
        pitch = 1500;                        // Reset frequency
        Serial.println("Unlinked from GUI"); // Output status
    }
}

/**
 * @brief Sends setup configuration as JSON to the serial monitor.
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
    doc["LASER STIM LENGTH"] = laser.getStimDuration();

    serializeJson(doc, Serial);
    Serial.println('\n');
}

// =======================================================
// ====================== SECTION 4 ======================
// =======================================================

/**
 * @brief Sends a periodic ping to ensure serial connection.
 */
void pingDevice() {
    uint32_t currentMillis = millis();
    if (currentMillis - previousPing >= pingInterval) {
        previousPing = currentMillis;
        Serial.println("ping");
    }
}

/**
 * @brief Controls cue tone operation.
 * 
 * Turns the cue tone on during its assigned period and off otherwise, if armed.
 * 
 * @param cue Pointer to the Cue object (optional).
 */
void manageCue(Cue* cue) {
    int32_t timestamp = millis();
    if (cue) {
        if (cue->isArmed()) {
            if (timestamp <= cue->getOffTimestamp() && timestamp >= cue->getOnTimestamp()) {
                cue->on(); // Turn on cue
                cue->setRunning(true);
            } else {
                cue->off(); // Turn cue off
                cue->setRunning(false);
            }
        }
    }
}

/**
 * @brief Controls pump operation.
 * 
 * Turns the pump on during the infusion period and off otherwise, if armed.
 * 
 * @param pump Pointer to the Pump object (optional).
 */
void managePump(Pump* pump) {
    int32_t timestamp = millis();
    if (pump->isArmed()) {
        if (timestamp <= pump->getInfusionEndTimestamp() && timestamp >= pump->getInfusionStartTimestamp()) {
            pump->on(); // Turn the pump on
            pump->setRunning(true);
        } else {
            pump->off(); // Turn the pump off
            pump->setRunning(false);
        }
    }
}

/**
 * @brief Monitors lever pressing with debouncing and variable interval logic.
 * 
 * Detects lever presses, applies debouncing, and resets the variable interval when elapsed.
 * 
 * @param programRunning Boolean indicating if the program is running.
 * @param lever Reference to a pointer to the Lever object.
 * @param cue Pointer to the Cue object (optional).
 * @param pump Pointer to the Pump object (optional).
 */
void monitorPressing(bool programRunning, Lever*& lever, Cue* cue, Pump* pump) {
    static uint32_t lastDebounceTime = 0; // Last time the lever input was toggled
    const uint32_t debounceDelay = 100;   // Debounce time in milliseconds
    manageCue(cue);                       // Manage cue delivery
    managePump(pump);                     // Manage infusion delivery
    if (lever->isArmed()) {
        bool currentLeverState = digitalRead(lever->getPin()); // Read current state
        if (currentLeverState != lever->getPreviousLeverState()) {
            lastDebounceTime = millis(); // Reset debouncing timer
        }
        if ((millis() - lastDebounceTime) > debounceDelay) {
            if (currentLeverState != lever->getStableLeverState()) {
                lever->setStableLeverState(currentLeverState); // Update stable state
                if (currentLeverState == LOW) { // Lever press detected
                    lever->setPressTimestamp(millis());
                    definePressActivity(programRunning, lever, cue, pump);
                } else { // Lever release detected
                    lever->setReleaseTimestamp(millis());
                    pressingDataEntry(lever, pump);
                }
            }
        }
        lever->setPreviousLeverState(currentLeverState); // Update previous state
    }
    if (millis() - lever->getIntervalStartTime() >= variableInterval) {
        lever->resetInterval();
    }
}

/**
 * @brief Monitors licking activity with debouncing.
 * 
 * Detects lick events, applies debouncing, and logs timestamps.
 * 
 * @param lickSpout Reference to the LickCircuit object.
 */
void monitorLicking(LickCircuit& lickSpout) {
    static uint32_t lastDebounceTime = 0; // Last time the lick input was toggled
    const uint32_t debounceDelay = 25;    // Debounce time in milliseconds
    if (lickSpout.isArmed()) {
        bool currentLickState = digitalRead(lickSpout.getPin());
        if (currentLickState != lickSpout.getPreviousLickState()) {
            lastDebounceTime = millis();
        }
        if ((millis() - lastDebounceTime) > debounceDelay) {
            if (currentLickState != lickSpout.getStableLickState()) {
                lickSpout.setStableLickState(currentLickState);
                if (currentLickState == HIGH) {
                    lickSpout.setLickTouchTimestamp(millis());
                } else {
                    lickSpout.setLickReleaseTimestamp(millis());
                    String lickEntry = "LICK_CIRCUIT,LICK," +
                                       String(lickSpout.getLickTouchTimestamp() - differenceFromStartTime) + "," +
                                       String(lickSpout.getLickReleaseTimestamp() - differenceFromStartTime);
                    Serial.println(lickEntry);
                }
            }
        }
        lickSpout.setPreviousLickState(currentLickState);
    }
}

/**
 * @brief Monitors and processes incoming serial commands.
 * 
 * Handles commands from the GUI to control the program and devices.
 */
void monitorSerialCommands() {
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();

        if (command == "LINK") {
            connectionJingle("LINK", cs);
        } else if (command == "UNLINK") {
            connectionJingle("UNLINK", cs);
        } else if (command == "START-PROGRAM") {
            sendSetupJSON();
            startProgram(IMAGING_TRIGGER);
            programIsRunning = true;
        } else if (command == "END-PROGRAM") {
            endProgram(IMAGING_TRIGGER);
            programIsRunning = false;
            delay(1000);
        } else if (command.startsWith("SET_VARIABLE_INTERVAL:")) {
            int32_t initVariableInterval = command.substring(String("SET_VARIABLE_INTERVAL:").length()).toInt();
            variableInterval = initVariableInterval * 1000; // Convert to milliseconds
        } else if (command.startsWith("SET_TIMEOUT_PERIOD_LENGTH:")) {
            int32_t initTimeoutPeriodLength = command.substring(String("SET_TIMEOUT_PERIOD_LENGTH:").length()).toInt();
            timeoutIntervalLength = initTimeoutPeriodLength;
        } else if (command == "ARM_LEVER_RH") {
            leverRH.arm();
        } else if (command == "DISARM_LEVER_RH") {
            leverRH.disarm();
        } else if (command == "ACTIVE_LEVER_RH") {
            activeLever = &leverRH;
            inactiveLever = &leverLH;
        } else if (command == "ARM_LEVER_LH") {
            leverLH.arm();
        } else if (command == "DISARM_LEVER_LH") {
            leverLH.disarm();
        } else if (command == "ACTIVE_LEVER_LH") {
            activeLever = &leverLH;
            inactiveLever = &leverRH;
        } else if (command == "ARM_CS") {
            cs.arm();
        } else if (command == "DISARM_CS") {
            cs.disarm();
        } else if (command.startsWith("SET_FREQUENCY_CS:")) {
            int32_t frequency = command.substring(String("SET_FREQUENCY_CS:").length()).toInt();
            cs.setFrequency(frequency);
        } else if (command.startsWith("SET_DURATION_CS:")) {
            int32_t duration = command.substring(String("SET_DURATION_CS:").length()).toInt();
            cs.setDuration(duration);
        } else if (command == "ARM_PUMP") {
            pump.arm();
        } else if (command == "DISARM_PUMP") {
            pump.disarm();
        } else if (command.startsWith("SET_TRACE_INTERVAL:")) {
            int32_t length = command.substring(String("SET_TRACE_INTERVAL:").length()).toInt();
            setTraceInterval(length);
        } else if (command == "PUMP_TEST_ON") {
            pump.on();
        } else if (command == "PUMP_TEST_OFF") {
            pump.off();
        } else if (command == "ARM_LASER") {
            laser.arm();
        } else if (command == "DISARM_LASER") {
            laser.disarm();
        } else if (command == "ARM_LICK_CIRCUIT") {
            lickCircuit.arm();
        } else if (command == "DISARM_LICK_CIRCUIT") {
            lickCircuit.disarm();
        } else {
            Serial.println(">>> command " + command + " is invalid.");
        }
        delay(50); // Short delay for command processing
    }
}

// =======================================================
// ====================== SECTION 5 ======================
// =======================================================

/**
 * @brief Interrupt service routine for frame signal detection.
 * 
 * Captures the timestamp of a frame signal, adjusted by the program start time.
 */
void frameSignalISR() {
    frameSignalReceived = true;
    frameSignalTimestamp = millis() - differenceFromStartTime;
}

/**
 * @brief Handles frame signal logging.
 * 
 * Logs the frame timestamp to serial when a signal is received.
 */
void handleFrameSignal() {
    if (frameSignalReceived) {
        noInterrupts(); // Disable interrupts for safe access
        frameSignalReceived = false;
        int32_t timestamp = frameSignalTimestamp;
        interrupts();   // Re-enable interrupts
        Serial.println("FRAME_TIMESTAMP," + String(timestamp));
    }
}

// =======================================================
// ====================== SECTION 6 ======================
// =======================================================

/**
 * @brief Main program function to manage device interactions.
 * 
 * Runs the core logic for lever pressing, licking, and frame handling when connected to the GUI.
 */
void PROGRAM() {
    if (linkedToGUI) {
        monitorPressing(programIsRunning, activeLever, &cs, &pump);
        monitorPressing(programIsRunning, inactiveLever, nullptr, nullptr);
        monitorLicking(lickCircuit);
        handleFrameSignal();
        pingDevice();
    }
}