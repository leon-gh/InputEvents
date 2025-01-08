/**
 * A basic example of using the EventSwitch.
 */
#include <EventSwitch.h>

const uint8_t switchPin = 2;   //Change to suit your wiring

/**
 * Utility function to print the switch events to Serial.
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
  case InputEventType::ON :
    Serial.print("ON");
    break;
  case InputEventType::OFF :
    Serial.print("OFF");
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
 * EventEncoder& defined as parameters.
 */
void onSwitchEvent(InputEventType et, EventSwitch& es) {
  Serial.print("onSwitchEvent: ");
  printEvent(et);
  Serial.println();
}


EventSwitch mySwitch(switchPin); //Create an EventSwitch

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySwitch.begin();
  delay(500);
  Serial.println("EventSwitch Basic Example");

  Serial.print("digitalRead is: ");
  Serial.print(HIGH == digitalRead(switchPin) ? "HIGH" : "LOW");
  Serial.print(" at startup.");
  Serial.print(" so this means the switch is: ");
  Serial.println(mySwitch.isOn() ? "ON" : "OFF");

  //You can reverse the on/off events rather than change your wiring.
  //mySwitch.reverseOnOff();

  //Link the event(s) to your function
  mySwitch.setCallback(onSwitchEvent);
}

void loop() {
  // You must call update() for every defined EventSwitch.
  // This will update the state of the switch and 
  // fire the appropriate events.
  mySwitch.update();
}
