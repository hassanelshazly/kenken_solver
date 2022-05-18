#ifndef BMBACKTRACKINGSOLVER_H
#define BMBACKTRACKINGSOLVER_H

#include "benchmarking.h"
#include "lib/solver/backtrackingsolver.h"

class BMBacktrackingSolver : public BacktrackingSolver, public Benchmarking
{
public:
  BMBacktrackingSolver();

  BMBacktrackingSolver(KenKenBoard board) : BacktrackingSolver(board) {}
  ~BMBacktrackingSolver() {}

  void solve() override {
     start();
     BacktrackingSolver::solve();
     end();
  }
};

#endif // BMBACKTRACKINGSOLVER_H
