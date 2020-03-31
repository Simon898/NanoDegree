#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "math.h"
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
  long memTotal, memFree, value;
  string line, titleText, unitText;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> titleText >> value >> unitText;
      if (titleText == "MemTotal:") {
        memTotal = value;
      }
      if (titleText == "MemFree:") {
        memFree = value;
      }
    }
  }
  return (memTotal-memFree)*1.0/memTotal;
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
   std::fstream myfile(kProcDirectory + kUptimeFilename, std::ios_base::in);
    long int a, b;
    myfile >> a >> b;
    long int totalTime = a + b;
   return totalTime; 
  }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long upTime = UpTime();
  long jif = sysconf(_SC_CLK_TCK);
  return upTime * jif; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  vector<string> activeJif; 
  string line;
  string text;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);  
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> text) {
      activeJif.push_back(text);
    }
  }

  // stol conversion string to long format
  return stol(activeJif[13]) + stol(activeJif[14])+ stol(activeJif[15])+ stol(activeJif[16]); 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
   vector<string> utilizationTimes = CpuUtilization();
    long totalActiveJiffies = stol(utilizationTimes[CPUStates::kUser_]) + stol(utilizationTimes[CPUStates::kNice_]) +
      stol(utilizationTimes[CPUStates::kSystem_]) + stol(utilizationTimes[CPUStates::kIRQ_]) +
      stol(utilizationTimes[CPUStates::kSoftIRQ_]) + stol(utilizationTimes[CPUStates::kSteal_]);
  return totalActiveJiffies; 
  }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
 vector<string> utilizationTimes = CpuUtilization();
 return stol(utilizationTimes[CPUStates::kIdle_]) + stol(utilizationTimes[CPUStates::kIOwait_]);
 }  

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  vector<string> v;
  string token;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    if (getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> token) {
        if (token == "cpu") {
          while (linestream >> token) {
            v.push_back(token);
          }
          return v;
        }
      }
    }
  }
  return v;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    string line;
    string key;
    string value;
    int totNumber;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if(key == "processes") {
          totNumber = std::stoi(value);
          }
        }
      }
    }
   // std::cout << nrOfProcesses << "\n";
   return totNumber;
   }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    string line;
    string key;
    string value;
    int totRun;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if(key == "procs_running") {
          totRun = std::stoi(value);
          }
        }
      }
    }
   return totRun;
   }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line, value;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> value) {
        }
      }
    }
    return value; 
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line, key;
  int value, mb;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if(key == "VmSize:") {
           mb = value / 1000;
          }
        }
      }
    }
  return std::to_string(mb); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
    string line, key, value, uid;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if(key == "Uid:") {
            uid = value;
          }
        }
      }
    }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line, user, ignore, key, user1;
  string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> ignore >> key) {
        if (key == uid) {
          user1 = user;
        }
      }
    }
  }
  return user1;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, value1, value2, value3, value4, value5, value6, value7, value8, value9, value10, value11,
               value12, value13, value14, value15, value16, value17, value18, value19, value20, value21;
  long int upTime;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        if(linestream>>value1>>value2>>value3>>value4>>value5>>value6>>value7>>value8>>value9>>value10>>
                       value11>>value12>>value13>>value14>>value15>>value16>>value17>>value18>>value19>>
                       value20>>value21>>upTime) {
            upTime = upTime/sysconf(_SC_CLK_TCK);
        }
      }
    }
   return upTime;
}