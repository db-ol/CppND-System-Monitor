#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  pid_ = pid;
  command_ = LinuxParser::Command(pid);
  user_ = LinuxParser::User(pid);
}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const {
  float cpu_utilization = 0.0;
  long utime, stime, cutime, cstime, starttime;
  long uptime = LinuxParser::UpTime();
  std::vector<std::string> cpu_times = LinuxParser::Cpu(pid_);
  if (cpu_times.size() != 5) {
    return cpu_utilization;
  }
  utime = std::stol(cpu_times[0]);
  stime = std::stol(cpu_times[1]);
  cutime = std::stol(cpu_times[2]);
  cstime = std::stol(cpu_times[3]);
  starttime = std::stol(cpu_times[4]);

  long total_time = utime + stime + cutime + cstime;
  float seconds = uptime - (starttime * 1.0 / sysconf(_SC_CLK_TCK));
  cpu_utilization = 100 * ((total_time * 1.0 / sysconf(_SC_CLK_TCK)) / seconds);
  return cpu_utilization;
}

// DONE: Return the command that generated this process
string Process::Command() { return command_; }

// DONE: Return this process's memory utilization
string Process::Ram() {
  long ram_kb = std::stoi(LinuxParser::Ram(pid_));
  long ram_mb = ram_kb / 1024;
  return std::to_string(ram_mb);
}

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() {
  return LinuxParser::UpTime() - LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);
}

// DONE: Overload the "more than" comparison operator for Process objects
bool Process::operator>(Process const& candidate) const {
  return CpuUtilization() > candidate.CpuUtilization();
}