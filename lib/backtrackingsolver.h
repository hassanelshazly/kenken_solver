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
   vector<uint8_t> get_domian(const Cell& cell) override {
    set<uint8_t> domain = m_board.total_domain();
    return vector<uint8_t>(domain.begin(), domain.end());
  }
};

#endif // BACKTRACKINGSOLVER_H
