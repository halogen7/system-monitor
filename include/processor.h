#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  double Utilization();  // See src/processor.cpp

  // Declare any necessary private members
 private:
 int _prev_idle = 0;
 int _prev_wait = 0;
 int _prev_non_idle = 0;
 int _prev_total = 0;
};

#endif