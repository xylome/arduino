/**
 * Washing machine delayed start.
 * 
 * Version 0.1
 * 
 * Copyright 2016  Xavier Héroult <xavier@placard.fr.eu.org>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License") 
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *  
 *   http://www.apache.org/licenses/LICENSE-2.0
 *   
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

const int buttonSet = 2;    // the number of the pushbutton pin
const int buttonValid = 3;
const int relayOnOff= 4;
const int relayPlayPause = 5;

const int ledPin = 13;      // the number of the LED pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int clicks = 0;

unsigned long units = 1000L * 60L * 60L; //
unsigned long total_time = 0;

int debounceDelay = 50;   // the debounce time; increase if the output flickers
int lastStates[4]   = {0, 0, 0, 0};
long lastDebounce[4] = {0, 0, 0, 0}; 


void setup() {
  pinMode(buttonSet, INPUT);
  pinMode(buttonValid, INPUT);
  pinMode(relayOnOff, OUTPUT);
  pinMode(relayPlayPause, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  digitalWrite(ledPin, ledState);

  while (!is_clicked(buttonValid)) {
    if (is_clicked(buttonSet)) {
      clicks++;
    }
  }
  total_time = clicks*units;
}

boolean is_debounce(int button) {
    int current_state = digitalRead(button);
    /**
    Serial.print(button);
    Serial.print(" - ");
    Serial.print(lastStates[button]);
    Serial.print(" - ");
    Serial.print(current_state);
    Serial.print(" - ");
    Serial.print(lastDebounce[button]);
    Serial.print(" - ");
    Serial.println(millis());
    delay(500);
    **/
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

void ignite() {
  digitalWrite(relayOnOff, HIGH);
  delay(1000);
  digitalWrite(relayOnOff, LOW);
  delay(1000);
  digitalWrite(relayPlayPause, HIGH);
  delay(1000);
  digitalWrite(relayPlayPause, LOW);
  delay(1000);
}

void loop() {
  while(total_time > 0) {
    Serial.println(total_time);
    digitalWrite(ledPin, ledState);
    ledState =! ledState;
    total_time -= 500;
    delay(500);
  }
  ignite();
  while(1) {}
}

