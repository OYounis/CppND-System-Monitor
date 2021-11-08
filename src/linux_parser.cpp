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
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
// This function only parses data.
// The calculations are done in System::MemoryUtilization()
int LinuxParser::MemoryUtilization(
    std::map<string, unsigned long int>& memInfo) {
  std::string key;
  unsigned long long int info;
  std::string line;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    for (int i = 0; i < 5; i++) {
      if (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        linestream >> key >> info;
        memInfo.insert({key, info});
      }
    }

    // make sure the write keys are parsed
    if (memInfo.count("MemTotal:") & memInfo.count("Cached:")) return 1;
  }

  return 0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;
  std::string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  std::string times = ParseStat(13, 16, pid);
  long utime, stime, cutime, cstime;
  std::istringstream timestream(times);
  timestream >> utime >> stime >> cutime >> cstime;
  return utime + stime +cutime + cstime; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<int> LinuxParser::CpuUtilization() {
  std::string cpu;
  int value;
  std::string line;
  std::vector<int> stats{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> cpu;
      while (linestream >> value) stats.push_back(value);
    }
  }
  return stats;
}

// TODO: Read and return the total number of processes
int LinuxParser::ParseStat(const std::string ref_key) {
  std::string key;
  int processes;
  std::string line;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> processes) {
        if (key == ref_key) return processes;
      }
    }
  }
  return 0;
}

int LinuxParser::TotalProcesses() { return ParseStat("processes"); }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return ParseStat("procs_running"); }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) return line;
  }
  return "";
}

int LinuxParser::ParseStatus(const std::string ref_key, const int pid) {
  std::string key;
  int processes;
  std::string line;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> processes) {
        if (key == ref_key) return processes;
      }
    }
  }
  return 0;
}
// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  return std::to_string(ParseStatus("VmSize:", pid) / 1000);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  return std::to_string(ParseStatus("Uid:", pid));
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  std::string user;
  std::string uid;
  std::string x;
  std::string line;
  std::string ref_uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid) {
        if (uid == ref_uid) return user;
      }
    }
  }

  return string();
}

std::string LinuxParser::ParseStat(const int begin, const int end,
                                   const int pid) {
  std::vector<std::string> parsed;
  std::string content;
  std::string stat = "";
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, content);
    std::istringstream linestream(content);
    while (linestream >> stat) parsed.push_back(stat);
    if (begin == end)
      return parsed[begin];
    else {
      for (int i = begin; i < end+1; i++) {
        stat = stat + parsed[i] + " ";
      }
      stat = stat + parsed[begin];
      return stat;
    }
  }
  return "";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  long uptime = 0;
  try{
    uptime = std::stol(ParseStat(21, 21, pid)) / sysconf(_SC_CLK_TCK);
  } catch(...) {uptime = 0;}

  return uptime;
}
