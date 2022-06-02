#include "bmarcconsistencysolver.h"

BMArcConsistencySolver::BMArcConsistencySolver()
  : ArcConsistencySolver(), Benchmarking() {}

void BMArcConsistencySolver::solve() {
  start();
  ArcConsistencySolver::solve();
  end();
}
