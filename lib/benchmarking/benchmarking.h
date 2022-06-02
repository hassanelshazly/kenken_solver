#ifndef BENCHMARKING_H
#define BENCHMARKING_H

#include <QElapsedTimer>
#include <cmath>

class Benchmarking {
public:
  Benchmarking();

  void start();
  void end();

  int64_t measured_nsecs();
  int64_t measured_usecs();
  int64_t measured_msecs();
  int64_t measured_secs();

private:
  QElapsedTimer timer;
  qint64 msecs = 0;
  qint64 nsecs = 0;
};

#endif // BENCHMARKING_H
