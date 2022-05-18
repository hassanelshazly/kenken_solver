#ifndef BMARCCONSISTENCYSOLVER_H
#define BMARCCONSISTENCYSOLVER_H

#include "benchmarking.h"
#include "lib/solver/arcconsistencysolver.h"

class BMArcConsistencySolver : public ArcConsistencySolver, public Benchmarking
{
public:
  BMArcConsistencySolver();

  BMArcConsistencySolver(KenKenBoard board) : ArcConsistencySolver(board) {}
  ~BMArcConsistencySolver() {}

  void solve() override {
     start();
     ArcConsistencySolver::solve();
     end();
  }
};

#endif // BMARCCONSISTENCYSOLVER_H
