#include "DisplayLiFi.h"
#include <Wire.h>

DisplayLiFi lifi = DisplayLiFi(0);

void setup() {
  Serial.begin(9600); 
}

void loop() {
  lifi.loop();
}

