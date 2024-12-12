/**
 * A basic example of using the EventButton.
 */
#include <EventButton.h>

const uint8_t buttonPin = 19; //2;  // the number of the pushbutton pin
const uint8_t ledPin = 13;    // the number of the LED pin

/**
 * Utility function to print the button events to Serial.
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
  if ( et == InputEventType::CLICKED ) {
    if ( eb.clickCount() > 1 ) {
      Serial.print(" - ");
      Serial.print(eb.clickCount());
      Serial.print(" click(s)"); 
    }
  }
  Serial.println();
}


EventButton myButton(buttonPin); // Create an EventButton.


void setup() {
  Serial.begin(9600);
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
}
