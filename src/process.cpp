#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "format.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return _pid; }

// TODO: Return this process's CPU utilization
double Process::CpuUtilization() {
  vector<string> data = LinuxParser::CPUstat(_pid);
  // Ensure there is data in the vector
  if (data.size() > 0) {
    long u_time = 0.0;
    long s_time = 0.0;
    long cu_time = 0.0;
    long cs_time = 0.0;

    // Check if data parsed is empty
    if (!data[0].empty()) {
      u_time = std::stol(data[0]);
    }
    if (!data[1].empty()) {
      s_time = std::stol(data[1]);
    }
    if (!data[2].empty()) {
      cu_time = std::stol(data[2]);
    }
    if (!data[3].empty()) {
      cs_time = std::stol(data[3]);
    }

    double total = (u_time + s_time + cu_time + cs_time) / sysconf(_SC_CLK_TCK);
    _cpu_util = total / UpTime();
  }
  return _cpu_util;
}

// Return the command that generated this process
string Process::Command() {
  if (_cmd.length() < 1) {
    _cmd = LinuxParser::Command(_pid);
  }
  return _cmd;
}

// Return this process's memory utilization
string Process::Ram() {
  long ram = 0;
  if (!LinuxParser::Ram(_pid).empty()) {
    ram = std::stol(LinuxParser::Ram(_pid));
  }

  _ram = to_string(ram / 1000);

  return _ram;
}

// Return the user (name) that generated this process
string Process::User() {
  if (_user.length() < 1) {
    _user = LinuxParser::User(Pid());
  }

  return _user;
}

// Return the age of this process (in seconds)
long Process::UpTime() {
  _uptime = LinuxParser::UpTime(Pid());
  return _uptime;
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return _cpu_util > a._cpu_util;
}