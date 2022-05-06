#ifndef ARCCONSISTENCYSOLVER_H
#define ARCCONSISTENCYSOLVER_H

#include "kenkensolver.h"

class ArcConsistencySolver : public KenKenSolver
{
public:
  ArcConsistencySolver();

  ArcConsistencySolver(KenKenBoard board) : KenKenSolver(board) {}
  virtual ~ArcConsistencySolver() {}

private:
  set<uint8_t> get_domian(const Cell& cell) override {
    return m_board.ar_domain(cell);
  }
};

#endif // ARCCONSISTENCYSOLVER_H
