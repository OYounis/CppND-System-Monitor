#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    int idle_ = 0;
    int nonidle_ = 0;
    int total_ = 0;
};

#endif