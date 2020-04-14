#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
std::string Lpad(const std::string &str, int paddedLength, char padChar = ' ');
std::string Rpad(const std::string &str, int paddedLength, char padChar = ' ');
};                                    // namespace Format

#endif