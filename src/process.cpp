#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

void Process::Pid(int pid) { pid_ = pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

void Process::CpuUtilization(float cpu) { cpu_ = cpu; }

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

void Process::User(std::string user) { user_ = user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

void Process::UpTime(long int uptime) { uptime_ = uptime; };

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }