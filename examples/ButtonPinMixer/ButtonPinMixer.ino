/**
 * An example of using the EventButton with a PinMixerAdapter.
 * 
 * It will automatically enable debouncing.
 *
 * As with the basic EventButton example, when the button is pressed the 
 * inbuilt LED on pin 13 will turn on, when released will turn off.
 * 
 * By using the standard GpioPinAdapter and the VirtualPinAdapter we can 
 * mix them with the PinMixerAdapter so every 10 seconds, the button will 
 * be virtually pressed and then released 2 seconds later, lighting the LED 
 * and firing PRESSED, 3 x LONG_PRESS, RELEASED and LONG_CLICKED events;
 *
 * Either source PinAdapter can interrupt the other and the GpioPinAdapter will
 * act exactly like a normal GPIO pin.
 *
 */
#include <EventButton.h>
#include "PinAdapter/GpioPinAdapter.h"
#include "PinAdapter/VirtualPinAdapter.h"
#include "PinAdapter/PinMixerAdapter.h"


const uint8_t buttonPin = 2;  // the number of the pushbutton pin
const uint8_t ledPin = 13;    // the number of the LED pin

/**
 * Utility function to print the button events to Serial.
 * You don't need this - it's just for the example.
 * See other examples for other event types
 */
void printEvent(InputEventType et) {
  switch (et) {
  case InputEventType::ENABLED :
    Serial.print("ENABLED");
    break;
  case InputEventType::DISABLED :
    Serial.print("DISABLED");
    break;
  case InputEventType::IDLE :
    Serial.print("IDLE");
    break;
  case InputEventType::PRESSED :
    Serial.print("PRESSED");
    break;
  case InputEventType::RELEASED :
    Serial.print("RELEASED");
    break;
  case InputEventType::CLICKED :
    Serial.print("CLICKED");
    break;
  case InputEventType::DOUBLE_CLICKED :
    Serial.print("DOUBLE_CLICKED");
    break;
  case InputEventType::MULTI_CLICKED :
    Serial.print("MULTI_CLICKED");
    break;
  case InputEventType::LONG_CLICKED :
    Serial.print("LONG_CLICKED");
    break;
  case InputEventType::LONG_PRESS :
    Serial.print("LONG_PRESS");
    break;
  default:
    Serial.print("Unknown event: ");
    Serial.print((uint8_t)et);
    break;
  }
}

/**
 * A function to handle the events
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
  Serial.print("onButtonEvent: ");
  printEvent(et); //Call the utility event to print the event type
  if ( et == InputEventType::MULTI_CLICKED ) {
    Serial.print(" - ");
    Serial.print(eb.clickCount());
    Serial.print(" clicks"); 
  }
  Serial.println();
}

VirtualPinAdapter virtualPin; //Create gloablly so we can access it
//Create an EventPutton with a mixed GPIO pin and a virtual pin
EventButton myButton(new PinMixerAdapter(new GpioPinAdapter(buttonPin), &virtualPin));

uint32_t lastVirtualPressMs = 0; //Timer for example
bool pressed = false; //we have virtually pressed

void setup() {
  Serial.begin(9600);
  myButton.begin();
  delay(500);
  Serial.println("EventButton Basic Example");
  pinMode(ledPin, OUTPUT);
  //Link the event(s) to your function (the callback handler)
  myButton.setCallback(onButtonEvent);
}

void loop() {
  // You must call update() for every defined EventButton.
  // This will update the state of the button and 
  // fire the appropriate events.
  myButton.update();

  uint32_t now = millis();
  if ( now > lastVirtualPressMs + 10000 ) {
    lastVirtualPressMs = now;
    virtualPin.press();
    pressed = true;
  }
  if ( pressed && now > lastVirtualPressMs + 2000 ) {
    virtualPin.release();
    pressed = false;
  }
}
