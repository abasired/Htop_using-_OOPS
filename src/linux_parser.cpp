#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"


using std::stof;
using std::string;
using std::to_string;
using std::vector;

// An example of how to read data from the filesystem
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

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version,  kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// 
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  // Based on any changes this len can be extended
  vector<float>memInfo; 
  float temp_mem;
  string line, title_str;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  //Rows considered in Mem utilization computation
  int num_MemInfo_rows = 2;

  if (stream.is_open()){
    for (int line_index = 0 ; line_index < num_MemInfo_rows; line_index ++){
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> title_str >> temp_mem;
      memInfo.push_back(temp_mem);
    }
  }
  return (memInfo[0]-memInfo[1])/memInfo[0]; 
}

//Read and return the system uptime
long LinuxParser::UpTime() { 

  string line, total_time;
  
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> total_time ;
  }
  return std::stol(total_time); 
  }


// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> Cpu_data;
  string line, cpu_text, cpu_info;
  bool found = false;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (!found) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> cpu_text;
      if (cpu_text.compare("cpu") == 0) {
        while(linestream >> cpu_info ){
          Cpu_data.push_back(cpu_info);
        }
        found = true;
      }
    }
  }
 
  return Cpu_data; 
  }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  
  string line, process_str;
  bool found = false;
  int process_cnt;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (!found){
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> process_str >> process_cnt;
      if (process_str.compare("processes") == 0) {
        found = true;
      }
    }
  }
  return process_cnt; 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 

  string line, running_process_str;
  bool found = false;
  int running_process_cnt;

  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (!found){
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> running_process_str >> running_process_cnt;
      if (running_process_str.compare("procs_running") == 0) {
        found = true;
      }
    }
  }
  
  return running_process_cnt; 
  }

// Read and return the command associated with a process

string LinuxParser::Command(int pid) { 
  
  string line{""};
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);
   if (filestream.is_open()) {
    std::getline(filestream, line); 
    return line; 
  }
  return line;
}

// Read and return the memory used by a process

string LinuxParser::Ram(int pid) { 
  string line, key, value{""};
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
   if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          return to_string(std::stol(value)/1000);
          //return value;
        }
      }
    }
  } 
  return value; 
} 

// Read and return the user ID associated with a process

string LinuxParser::Uid(int pid) { 
  
  string line, key, value{""};
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
   if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          return value;
        }
      }
    }
  }
  return value; 
}

// Read and return the user associated with a process

string LinuxParser::User(int pid) { 

  string line, key, value{""}, text;

  std::ifstream filestream(kPasswordPath);
   if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> text >>  key) {
        if (key == LinuxParser::Uid(pid)) {
          return value;
        }
      }
    }
   }
   return value;
 }

// Read and return the uptime of a process

long LinuxParser::proc_UpTime(int pid) { 
  
  string line, value;
  long up_time{0};
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
   if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 1; i <= kstarttime_ ; i++){
      linestream >> value;
    }
    up_time =  std::stol(value)/sysconf(_SC_CLK_TCK);
  }
  return UpTime() - up_time; 
  }

float LinuxParser::Proc_cpuUtilization (int pid){ 
    
    string line, value;
    float total_time{0.0}, seconds,cpu_util,starttime;
    std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 1; i <= kstarttime_ ; i++){
      
      linestream >> value;
      if ((i == kutime_) || (i == kstime_) || (i == kcutime_) || (i == kcstime_)){
        
        total_time += stof(value)/sysconf(_SC_CLK_TCK);
      }
    }
    starttime = stof(value);
    seconds = LinuxParser::UpTime() - starttime/sysconf(_SC_CLK_TCK);
    //seconds = LinuxParser::UpTime() - LinuxParser::proc_UpTime(pid);
    cpu_util = total_time/(seconds);
  }

    return cpu_util;
}