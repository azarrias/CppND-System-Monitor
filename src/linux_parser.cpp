#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Search for value in a key-value file, given its filename and a key
template <typename T>
T FindValueInFile(string filename, string key) {
  string line = "";
  string parameter = "";
  T value{};

  std::ifstream stream(filename);
  while (stream.is_open() && getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> parameter >> value;
    if (parameter.find(key) != string::npos)
      break;
  }
  return value;
}

// Search for token value in a file, given a line number, a position 
// and a separator (default ' ')
string FindValueInFile(string filename, unsigned int lineNum, 
                unsigned int pos, char separator = ' ') {
  string line = "";
  string value = "";
  unsigned int lineCount = 1;

  std::ifstream stream(filename);
  while (stream.is_open() && getline(stream, line)) {
    if (lineCount == lineNum) {
      std::istringstream linestream(line);
      for (unsigned int i = 0; i < pos; ++i) {
        if (!std::getline(linestream, value, separator))
          break;
      }
    }
    else if (lineCount > lineNum)
      break;
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float memTotal = FindValueInFile<float>(kProcDirectory + kMeminfoFilename, "MemTotal");
  float memFree = FindValueInFile<float>(kProcDirectory + kMeminfoFilename, "MemFree");

  return (memTotal - memFree) / memTotal; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string result = FindValueInFile(kProcDirectory + kUptimeFilename, 1, 1);
  return stol(result);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  long utime = 0, stime = 0, cutime = 0, cstime = 0; 
  string measure;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    // discard the first 13 values of the line
    for (int i=0; i<13; ++i) {
      linestream >> measure;
    }
    if(linestream >> measure) // 14 utime
      utime = stol(measure);
    if(linestream >> measure) // 15 stime
      stime = stol(measure);
    if(linestream >> measure) // 16 cutime
      cutime = stol(measure);
    if(linestream >> measure) // 17 cstime
      cstime = stol(measure);
  }
  return utime + stime + cutime + cstime; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> data;
  string measure;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    // discard "cpu"
    linestream >> measure;
    while(linestream >> measure) {
      data.push_back(measure);
    }
  }
  return data; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return FindValueInFile<int>(kProcDirectory + kStatFilename, "processes");
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return FindValueInFile<int>(kProcDirectory + kStatFilename, "procs_running");
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  std::replace(line.begin(), line.end(), '\000', ' ');
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  // Using VmData instead of VmSize
  // VmSize is actually the sum of all the virtual memory
  // VmData gives the exact physical memory being used
  // http://man7.org/linux/man-pages/man5/proc.5.html
  double vmData = FindValueInFile<double>(kProcDirectory + to_string(pid)
                 + kStatusFilename, "VmData");
  vmData = vmData / 1024; // convert Kb to Mb
  std::ostringstream oss;
  oss << std::setprecision(2) << std::fixed << vmData;
  return oss.str();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  return FindValueInFile<string>(kProcDirectory + to_string(pid) 
                  + kStatusFilename, "Uid");
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string user_uid = Uid(pid);
  string line, user, filler, uid;
  char delimiter = ':';
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      std::getline(linestream, user, delimiter);
      std::getline(linestream, filler, delimiter);
      std::getline(linestream, uid, delimiter);
      if (uid == user_uid)
        break;
    }
  }
  return user; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long sysuptime = LinuxParser::UpTime();
  long starttime = 0; 
  string measure;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    // discard the first 21 values of the line
    for (int i=0; i<21; ++i) {
      linestream >> measure;
    }
    if(linestream >> measure) // 22 starttime
      starttime = stol(measure);
  }
  // convert starttime from clock ticks to seconds and calculate
  return sysuptime - (double)starttime / sysconf(_SC_CLK_TCK);
}
