#ifndef FORWARDCHECKINGSOLVER_H
#define FORWARDCHECKINGSOLVER_H

#include "kenkensolver.h"

class ForwardCheckingSolver : public KenKenSolver
{
public:
  ForwardCheckingSolver();
  ForwardCheckingSolver(KenKenBoard board) : KenKenSolver(board) {}
  virtual ~ForwardCheckingSolver() {}

protected:
  vector<uint8_t> get_domian(const Cell& cell) override {
    set<uint8_t> domain = m_board.fc_domain(cell);
    return vector<uint8_t>(domain.begin(), domain.end());
  }

  bool check_domains(const optional<Cell>& cell) override {
    if(!cell)
      return true;
    vector<Cell> cells;
    for(uint8_t i = cell->first + 1; i < m_board.size(); i++) {
      cells.push_back({i, cell->second});
    }

    for(uint8_t i = cell->second + 1; i < m_board.size(); i++) {
      cells.push_back({cell->first, i});
    }

    for(Cell related_cell : m_board.get_constraint(*cell).cells()) {
      if(related_cell != *cell)
        cells.push_back(related_cell);
    }

    for(Cell related_cell : cells) {
      if(m_board.fc_domain(related_cell).size() == 0)
        return false;
    }
    return true;
  }
};

#endif // FORWARDCHECKINGSOLVER_H
