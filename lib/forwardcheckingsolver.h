#ifndef FORWARDCHECKINGSOLVER_H
#define FORWARDCHECKINGSOLVER_H

#include "kenkensolver.h"

class ForwardCheckingSolver : public KenKenSolver
{
public:
  ForwardCheckingSolver();
  ForwardCheckingSolver(KenKenBoard board) : KenKenSolver(board) {}
  virtual ~ForwardCheckingSolver() {}

private:
  set<uint8_t> get_domian(const Cell& cell) override {
    return m_board.fd_domain(cell);
  }
};

#endif // FORWARDCHECKINGSOLVER_H
