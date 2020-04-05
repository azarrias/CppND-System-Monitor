#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid() const;                               // TODO: See src/process.cpp
  void Pid(int);
  std::string User() const;                      // TODO: See src/process.cpp
  void User(std::string);
  std::string Command() const;                   // TODO: See src/process.cpp
  void Command(std::string);
  float CpuUtilization() const;                  // TODO: See src/process.cpp
  void CpuUtilization(float);
  std::string Ram() const;                       // TODO: See src/process.cpp
  void Ram(std::string);
  long int UpTime() const;                       // TODO: See src/process.cpp
  void UpTime(long int);
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string user_;
  long int uptime_;
  float cpu_;
  std::string mem_;
  std::string command_;
};

#endif