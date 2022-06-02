#ifndef KENKENSOLVER_H
#define KENKENSOLVER_H

#include <optional>

#include "lib/board/kenkenboard.h"

class KenKenSolver {
public:
  KenKenSolver();
  KenKenSolver(KenKenBoard board);
  virtual ~KenKenSolver() {}

  virtual void solve();

  KenKenBoard &board();
  void set_board(const KenKenBoard &board);

protected:
  virtual vector<uint8_t> get_domian(const Cell &cell);
  virtual bool check_domains(const optional<Cell> &cell);
  virtual optional<Cell> next_cell(const Cell &cell);

  bool backtrack_solve(const optional<Cell> &opt_cell);

protected:
  KenKenBoard m_board;
};

#endif // KENKENSOLVER_H
