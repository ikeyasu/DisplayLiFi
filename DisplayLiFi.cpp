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

#ifdef SERIAL_DEBUG_DISPLAY_LIFI_VERBOSE
#define DP_V(x) Serial.print(x)
#define DPLN_V(x) Serial.println(x)
#else
#define DP_V(x) (void)(x)
#define DPLN_V(x) (void)(x)
#endif

void DisplayLiFi::clearReceivedString() {
  for (int i = 0; i < 64; i++) {
    receivedString_[i] = 0;
  }
}

DisplayLiFi::DisplayLiFi(int analogPort) :
  bufferIndex_(0),
  buffer_(0),
  previousTime_(0),
  previousSignal_(0),
  analogPort_(0),
  signalThreshold_(9) {
  init(analogPort);
  // DO NOT use Serial debug in constructor
  // because Serial may not be initialized.
}

DisplayLiFi::DisplayLiFi(int analogPort, LeaderCodeListener leaderCodeListener) :
  bufferIndex_(0),
  buffer_(0),
  previousTime_(0),
  previousSignal_(0),
  analogPort_(0),
  signalThreshold_(9) {
  init(analogPort, leaderCodeListener);
  // DO NOT use Serial debug in constructor
  // because Serial may not be initialized.
}

void DisplayLiFi::init(int analogPort) {
  init(analogPort, NULL);
}

void DisplayLiFi::init(int analogPort, LeaderCodeListener leaderCodeListener) {
  analogPort_ = analogPort;
  leaderCodeListener_ = leaderCodeListener;
  clearReceivedString();
}

void DisplayLiFi::setSignalThreshold(int threshold) {
  signalThreshold_ = threshold;
}

int DisplayLiFi::getSignalThreshold() {
  return signalThreshold_;
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
    DPLN_V("LEADER");
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
    DP_V(", ");
    DPLN_V(buffer_);
    DPLN_V(receivedString_);
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
  if (val > signalThreshold_ && previousSignal_ <= signalThreshold_) {
    unsigned long now = millis();
    unsigned long diff = now - previousTime_;
    debugPrintLowHigh(diff);
    if (!isLeaderCode(diff)) {
      pushSignal(detectLowHigh(diff));
    } else {
      callLeaderCodeListener();
      DP("LEADER CODE:");
      DPLN(receivedString_);
      receivedIndex_ = 0;
      buffer_ = 0;
      bufferIndex_ = 0;
      clearReceivedString();
    }
    previousTime_ = now;
  }
  previousSignal_ = val;
}

void DisplayLiFi::callLeaderCodeListener() {
  if (leaderCodeListener_ == NULL) {
    return;
  }
  (leaderCodeListener_)(getReceivedString());
}
