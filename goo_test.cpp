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

TEST(GooTest, TestProcessCommandRead) {
  RecordProperty("cpptest_filename", __FILE__);
  RecordProperty("req", "ASA-513");

  SENSOR_DATA = 0;
  EXPECT_EQ(processCommand("READ", ""), 0);
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
  EXPECT_EQ(processor("SET", "50"), 50 + DEFAULT_VALUE);
}
