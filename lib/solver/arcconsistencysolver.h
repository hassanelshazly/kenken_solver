#ifndef ARCCONSISTENCYSOLVER_H
#define ARCCONSISTENCYSOLVER_H

#include "kenkensolver.h"

class ArcConsistencySolver : public KenKenSolver {
public:
  ArcConsistencySolver();

  ArcConsistencySolver(KenKenBoard board);
  ~ArcConsistencySolver() {}

  virtual void solve() override;

private:
  bool ac_solve(const optional<Cell> &cell,
                vector<vector<set<uint8_t>>> domains);

  bool ac3(Cell cell, vector<vector<set<uint8_t>>> &domains);
  bool remove_inconsistent_values(Cell xi, Cell xj,
                                  vector<vector<set<uint8_t>>> &domains);
  bool can_satisfy(Cell xi, uint8_t val_i, Cell xj, uint8_t val_j);

  void initialize_arcs();
  void add_not_equal_arcs(Cell cell);
  void add_arithmetic_arcs(Cell cell);
  void clear_arcs();

  void print_domains(const vector<vector<set<uint8_t>>> &domains);

protected:
  map<Cell, map<Cell, vector<function<bool(uint8_t, uint8_t)>>>> arcs;
};

#endif // ARCCONSISTENCYSOLVER_H
