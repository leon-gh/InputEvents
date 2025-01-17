/**
 * A basic example of using the EventJoystick.
 * Note: If there is a button on the joystick, it must be configured 
 * as a plain EventButton.
 * ie this basic joystick example will not fire button events!
 * Unlike the EventEncoderButton, there is no CHANGED_PRESSED for joysticks
 * primarily because it is quite a challenge to physically move the joystick 
 * accurately while pressed.
 */
#include <EventJoystick.h>

const uint8_t analogPin1 = A0;;   //Change to suit your wiring, must be an analog pin
const uint8_t analogPin2 = A1;;   //Change to suit your wiring, must be an analog pin

/**
 * Utility function to print the joystick events to Serial.
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
  case InputEventType::CHANGED_X :
    Serial.print("CHANGED_X");
    break;
  case InputEventType::CHANGED_Y :
    Serial.print("CHANGED_Y");
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
 * EventJoystick& defined as parameters.
 */
void onJoystickEvent(InputEventType et, EventJoystick& ea) {
  Serial.print("onJoystickEvent: ");
  printEvent(et);
  if ( et == InputEventType::CHANGED_X ) {
    Serial.print(", position: ");
    Serial.print(ea.x.position());
  }
  if ( et == InputEventType::CHANGED_Y ) {
    Serial.print(", position: ");
    Serial.print(ea.y.position());
  }
  Serial.println();
}


EventJoystick myJoystick(analogPin1, analogPin2); //Create an EventJoystick

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myJoystick.begin();
  delay(500);
  Serial.println("EventJoystick Basic Example");
  //Initialise both potentiometers at their current position - this is normally
  //done in the constructor but because we are creating the EventJoysting as a
  //global variable, readAnalog is not available in Arduino until setup().
  myJoystick.setStartValues();
  //You can reverse the position of either axis rather than change your wiring.
  //myJoystick.x.reversePosition();
  //By default the joystick is configured with a fairly large central 'dead zone'
  //or it can be far too easy to fire changed events while pressing the joystick button.
  //To make the zone smaller:
  //myJoystick.setCentreBoundary(100); //default is 200
  // See docs for other options
  
  //Link the event(s) to your function
  myJoystick.setCallback(onJoystickEvent);
}

void loop() {
  // You must call update() for every defined EventJoystick.
  // This will update the state of the encoder and 
  // fire the appropriate events.
  myJoystick.update();
}
