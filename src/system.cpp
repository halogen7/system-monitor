#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
using namespace std;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  if (processes_.size() == 0) {
    vector<int> pids(LinuxParser::Pids());
    for (int pid : pids) {
      processes_.emplace_back(pid);
    }
  }
  sort(processes_.begin(), processes_.end());
  return processes_;
}

// Return the system's kernel identifier (string)
string System::Kernel() {
  if (_kernel.length() < 1) {
    _kernel = LinuxParser::Kernel();
  }
  return _kernel;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
string System::OperatingSystem() {
  if (_os.length() < 1) {
    _os = LinuxParser::OperatingSystem();
  }
  return _os;
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() {
  return LinuxParser::UpTime();
}