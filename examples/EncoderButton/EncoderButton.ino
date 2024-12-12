/**
 * A basic example of using the EventEncoderButton.
 */
#include <Encoder.h> 
#include <EventEncoderButton.h>

const uint8_t encoderPin1 = 2;  //must be in interrupt pin
const uint8_t encoderPin2 = 3;  //should be in interrupt pin
const uint8_t buttonPin = 7;  //change to suit your wiring


/**
 * Utility function to print the encoder button events to Serial.
 * See other examples for other event types
 */
void printEvent(InputEventType iet) {
  switch (iet) {
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
  case InputEventType::CHANGED :
    Serial.print("CHANGED");
    break;
  case InputEventType::CHANGED_PRESSED :
    Serial.print("CHANGED_PRESSED");
    break;
  case InputEventType::CHANGED_RELEASED :
    Serial.print("CHANGED_RELEASED");
    break;
  default:
    Serial.print("Unknown event: ");
    Serial.print((uint8_t)iet);
    break;
  }
}

/**
 * A function to handle the events
 * Can be called anything but requires InputEventType and
 * EventEncoderButton& defined as parameters.
 */
void onEncoderButtonEvent(InputEventType et, EventEncoderButton& eeb) {
  Serial.print("onEncoderButtonEvent: ");
  printEvent(et);
  if ( et == InputEventType::CLICKED ) {
    if ( eeb.clickCount() > 1 ) {
      Serial.print(" - ");
      Serial.print(eeb.clickCount());
      Serial.print(" click(s)"); 
    }
  }
  if ( et == InputEventType::CHANGED ) {
    Serial.print(", increment: ");
    Serial.print(eeb.increment());
    Serial.print(", position: ");
    Serial.print(eeb.position());
  }
  if ( et == InputEventType::CHANGED_PRESSED ) {
    Serial.print(", increment: ");
    Serial.print(eeb.increment());
    Serial.print(", pressed position: ");
    Serial.print(eeb.pressedPosition());
  }
  Serial.println();
}

EventEncoderButton myEncoderButton(encoderPin1, encoderPin2, buttonPin); //Create and EventEncoderButton

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(500);
  Serial.println("EventEncoderButton Basic Example");

  //Link the event(s) to your function
  myEncoderButton.setCallback(onEncoderButtonEvent);
}

void loop() {
  // You must call update() for every defined EventEncoderButton.
  // This will update the state of the encoder button and 
  // fire the appropriate events.
  myEncoderButton.update();
}
