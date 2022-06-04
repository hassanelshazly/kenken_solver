#include "forwardcheckingsolver.h"

ForwardCheckingSolver::ForwardCheckingSolver() : KenKenSolver() {}

vector<uint8_t> ForwardCheckingSolver::get_domian(const Cell &cell) {
  set<uint8_t> domain = m_board.fc_domain(cell);
  return vector<uint8_t>(domain.begin(), domain.end());
}

bool ForwardCheckingSolver::check_domains(const optional<Cell> &cell) {
  if (!cell)
    return true;
  vector<Cell> cells;
  for (uint8_t i = cell->first + 1; i < m_board.size(); i++) {
    cells.push_back({i, cell->second});
  }

  for (uint8_t i = cell->second + 1; i < m_board.size(); i++) {
    cells.push_back({cell->first, i});
  }

  for (Cell related_cell : m_board.get_constraint(*cell).cells()) {
    if (related_cell != *cell)
      cells.push_back(related_cell);
  }

  for (Cell related_cell : cells) {
    if (m_board.fc_domain(related_cell).size() == 0)
      return false;
  }
  return true;
}
