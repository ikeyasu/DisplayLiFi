#include "DisplayLiFi.h"

#define LED 4

void leaderCodeListener(char* receivedString) {
  Serial.println(receivedString);
}

DisplayLiFi lifi = DisplayLiFi(0, leaderCodeListener);

void setup() {
  Serial.begin(9600);
  delay(100);
}

void loop() {
  lifi.loop();
}

