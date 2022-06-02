#ifndef BMHEURISTICARCCONSISTENCYSOLVER_H
#define BMHEURISTICARCCONSISTENCYSOLVER_H

#include "benchmarking.h"
#include "lib/solver/heuristicfcsolver.h"

class BMHeuristicFCSolver : public HeuristicFCSolver, public Benchmarking {
public:
  BMHeuristicFCSolver();

  BMHeuristicFCSolver(KenKenBoard board) : HeuristicFCSolver(board) {}
  ~BMHeuristicFCSolver() {}

  void solve() override;
};

#endif // BMHEURISTICARCCONSISTENCYSOLVER_H
