# EventJoystick Class

The [`EventJoystick`](EventJoystick.md) class contains two [`EventAnalog`](EventAnalog.md) inputs configured as a joystick.

![button](../images/thumb-joystick.jpg)

## Basic Usage

```cpp
#include <EventJoystick.h>
// Create an EventJoystick input
EventJoystick myJoystick(A0, A1);
// Create a callback handler function
void onJoystickEvent(InputEventType et, EventJoystick& ej) {
    Serial.print("Joystick event fired. X position is: ");
    Serial.print(ej.x.position());
    Serial.print(" Y position is: ");
    Serial.println(ej.y.position());
}
void setup() {
    Serial.begin(9600);
    myJoystick.begin();
    // Link the joystick's callback to function defined above
    myJoystick.setCallback(onJoystickEvent);
    myJoystick.setStartValues();
}
void loop() {
    // Call 'update' for every EventJoystick
    myJoystick.update();
}
```

See [example Joystick.ino](../examples/Joystick/Joystick.ino) for a slightly more detailed sketch.


## Event Types

In addition to the [common events](Common.md#common-events) (Enabled, Disabled and Idle) the following event types are fired by EventJoystick:


----

#### `InputEventType::CHANGED_X` 
Will be fired on each change of increment on the X axis.

----

#### `InputEventType::CHANGED_Y` 
Will be fired on each change of increment on the Y axis.


## Constructor

Construct an EventJoystick
```cpp
EventJoystick(byte analogPinX, byte analogPinY);
```

The `analogPinX` and `analogPinY` parameters *must* be an analog pins. For ESP32 avoid using pins attached to ADC2 (GPIO 0, 2, 4, 12-15, 25-27) as these are shared by the WiFi module.

For most boards this constructor will work fine but if your board has an ADC (analog to digital converter) resolution that is higher than the standard Arduino 10 bits, pass the resolution (in bits) of your board to the contructor:

```cpp
EventJoystick(byte analogPinX, byte analogPinY, uint8_t adcBits);
```
The Arduino Due, Zero, MKR; ESP32, ARM, SAMD & STM32 based boards all use 12 bit ADCs. Teensy boards have 12 bit ADCs but default to 10 bit.

On boards with an ADC greater than 10 bits, the function [`analogReadResolution(bits)`](https://docs.arduino.cc/language-reference/en/functions/analog-io/analogReadResolution/) can be used to change the ADC resolution.

## Class Methods

#### `void update()`

Must be called within `loop()`. See [common methods](Common.md#void-update) for details.


### Setup

Setup methods are typically called from within `setup()` but can also be updated at runtime.

Only `begin()` and `setCallback()` are required methods.

In most cases the methods below are not required but provide a range of options to change the configuration to suit your hardware and functional requirements.

In addition to the [common methods](Common.md#common-methods) the following are available for EventJoystick:

----

#### `void setCallback(CallbackFunction func)`

See [common methods](Common.md#void-setcallbackcallbackfunction-func) for details.

----

#### `void setStartValues()`
Set the underlying analog values for x & y that equates to the zero position - read from current position.

----

#### `void setNumIncrements(uint8_t numIncr=10)`
Split the analog range of both axis into this number of slices. A changed callback will be fire each time the increment changes. You can also set increments on individial axis after this has been called.

----

#### `void setNumNegativeIncrements(uint8_t numIncr=10)`
Normally increments are set with setNumIncrements but you can also set the negative and positive sides individually.

----

#### `void setNumPositiveIncrements(uint8_t numIncr=10)`
Normally increments are set with setNumIncrements but you can also set the negative and positive sides individually.

----

#### `void setCentreBoundary(uint16_t width=200)`
This sets a 'dead zone' in the centre of the joystick - very useful if the joystick has a button which is often a challenge to press without triggering a changed event.


----

#### `void setOuterBoundary(uint16_t width=100)`
Set the outer boundary of the joystick. This is useful to 'trim' the corners of the joystick so you get the maximum position in a circumference rather than in the corners.


----

#### `void enableAutoCalibrate(bool allow=true)`
When enableAutoCalibrate is set to true (the default), will auto calibrate minValue and maxValue. This will be done even when input is disabled (ie no callbacks fired). Apllied to both x and y axis.

----

All the [`EventAnalog` setup](EventAnalog.md#setup) methods are available for each axis via `myJoystick.x` and `myJoystick.y`.

### State

#### `bool hasChanged()`
Returns `true` if either X or Y position has changed since previous `update()`.

----

All the [`EventAnalog` state](EventAnalog.md#state) methods are available for each axis via `myJoystick.x` and `myJoystick.y`.
