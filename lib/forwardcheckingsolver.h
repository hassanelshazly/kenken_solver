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
  vector<uint8_t> get_domian(const Cell& cell) override {
    set<uint8_t> domain = m_board.fd_domain(cell);
    return vector<uint8_t>(domain.begin(), domain.end());
  }
};

#endif // FORWARDCHECKINGSOLVER_H
