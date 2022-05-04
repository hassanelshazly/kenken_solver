#ifndef KENKENBOARD_H
#define KENKENBOARD_H

#include <QDebug>
#include <exception>
#include <inttypes.h>
#include <set>
#include <vector>

#include "constraint.h"

using namespace std;

class KenKenBoard {
public:
  KenKenBoard();
  KenKenBoard(uint8_t size);

  bool vaild_solution() {
    if (!check_rows_and_cols())
      return false;
    if (!follow_constraints())
      return false;
    return true;
  }

  bool follow_constraints() {
    for (const Constraint &constraint : m_constraints)
      if (!follow_constraint(constraint))
        return false;
    return true;
  }

  uint8_t get(const Cell &cell) { return m_board[cell.first][cell.second]; }

  bool belongs_to_constraint(const Cell &cell) {
    for (const Constraint &constraint : m_constraints)
      if (constraint.includes(cell))
        return true;
    return false;
  }

  void set(const Cell &cell, uint8_t value) {
    check_cell(cell);
    check_value(value);
    m_board[cell.first][cell.second] = value;
  }

  void set_board(const vector<vector<uint8_t>> &board) {
    assert(board.size() == m_size);
    for (const auto &row : board) {
      assert(row.size() == m_size);
      for (const auto &value : row)
        check_value(value);
    }
    m_board = board;
  }

  void add_constraint(const Constraint &constraint) {
    check_constraint(constraint);
    m_constraints.push_back(constraint);
  }

  class InvaildSizeException : exception {};
  class InvaildCellException : exception {};
  class InvaildValueException : exception {};
  class InvaildConstraintException : exception {};

  friend QDebug operator<<(QDebug dbg, const KenKenBoard &board);

private:
  void check_cell(const Cell &cell) {
    if (cell.first >= m_size || cell.second >= m_size)
      throw InvaildCellException();
  }

  void check_value(uint8_t value) {
    if (value == 0 || value > m_size)
      throw InvaildValueException();
  }

  void check_constraint(const Constraint &constraint) {
    if (!constraint.vaild())
      throw InvaildConstraintException();

    for (const Cell &cell : constraint.cells()) {
      check_cell(cell);
      if (belongs_to_constraint(cell))
        throw InvaildConstraintException();
    }
  }

  bool follow_constraint(const Constraint &constraint) {
    vector<uint8_t> values;
    for (const Cell &cell : constraint.cells()) {
      values.push_back(get(cell));
    }
    return constraint.vaild_values(values);
  }

  bool check_rows_and_cols() {
    std::set<uint8_t> st;
    for (int type = 0; type < 2; type++) {
      for (int i = 0; i < m_size; i++) {
        st.clear();
        for (int j = 0; j < m_size; j++) {
          uint8_t val = type ? m_board[i][j] : m_board[j][i];
          if (val == 0)
            return false;
          st.insert(val);
        }
        if (st.size() != m_size)
          return false;
      }
    }
    return true;
  }

  bool missed_cells() {
    map<Cell, int> cells_mp;
    for (const Constraint &constraint : m_constraints)
      for (const Cell &cell : constraint.cells())
        cells_mp[cell]++;
    if (cells_mp.size() != m_size * m_size)
      return true;

    for (const auto &element : cells_mp)
      if (element.second != 1)
        return true;

    return false;
  }

private:
  uint8_t m_size;
  vector<vector<uint8_t>> m_board;
  vector<Constraint> m_constraints;

private:
  static constexpr uint8_t MIN_BOARD_SIZE = 2;
  static constexpr uint8_t MAX_BOARD_SIZE = 16;
};

#endif // KENKENBOARD_H
