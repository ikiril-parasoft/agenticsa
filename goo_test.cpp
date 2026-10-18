#include <gtest/gtest.h>
#include <string>
#include "goo.h"

// 
TEST(GooTest, TestValueAboveMaxTreshold) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-509");
 
  EXPECT_EQ(computeControl(600, 10), 60);
}

// 
TEST(GooTest, TestGetValueBelowMaxTreshold) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-511");

  EXPECT_EQ(computeControl(200, 10), 400);
}

TEST(GooTest, SetMotorSpeedZero) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-511");

  EXPECT_EQ(processCommand("SET", "0"), 0);
  EXPECT_EQ(MOTOR_SPEED, 0);
}

TEST(GooTest, Asa513ComputeControlAtLowThresholdReturnsDefault) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(computeControl(100, 10), DEFAULT_VALUE);
}

TEST(GooTest, Asa513ProcessCommandReadReturnsSensorValue) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 321;
  EXPECT_EQ(processCommand("READ", "ignored"), 321);
}

TEST(GooTest, Asa513ProcessCommandUnknownReturnsError) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("NOOP", "ignored"), -1);
}

TEST(GooTest, Asa513SetRejectsEmptyString) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", ""), -1);
}

TEST(GooTest, Asa513SetRejectsTrailingCharacters) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "1x"), -1);
}

TEST(GooTest, Asa513SetRejectsOverflowWithErrno) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "999999999999999999999999999999"), -1);
}

TEST(GooTest, Asa513SetRejectsNegativeValue) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "-1"), -1);
}

TEST(GooTest, Asa513SetRejectsAboveUint16Range) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "65536"), -1);
}

TEST(GooTest, Asa513SetRejectsAboveIntRange) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "3000000000"), -1);
}

TEST(GooTest, Asa513SetAcceptsUint16MaxValue) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "65535"), 65535);
  EXPECT_EQ(MOTOR_SPEED, 65535);
}

TEST(GooTest, Asa513ProcessorAddsCommandResultAndControl) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 200;
  EXPECT_EQ(processor("READ", "ignored"), 600);
}
