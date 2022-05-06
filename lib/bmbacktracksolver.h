#ifndef BMBACKTRACKSOLVER_H
#define BMBACKTRACKSOLVER_H

#include "benchmarking.h"
#include "backtracksolver.h"

class BMBacktrackSolver : public BacktrackSolver, public Benchmarking
{
public:
  BMBacktrackSolver();

  BMBacktrackSolver(KenKenBoard board) : BacktrackSolver(board) {}
  ~BMBacktrackSolver() {}

  void solve() override {
     start();
     BacktrackSolver::solve();
     end();
  }
};

#endif // BMBACKTRACKSOLVER_H
