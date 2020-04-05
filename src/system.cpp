#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Parse things that can be initialized just once at the beginning
System::System() 
: os_(LinuxParser::OperatingSystem()), kernel_(LinuxParser::Kernel())
{}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids();
    processes_.clear();
    for (int pid : pids) {
        Process p;
        p.Pid(pid);
        p.User(LinuxParser::User(pid));
        p.UpTime(LinuxParser::UpTime(pid));
        // convert process clock ticks to seconds and calculate
        p.CpuUtilization((double)LinuxParser::ActiveJiffies(pid) / sysconf(_SC_CLK_TCK) / p.UpTime());
        p.Ram(LinuxParser::Ram(pid));

        processes_.push_back(p);
    }
    std::sort(processes_.begin(), processes_.end(), 
        [](const Process &a, const Process &b) {
            return a.CpuUtilization() < b.CpuUtilization();
        });
    std::reverse(processes_.begin(), processes_.end());
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return os_; }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }