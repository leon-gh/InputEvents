# EventButton Class

The `EventAnalog` class is for analog inputs - slice an analog range into configurable number of increments. 

Probably the easiest way to implement a button - a single function and three lines of code!

It is effectively an event wrapper for Thomas Fredericks' [Bounce2 library](https://github.com/thomasfredericks/Bounce2) - huge thanks to the Thomas - I am standing on the shoulders of giants.

![button](../images/button.jpg)


## Basic Usage

```cpp
#include <EventButton.h>
// Create a EventButton input.
EventButton myButton(19);
// Create a callback handler function
void onButtonEvent(InputEventType et, EventButton& eb) {
    Serial.print("Button event fired.");
}
void setup() {
  Serial.begin(9600);
  delay(500);
  //Link the event(s) to your function
  myButton.setCallback(onButtonEvent);
}
void loop() {
    // Call 'update' for every EventAnalog
  myButton.update();
}
```
See [example Button.ino](../examples/Button/Button.ino) for a slightly more detailed sketch.


## Event Types

In addition to the [common events](Common.md#common-events) (Enabled, Disabled and Idle) the following event types are fired by EventButton:


#### `InputEventType::PRESSED`
Fired after (as) the button is pressed.

#### `InputEventType::RELEASED`
Fired when the button is released.

#### `InputEventType::CLICKED`
Fired after `RELEASED` if not `LONG_CLICKED`. The method `clickCount()` returns the number of clicks.

#### `InputEventType::LONG_CLICKED`
Fired instead of `CLICKED` after `RELEASED` if pressed duration is greater than `setLongClickDuration()` (default is 750ms).

#### `InputEventType::LONG_PRESS`
Fired once *during* a button press (hence change of tense) after the long click duration. The event will then be fired repeatedly every `setLongPressInterval(ms)` (default 500ms) if `enableLongPressRepeat()` has not been set to `false` (default is `true`).



## Constructor

Construct an EventButton
```cpp
EventButton(byte buttonPin);
```
## Class Methods

In addition to the [common methods](Common.md#common-methods) the following are available for EventButton:


#### `void update()`

Must be called within `loop()`. See [common methods](Common.md#void-update) for details.


### Setup

Setup methods are typically called from within `setup()` but can be updated at runtime.

#### `void setCallback(CallbackFunction func)`

See [common methods](Common.md#void-setcallbackcallbackfunction-func) for details.

#### `void setLongClickDuration(unsigned int longDurationMs=750)`
The number of milliseconds that define a long click duration. The `LONG_PRESS` event will fire after this duration, then repeat at the interval `setLongPressInterval(ms)` (default 500ms) if `enableLongPressRepeat()` is true. After the button is released, a `LONG_CLICK` event will fire.

#### `void enableLongPressRepeat(bool repeat=true)`
Choose to repeat the long press callback (default is 'true')

#### `void setLongPressInterval(unsigned int intervalMs=500)`
Set the time interval in ms between `LONG_PRESS` events *after* the first has fired.

#### `void setMultiClickInterval(unsigned int intervalMs=250)`
Set the interval in ms between double, triple or multi clicks

#### `void setDebounceInterval(unsigned int intervalMs=10)`
Default is set in the Bounce2 library (currently 10ms) - unless you have particularly bad button bounce, no change should be required.


### State

#### `unsigned char clickCount()`
The number of multi-clicks that have been fired in the clicked event. This is reset to zero after `CLICKED` is fired.

#### `bool isPressed()`
Returns true if button is currently pressed

#### `uint8_t longPressCount()`
The number of times the long press event has  occurred in the  button pressed event. This is reset to zero after `LONG_CLICKED` is fired.

#### `unsigned long currentDuration()`
Directly get the duration of the button current state from Bounce2.

#### `unsigned long previousDuration()`
Directly get the duration of the button previous state from Bounce2.

#### `bool buttonState()`
Directly get the current button state from Bounce2.







