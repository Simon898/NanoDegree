#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() const {
     return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { 
    return cpuActive_;
    }

void Process::UpdateCpuUtilization()
{
    float currentCPUActive = static_cast<float>(LinuxParser::ActiveJiffies(Pid()));
    float currentCPUIdle = static_cast<float>(LinuxParser::IdleJiffies());
    float cpuActiveDelta = currentCPUActive - previousCPUActive_; 
    float cpuIdleDelta = currentCPUIdle - previousCPUIdle_; 
    previousCPUActive_ = currentCPUActive;
    previousCPUIdle_ = currentCPUIdle;
    cpuActive_ = cpuActiveDelta/(cpuIdleDelta + cpuActiveDelta); 
}

// TODO: Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(Pid()); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(Pid());
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(Pid()); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(Pid());
}

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {   
    return CpuUtilization() < a.CpuUtilization();
}

bool Process::operator>(Process const& a) const {   
    return CpuUtilization() > a.CpuUtilization();
}