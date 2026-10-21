#include <gtest/gtest.h>
#include "goo.h"

class GooTest : public ::testing::Test {
 protected:
  void SetUp() override {
    SENSOR_DATA = 0;
    MOTOR_SPEED = 0;
    SENSOR_HIGH_THRESHOLD = 500;
    SENSOR_LOW_THRESHOLD = 100;
    DEFAULT_VALUE = 10;
    MULTIPLIER = 2;
  }
};

TEST_F(GooTest, ComputeControlAboveHighThresholdUsesDivision) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-509");

  EXPECT_EQ(computeControl(600, 10), 60);
}

TEST_F(GooTest, ComputeControlAboveHighThresholdDivisorZeroReturnsDefault) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(computeControl(600, 0), 10);
}

TEST_F(GooTest, ComputeControlBetweenThresholdsUsesMultiplier) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-511");

  EXPECT_EQ(computeControl(200, 10), 400);
}

TEST_F(GooTest, ComputeControlAtOrBelowLowThresholdReturnsDefault) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(computeControl(100, 10), 10);
}

TEST_F(GooTest, ProcessCommandSetZeroSetsMotorSpeedAndReturnsZero) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "0"), 0);
  EXPECT_EQ(MOTOR_SPEED, 0);
}

TEST_F(GooTest, ProcessCommandSetRejectsEmptyArgument) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", ""), -1);
}

TEST_F(GooTest, ProcessCommandSetRejectsTrailingCharacters) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "12abc"), -1);
}

TEST_F(GooTest, ProcessCommandSetRejectsNegativeValue) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "-1"), -1);
}

TEST_F(GooTest, ProcessCommandSetRejectsTooLargeValue) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "70000"), -1);
}

TEST_F(GooTest, ProcessCommandReadReturnsSensorValue) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 321;
  EXPECT_EQ(processCommand("READ", "unused"), 321);
}

TEST_F(GooTest, ProcessCommandUnknownReturnsError) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("BAD", "1"), -1);
}

TEST_F(GooTest, ProcessorCombinesCommandResultAndControl) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 600;
  EXPECT_EQ(processor("SET", "5"), 15);
  EXPECT_EQ(MOTOR_SPEED, 5);
}

TEST_F(GooTest, ProcessorHandlesReadCommandPath) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 50;
  EXPECT_EQ(processor("READ", "unused"), 60);
}
