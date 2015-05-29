// Created by Yasuki Ikeuchi on 15/05/27.

#include "gtest/gtest.h"
#include "../DisplayLiFi.h"
#include <Arduino.h>

using ::testing::Return;
using ::testing::InSequence;

TEST(pushSignal, normal) {
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
}

TEST(loop, normal_once) {
  DisplayLiFi *lifi = new DisplayLiFi(0);
  ArduinoMock* arduinoMock = arduinoMockInstance();
  EXPECT_CALL(*arduinoMock, analogRead(0)).WillOnce(Return(10));
  EXPECT_CALL(*arduinoMock, millis()).WillOnce(Return(0));
  lifi->loop();
  releaseArduinoMock();
}

TEST(loop, normal_one_signal) {
  InSequence dummy;
  DisplayLiFi *lifi = new DisplayLiFi(0);
  ArduinoMock* arduinoMock = arduinoMockInstance();

  EXPECT_CALL(*arduinoMock, analogRead(0)).WillOnce(Return(10));
  EXPECT_CALL(*arduinoMock, millis()).WillOnce(Return(0));
  lifi->loop();

  EXPECT_CALL(*arduinoMock, analogRead(0)).WillOnce(Return(0));
  lifi->loop();

  EXPECT_CALL(*arduinoMock, analogRead(0)).WillOnce(Return(10));
  EXPECT_CALL(*arduinoMock, millis()).WillOnce(Return(360));
  lifi->loop();

  ASSERT_EQ(1, lifi->getBuffer());

  releaseArduinoMock();
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
