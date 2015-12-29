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

DisplayLiFi::DisplayLiFi(int analogPort) {
  init(analogPort);
  // DO NOT use Serial debug in constructor
  // because Serial may not be initialized.
}

DisplayLiFi::DisplayLiFi(int analogPort, LeaderCodeListener leaderCodeListener) {
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
  signalThresholdHigh_ = 700;
  signalThresholdLow_ = 100;
  bufferIndex_ = 0;
  buffer_ = 0;
  previousTime_ = 0;
  previousSignal_ = 0;
  isCalibration_ = false;
  calibrationStartTime_ = 0;
  calibrationMax_ = 0;
  calibrationMin_ = 1024;
  calibrationDurationMSec_ = 0;
  clearReceivedString();
}

void DisplayLiFi::setSignalThresholdHigh(int threshold) {
  signalThresholdHigh_ = threshold;
}

void DisplayLiFi::setSignalThresholdLow(int threshold) {
  signalThresholdLow_ = threshold;
}

int DisplayLiFi::getSignalThresholdHigh() {
  return signalThresholdHigh_;
}

int DisplayLiFi::getSignalThresholdLow() {
  return signalThresholdLow_;
}

inline boolean isLeaderCode(unsigned long diff) {
  return diff > 800;
}

inline int detectLowHigh(unsigned long diff) {
  return diff > 200 ? 1 : 0;
}

#ifdef SERIAL_DEBUG_DISPLAY_LIFI
void debugPrintLowHigh(int diff) {
  if (isLeaderCode(diff)) {
    DPLN_V("LEADER");
  } else {
    DP(detectLowHigh(diff));
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

void DisplayLiFi::startCalibration(int milliseconds) {
  isCalibration_ = true;
  calibrationStartTime_ = millis();
  calibrationDurationMSec_ = milliseconds;
  DPLN("CALIBRATION Start");
}

void DisplayLiFi::loop() {
  int val = analogRead(analogPort_);
  if (isCalibration_) {
    calibrationMax_ = max(calibrationMax_, val);
    calibrationMin_ = min(calibrationMin_, val);
    if (millis() - calibrationStartTime_ >= calibrationDurationMSec_) {
      isCalibration_ = false;
      int thresholdHigh = calibrationMax_ * 0.8;
      int thresholdLow = (calibrationMin_ == 0) ? thresholdHigh / 3 : calibrationMin_ * 1.2;
      DPLN("CALIBRATION DONE");
      DP("Max="); DP(calibrationMax_);
      DP(" Min="); DPLN(calibrationMin_);
      DP("ThresholdHigh="); DP(thresholdHigh);
      DP(" ThresholdLow="); DPLN(thresholdLow);
      if (thresholdLow >= thresholdHigh) {
        DPLN("CALIBRATION ERROR");
        // calibration is canceled
        return;
      }
      setSignalThresholdHigh(thresholdHigh);
      setSignalThresholdLow(thresholdLow);
    }
    return;
  }

  int signal = detectSignal(val);
  if (signal < 0) return;

  if (signal == 1 && previousSignal_ == 0) {
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
  previousSignal_ = signal;
}

int DisplayLiFi::detectSignal(int val) const {
  if (val > signalThresholdHigh_) {
    return 1;
  } else if (val < signalThresholdLow_) {
    return 0;
  }
  return -1;
}

void DisplayLiFi::callLeaderCodeListener() {
  if (leaderCodeListener_ == NULL) {
    return;
  }
  (leaderCodeListener_)(receivedString_);
}
