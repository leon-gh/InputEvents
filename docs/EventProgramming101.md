# Event Programming 101

In traditional Arduino programming, all the action often takes place in the `loop()` function. Sure, you may explicitly call a function or two but the bulk of the logic ends up directly in the `loop()`.

The basic concept of [event programming](https://en.wikipedia.org/wiki/Event_(computing)) is to only execute certain code (usually functions or class methods) when certain events are triggered or 'fired'. 

In traditional PC or server programming, those events would likely be fired from a background processs and the main process would then handle those events via a 'callback handler` (a fancy name for a function that does something when an event happens). 

In more complex systems you might have multiple functions/methods (in speparate threads) 'listening' for events and taking action (handling them). We don't do that. Strictly one callback per input, although multiple inputs can share a common callback function/method (see HAL example).

For microcontrollers we don't usually have the luxury of multiple processes/threads, so the `InputEvents` events are 'blocking'. This simply means they 'block' the execution of the next instruction until the current callback handler has finished its business. This has two side effects:
1. The 'state' of the input remains consistent while the callback is handled. Tthis is a Good Thing&trade;
2. Other events are not fired until the current event has finished. This is not such a good thing and means the order of input `update()` calls in `loop()` can have an effect.

Speaking of `update()` calls in `loop()` - we could have chosen to use pin 'interupts' to fire events but many microcontrollers have a limited number of interrupt pins (the UNO only has two!). But we're getting into implementation weeds here, so suffice to say, events are fired from within the `loop()` function (although the encoder uses interupts to enable its 'lossless-ness', the events themselves are still fired from within `loop()`).


Here is the traditional [Button example](https://www.arduino.cc/en/Tutorial/BuiltInExamples/Button) from the Arduino IDE:

---

```cpp
// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}
void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}
```
---
And this is the exact same functionality using event programming:

```cpp
const uint8_t buttonPin = 19; //2;  // the number of the pushbutton pin
const uint8_t ledPin = 13;    // the number of the LED pin

EventButton myButton(buttonPin); // Create an EventButton.

/**
 * A function to handle the button events
 * Can be called anything but requires InputEventType and
 * EventButton& defined as parameters.
 */
void onButtonEvent(InputEventType et, EventButton& eb) {
  if ( et == InputEventType::PRESSED ) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else if ( et == InputEventType::RELEASED ) {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}
void setup() {
  pinMode(ledPin, OUTPUT);
  //Link the event(s) to your function (the callback handler)
  myButton.setCallback(onButtonEvent);
}
void loop() {
  myButton.update();
}
```
---

Now, at first glance, you might not think this is so much better or easier - after all, it is pretty much the same amount of code. But look at the `loop()` - just one instruction related to the button. There might be many inputs here or quite complex logic for your other stuff but it is not polluted with your button's affairs. All that is now encapsulated in the function `onButtonEvent()`.

Say we want to change the behaviour of the LED from on/of on pressed and released to toggled by the button? We just update the `onButtonEvent()` function:

---
```cpp
void onButtonEvent(InputEventType et, EventButton& eb) {
  if ( et == InputEventType::CLICKED ) {
    // toggle the LED:
    bool ledState = digitalRead(ledPin);
    digitalWrite(ledPin, !ledState);
  }
}
```
---
No changes to your `loop()` logic.

An event handler, aka a 'callback' (in this example, the `onButtonEvent()`) can choose to act on any or all events (one at a time!) that may be fired by the input(s). It could call one function on [`LONG_PRESS`](EventButton.md#inputeventtypelong_press) and a different one on [`LONG_CLICKED`](EventButton.md#inputeventtypelong_clicked) (see [`EventButton` docs](EventButton.md#event-types) for why these two event names have different tenses).

Long press might increment a counter and then long click might take action on the value of that counter.

Hopefully this 101 has shown how flexible, powerful and *easy* it is to use events to build and maintain good code.