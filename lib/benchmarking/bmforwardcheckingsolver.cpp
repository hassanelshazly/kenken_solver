#include "bmforwardcheckingsolver.h"

BMForwardCheckingSolver::BMForwardCheckingSolver()
    : ForwardCheckingSolver(), Benchmarking() {}

void BMForwardCheckingSolver::solve() {
  start();
  ForwardCheckingSolver::solve();
  end();
}
