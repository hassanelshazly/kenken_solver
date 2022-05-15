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

  ArcConsistencySolver(KenKenBoard board) : KenKenSolver(board) {}
  ~ArcConsistencySolver() {}

private:
  virtual vector<uint8_t> get_domian(const Cell& cell) override {
    set<uint8_t> domain = m_board.ar_domain(cell);
    return vector<uint8_t>(domain.begin(), domain.end());
  }

  bool still_vaild(const optional<Cell>& cell) {
    vector<vector<set<uint8_t>>> domains (m_board.size(), vector<set<uint8_t>>(m_board.size()));
    set<Cell> remaining;

    for(uint8_t i = 0; i < m_board.size(); i++)
      for(uint8_t j = 0; j < m_board.size(); j++) {
        domains[i][j] = m_board.ar_domain({i, j});
        if(domains[i][j].empty())
          return false;
        remaining.insert({i, j});
      }


    while(remaining.size()) {
      Cell front_cell = *remaining.begin();
      remaining.erase(remaining.begin());

      if(remove_inconsistent_values(front_cell, domains)) {
        if(domains[front_cell.first][front_cell.second].empty())
          return false;
        for(const Cell& related_cell :m_board.get_constraint(front_cell).cells()) {
          remaining.insert(related_cell);
        }
      }
    }
    return true;
  }

  bool remove_inconsistent_values(const Cell& cell, vector<vector<set<uint8_t>>>& domains) {
    set<uint8_t> domain = domains[cell.first][cell.second];
    Constraint constraint = m_board.get_constraint(cell);
    set<Cell> related_cells = constraint.cells();

    set<uint8_t> reduced_domain;
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
};

#endif // ARCCONSISTENCYSOLVER_H
