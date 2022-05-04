#include "kenkensolver.h"

KenKenSolver::KenKenSolver()
{

}

KenKenBoard KenKenSolver::board() const
{
  return m_board;
}

void KenKenSolver::set_board(const KenKenBoard &board)
{
  assert(board.vaild_board());
  m_board = board;
}
