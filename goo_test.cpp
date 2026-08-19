#include <gtest/gtest.h>
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
}

TEST(GooTest, TestValueBelowLowThreshold) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(computeControl(50, 10), DEFAULT_VALUE);
}

TEST(GooTest, TestReadSensor) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 42;
  EXPECT_EQ(readSensor(), 42);
  SENSOR_DATA = 0;
}

TEST(GooTest, TestProcessCommandRead) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 7;
  EXPECT_EQ(processCommand("READ", ""), 7);
  SENSOR_DATA = 0;
}

TEST(GooTest, TestProcessCommandUnknown) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  EXPECT_EQ(processCommand("UNKNOWN", ""), -1);
}

TEST(GooTest, TestProcessor) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 0;
  // sensor=0 -> computeControl(0,0) -> DEFAULT_VALUE=10; processCommand("SET","5") -> 5
  EXPECT_EQ(processor("SET", "5"), 15);
}
