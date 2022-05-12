#include "kenkensolver.h"

KenKenSolver::KenKenSolver()
{

}

KenKenBoard& KenKenSolver::board()
{
  return m_board;
}

void KenKenSolver::set_board(const KenKenBoard &board)
{
//  assert(board.valid_board());
  m_board = board;
}
