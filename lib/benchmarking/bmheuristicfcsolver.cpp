#include "bmheuristicfcsolver.h"

BMHeuristicFCSolver::BMHeuristicFCSolver()
  : HeuristicFCSolver(), Benchmarking() {}

void BMHeuristicFCSolver::solve() {
  start();
  HeuristicFCSolver::solve();
  end();
}
