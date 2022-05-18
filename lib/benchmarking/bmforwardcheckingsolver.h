#ifndef BMFORWARDCHECKINGSOLVER_H
#define BMFORWARDCHECKINGSOLVER_H

#include "benchmarking.h"
#include "lib/solver/forwardcheckingsolver.h"

class BMForwardCheckingSolver : public ForwardCheckingSolver, public Benchmarking
{
public:
  BMForwardCheckingSolver();

  BMForwardCheckingSolver(KenKenBoard board) : ForwardCheckingSolver(board) {}
  ~BMForwardCheckingSolver() {}

  void solve() override {
     start();
     ForwardCheckingSolver::solve();
     end();
  }
};

#endif // BMFORWARDCHECKINGSOLVER_H
