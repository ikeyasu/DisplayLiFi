#ifndef DISPLAYLIFILIB_DISPLAYLIFI_H
#define DISPLAYLIFILIB_DISPLAYLIFI_H

#include <Arduino.h>

#define SERIAL_DEBUG_DISPLAY_LIFI_VERBOSE
#define SERIAL_DEBUG_DISPLAY_LIFI

#if defined(UNITTEST_BUILD) && defined(SERIAL_DEBUG_DISPLAY_LIFI)
#undef SERIAL_DEBUG_DISPLAY_LIFI
#endif
#if defined(UNITTEST_BUILD) && defined(SERIAL_DEBUG_DISPLAY_LIFI)
#undef SERIAL_DEBUG_DISPLAY_LIFI_VERBOSE
#endif

class DisplayLiFi {
  public:
    enum ERROR {
        NONE,
        CALIBRATION,
        INVALID_SIGNAL
    };
    static const int LEADER_CODE_SIGNAL_DURATION = 800;
  private:
    typedef void (*LeaderCodeListener)(char* receivedString);
    int analogPort_;
    int previousSignal_;
    unsigned long previousTime_;
    int bufferIndex_;
    char receivedString_[64]; // TODO: reduce memory usage
    int receivedIndex_;
    int buffer_;
    int signalThresholdHigh_;
    int signalThresholdLow_;
    LeaderCodeListener leaderCodeListener_;
    void clearReceivedString();
    void init(int analogPort);
    void init(int analogPort, LeaderCodeListener leaderCodeListener1);
    void callLeaderCodeListener();

    boolean isCalibration_;
    unsigned long calibrationStartTime_;
    unsigned long calibrationUpdatedTime_;
    int calibrationMax_;
    int calibrationMin_;
    int calibrationMaxRecently_;
    int calibrationMinRecently_;
    int calibrationDurationMSec_;
    enum ERROR error_;
    void setError(enum ERROR err) {
      error_ = err;
    }

  public:
    DisplayLiFi(int analogPort);
    DisplayLiFi(int analogPort, LeaderCodeListener leaderCodeListener);
    void setSignalThresholdHigh(int threshold);
    void setSignalThresholdLow(int threshold);
    int getSignalThresholdHigh();
    int getSignalThresholdLow();
    void pushSignal(int signal);
    char* getReceivedString();
    void startCalibration(int milliseconds);
    boolean loop();

    int getBuffer() {
      return buffer_;
    };

    enum ERROR getError() {
      return error_;
    }
    int detectSignal(int val) const;
};

#endif //DISPLAYLIFILIB_DISPLAYLIFI_H
