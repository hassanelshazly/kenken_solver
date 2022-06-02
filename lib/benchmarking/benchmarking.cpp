#include "benchmarking.h"

Benchmarking::Benchmarking() {}

void Benchmarking::start() {
  timer.start();
}

void Benchmarking::end() {
  msecs = timer.elapsed();
  nsecs = timer.nsecsElapsed();
  timer.invalidate();
}

int64_t Benchmarking::measured_nsecs() {
  return nsecs;
}

int64_t Benchmarking::measured_usecs() {
  return round(nsecs / 1000.0);
}

int64_t Benchmarking::measured_msecs() {
  return msecs;
}

int64_t Benchmarking::measured_secs() {
  return round(msecs / 1000.0);
}
