#ifndef FORWARDCHECKINGSOLVER_H
#define FORWARDCHECKINGSOLVER_H

#include "kenkensolver.h"

class ForwardCheckingSolver : public KenKenSolver
{
public:
  ForwardCheckingSolver();
  ForwardCheckingSolver(KenKenBoard board) : KenKenSolver(board) {}
  ~ForwardCheckingSolver() {}

  void solve() override {
    backtrack_solve(optional<Cell>({0, 0}));
  }

private:
  bool backtrack_solve(const optional<Cell>& opt_cell) {
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
    for(int i : m_board.cell_domain(cell)) {
      m_board.set(cell, i);
      if(backtrack_solve(next_cell(cell)))
        return true;
      else
        m_board.clear(cell);
    }

    return false;
  }
};

#endif // FORWARDCHECKINGSOLVER_H
