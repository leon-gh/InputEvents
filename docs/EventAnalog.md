# EventAnalog Class

The [`EventAnalog`](EventAnalog.md) class is for analog inputs - slice an analog range into configurable number of increments. 

For many uses of an analog input, the 1024 'slices' in the standard 10 bit analog range are more than is necessary. For 12 bit (4096), 14 bit (16384) and 16 bit (65536!)), even more so and with those higher numbers comes greate issues with noise, often resultining in a continuous fluctuation of values. 

This library allows you to reduce those 1024 (or up to 65536!) slices to a more managable number, calling a handler function or method each time a defined slice increments up or down.

This approach also provides very effective noise reduction.

![button](../images/potentiometer.png) ![button](../images/pcb-potentiometer.png)


## Basic Usage


```cpp
#include <EventAnalog.h>
// Create an EventAnalog input
EventAnalog myAnalog(A0);
// Create a callback handler function
void onAnalogEvent(InputEventType et, EventAnalog& ea) {
    Serial.print("Analog event fired. Position is: ");
    Serial.println(ea.position());
}
void setup() {
    Serial.begin(9600);
    myAnalog.begin();
    // Link the button's callback to function defined above
    myAnalog.setCallback(onAnalogEvent);
}
void loop() {
    // Call 'update' for every EventAnalog
    myAnalog.update();
}
```

See [example Analog.ino](../examples/Analog/Analog.ino) for a slightly more detailed sketch.


## Event Types

In addition to the [common events](Common.md#common-events) (Enabled, Disabled and Idle) the following event types are fired by EventAnalog:


#### `InputEventType::CHANGED` 
Will be fired on each change of increment.



## Constructor

Construct an EventAnalog
```cpp
EventAnalog(byte analogPin);
```
The `analogPin` parameter *must* be an analog pin. For ESP32 avoid using pins attached to ADC2 (GPIO 0, 2, 4, 12-15, 25-27) as these are shared by the WiFi module.

For most boards this constructor will work fine but if your board has an ADC (analog to digital converter) resolution that is higher than the standard Arduino 10 bits, pass the resolution (in bits) of your board to the contructor:

```cpp
EventAnalog(byte analogPin, uint8_t adcBits);
```
The Arduino Due, Zero, MKR; ESP32, ARM, SAMD & STM32 based boards all use 12 bit ADCs. Teensy boards have 12 bit ADCs but default to 10 bit.

On boards with an ADC greater than 10 bits, the function [`analogReadResolution(bits)`](https://docs.arduino.cc/language-reference/en/functions/analog-io/analogReadResolution/) can be used to change the ADC resolution.

## Class Methods

In addition to the [common methods](Common.md#common-methods) the following are available for EventAnalog:

#### `void update()`

Must be called within `loop()`. See [common methods](Common.md#void-update) for details.


### Setup

Setup methods are typically called from within `setup()` but can also be updated at runtime.

Only `begin()` and `setCallback()` are required methods.

In most cases the methods below are not required but provide a range of options to change the configuration to suit your hardware and functional requirements.

In addition to the [common methods](Common.md#common-methods) the following are available for EventAnalog:


#### `void setCallback(CallbackFunction func)`

See [common methods](Common.md#void-setcallbackcallbackfunction-func) for details.

----

#### `void setNumIncrements(uint8_t)`

Split the analog range into this number of slices. Note: This library is intended to reduce the usual 0-1024 range of `analogRead()` to a much smaller, more manageable number of 'slices'. The default is 25. Much higher numbers may produce variable results as a slice width is the ADC resolution (1024) divided by the number of increments. 

An `InputEventType::CHANGED` event will be fired each time the increment changes. 

----

#### `void setNumNegativeIncrements(uint8_t)` </br>`void setNumPositiveIncrements(uint8_t)`
Normally increments are set with `setNumIncrements` but you can also set the negative and positive sides of 'centre normal' individually.

----

#### `void setStartValue(uint16_t)`
Set the *analog* value for the 'starting' position. For normal potentiometers this would be 0 (the default) but for joysticks it would be the centre position of the joystick's potentionmeter (normally 512).

#### `void setStartValue()`
Will set the start value as the current position. Useful for joysticks that 'rest' in the centre but can also be called from a button push to change the behaviour of an analof input.

----

#### `void setMinValue(uint16_t val)`
By default the minValue is set to 5% of the ADC range and auto calibrated as the input is used. 
You ccan manually set it here if you have confidence in your potentiometer range.


#### `void setMaxValue(uint16_t val)`
By default the maxValue is set to 95% of the ADC range and auto calibrated as the input is used. 
You ccan manually set it here if you have confidence in your potentiometer range.

----
#### Boundaries

Boundaries set an area at the start or end of travel that is ignored. Some potentiometers are very inconsistent at either end and for joysticks it is impossible to reach min/max 'on the diagonal'.

#### `void setStartBoundary(uint16_t)`
Used primarily for joysticks - it is very difficult to press the joystick button without moving the stick so with this we can create a central 'deadzone'. Parameter is the analog value, not increment position. 

#### `void setEndBoundary(uint16_t)`
Useful if your potentiometer is a bit inconsistent at the higer level (eg, sometimes you can't hit the highest increment).
Also used for joysticks to create an outer 'deadzone' where joysticks are notoriously inconsistent. Parameter is the analog value, not the increment position.

----

#### `void setRateLimit(uint16_t ms)`
The EventAnalog callbacks are normally fired every time the position changes but to limit the number of events fired when the potentiometer is moved quickly, you can set a rate limit here. Note: A high rate limit may reduce responsiveness.

----

#### `void reversePosition(bool rev=true)`
If your position is coming out backawards (negative), you can set this rather that rewire your input.

#### `bool isPositionReversed()`
Returns true if position is reversed.

----

#### `void enableAutoCalibrate(bool allow=true)`
When enableAutoCalibrate is set to true (the default), will auto calibrate minValue and maxValue. This will be done even when input is disabled (ie no callbacks fired)

----

### State

#### `int16_t position()`
Returns the current position - this is not the analog value but the mapped position of the increments you have defined. Can be negative if `setStartValue` is greater than the minimum position (eg for joysticks).

#### `int16_t previousPosition()`
Returns the previous position - this is not the analog value but the mapped position of the increments you have defined.

----

#### `bool hasChanged()`
Returns `true` if position has changed since previous `update()`.

