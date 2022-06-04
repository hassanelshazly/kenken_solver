#ifndef HEURISTICARCCONSISTENCYSOLVER_H
#define HEURISTICARCCONSISTENCYSOLVER_H

#include "forwardcheckingsolver.h"

class HeuristicFCSolver : public ForwardCheckingSolver {
public:
  HeuristicFCSolver();

  HeuristicFCSolver(KenKenBoard board);
  ~HeuristicFCSolver() {}

  void solve() override;

private:
  vector<uint8_t> get_domian(const Cell &cell) override;
  optional<Cell> next_cell(const Cell &cell) override;

  void order();
  void invalidate_order();

private:
  Cell m_first_cell;
  map<Cell, optional<Cell>> m_order;
};

#endif // HEURISTICARCCONSISTENCYSOLVER_H
