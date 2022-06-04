#include "heuristicfcsolver.h"

HeuristicFCSolver::HeuristicFCSolver() : ForwardCheckingSolver() {}

HeuristicFCSolver::HeuristicFCSolver(KenKenBoard board)
    : ForwardCheckingSolver(board) {}

void HeuristicFCSolver::solve() {
  order();
  backtrack_solve(make_optional(m_first_cell));
  invalidate_order();
}

vector<uint8_t> HeuristicFCSolver::get_domian(const Cell &cell) {
  set<uint8_t> domain = m_board.fc_domain(cell);
  return vector<uint8_t>(domain.rbegin(), domain.rend());
}

optional<Cell> HeuristicFCSolver::next_cell(const Cell &cell) {
  return m_order.find(cell)->second;
}

void HeuristicFCSolver::order() {
  if (m_order.size())
    return;
  vector<ArithmeticConstraint> constraints = m_board.constraints();
  vector<vector<bool>> marked(m_board.size(), vector<bool>(m_board.size()));

  vector<Cell> ordered_cells;
  for (const ArithmeticConstraint &constraint : constraints)
    if (constraint.operation() == ArithmeticConstraint::EQUAL)
      for (const Cell &cell : constraint.cells()) {
        ordered_cells.push_back(cell);
        marked[cell.first][cell.second] = true;
      }

  for (int i = 0; i < m_board.size(); i++)
    for (int j = 0; j < m_board.size(); j++)
      if (!marked[i][j])
        ordered_cells.push_back({i, j});

  if (ordered_cells.empty()) {
    m_first_cell = {0, 0};
    return;
  }

  m_first_cell = ordered_cells.front();
  for (size_t i = 0; i < ordered_cells.size() - 1; i++)
    m_order[ordered_cells[i]] = ordered_cells[i + 1];
  m_order[ordered_cells.back()] = nullopt;
}

void HeuristicFCSolver::invalidate_order() {
  m_first_cell = {0, 0};
  m_order.clear();
}
