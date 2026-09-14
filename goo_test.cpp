#include <gtest/gtest.h>
#include "goo.h"

class GooTest : public ::testing::Test {
 protected:
  void SetUp() override {
    SENSOR_DATA = 0U;
    MOTOR_SPEED = 0U;
    SENSOR_HIGH_THRESHOLD = 500U;
    SENSOR_LOW_THRESHOLD = 100U;
    DEFAULT_VALUE = 10U;
    MULTIPLIER = 2U;
  }
};

TEST_F(GooTest, TestValueAboveMaxTreshold) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-509");

  EXPECT_EQ(computeControl(600, 10), 60);
}

TEST_F(GooTest, TestGetValueBelowMaxTreshold) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-511");

  EXPECT_EQ(computeControl(200, 10), 400);
}

TEST_F(GooTest, SetMotorSpeedZero) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-511");

  EXPECT_EQ(processCommand("SET", "0"), 0);
  EXPECT_EQ(MOTOR_SPEED, 0U);
}

TEST_F(GooTest, ComputeControlHighThresholdAndZeroDivisorUsesDefault) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(computeControl(700, 0), 10);
}

TEST_F(GooTest, ComputeControlLowRangeUsesDefault) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(computeControl(50, 3), 10);
}

TEST_F(GooTest, ProcessCommandSetRejectsNullArgument) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", nullptr), -1);
}

TEST_F(GooTest, ProcessCommandSetRejectsNonNumericInput) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "abc"), -1);
}

TEST_F(GooTest, ProcessCommandSetRejectsTrailingCharacters) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "12x"), -1);
}

TEST_F(GooTest, ProcessCommandSetRejectsNegativeValue) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "-1"), -1);
}

TEST_F(GooTest, ProcessCommandSetRejectsOverflowValue) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "70000"), -1);
}

TEST_F(GooTest, ProcessCommandReadReturnsSensorData) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 123U;
  EXPECT_EQ(processCommand("READ", "ignored"), 123);
}

TEST_F(GooTest, ProcessCommandUnknownCommandReturnsError) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("NOOP", "1"), -1);
}

TEST_F(GooTest, ProcessorCombinesControlAndSetCommandResult) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 900U;
  EXPECT_EQ(processor("SET", "20"), 30);
  EXPECT_EQ(MOTOR_SPEED, 20U);
}

TEST_F(GooTest, ProcessorCombinesControlAndReadCommandResult) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 200U;
  EXPECT_EQ(processor("READ", "ignored"), 600);
}
