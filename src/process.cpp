#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"

#include "process.h"
using namespace LinuxParser;

using std::string;
using std::to_string;
using std::vector;

Process::Process(int i) : pid(i){};

// Return this process's ID
int Process::Pid() { return pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return LinuxParser::Proc_cpuUtilization (pid); }

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

//Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::proc_UpTime(pid); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process & a)  { 
    
    bool result;
    (CpuUtilization() < a.CpuUtilization()) ? (result = true) : (result = false);
    
    return result; }

