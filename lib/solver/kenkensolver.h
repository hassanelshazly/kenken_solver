#ifndef KENKENSOLVER_H
#define KENKENSOLVER_H

#include "lib/board/kenkenboard.h"

class KenKenSolver
{
public:
  KenKenSolver();
  KenKenSolver(KenKenBoard board) : m_board(board) {
    assert(board.valid_board());
  }
  virtual ~KenKenSolver(){}

  virtual void solve() {
     backtrack_solve(optional<Cell>({0, 0}));
  }

  KenKenBoard &board();
  void set_board(const KenKenBoard &board);

protected:
  virtual vector<uint8_t> get_domian(const Cell& cell) = 0;

  virtual bool still_vaild(const optional<Cell>& cell) {
    return true;
  }

  virtual optional<Cell> next_cell(const Cell& cell)  {
    if(cell.second + 1 < m_board.size())
      return optional<Cell>({cell.first, cell.second + 1});
    else if(cell.first + 1 < m_board.size())
      return optional<Cell>({cell.first + 1, 0});

    return nullopt;
  }

protected:
  bool backtrack_solve(const optional<Cell>& opt_cell) {
//#define DEBUG_SOLVER
#ifdef DEBUG_SOLVER
    qDebug() << "Valid:" << m_board.valid_solution() ;
    qDebug() << "Compelete:" << m_board.complete_solution() ;
    qDebug() << m_board;
#endif

    if(m_board.valid_solution())
      return true;

    if(m_board.complete_solution())
      return false;

    if(!opt_cell.has_value())
      return false;

    Cell cell = opt_cell.value();
    for(uint8_t i : get_domian(cell)) {
      m_board.set_value(cell, i);
//      qDebug() << m_board;

      if(!still_vaild(next_cell(cell))) {
        m_board.clear(cell);
        continue;
      }

      if(backtrack_solve(next_cell(cell)))
        return true;

      m_board.clear(cell);
    }

    return false;
  }

protected:
  KenKenBoard m_board;

};

#endif // KENKENSOLVER_H
