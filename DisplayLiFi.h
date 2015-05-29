#ifndef DISPLAYLIFILIB_DISPLAYLIFI_H
#define DISPLAYLIFILIB_DISPLAYLIFI_H

// #define SERIAL_DEBUG_DISPLAY_LIFI

#if defined(UNITTEST_BUILD) && defined(SERIAL_DEBUG_DISPLAY_LIFI)
#undef SERIAL_DEBUG_DISPLAY_LIFI
#endif

class DisplayLiFi {
  private:
    int analogPort_;
    int previousSignal_;
    unsigned long previousTime_;
    int bufferIndex_;
    char receivedString_[512]; // TODO: reduce memory usage
    int receivedIndex_;
    int buffer_;
    void clearReceivedString();

  public:
    DisplayLiFi(int analogPort);
    void pushSignal(int signal);
    char* getReceivedString();
    void loop();

    int getBuffer() {return buffer_; };
};

#endif //DISPLAYLIFILIB_DISPLAYLIFI_H
