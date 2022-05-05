#include "kenkenboard.h"

#undef DEBUG_CONSTRAINTS

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

KenKenBoard::KenKenBoard() : m_size(MIN_BOARD_SIZE) {}

KenKenBoard::KenKenBoard(uint8_t size)
    : m_size(size), m_board(size, vector<uint8_t>(size)) {
  if (m_size < MIN_BOARD_SIZE || m_size > MAX_BOARD_SIZE) {
    throw InvaildSizeException();
  }
}

uint8_t KenKenBoard::size() const
{
  return m_size;
}
