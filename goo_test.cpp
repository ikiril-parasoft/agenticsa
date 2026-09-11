#include <gtest/gtest.h>
#include "goo.h"

TEST(GooTest, TestValueAboveMaxTreshold) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-509");
 
  EXPECT_EQ(computeControl(600, 10), 60);
}

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

TEST(GooTest, ComputeControlDivisorZeroReturnsDefault_ASA513) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(computeControl(600, 0), DEFAULT_VALUE);
}

TEST(GooTest, ComputeControlAtOrBelowLowReturnsDefault_ASA513) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(computeControl(100, 10), DEFAULT_VALUE);
}

TEST(GooTest, ProcessCommandReadReturnsSensor_ASA513) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 123;
  EXPECT_EQ(processCommand("READ", "unused"), 123);
}

TEST(GooTest, ProcessCommandUnknownReturnsError_ASA513) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("PING", "0"), -1);
}

TEST(GooTest, ProcessCommandSetInvalidErrnoCondition_ASA513) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "999999999999999999999999999999999999"), -1);
}

TEST(GooTest, ProcessCommandSetInvalidNoDigitsCondition_ASA513) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "abc"), -1);
}

TEST(GooTest, ProcessCommandSetInvalidTrailingCharsCondition_ASA513) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "12x"), -1);
}

TEST(GooTest, ProcessCommandSetInvalidNegativeCondition_ASA513) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "-1"), -1);
}

TEST(GooTest, ProcessCommandSetInvalidRangeCondition_ASA513) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "70000"), -1);
}

TEST(GooTest, ProcessorCombinesReadAndControl_ASA513) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 600;
  EXPECT_EQ(processor("READ", "unused"), static_cast<int>(SENSOR_DATA) + DEFAULT_VALUE);
}

TEST(GooTest, ProcessorWithLowSensorUsesDefaultControl_ASA513) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 10;
  EXPECT_EQ(processor("READ", "unused"), static_cast<int>(SENSOR_DATA) + DEFAULT_VALUE);
}

TEST(GooTest, ProcessorReturnsControlMinusOneForUnknownCommand_ASA513) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 700;
  EXPECT_EQ(processor("BAD", "0"), DEFAULT_VALUE - 1);
}

