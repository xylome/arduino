/*
 Debounce

 Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
 press), the output pin is toggled from LOW to HIGH or HIGH to LOW.  There's
 a minimum delay between toggles to debounce the circuit (i.e. to ignore
 noise).

 The circuit:
 * LED attached from pin 13 to ground
 * pushbutton attached from pin 2 to +5V
 * 10K resistor attached from pin 2 to ground

 * Note: On most Arduino boards, there is already an LED on the board
 connected to pin 13, so you don't need any extra components for this example.


 created 21 November 2006
 by David A. Mellis
 modified 30 Aug 2011
 by Limor Fried
 modified 28 Dec 2012
 by Mike Walters

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Debounce
 */

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;//50;    // the debounce time; increase if the output flickers

int lastStates[4]   = {0, 0, 0, 0};
int lastDebounce[4] = {0, 0, 0, 0}; 


void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  // set initial LED state
  digitalWrite(ledPin, ledState);
}

boolean is_debounce(int button) {
    int current_state = digitalRead(button);
    if (lastStates[button] != current_state) {
      if (millis() - lastDebounce[button] > debounceDelay) {
        //Serial.println(" -= Debounce =-");
        lastDebounce[button] = millis();
        lastStates[button] = current_state;
        return true;
      } 
    }
    //Serial.println(" -= No debounce =- ");
    return false;
}

boolean is_clicked(int button) {
  if (is_debounce(button) && digitalRead(button) == LOW) {
    //Serial.println("Is clicked");
    return true;
  }
  //Serial.println("Is not clicked");
  return false;
}

void loop() {
  // read the state of the switch into a local variable:
 
    if (is_clicked(buttonPin)) {
      ledState = !ledState;
    }
  
  //delay(250);
  
  digitalWrite(ledPin, ledState);
}

