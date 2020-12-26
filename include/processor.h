#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  //     See src/processor.cpp

  //     Declare any necessary private members
 
 private:

    float PrevTotal{0.0}, PrevIdle{0.0};

};

#endif