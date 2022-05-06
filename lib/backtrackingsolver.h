#ifndef BACKTRACKINGSOLVER_H
#define BACKTRACKINGSOLVER_H

#include "kenkensolver.h"

class BacktrackingSolver : public KenKenSolver
{
public:
  BacktrackingSolver();
  BacktrackingSolver(KenKenBoard board) : KenKenSolver(board) {}
  virtual ~BacktrackingSolver() {}

private:
   set<uint8_t> get_domian(const Cell& cell) override {
    return m_board.total_domain();
  }
};

#endif // BACKTRACKINGSOLVER_H
