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
  EXPECT_EQ(MOTOR_SPEED, 0);
}

TEST_F(GooTest, ComputeControlBelowOrEqualLowThreshold) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(computeControl(100, 10), 10);
}

TEST_F(GooTest, ProcessCommandSetWithValidInteger) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "42"), 42);
  EXPECT_EQ(MOTOR_SPEED, 42);
}

TEST_F(GooTest, ProcessCommandSetWithTrailingCharacters) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "12x"), 0);
  EXPECT_EQ(MOTOR_SPEED, 0);
}

TEST_F(GooTest, ProcessCommandSetWithNonNumericString) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "abc"), 0);
  EXPECT_EQ(MOTOR_SPEED, 0);
}

TEST_F(GooTest, ProcessCommandSetWithValueAboveIntRange) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "2147483648"), 0);
  EXPECT_EQ(MOTOR_SPEED, 0);
}

TEST_F(GooTest, ProcessCommandSetWithValueBelowIntRange) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "-2147483649"), 0);
  EXPECT_EQ(MOTOR_SPEED, 0);
}

TEST_F(GooTest, ProcessCommandSetWithStrtolOverflow) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "999999999999999999999999999999999999"), 0);
  EXPECT_EQ(MOTOR_SPEED, 0);
}

TEST_F(GooTest, ProcessCommandReadReturnsSensorData) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 321;
  EXPECT_EQ(processCommand("READ", "unused"), 321);
}

TEST_F(GooTest, ProcessCommandUnknownCommandReturnsMinusOne) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("UNKNOWN", "unused"), -1);
}

TEST_F(GooTest, ProcessorReturnsCommandAndControlSum) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 50;
  EXPECT_EQ(processor("READ", "unused"), 60);
}
