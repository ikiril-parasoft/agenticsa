#include <gtest/gtest.h>
#include "goo.h"

class GooTestWithReset : public ::testing::Test {
 protected:
  void SetUp() override {
    SENSOR_DATA = 0;
    MOTOR_SPEED = 0;
  }
};

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

TEST_F(GooTestWithReset, ComputeControlLowThresholdReturnsDefault) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(computeControl(100, 10), DEFAULT_VALUE);
}

TEST_F(GooTestWithReset, ProcessCommandSetNullArgReturnsError) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", NULL), -1);
}

TEST_F(GooTestWithReset, ProcessCommandSetEmptyStringReturnsError) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", ""), -1);
}

TEST_F(GooTestWithReset, ProcessCommandSetTrailingCharactersReturnsError) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "12x"), -1);
}

TEST_F(GooTestWithReset, ProcessCommandSetNegativeValueReturnsError) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "-1"), -1);
}

TEST_F(GooTestWithReset, ProcessCommandSetTooLargeValueReturnsError) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "70000"), -1);
}

TEST_F(GooTestWithReset, ProcessCommandSetOverflowReturnsError) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("SET", "999999999999999999999999999999"), -1);
}

TEST_F(GooTestWithReset, ReadSensorReturnsCurrentRegisterValue) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 77;
  EXPECT_EQ(readSensor(), 77);
}

TEST_F(GooTestWithReset, ProcessCommandReadReturnsSensorValue) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 123;
  EXPECT_EQ(processCommand("READ", "ignored"), 123);
}

TEST_F(GooTestWithReset, ProcessCommandUnknownReturnsError) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("UNKNOWN", "1"), -1);
}

TEST_F(GooTestWithReset, ProcessorCombinesCommandAndControl) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 80;
  EXPECT_EQ(processor("READ", NULL), 90);
}
