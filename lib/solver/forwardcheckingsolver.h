#ifndef FORWARDCHECKINGSOLVER_H
#define FORWARDCHECKINGSOLVER_H

#include "kenkensolver.h"

class ForwardCheckingSolver : public KenKenSolver {
public:
  ForwardCheckingSolver();
  ForwardCheckingSolver(KenKenBoard board) : KenKenSolver(board) {}
  virtual ~ForwardCheckingSolver() {}

protected:
  vector<uint8_t> get_domian(const Cell &cell) override;
  bool check_domains(const optional<Cell> &cell) override;
};

#endif // FORWARDCHECKINGSOLVER_H
