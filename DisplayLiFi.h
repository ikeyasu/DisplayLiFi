#ifndef DISPLAYLIFILIB_DISPLAYLIFI_H
#define DISPLAYLIFILIB_DISPLAYLIFI_H

//#define SERIAL_DEBUG_DISPLAY_LIFI_VERBOSE
//#define SERIAL_DEBUG_DISPLAY_LIFI

#if defined(UNITTEST_BUILD) && defined(SERIAL_DEBUG_DISPLAY_LIFI)
#undef SERIAL_DEBUG_DISPLAY_LIFI
#endif
#if defined(UNITTEST_BUILD) && defined(SERIAL_DEBUG_DISPLAY_LIFI)
#undef SERIAL_DEBUG_DISPLAY_LIFI_VERBOSE
#endif

class DisplayLiFi {
  private:
    typedef void (*LeaderCodeListener)(char* receivedString);
    int analogPort_;
    int previousSignal_;
    unsigned long previousTime_;
    int bufferIndex_;
    char receivedString_[64]; // TODO: reduce memory usage
    int receivedIndex_;
    int buffer_;
    int signalThreshold_;
    LeaderCodeListener leaderCodeListener_;
    void clearReceivedString();
    void init(int analogPort);
    void init(int analogPort, LeaderCodeListener leaderCodeListener1);
    void callLeaderCodeListener();

  public:
    DisplayLiFi(int analogPort);
    DisplayLiFi(int analogPort, LeaderCodeListener leaderCodeListener);
    void setSignalThreshold(int threshold);
    int getSignalThreshold();
    void pushSignal(int signal);
    char* getReceivedString();
    void loop();

    int getBuffer() {
      return buffer_;
    };
};

#endif //DISPLAYLIFILIB_DISPLAYLIFI_H
