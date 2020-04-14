#include <string>
#include <sstream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: D day(s), HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    unsigned int days = seconds / 86400;
    unsigned int hours = (seconds / 3600) % 24;
    unsigned int minutes = (seconds / 60) % 60;
    seconds = seconds % 60;

    // add days too
    string result;
    result = (days == 1) ? "1 day, " : std::to_string(days) + " days, ";
    result = result + Lpad(std::to_string(hours), 2, '0') + ':';
    result = result + Lpad(std::to_string(minutes), 2, '0') + ':';
    result = result + Lpad(std::to_string(seconds), 2, '0');

    return result;
}

// Add these functions to avoid leftovers from previous values
string Format::Lpad(const string &str, int paddedLength, char padChar) {
  string str2(str.substr(0, paddedLength));
  str2.insert(str2.begin(), paddedLength - str2.length(), padChar);
  return str2;
}

string Format::Rpad(const string &str, int paddedLength, char padChar) {
  string str2(str.substr(0, paddedLength));
  str2.append(paddedLength - str2.length(), padChar);
  return str2;
}