//
// Created by Yasuki Ikeuchi on 15/05/27.
//

#include "DisplayLiFi.h"
#include <Arduino.h>

#ifdef SERIAL_DEBUG_DISPLAY_LIFI
#define DP(x) Serial.print(x)
#define DPLN(x) Serial.println(x)
#else
#define DP(x) (void)(x)
#define DPLN(x) (void)(x)
#endif

void DisplayLiFi::clearReceivedString() {
  for (int i = 0; i < 512; i++) {
    receivedString_[i] = 0;
  }
}

DisplayLiFi::DisplayLiFi(int analogPort) :
    bufferIndex_(0),
    buffer_(0),
    previousTime_(0),
    previousSignal_(0),
    analogPort_(0) {
  analogPort_ = analogPort;
  clearReceivedString();
}

inline boolean isLeaderCode(unsigned long diff) {
  return diff > 400;
}

inline int detectLowHigh(unsigned long diff) {
  return diff > 200 ? 1 : 0;
}

#ifdef SERIAL_DEBUG_DISPLAY_LIFI
void debugPrintLowHigh(int diff) {
  if (diff > 400) {
    DPLN("LEADER");
  } else if (diff > 200) {
    DP("1");
  } else {
    DP("0");
  }
}
#else
inline void debugPrintLowHigh(int diff) { }
#endif

void DisplayLiFi::pushSignal(int signal) {
  buffer_ = (buffer_ << 1) | signal;
  bufferIndex_++;
  if (bufferIndex_ >= 8) {
    receivedString_[receivedIndex_] = buffer_;
    DP(", ");
    DPLN(buffer_); 
    DPLN(receivedString_);
    receivedIndex_++;
    buffer_ = 0;
    bufferIndex_ = 0;
  }
}

char *DisplayLiFi::getReceivedString() {
  return receivedString_;
}

void DisplayLiFi::loop() {
  int val = analogRead(0);
  if (val > 3 && previousSignal_ <= 3) {
    unsigned long now = millis();
    unsigned long diff = now - previousTime_;
    debugPrintLowHigh(diff);
    if (!isLeaderCode(diff)) {
      pushSignal(detectLowHigh(diff));
    } else {
      receivedIndex_ = 0;
      buffer_ = 0;
      bufferIndex_ = 0;
      clearReceivedString();
    }
    previousTime_ = now;
  }
  previousSignal_ = val;
}
