#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  long second, minute, hour;
  string second_str, minute_str, hour_str;
  minute = seconds / 60;
  second = seconds % 60;
  hour = minute / 60;
  minute = minute % 60;

  second_str = std::to_string(second);
  minute_str = std::to_string(minute);
  hour_str = std::to_string(hour);
  if (second < 10) {
    second_str = "0" + second_str;
  }
  if (minute < 10) {
    minute_str = "0" + minute_str;
  }
  if (hour < 10) {
    hour_str = "0" + hour_str;
  }

  return hour_str + ":" + minute_str + ":" + second_str;
}