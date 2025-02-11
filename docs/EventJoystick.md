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


## API Docs

See EventJoystick's [Doxygen generated API documentation](https://stutchbury.github.io/InputEvents/api/classEventJoystick.html) for more information.


