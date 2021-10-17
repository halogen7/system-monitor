#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
  string os, kernel, version;
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
      if (std::all_of(filename.begin(), filename.end(), ::isdigit)) {
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
  string mem_total, mem_free, available, buffers;
  string line, line_total, line_free, line_avail, line_buffers, text;

  // Open /pid/status file
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    // Get the necessary memory values and exit the while loop
    std::getline(stream, line);
    std::istringstream linestream1(line);
    linestream1 >> text >> mem_total;

    std::getline(stream, line);
    std::istringstream linestream2(line);
    linestream2 >> text >> mem_free;

    std::getline(stream, line);
    std::istringstream linestream3(line);
    linestream3 >> text >> available;

    std::getline(stream, line);
    std::istringstream linestream4(line);
    linestream4 >> text >> buffers;
  }

  float total_used =
      1.0 - (std::stof(mem_free) / (std::stof(mem_total) - std::stof(buffers)));

  // Return the memory usage
  return total_used;
}

// Read and return the system up time
long LinuxParser::UpTime() {
  string up_time, idle_time, line;

  // Open up time file
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    // Extract the space separated values
    stream >> up_time >> idle_time;
  }

  // Convert up_time string to type long
  return std::stol(up_time);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, cpu, user, nice, system, idle, iowait, irq, softirq, steal,
      guest, guest_nice;
  vector<string> utilization;

  // Open stat file
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    // Get the first line and create a string stream from it
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;
    utilization = {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
  }
  // Return the vector of utilization values
  return utilization;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string total_procs, line, text;

  // Open stat file
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    // While the stream has lines
    while (std::getline(stream, line)) {
      // When processes is found, get the value and exit the while loop
      if (line.compare(0, processes.length(), processes) == 0) {
        std::istringstream linestream(line);
        linestream >> text >> total_procs;
        break;
      }
    }
  }

  // Convert total_procs string to type long and return the value
  return std::stoi(total_procs);
}

//  Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string num_procs, line, text;

  // Open stat file
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    // While the stream has lines
    while (std::getline(stream, line)) {
      // When procs_running is found, get the value and exit the while loop
      if (line.compare(0, running.length(), running) == 0) {
        std::istringstream linestream(line);
        linestream >> text >> num_procs;
        break;
      }
    }
  }

  // Convert num_procs string to type long and return the value
  return std::stoi(num_procs);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmd;

  // Open /pid/cmdline file
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    // Extract the command
    stream >> cmd;
  }

  // Return the command string
  return cmd;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  /* Using VmRSS as it was recommended by the reviewer.
    VmSize returns the sum of all virtual memory according to the manpages,
    whereas VmRSS is the resident memory usage (although inaccurate; see smaps for more accurate usage)
  */
  string mem, line, text;

  // Open /pid/status file
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      // When VmSize is found, get the value and exit the while loop
      if (line.compare(0, vm_rss.length(), vm_rss) == 0) {
        std::istringstream linestream(line);
        linestream >> text >> mem;
        break;
      }
    }
  }
  // Return the memory usage
  return mem;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string user_id, line, text;

  // Open /pid/status file
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      // When Uid is found, get the value and exit the while loop
      if (line.compare(0, uid.length(), uid) == 0) {
        std::istringstream linestream(line);
        linestream >> text >> user_id;
        break;
      }
    }
  }

  // Return the user_id string
  return user_id;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  // Get the user id
  string user_id = Uid(pid);
  string line, user, x, id;

  // Open the etc/passwd file
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      // Extract the user/app name and id
      linestream >> user >> x >> id;

      // If the id from the line equals the user id, exit the while loop
      if (user_id.compare(0, id.length(), id) == 0) {
        break;
      }
    }
  }

  // Return the extracted user name
  return user;
}

// Read and return the up time of a process
long LinuxParser::UpTime(int pid) {
  int index(0);
  string start_time, line;
  // Open stat file
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    // Get the 22nd value, which represents the process start time
    while (index < 22) {
      linestream >> start_time;
      index++;
    }
  }

  // Convert return process up time
  return UpTime() - std::stol(start_time) / sysconf(_SC_CLK_TCK);
}

vector<string> LinuxParser::CPUstat(int pid) {
  int index(0);
  vector<string> cpu_time;
  string u_time, s_time, cu_time, cs_time;
  string next, line;
  // Open stat file
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    // Get 14-17 value, which represents the process start time
    while (index < 13) {
      linestream >> next;
      index++;
    }
    linestream >> u_time >> s_time >> cu_time >> cs_time;
    cpu_time.push_back(u_time);
    cpu_time.push_back(s_time);
    cpu_time.push_back(cu_time);
    cpu_time.push_back(cs_time);
  }

  // Convert return process up_time
  return cpu_time;
}
