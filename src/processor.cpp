#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

using std::string;
using std::vector;
using namespace LinuxParser;

//Return the aggregate CPU utilization
float Processor::Utilization() { 
    
    vector<string> cpu_data = LinuxParser::CpuUtilization();
    vector<long> cpu_info;

    for (auto str : cpu_data){
        cpu_info.push_back(std::stol (str, nullptr));
    }

    float Idle = cpu_info[LinuxParser::kIdle_] + cpu_info[LinuxParser::kIOwait_];
    float NonIdle = cpu_info[LinuxParser::kUser_] + cpu_info[LinuxParser::kNice_] +
                    cpu_info[LinuxParser::kSystem_] + cpu_info[LinuxParser::kIRQ_] +
                    cpu_info[LinuxParser::kSoftIRQ_] + cpu_info[LinuxParser::kSteal_];
                   
 
    float total_cpu_time = Idle + NonIdle;

    float cpu_percentage = ((total_cpu_time - PrevTotal) - (Idle - PrevIdle))/(total_cpu_time - PrevTotal) ;

    PrevTotal  = total_cpu_time;
    PrevIdle = Idle;
    
    return cpu_percentage; 
    }