#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int minutes = seconds / 60;
  int seconds_ = seconds % 60;
  int hours = minutes / 60;
  int minutes_ = minutes % 60;
  return std::to_string(hours) + ":" + std::to_string(minutes_) + ":" +
         std::to_string(seconds_);
  // return std::format("{}:{}:{}", hours, minutes_, seconds_);
}
