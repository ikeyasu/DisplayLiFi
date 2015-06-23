// Created by Yasuki Ikeuchi on 15/05/27.

#include "gtest/gtest.h"
#include "../DisplayLiFi.h"
#include <Arduino.h>

using ::testing::Return;
using ::testing::InSequence;

int expectedNumberOfCalled = 0;

TEST(pushSignal, normal) {
  ArduinoMock* arduinoMock = arduinoMockInstance();
  DisplayLiFi *lifi = new DisplayLiFi(0);
  lifi->pushSignal(0);
  lifi->pushSignal(1);
  lifi->pushSignal(1);
  lifi->pushSignal(0);
  lifi->pushSignal(0);
  lifi->pushSignal(0);
  lifi->pushSignal(0);
  lifi->pushSignal(1);
  char *string = lifi->getReceivedString();
  ASSERT_EQ('a', string[0]);
  delete lifi;
  releaseArduinoMock();
}

TEST(loop, normal_once) {
  ArduinoMock* arduinoMock = arduinoMockInstance();
  EXPECT_CALL(*arduinoMock, analogRead(0)).WillOnce(Return(10));
  EXPECT_CALL(*arduinoMock, millis()).WillOnce(Return(0));
  DisplayLiFi *lifi = new DisplayLiFi(0);
  lifi->loop();
  delete lifi;
  releaseArduinoMock();
}

TEST(loop, normal_one_signal) {
  InSequence dummy;
  ArduinoMock* arduinoMock = arduinoMockInstance();
  DisplayLiFi *lifi = new DisplayLiFi(0);

  EXPECT_CALL(*arduinoMock, analogRead(0)).WillOnce(Return(10));
  EXPECT_CALL(*arduinoMock, millis()).WillOnce(Return(0));
  lifi->loop();

  EXPECT_CALL(*arduinoMock, analogRead(0)).WillOnce(Return(0));
  lifi->loop();

  EXPECT_CALL(*arduinoMock, analogRead(0)).WillOnce(Return(10));
  EXPECT_CALL(*arduinoMock, millis()).WillOnce(Return(360));
  lifi->loop();

  ASSERT_EQ(1, lifi->getBuffer());

  delete lifi;
  releaseArduinoMock();
}

void leaderCodeListener(char* receivedString) {
  expectedNumberOfCalled--;
}

TEST(loop, leader_code) {
  InSequence dummy;
  ArduinoMock* arduinoMock = arduinoMockInstance();
  DisplayLiFi *lifi = new DisplayLiFi(0, leaderCodeListener);

  expectedNumberOfCalled = 1;

  EXPECT_CALL(*arduinoMock, analogRead(0)).WillOnce(Return(10));
  EXPECT_CALL(*arduinoMock, millis()).WillOnce(Return(0));
  lifi->loop();

  EXPECT_CALL(*arduinoMock, analogRead(0)).WillOnce(Return(0));
  lifi->loop();

  EXPECT_CALL(*arduinoMock, analogRead(0)).WillOnce(Return(10));
  EXPECT_CALL(*arduinoMock, millis()).WillOnce(Return(600));
  lifi->loop();

  ASSERT_EQ(0, lifi->getBuffer());
  ASSERT_EQ(0, expectedNumberOfCalled);

  delete lifi;
  releaseArduinoMock();
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
