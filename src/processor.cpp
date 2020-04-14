#include "processor.h"
#include <string>
#include <vector>
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
double Processor::Utilization() { 
    std::vector<std::string> data = LinuxParser::CpuUtilization();
    int previous_idle = idle_;
    int previous_non_idle = non_idle_;

    int idle = std::stoi(data[3]);
    int iowait = std::stoi(data[4]);
    idle_ = idle + iowait;

    int user = std::stoi(data[0]);
    int nice = std::stoi(data[1]);
    int system = std::stoi(data[2]);
    int irq = std::stoi(data[5]);
    int softirq = std::stoi(data[6]);
    int steal = std::stoi(data[7]);
    non_idle_ = user + nice + system + irq + softirq + steal;

    int previous_total = previous_idle + previous_non_idle;
    int total = idle_ + non_idle_;
    int delta_total = total - previous_total;
    int delta_idle = idle_ - previous_idle;

    return (double)(delta_total - delta_idle) / delta_total; 
}