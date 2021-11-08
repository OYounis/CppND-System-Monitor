#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<int> vec = LinuxParser::CpuUtilization();

    int Idle   = vec[3] + vec[4];
    int NonIdle = vec[0] + vec[1] + vec[2] + vec[5] + vec[6] + vec[7];
    int total   = Idle + NonIdle;

    int totald = total - total_;
    int idled  = Idle  - idle_; 

    idle_    = Idle;
    nonidle_ = NonIdle;
    total_   = total;
    
    return float(totald -  idled) / float(totald);
}