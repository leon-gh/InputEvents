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

## API Docs

See EventAnalog's [Doxygen generated API documentation](https://stutchbury.github.io/InputEvents/api/classEventAnalog.html) for more information.

