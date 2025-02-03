# EventEncoder Class

The [`EventEncoder`](EventEncoder.md) class is for quadrature encoder inputs providing the position & encoder increment, event rate limiting without losing steps (eg for easy acceleration or to reduce events sent over Serial). 

It is effectively an event wrapper around a low level encoder library. By default, Paul Stoffregen's [Encoder library](https://www.pjrc.com/teensy/td_libs_Encoder.html) is used but adapters can easily be created for others (and more will be added).

Huge thanks to Paul - again, I am standing on the shoulders of giants.

![button](../images/mpg.jpg)


## Basic Usage

> Note: Unlike other inputs, `EventEncoder` and `EventEncoderButton` are not directly linked to the pins but through an `EncoderAdapter`. This allows the classes to use different underlying encoder libraries. 
 
You must explicitly `#include` both the chosen encoder library and its adapter and then pass the adapter to the `EventEncoder`:

```cpp
//First include your chosen encoder library
#include <Encoder.h> //PJRC's Encoder library
//Then include the adapter for the encoder library
#include <PjrcEncoderAdapter.h> //Adapter for PJRC's Encoder
//Then include EventEncoder
#include <EventEncoder.h>
//Create an adapter for PJRC's Encoder.
PjrcEncoderAdapter encoderAdapter(2,3); //Should be interrupt pins
//Create an EventEncoder, passing a reference to the adapter
EventEncoder myEncoder(&encoderAdapter);
// Create a callback handler function
void onEncoderEvent(InputEventType et, EventEncoder& ee) {
    Serial.print("Encoder event fired. Position is: ");
    Serial.println(ee.position());
}
void setup() {
    Serial.begin(9600);
    myEncoder.begin();
    // Link the encoder's callback to function defined above
    myEncoder.setCallback(onEncoderEvent);
}
void loop() {
    // Call 'update' for every EventEncoder
    myEncoder.update();
}
```

See [example Encoder.ino](../examples/Encoder/Encoder.ino) for a slightly more detailed sketch.


## Event Types

In addition to the [common events](Common.md#common-events) (Enabled, Disabled and Idle) the following event types are fired by EventEncoder:


#### `InputEventType::CHANGED` 
Will be fired on each change of encoder increment.



## Constructor

Construct an EventEncoder
```cpp
EventEncoder(EncoderAdapter encoderAdapter);
```

## Class Methods

In addition to the [common methods](Common.md#common-methods) the following are available for EventAnalog:

#### `void update()`

Must be called within `loop()`. See [common methods](Common.md#void-update) for details.



### Setup

Setup methods are typically called from within `setup()` but can be updated at runtime.

#### `void setCallback(CallbackFunction func)`

See [common methods](Common.md#void-setcallbackcallbackfunction-func) for details.

----

#### `void setPositionDivider(uint8_t divider=4)`
Quadrature encoders usually have four states for each 'click' of the rotary control, although some have just 2. Normally we only want to fire an event once per click so the default  positionDivider is 4.

You can set this to any positive integer eg 8 would increment the position every 2 clicks. 

Affects pressed+turning too for the [`EventEncoderButton`](EventEncoderButton.md).

----

#### `uint8_t getPositionDivider()`
Get the position divider.

----

#### `void resetPosition(long pos = 0)`
Reset the counted position of the encoder. 


----

#### `void setRateLimit(uint16_t ms)`
Encoder callbacks are normally fired on every loop() but for MPG style encoders this can fire a huge number of events (that may for example, swamp a serial connection).    
The encoder interupts will still be called but this setting will limit the call back firing to every set ms - read the `EventEncoder.increment()` for lossless counting of encoder.   
This is also how you can easily implement acceleration - simply set an appropriate rate limit and then multiply `increment() * increment()` in your callback function. 
Set to zero (default) for no rate limit.



### State

#### `int16_t increment()`
Returns a positive (CW) or negative (CCW) integer. Is normally 1 or -1 but if your loop() has lots of processing, your Arduino is slow or you `setRateLimit()` this will return the actual number of increments made by the encoder since the last encoder handler event.

----

#### `long position()`
The current position of the encoder. Can be reset by `resetPosition()`.


