#ifndef ARCCONSISTENCYSOLVER_H
#define ARCCONSISTENCYSOLVER_H

#include "kenkensolver.h"

#include <queue>

template<typename value_t>
QDebug operator<<(QDebug dbg, const set<value_t> &st) {
  for(const value_t& val : st)
    dbg.nospace() << val << " ";
  dbg.nospace() << "\n";
  return dbg;
}


class ArcConsistencySolver : public KenKenSolver
{
public:
  ArcConsistencySolver();

  ArcConsistencySolver(KenKenBoard board)
    : KenKenSolver(board),  domains(m_board.size(), vector<set<uint8_t>>(m_board.size())) {
    for(uint8_t i = 0; i < m_board.size(); i++)
      for(uint8_t j = 0; j < m_board.size(); j++)
        domains[i][j] = m_board.total_domain();
  }
  ~ArcConsistencySolver() {}

  virtual void solve() override {
    ac_solve(make_optional(make_pair(0, 0)));
  }

private:
  virtual vector<uint8_t> get_domian(const Cell& cell) override {
    set<uint8_t> domain = m_board.ar_domain(cell);
    return vector<uint8_t>(domain.begin(), domain.end());
  }

protected:
  bool ac_solve(const optional<Cell>& cell) {
    if(m_board.valid_solution())
      return true;

    if(!cell || m_board.complete_solution() || !ac3(*cell))
      return false;

    for(uint8_t i : get_domian(*cell)) {
      m_board.assign(*cell, i);

    if(ac_solve(next_cell(*cell)))
       return true;

      m_board.unassign(*cell);
    }

    return false;
  }

  bool ac3(const optional<Cell>& cell) {
    if(!cell)
      return true;

    vector<Cell> cells = m_board.related_cells(*cell);
    set<Cell> remaining;
    while(remaining.size()) {
      Cell front_cell = *remaining.begin();
      remaining.erase(remaining.begin());

      if(remove_inconsistent_values(front_cell)) {
        if(domains[front_cell.first][front_cell.second].empty())
          return false;
        for(const Cell& related_cell :m_board.get_constraint(front_cell).cells()) {
          remaining.insert(related_cell);
        }
      }
    }
    return true;
  }

  bool remove_inconsistent_values(const Cell& cell) {
    set<uint8_t> domain = domains[cell.first][cell.second];
    ArithmeticConstraint constraint = m_board.get_constraint(cell);
    set<Cell> related_cells =  constraint.cells();

    set<uint8_t> reduced_domain;
    related_cells.erase(cell);
    if(related_cells.empty()) {
      reduced_domain.insert(constraint.result());
    }

    for(const Cell& related_cell : related_cells) {
      for(const uint8_t& val_i : domain) {
        set<uint8_t> related_cell_domain = domains[related_cell.first][related_cell.second];
        for(const uint8_t& val_j : related_cell_domain) {
          if(constraint.can_satisfy(vector<uint8_t>{val_i, val_j}))
            reduced_domain.insert(val_i);
        }
      }
    }

    domains[cell.first][cell.second] = reduced_domain;
    return reduced_domain.size() != domain.size();
  }

protected:
  vector<vector<set<uint8_t>>> domains;
};

#endif // ARCCONSISTENCYSOLVER_H
