#ifndef ARCCONSISTENCYSOLVER_H
#define ARCCONSISTENCYSOLVER_H

#include "kenkensolver.h"

class ArcConsistencySolver : public KenKenSolver
{
public:
  ArcConsistencySolver();

  ArcConsistencySolver(KenKenBoard board) : KenKenSolver(board) {}
  ~ArcConsistencySolver() {}

private:
  vector<uint8_t> get_domian(const Cell& cell) override {
    set<uint8_t> domain = m_board.ar_domain(cell);
    return vector<uint8_t>(domain.begin(), domain.end());
  }
};

#endif // ARCCONSISTENCYSOLVER_H
