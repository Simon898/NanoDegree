#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:

  Process(int Pid): pid_(Pid){};

  int Pid() const;                         // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization() const;            // TODO: See src/process.cpp
  void UpdateCpuUtilization();
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  bool operator>(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
    int pid_;
    float cpuActive_{0.0}, previousCPUActive_{0.0}, previousCPUIdle_{0.0};
};

#endif