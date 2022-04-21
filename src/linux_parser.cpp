#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template <typename T>
T LinuxParser::FindValueByKey(std::string const &keyFilter, std::string const &filename) {
  std::string line, key;
  T value;

  std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          stream.close();
          return value;
        }
      }
    }
  }
  stream.close();
  return value;
};

std::string LinuxParser::FindValueByIdx(int const target_idx, std::string const &filename){
  std::string line, value;
  int idx = 0;

  std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open() && std::getline(stream, line)) {
    std::istringstream linestream(line);
    while ((idx++ <= target_idx) && (linestream >> value)) {}
  }
  stream.close();
  return value;
};

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
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  int mem_total = FindValueByKey<int>(filterMemTotalString, kMeminfoFilename);
  int mem_free = FindValueByKey<int>(filterMemFreeString, kMeminfoFilename);
  float memory_utilization = (mem_total - mem_free) * 1.0 / mem_total;
  return memory_utilization;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  long value = 0;
  string time_str = FindValueByIdx(0, kUptimeFilename);
  if (!time_str.empty()) {
    value = stol(time_str);
  }
  return value;
}

// DONE: Read and return CPU utilization
std::vector<std::string> LinuxParser::CpuUtilization() {
  string line;
  string key, value;
  std::vector<std::string> results;
  int cpu_states_count = static_cast<int>(CPUStates::Count);
  results.resize(cpu_states_count);
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open() && std::getline(filestream, line)) {
    std::istringstream linestream(line);
    if (linestream >> key) {
      if (key == filterCpu) {
        int cpu_states_idx = 0;
        while ((linestream >> value) && (cpu_states_idx < cpu_states_count)) {
          results[cpu_states_idx++] = value;
        }
      } else {
        filestream.close();
        return results;
      }
    }
  }
  filestream.close();
  return results;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return FindValueByKey<int>(filterProcesses, kStatFilename);
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return FindValueByKey<int>(filterRunningProcesses, kStatFilename);
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line = FindValueByIdx(0, std::to_string(pid) + kCmdlineFilename);
  if (line.size() > 50) {
    line = line.substr(0, 50) + "...";
  }
  return line;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  // Use VmRSS to give the exact physical memory being used as a part of Physical RAM
  return FindValueByKey<string>(filterProcMem, std::to_string(pid) + kStatusFilename);
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  return FindValueByKey<string>(filterUID, std::to_string(pid) + kStatusFilename);
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string name, letter_x, uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      if ((linestream >> name >> letter_x >> uid) && (uid == Uid(pid))) {
        return name;
      }
    }
  }
  return name;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  constexpr int starttime_idx = 21;
  long uptime = 0;
  string uptime_str = FindValueByIdx(starttime_idx, std::to_string(pid) + kStatFilename);
  if (!uptime_str.empty()) {
    uptime = stol(uptime_str);
  }
  return uptime;
}

// DONE: Read and return CPU utilization of a process
std::vector<std::string> LinuxParser::Cpu(int pid) {
  std::vector<std::string> results;
  constexpr int utime_idx = 13, stime_idx = 14, cutime_idx = 15,
                cstime_idx = 16, starttime_idx = 21;
  int idx = 0;
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open() && std::getline(filestream, line)) {
    std::istringstream linestream(line);
    while ((linestream >> value) && (idx <= starttime_idx)) {
      if ((idx == utime_idx) || (idx == stime_idx) || (idx == cutime_idx) ||
          (idx == cstime_idx) || (idx == starttime_idx)) {
        results.emplace_back(value);
      }
      idx ++;
    }
  }
  return results;
}