#include <gtest/gtest.h>
#include "goo.h"

// Match the implementation signature in goo.cpp for direct testing.
extern int processor(char* cmd, char* arg);

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
}

TEST(GooTest, TestValueAtOrBelowLowThreshold) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-511");

  EXPECT_EQ(computeControl(100, 10), 10);
  EXPECT_EQ(computeControl(50, 10), 10);
}

TEST(GooTest, ReadCommandReturnsSensorValue) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-511");

  SENSOR_DATA = 321;
  EXPECT_EQ(processCommand("READ", "ignored"), 321);
}

TEST(GooTest, UnknownCommandReturnsError) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-511");

  EXPECT_EQ(processCommand("BAD", "1"), -1);
}

TEST(GooTest, ProcessorReadCommandPath) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-511");

  SENSOR_DATA = 200;
  char cmd[] = "READ";
  char arg[] = "0";

  EXPECT_EQ(processor(cmd, arg), 600);
}
