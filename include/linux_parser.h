#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
using namespace std;

namespace LinuxParser {
// Paths
const string kProcDirectory{"/proc/"};
const string kCmdlineFilename{"/cmdline"};
const string kCpuinfoFilename{"/cpuinfo"};
const string kStatusFilename{"/status"};
const string kStatFilename{"/stat"};
const string kUptimeFilename{"/uptime"};
const string kMeminfoFilename{"/meminfo"};
const string kVersionFilename{"/version"};
const string kOSPath{"/etc/os-release"};
const string kPasswordPath{"/etc/passwd"};

//
const string processes("processes");
const string running("procs_running");
const string vm_rss("VmRSS:");
const string uid("Uid:");

// System
float MemoryUtilization();
long UpTime();
vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
string OperatingSystem();
string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
vector<string> CpuUtilization();

// Processes
string Command(int pid);
string Ram(int pid);
string Uid(int pid);
string User(int pid);
long int UpTime(int pid);
vector<string> CPUstat(int pid);
};  // namespace LinuxParser

#endif