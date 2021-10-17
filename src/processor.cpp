#include "processor.h"

#include <iostream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
double Processor::Utilization() {
  vector<string> data = LinuxParser::CpuUtilization();


  int idle = std::stoi(data[3]) + std::stoi(data[4]);
  int non_idle = std::stoi(data[0]) + std::stoi(data[1]) + std::stoi(data[2]) +
                std::stoi(data[5]) + std::stoi(data[6]) + std::stoi(data[7]);
  int total = idle + non_idle;

  // Get the difference between current and previous iteration
  int total_diff = total - _prev_total;
  int idle_diff = idle - _prev_idle;

  // Store values to be used in next iteration calculation
  _prev_idle = idle;
  _prev_non_idle = non_idle;
  _prev_total = total;

  return (total_diff - idle_diff) / (double)total_diff;
  
}