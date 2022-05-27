#ifndef HEURISTICARCCONSISTENCYSOLVER_H
#define HEURISTICARCCONSISTENCYSOLVER_H

#include "kenkensolver.h"

class HeuristicSolver : public KenKenSolver
{
public:
  HeuristicSolver();

  HeuristicSolver(KenKenBoard board) : KenKenSolver(board) {}
  ~HeuristicSolver() {}

  void solve() override {
    order();
    backtrack_solve(make_optional(m_first_cell));
    invalidate_order();
  }

private:
  vector<uint8_t> get_domian(const Cell& cell) override {
    set<uint8_t> domain = m_board.fc_domain(cell);
    return vector<uint8_t>(domain.rbegin(), domain.rend());
  }

  optional<Cell> next_cell(const Cell& cell) override {
    auto it = m_order.find(cell);
    if(it != m_order.end())
      return make_optional(it->second);
    return nullopt;
  }

  void order() {
    if(m_order.size())
      return;
    vector<ArithmeticConstraint> constraints = m_board.constraints();
    vector<vector<bool>> marked(m_board.size(), vector<bool>(m_board.size()));

    vector<Cell> ordered_cells;
    for(const ArithmeticConstraint& constraint : constraints)
      if(constraint.operation() == ArithmeticConstraint::EQUAL)
        for(const Cell& cell : constraint.cells()) {
          ordered_cells.push_back(cell);
          marked[cell.first][cell.second] = true;
        }

    for(int i = 0; i < m_board.size(); i++)
      for(int j = 0; j < m_board.size(); j++)
        if(!marked[i][j])
          ordered_cells.push_back({i, j});

    if(ordered_cells.empty()) {
      m_first_cell = {0, 0};
      return;
    }

    m_first_cell = ordered_cells.front();
    for(size_t i = 0; i < ordered_cells.size() - 1; i++)
      m_order[ordered_cells[i]] = ordered_cells[i + 1];
  }

  void invalidate_order() {
    m_first_cell = {0, 0};
    m_order.clear();
  }

private:
  Cell m_first_cell;
  map<Cell, Cell> m_order;
};

#endif // HEURISTICARCCONSISTENCYSOLVER_H
