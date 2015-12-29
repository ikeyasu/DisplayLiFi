#include "DisplayLiFi.h"

#define LED 4

void leaderCodeListener(char* receivedString) {
  Serial.print("L:");
  Serial.println(receivedString);
}

DisplayLiFi lifi = DisplayLiFi(A0, leaderCodeListener);

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  delay(100);
  Serial.println("");
  Serial.println("TEST");
  lifi.startCalibration(5000);
}

void loop() {
  lifi.loop();
  //delay(3);
}
