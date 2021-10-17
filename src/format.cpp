#include "format.h"

#include <iostream>
#include <string>

using std::string;

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  // Get initial remainder
  int mod = seconds % 3600;

  // Get hours, minutes, seconds
  string hours = std::to_string(seconds / 3600);
  string minutes = std::to_string(mod / 60);
  string sec = std::to_string(mod % 60);

  if (hours.length() < 2) {
    hours = "0" + hours;
  }
  if (minutes.length() < 2) {
    minutes = "0" + minutes;
  }
  if (sec.length() < 2) {
    sec = "0" + sec;
  }

  return hours + ":" + minutes + ":" + sec;
}