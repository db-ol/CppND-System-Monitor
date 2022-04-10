#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

System::System() {
  operating_system_ = LinuxParser::OperatingSystem();
  kernel_ = LinuxParser::Kernel();
}

void System::Refresh() {
  memory_utilization_ = LinuxParser::MemoryUtilization();
  total_processes_ = LinuxParser::TotalProcesses();
  running_processes_ = LinuxParser::RunningProcesses();
}

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return memory_utilization_; }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return operating_system_; }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return running_processes_; }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return total_processes_; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return 0; }
