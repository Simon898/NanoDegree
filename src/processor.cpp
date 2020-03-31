#include <string>

#include "linux_parser.h"
#include "processor.h"


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    float currentCPUaActive = static_cast<float>(LinuxParser::ActiveJiffies());
    float currentCPUIdle = static_cast<float>(LinuxParser::IdleJiffies());
    float cpuActiveDelta = currentCPUaActive - previousCPUActive_; 
    float cpuIdleDelta = currentCPUIdle - previousCPUIdle_; 
    previousCPUActive_ = currentCPUaActive;
    previousCPUIdle_ = currentCPUIdle;
    
    return cpuActiveDelta/(cpuIdleDelta + cpuActiveDelta);
    }