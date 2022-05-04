#ifndef BENCHMARKINGSOLVER_H
#define BENCHMARKINGSOLVER_H

#include "kenkensolver.h"

#include <QElapsedTimer>
#include <memory>
#include <cmath>

using namespace std;

class BenchmarkingSolver
{
public:
  BenchmarkingSolver(const shared_ptr<KenKenSolver>& solver) : m_solver(solver) {}

  void start() {
    timer.start();
  }

  void solve() {
    start();
    m_solver->solve();
    end();
  }

  void end() {
    msecs = timer.elapsed();
    timer.invalidate();
  }

  int64_t measured_msecs() {
    return msecs;
  }

  int64_t measured_secs() {
    return round(msecs / 1000.0);
  }

  KenKenSolver* operator ->() {
    return m_solver.get();
  }

private:
  shared_ptr<KenKenSolver> m_solver;
  QElapsedTimer timer;
  qint64 msecs;
};

#endif // BENCHMARKINGSOLVER_H
