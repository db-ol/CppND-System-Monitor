#include "processor.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  long user, nice, system, idle, iowait, irq, softirq, steal;
  long idle_time, total_time, idle_time_delta, total_time_delta;
  std::vector<std::string> cpu_utilization = LinuxParser::CpuUtilization();
  if (cpu_utilization.size() != static_cast<int>(LinuxParser::CPUStates::Count)) {
    return 0.0;
  }
  user = std::stoi(cpu_utilization[static_cast<int>(LinuxParser::CPUStates::kUser_)]);
  nice = std::stoi(cpu_utilization[static_cast<int>(LinuxParser::CPUStates::kNice_)]);
  system = std::stoi(cpu_utilization[static_cast<int>(LinuxParser::CPUStates::kSystem_)]);
  idle = std::stoi(cpu_utilization[static_cast<int>(LinuxParser::CPUStates::kIdle_)]);
  iowait = std::stoi(cpu_utilization[static_cast<int>(LinuxParser::CPUStates::kIOwait_)]);
  irq = std::stoi(cpu_utilization[static_cast<int>(LinuxParser::CPUStates::kIRQ_)]);
  softirq = std::stoi(cpu_utilization[static_cast<int>(LinuxParser::CPUStates::kSoftIRQ_)]);
  steal = std::stoi(cpu_utilization[static_cast<int>(LinuxParser::CPUStates::kSteal_)]);

  idle_time = idle + iowait;
  total_time = idle_time + user + nice + system + irq + softirq + steal;

  idle_time_delta = idle_time - prev_idle_time_;
  total_time_delta = total_time - prev_total_time_;

  utilization_ = (total_time_delta - idle_time_delta) * 1.0 / total_time_delta;

  prev_idle_time_ = idle_time;
  prev_total_time_ = total_time;

  return utilization_;
}