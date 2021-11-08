#include <string>
#include <iomanip>
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

  std::ostringstream stream;
  stream << std::setw(2) << std::setfill('0') << hours << ":" 
        << std::setw(2) << std::setfill('0') << minutes_ << ":"
        << std::setw(2) << std::setfill('0') << seconds_;
  return stream.str();

/*
  std::string hours_padding   = hours    < 10? "0" : "";
  std::string minutes_padding = minutes_ < 10? "0" : "";
  std::string seconds_padding = seconds_ < 10? "0" : "";
  return hours_padding + std::to_string(hours) + ":" + minutes_padding + std::to_string(minutes_) + ":" +
         seconds_padding + std::to_string(seconds_);*/
  // return std::format("{}:{}:{}", hours, minutes_, seconds_);
}
