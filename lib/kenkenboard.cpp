#include "kenkenboard.h"

KenKenBoard::KenKenBoard() : m_size(MIN_BOARD_SIZE)
{

}

uint8_t KenKenBoard::size() const
{
  return m_size;
}

vector<Constraint> KenKenBoard::constraints() const
{
  return m_constraints;
}

QDebug operator<<(QDebug dbg, const KenKenBoard &board) {
  for (int i = 0; i < board.m_size; i++) {
    for (int j = 0; j < board.m_size; j++)
      dbg.nospace() << board.m_board[i][j] << " ";
    dbg.nospace() << "\n";
  }

#ifdef DEBUG_CONSTRAINTS
  for (size_t i = 0; i < board.m_constraints.size(); i++)
    dbg.nospace() << "Constraint #" << i << "\n" << board.m_constraints[i];
#endif
  return dbg;
}
