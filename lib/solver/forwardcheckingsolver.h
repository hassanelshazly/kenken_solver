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

  bool check_domains(const optional<Cell>& cell) {
    if(!cell)
      return true;
    vector<Cell> cells = m_board.related_cells(*cell);
    for(const Cell& related_cell : cells) {
      if(m_board.ar_domain(related_cell).size() == 0)
        return false;
    }
    return true;
  }
};

#endif // FORWARDCHECKINGSOLVER_H
