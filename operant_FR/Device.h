#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>

/**
 * @file Device.h
 * @brief Defines the base Device class for hardware components.
 * 
 * The Device class serves as a foundation for hardware-related objects, providing
 * basic functionality for pin assignment and arming/disarming states.
 */

/**
 * @class Device
 * @brief Base class for hardware devices connected to Arduino pins.
 * 
 * Provides a framework for managing a device’s pin and armed state, with virtual
 * methods for arming and disarming that can be overridden by subclasses.
 */
class Device {
protected:
    const byte pin; ///< The digital pin on the Arduino to which the device is connected.
    bool armed;     ///< Indicates whether the device is armed and able to operate.

public:
    /**
     * @brief Constructor for the Device class.
     * 
     * @param initPin The digital pin to assign to the device.
     */
    Device(byte initPin);

    /**
     * @brief Arms the device to enable its operation.
     * 
     * Virtual method that can be overridden by subclasses.
     */
    virtual void arm();

    /**
     * @brief Disarms the device to disable its operation.
     * 
     * Virtual method that can be overridden by subclasses.
     */
    virtual void disarm();

    /**
     * @brief Gets the pin assigned to the device.
     * 
     * @return The digital pin number (byte).
     */
    byte getPin() const;

    /**
     * @brief Checks if the device is armed.
     * 
     * @return Boolean indicating the armed state.
     */
    bool isArmed() const;
};

#endif // DEVICE_H