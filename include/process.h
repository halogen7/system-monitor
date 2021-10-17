#ifndef PROCESS_H
#define PROCESS_H

#include <string>
using std::string;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // See src/process.cpp
  std::string User();                      // See src/process.cpp
  std::string Command();                   // See src/process.cpp
  double CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // See src/process.cpp
  long int UpTime();                       // See src/process.cpp
  bool operator<(Process const& a) const;  // See src/process.cpp
  Process(int id) : _pid(id){}

  // Declare any necessary private members
 private:
  int _pid;
  string _cmd;
  string _ram;
  string _user;
  long _uptime;
  float _cpu_util;
};

#endif