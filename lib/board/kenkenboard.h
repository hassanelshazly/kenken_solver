#ifndef KENKENBOARD_H
#define KENKENBOARD_H

#include <QDebug>
#include <exception>
#include <inttypes.h>
#include <set>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <unordered_map>
#include <QFile>

#include "lib/constraint/arithmeticconstraint.h"

using namespace std;

class KenKenBoard {
public:
  KenKenBoard();
  KenKenBoard(uint8_t size)
    : m_size(size), m_board(size, vector<uint8_t>(size)) {
    if (m_size < MIN_BOARD_SIZE || m_size > MAX_BOARD_SIZE) {
      throw InvalidSizeException();
    }
  }

  bool valid_solution() const {
    if (!check_rows_and_cols())
      return false;
    if (!satisfy_constraints())
      return false;
    if (missed_cells())
      return false;
    return true;
  }

  bool complete_solution() const {
    for (int i = 0; i < m_size; i++)
      for (int j = 0; j < m_size; j++)
        if (m_board[i][j] == 0)
          return false;
    return true;
  }

  bool valid_board() const {
    try {
      for (const ArithmeticConstraint &constraint : m_constraints)
        if (!valid_constraint(constraint))
          return false;
    } catch (const exception &ex) {
      return false;
    }

    if (missed_cells())
      return false;
    return true;
  }

  bool satisfy_constraints() const {
    for (const ArithmeticConstraint &constraint : m_constraints)
      if (!satisfy_constraint(constraint))
        return false;
    return true;
  }

  uint8_t get(const Cell &cell) const {
    check_cell(cell);
    return m_board[cell.first][cell.second];
  }

  void unassign(const Cell &cell) {
    check_cell(cell);
    m_board[cell.first][cell.second] = 0;
  }

  bool vaild_assignment(const Cell &cell) {
    check_cell(cell);
    set<uint8_t> row, col;
    for (int i = 0; i < m_size; i++) {
      if(m_board[cell.first][i] != 0) {
        if(row.count(m_board[cell.first][i]))
          return false;
        row.insert(m_board[cell.first][i]);
      }
      if(m_board[i][cell.second] != 0) {
        if(col.count(m_board[i][cell.second]))
          return false;
        col.insert(m_board[i][cell.second]);
      }
    }
    ArithmeticConstraint constraint = get_constraint(cell);
    return constraint.can_satisfy(get_values(constraint));
  }

  void clear() {
    for (int i = 0; i < m_size; i++)
      for (int j = 0; j < m_size; j++)
        m_board[i][j] = 0;
  }

  bool belongs_to_constraint(const Cell &cell) const {
    for (const ArithmeticConstraint &constraint : m_constraints)
      if (constraint.includes(cell))
        return true;
    return false;
  }

  ArithmeticConstraint get_constraint(const Cell &cell) const {
    for (const ArithmeticConstraint &constraint : m_constraints)
      if (constraint.includes(cell))
        return constraint;
    throw InvalidCellException();
  }

