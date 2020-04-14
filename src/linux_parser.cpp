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
  string key, value;
  string line;
  std::map<string, float> measures { 
    {"MemTotal", 0}, {"MemFree", 0}, {"MemAvailable", 0}, {"Buffers", 0}};
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    for (unsigned int i=0; i<measures.size(); ++i) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
      key.pop_back(); // remove the colon at the end
      if (measures.find(key) != measures.end())
        measures[key] = stof(value);
    }
  }
  return (measures["MemTotal"] - measures["MemFree"]) / measures["MemTotal"]; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return stol(uptime);
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
  int processes = 0;
  string key;
  string value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        processes = stoi(value);
        break;
      }
    }
  }
  return processes; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int processes = 0;
  string key;
  string value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        processes = stoi(value);
        break;
      }
    }
  }
  return processes; 
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
  string mem, key, line, element;
  double value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> element;
      if (key == "VmSize:") {
        value = stod(element) / 1024;
        std::ostringstream oss;
        oss << std::setprecision(2) << std::fixed << value;
        mem = oss.str();
        break;
      }
    }
  }
  return mem; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string uid, key, value, line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        uid = value;
        break;
      }
    }
  }
  return uid; 
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
