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
    optional<Cell> next = cell;
    while(next.has_value()) {
      if(get_domian(next.value()).size() == 0)
        return false;
      next = next_cell(next.value());
    }
    return true;
  }
};

#endif // ARCCONSISTENCYSOLVER_H
