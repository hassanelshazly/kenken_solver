#ifndef BMHEURISTICARCCONSISTENCYSOLVER_H
#define BMHEURISTICARCCONSISTENCYSOLVER_H

#include "benchmarking.h"
#include "lib/solver/heuristicarcconsistencysolver.h"

class BMHeuristicArcConsistencySolver : public HeuristicArcConsistencySolver, public Benchmarking
{
public:
  BMHeuristicArcConsistencySolver();

  BMHeuristicArcConsistencySolver(KenKenBoard board) : HeuristicArcConsistencySolver(board) {}
  ~BMHeuristicArcConsistencySolver() {}

  void solve() override {
     start();
     HeuristicArcConsistencySolver::solve();
     end();
  }
};

#endif // BMHEURISTICARCCONSISTENCYSOLVER_H
