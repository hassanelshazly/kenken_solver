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
    set<uint8_t> domain = m_board.ar_domain(cell);
    return vector<uint8_t>(domain.begin(), domain.end());
  }

  bool still_vaild(const optional<Cell>& cell) {
    optional<Cell> next = cell;
    while(next.has_value()) {
      if(get_domian(next.value()).size() == 0)
        return false;
      next = next_cell(next.value());
    }
    return true;
  }
};

#endif // FORWARDCHECKINGSOLVER_H
