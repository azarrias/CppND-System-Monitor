#include <string>
#include <sstream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: D day(s), HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    // [0] - days
    // [1] - hours
    // [2] - minutes
    // [3] - seconds
    long time[4] { seconds / 86400 };
    seconds = seconds % 86400;
    time[1] = seconds / 3600;
    seconds = seconds % 3600;
    time[2] = seconds / 60;
    time[3] = seconds % 60;

    string result;
    string tmp;

    // add days too
    result = (time[0] == 1) ? "1 day, " : std::to_string(time[0]) + " days, ";

    for (int i=1; i<4; ++i) {
        tmp = std::to_string(time[i]);
        tmp.insert(tmp.begin(), 2 - tmp.length(), '0'); //lpad
        result += (tmp + ':');
    }
    result.pop_back();

    return result; 
}