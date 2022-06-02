#include "kenkensolver.h"

KenKenSolver::KenKenSolver() {}

KenKenSolver::KenKenSolver(KenKenBoard board) : m_board(board) {
  assert(board.valid_board());
}

void KenKenSolver::solve() { backtrack_solve(make_optional(make_pair(0, 0))); }

KenKenBoard &KenKenSolver::board() { return m_board; }

void KenKenSolver::set_board(const KenKenBoard &board) {
  assert(board.valid_board());
  m_board = board;
}

vector<uint8_t> KenKenSolver::get_domian(const Cell &cell) {
  set<uint8_t> domain = m_board.total_domain();
  return vector<uint8_t>(domain.begin(), domain.end());
}

bool KenKenSolver::check_domains(const optional<Cell> &cell) { return true; }

optional<Cell> KenKenSolver::next_cell(const Cell &cell) {
  if (cell.second + 1 < m_board.size())
    return make_optional(make_pair(cell.first, cell.second + 1));

  if (cell.first + 1 < m_board.size())
    return make_optional(make_pair(cell.first + 1, 0));

  return nullopt;
}

bool KenKenSolver::backtrack_solve(const optional<Cell> &opt_cell) {
  if (m_board.valid_solution())
    return true;

  if (!opt_cell || m_board.complete_solution())
    return false;

  Cell cell = opt_cell.value();
  for (uint8_t i : get_domian(cell)) {
    m_board.assign(cell, i);

    if (m_board.vaild_assignment(cell) && check_domains(cell)) {
      if (backtrack_solve(next_cell(cell)))
        return true;
    }

    m_board.unassign(cell);
  }

  return false;
}
