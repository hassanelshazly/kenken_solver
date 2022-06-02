#include "bmbacktrackingsolver.h"

BMBacktrackingSolver::BMBacktrackingSolver()
  : BacktrackingSolver(), Benchmarking() {}

void BMBacktrackingSolver::solve() {
  start();
  BacktrackingSolver::solve();
  end();
}
