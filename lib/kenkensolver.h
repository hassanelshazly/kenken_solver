#ifndef KENKENSOLVER_H
#define KENKENSOLVER_H

#include "kenkenboard.h"

class KenKenSolver
{
public:
  KenKenSolver();

  KenKenSolver(KenKenBoard board) : m_board(board) {
    assert(board.vaild_board());
  }

  virtual ~KenKenSolver(){}

  virtual void solve() {}

  KenKenBoard board() const;
  void set_board(const KenKenBoard &board);

protected:
  KenKenBoard m_board;

  optional<Cell> next_cell(const Cell& cell) {
    if(cell.second + 1 < m_board.size())
      return optional<Cell>({cell.first, cell.second + 1});
    else if(cell.first + 1 < m_board.size())
      return optional<Cell>({cell.first + 1, 0});

    return nullopt;
  }

};

#endif // KENKENSOLVER_H
