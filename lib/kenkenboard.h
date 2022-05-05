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

  bool valid_solution() const {
    if (!check_rows_and_cols())
      return false;
    if (!satisfy_constraints())
      return false;
    if(missed_cells())
      return false;
    return true;
  }

  bool complete_solution() const {
    for (int i = 0; i < m_size; i++)
      for (int j = 0; j < m_size; j++)
        if(m_board[i][j] == 0)
          return false;
    return true;
  }

  bool valid_board() const {
    try {
      for (const Constraint &constraint : m_constraints)
        if(!valid_constraint(constraint))
          return false;
    } catch (const exception& ex) {
      return false;
    }

    if(missed_cells())
      return false;
    return true;
  }

  bool satisfy_constraints() const {
    for (const Constraint &constraint : m_constraints)
      if (!satisfy_constraint(constraint))
        return false;
    return true;
  }

  uint8_t get(const Cell &cell) const {
    check_cell(cell);
    return m_board[cell.first][cell.second];
  }

  void clear(const Cell &cell) {
    check_cell(cell);
    m_board[cell.first][cell.second] = 0;
  }

  void clear() {
    for (int i = 0; i < m_size; i++)
      for (int j = 0; j < m_size; j++)
        m_board[i][j] = 0;
  }

  bool belongs_to_constraint(const Cell &cell) const {
    for (const Constraint &constraint : m_constraints)
      if (constraint.includes(cell))
        return true;
    return false;
  }

  Constraint get_constraint(const Cell &cell) const {
    for (const Constraint &constraint : m_constraints)
      if (constraint.includes(cell))
        return constraint;
    throw InvalidCellException();
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

  vector<uint8_t> cell_domain(const Cell& cell) const {
    check_cell(cell);
    std::set<uint8_t> domain = constraint_domain(cell);

    vector<uint8_t> total_domain;
    for(const uint8_t value : rc_domain(cell)) {
      if(domain.count(value))
        total_domain.push_back(value);
    }

    return total_domain;
  }

  std::set<uint8_t> constraint_domain(const Cell& cell) const {
    check_cell(cell);
    Constraint constraint = get_constraint(cell);
    vector<uint8_t> values = get_values(constraint);
    return  constraint.get_domain(values, m_size);
  }

  std::set<uint8_t> rc_domain(const Cell& cell) const {
    check_cell(cell);
    std::set<uint8_t> domain;
    for(int i = 1; i <= m_size; i++)
      domain.insert(i);

    for(int i = 0; i < m_size; i++){
      domain.erase(m_board[cell.first][i]);
      domain.erase(m_board[i][cell.second]);
    }
    return domain;
  }

  class InvalidSizeException : exception {};
  class InvalidCellException : exception {};
  class InvalidValueException : exception {};
  class InvalidConstraintException : exception {};

  friend QDebug operator<<(QDebug dbg, const KenKenBoard &board);

  uint8_t size() const;

  vector<Constraint> constraints() const;

private:
  void check_cell(const Cell &cell) const {
    if (cell.first >= m_size || cell.second >= m_size)
      throw InvalidCellException();
  }

  void check_value(uint8_t value) const {
    if (value == 0 || value > m_size)
      throw InvalidValueException();
  }

  void check_constraint(const Constraint &constraint) const {
    if (!valid_constraint(constraint))
      throw InvalidConstraintException();

    for (const Cell &cell : constraint.cells()) {
      if (belongs_to_constraint(cell))
        throw InvalidConstraintException();
    }
  }

  bool valid_constraint(const Constraint &constraint) const {
    if (!constraint.valid())
      return false;

    try {
      for (const Cell &cell : constraint.cells())
        check_cell(cell);
    } catch (const InvalidCellException& ex) {
      return false;
    }
    return true;
  }

  bool satisfy_constraint(const Constraint &constraint) const {
    vector<uint8_t> values = get_values(constraint);
    return constraint.satisfy(values);
  }

  vector<uint8_t> get_values(const Constraint &constraint) const {
    vector<uint8_t> values;
    for (const Cell &cell : constraint.cells()) {
      uint8_t value = m_board[cell.first][cell.second];
      if(value != 0)
        values.push_back(value);
    }
    return values;
  }

  bool check_rows_and_cols() const {
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

  bool missed_cells() const{
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
