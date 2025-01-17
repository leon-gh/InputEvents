/**
 * A basic example of using the EventAnalog with 12bit ADC resolution (such as an ESP32).
 */
#include <EventAnalog.h>


const uint8_t analogPin = 2; //A0;;   //Change to suit your wiring, must be an analog pin


/**
 * Utility function to print the analog events to Serial.
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
  case InputEventType::CHANGED :
    Serial.print("CHANGED");
    break;
  default:
    Serial.print("Unknown event: ");
    Serial.print((uint8_t)iet);
    break;
  }
}

/**
 * Create a function to handle the events
 * Can be called anything but requires InputEventType and
 * EventAnalog& defined as parameters.
 */
void onAnalogEvent(InputEventType et, EventAnalog& ea) {
  Serial.print("onAnalogEvent: ");
  printEvent(et);
  if ( et == InputEventType::CHANGED ) {
    Serial.print(", position: ");
    Serial.print(ea.position());
  }
  Serial.println();
}


EventAnalog myAnalog(analogPin, 12); // Create an EventAnalog, passing 12 for ESP32 ADC resolution

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myAnalog.begin();
  delay(500);
  Serial.println("EventAnalog Basic Example for 12bit ADC");
  //Optionally initialise at the low end of the potentiometer
  myAnalog.setStartValue(0); //(any value from 0-1024)
  //You can reverse the position rather than change your wiring.
  //myAnalog.setStartValue(1024);
  //myAnalog.reversePosition();
  //Link the event(s) to your function
  myAnalog.setCallback(onAnalogEvent);
}

void loop() {
  // You must call update() for every defined EventAnalog.
  // This will update the state of the encoder and 
  // fire the appropriate events.
  myAnalog.update();
}
