/**
   Washing machine delayed start.

   Version 0.1

   Copyright 2016  Xavier Héroult <xavier@placard.fr.eu.org>

   Licensed under the Apache License, Version 2.0 (the "License")
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

const int BUTTON_SET = 5;
const int BUTTON_VALID = 4;
const int RELAY_ON_OFF = 2;
const int RELAY_PLAY_PAUSE = 3;
const int LED_STATUS = 13;

const int BARGRAPH[] = {10, 9, 8, 7, 6};
const int BARGRAPH_SIZE = 5;

int led_state = HIGH;
int clicks = 0;

const unsigned long UNITS = 1000L * 60L * 60L; // Units are 10 secs for debugging
unsigned long total_time = 0;
const int DELAY_LOOP = 500;

const int DEBOUNCE_DELAY = 50;   // the debounce time; increase if the output flickers
int last_states[6]   = {0, 0, 0, 0, 0, 0};
long last_debounces[6] = {0, 0, 0, 0, 0, 0};


void init_bargraph() {
  int i = 0;
  for (i; i < BARGRAPH_SIZE; i++) {
    pinMode(BARGRAPH[i], OUTPUT);
  }
}


void display_bargraph(int value) {
  int i = 0;
  if (value > BARGRAPH_SIZE) {
    value = BARGRAPH_SIZE;
  }

  for (i; i < BARGRAPH_SIZE; i++) {
    if (i < value) {
      digitalWrite(BARGRAPH[i], HIGH);
    } else {
      digitalWrite(BARGRAPH[i], LOW);
    }
  }
}


boolean is_debounce(int button) {
  int current_state = digitalRead(button);
  
  if (last_states[button] != current_state) {
    if (millis() - last_debounces[button] > DEBOUNCE_DELAY) {
      last_debounces[button] = millis();
      last_states[button] = current_state;
      return true;
    }
  }
  return false;
}


boolean is_clicked(int button) {
  if (is_debounce(button) && digitalRead(button) == LOW) {
    
    Serial.print(button);
    Serial.println(" is clicked");
    return true;
  }
  //Serial.println("Is not clicked");
  return false;
}


void ignite() {
  digitalWrite(RELAY_ON_OFF, LOW);
  delay(500);
  digitalWrite(RELAY_ON_OFF, HIGH);
  delay(7000);
  digitalWrite(RELAY_PLAY_PAUSE, LOW);
  delay(500);
  digitalWrite(RELAY_PLAY_PAUSE, HIGH);
  delay(1000);
}


int total_time_to_clicks(unsigned long total_time) {
  unsigned long result = total_time / UNITS;
  return result;
}


void setup() {
  pinMode(BUTTON_SET, INPUT);
  pinMode(BUTTON_VALID, INPUT);
  pinMode(RELAY_ON_OFF, OUTPUT);
  pinMode(RELAY_PLAY_PAUSE, OUTPUT);
  digitalWrite(RELAY_ON_OFF, HIGH);
  digitalWrite(RELAY_PLAY_PAUSE, HIGH);
  pinMode(LED_STATUS, OUTPUT);
  init_bargraph();
  display_bargraph(clicks);
  Serial.begin(9600);
  digitalWrite(LED_STATUS, led_state);

  while (!is_clicked(BUTTON_VALID)) {
    if (is_clicked(BUTTON_SET)) {
      clicks++;
      if (clicks > BARGRAPH_SIZE) {
        clicks = 0;
      }
      Serial.println(clicks);
      display_bargraph(clicks);
    }
  }
  total_time = clicks * UNITS;
}


void loop() {
  while (total_time > 0) {
    digitalWrite(LED_STATUS, led_state);
    led_state = ! led_state;
    total_time -= DELAY_LOOP;
    display_bargraph(total_time_to_clicks(total_time)); 
    delay(DELAY_LOOP);
  }
  ignite();
  while (1) {}
}

