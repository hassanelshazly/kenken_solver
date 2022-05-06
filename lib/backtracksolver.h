#ifndef BACKTRACKSOLVER_H
#define BACKTRACKSOLVER_H

#include "kenkensolver.h"

class BacktrackSolver : public KenKenSolver
{
public:
  BacktrackSolver();
  BacktrackSolver(KenKenBoard board) : KenKenSolver(board) {}
  ~BacktrackSolver() {}

private:
   set<uint8_t> get_domian(const Cell& cell) override {
    return m_board.total_domain();
  }
};

#endif // BACKTRACKSOLVER_H
