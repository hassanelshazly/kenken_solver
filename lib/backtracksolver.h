#ifndef KENKENBACKTRACKSOLVER_H
#define KENKENBACKTRACKSOLVER_H

#include "kenkensolver.h"

class BacktrackSolver : public KenKenSolver
{
public:
  BacktrackSolver();

  BacktrackSolver(KenKenBoard board) : KenKenSolver(board) {}

  ~BacktrackSolver() {}

  void solve() override {
    backtrack_solve(optional<Cell>({0, 0}));
  }

private:

  bool backtrack_solve(const optional<Cell>& opt_cell) {
//    qDebug() << "Vaild:" << m_board.vaild_solution() ;
//    qDebug() << "Compelete:" << m_board.complete_solution() ;
//    qDebug() << m_board;

    if(m_board.vaild_solution()) {
      return true;
    }

    if(m_board.complete_solution())
      return false;

    if(!opt_cell.has_value())
      return false; // No solution TODO: find a better way

    Cell cell = opt_cell.value();
    for(int i = 1; i <= m_board.size(); i++) {
      m_board.set(cell, i);
      if(backtrack_solve(next_cell(cell)))
        return true;
      else
        m_board.clear(cell);
    }

     return false;
  }
};

#endif // KENKENBACKTRACKSOLVER_H
