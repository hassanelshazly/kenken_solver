#ifndef BMHEURISTICARCCONSISTENCYSOLVER_H
#define BMHEURISTICARCCONSISTENCYSOLVER_H

#include "benchmarking.h"
#include "lib/solver/heuristicsolver.h"

class BMHeuristicSolver : public HeuristicSolver, public Benchmarking
{
public:
  BMHeuristicSolver();

  BMHeuristicSolver(KenKenBoard board) : HeuristicSolver(board) {}
  ~BMHeuristicSolver() {}

  void solve() override {
     start();
     HeuristicSolver::solve();
     end();
  }
};

#endif // BMHEURISTICARCCONSISTENCYSOLVER_H