  void assign(const Cell &cell, uint8_t value) {
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

  void add_constraint(const ArithmeticConstraint &constraint) {
    check_constraint(constraint);
    m_constraints.push_back(constraint);
  }

  std::set<uint8_t> total_domain() const {
    std::set<uint8_t> domain;
    for (int i = 1; i <= m_size; i++)
      domain.insert(i);
    return domain;
  }

  std::set<uint8_t> fc_domain(const Cell &cell) const {
    uint8_t val = get(cell);
    if(val != 0)
      return set<uint8_t>{val};

    std::set<uint8_t> const_domain = constraint_domain(cell);
    std::set<uint8_t> domain;
    for (const uint8_t value : rc_domain(cell)) {
      if (const_domain.count(value))
        domain.insert(value);
    }

    return domain;
  }

  set<uint8_t> constraint_domain(const Cell &cell) const {
    check_cell(cell);
    ArithmeticConstraint constraint = get_constraint(cell);
    vector<uint8_t> values = get_values(constraint);
    return constraint.get_domain(values, m_size);
  }

  set<uint8_t> rc_domain(const Cell &cell) const {
    check_cell(cell);
    set<uint8_t> domain = total_domain();

    for (int i = 0; i < m_size; i++) {
      domain.erase(m_board[cell.first][i]);
      domain.erase(m_board[i][cell.second]);
    }
    return domain;
  }

  vector<Cell> related_cells(const Cell &cell) const {
    vector<Cell> cells;
    for (int i = 0; i < m_size; i++) {
      if(i != cell.second)
        cells.push_back({cell.first, i});

      if(i != cell.first)
        cells.push_back({i, cell.second});
    }

    for(const Cell& related_cell : get_constraint(cell).cells())
      if(related_cell != cell)
        cells.push_back(related_cell);

    return cells;
  }

  void save(const QString& file_path) {
    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      throw FileNotFoundException();

    QTextStream out(&file);
    out << m_size << endl;

    int64_t max_result = 0;
    for(const ArithmeticConstraint& constraint : m_constraints)
      max_result = max(max_result, constraint.result());
    int spaces = log10(max_result) + 2;

    for(const ArithmeticConstraint& constraint : m_constraints) {
      out << constraint.operation() << "  " << constraint.result();
      for(int i = spaces, n = log10(constraint.result()); i > n; i--)
        out << " ";

      for(const Cell& cell : constraint.cells())
        out << "(" << cell.first << "," << cell.second << ") ";

      out << endl;
    }

    file.close();
  }

  void save_solution(const QString& file_path) {
    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      throw FileNotFoundException();

    QTextStream out(&file);

    for (int i = 0; i < m_size; i++) {
      for (int j = 0; j < m_size; j++)
        out << m_board[i][j] << " ";
      out << endl;
    }

    file.close();
  }

  class InvalidSizeException : exception {};
  class InvalidCellException : exception {};
  class InvalidValueException : exception {};
  class InvalidConstraintException : exception {};
  class FileNotFoundException : exception {};

  friend QDebug operator<<(QDebug dbg, const KenKenBoard &board);
  uint8_t size() const;
  vector<ArithmeticConstraint> constraints() const;

  const vector<vector<uint8_t> > &board() const;

private:
  void check_cell(const Cell &cell) const {
    if (cell.first >= m_size || cell.second >= m_size)
      throw InvalidCellException();
  }

  void check_value(uint8_t value) const {
    if (value == 0 || value > m_size)
      throw InvalidValueException();
  }

  void check_constraint(const ArithmeticConstraint &constraint) const {
    if (!valid_constraint(constraint))
      throw InvalidConstraintException();

    for (const Cell &cell : constraint.cells()) {
      if (belongs_to_constraint(cell))
        throw InvalidConstraintException();
    }
  }

  bool valid_constraint(const ArithmeticConstraint &constraint) const {
    if (!constraint.valid())
      return false;

    try {
      for (const Cell &cell : constraint.cells())
        check_cell(cell);
    } catch (const InvalidCellException &ex) {
      return false;
    }
    return true;
  }

  bool satisfy_constraint(const ArithmeticConstraint &constraint) const {
    vector<uint8_t> values = get_values(constraint);
    return constraint.satisfy(values);
  }

  vector<uint8_t> get_values(const ArithmeticConstraint &constraint) const {
    vector<uint8_t> values;
    for (const Cell &cell : constraint.cells()) {
      uint8_t value = m_board[cell.first][cell.second];
      if (value != 0)
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

  bool missed_cells() const {
    map<Cell, int> cells_mp;
    for (const ArithmeticConstraint &constraint : m_constraints)
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
  vector<ArithmeticConstraint> m_constraints;

private:
  static constexpr uint8_t MIN_BOARD_SIZE = 2;
  static constexpr uint8_t MAX_BOARD_SIZE = 16;
};

#endif // KENKENBOARD_H
