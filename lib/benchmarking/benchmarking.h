#ifndef BENCHMARKING_H
#define BENCHMARKING_H

#include <QElapsedTimer>
#include <cmath>

class Benchmarking
{
public:
  Benchmarking();

  void start() {
    timer.start();
  }

  void end() {
    msecs = timer.elapsed();
    nsecs = timer.nsecsElapsed();
    timer.invalidate();
  }

  int64_t measured_nsecs() {
    return nsecs;
  }

  int64_t measured_usecs() {
    return round(nsecs / 1000.0);
  }

  int64_t measured_msecs() {
    return msecs;
  }

  int64_t measured_secs() {
    return round(msecs / 1000.0);
  }

private:
  QElapsedTimer timer;
  qint64 msecs = 0;
  qint64 nsecs = 0;
};

#endif // BENCHMARKING_H
