#ifndef KENKENBOARD_H
#define KENKENBOARD_H

#include <QDebug>
#include <QFile>
#include <algorithm>
#include <chrono>
#include <exception>
#include <inttypes.h>
#include <random>
#include <set>
#include <unordered_map>
#include <vector>

#include "lib/constraint/arithmeticconstraint.h"

using namespace std;

class KenKenBoard {
public:
  KenKenBoard();
  KenKenBoard(uint8_t size);

  bool valid_solution() const;
  bool complete_solution() const;
  bool valid_board() const;
  bool satisfy_constraints() const;
  bool vaild_assignment(const Cell &cell);

  uint8_t get(const Cell &cell) const;
  void assign(const Cell &cell, uint8_t value);
  void unassign(const Cell &cell);
  void clear();

  bool belongs_to_constraint(const Cell &cell) const;
  ArithmeticConstraint get_constraint(const Cell &cell) const;
  void set_board(const vector<vector<uint8_t>> &board);
  void add_constraint(const ArithmeticConstraint &constraint);

  set<uint8_t> total_domain() const;
  set<uint8_t> fc_domain(const Cell &cell) const;
  set<uint8_t> constraint_domain(const Cell &cell) const;
  set<uint8_t> rc_domain(const Cell &cell) const;
  vector<Cell> related_cells(const Cell &cell) const;

  void save(const QString &file_path);
  void save_solution(const QString &file_path) const;

  class InvalidSizeException : exception {};
  class InvalidCellException : exception {};
  class InvalidValueException : exception {};
  class InvalidConstraintException : exception {};
  class FileNotFoundException : exception {};

  uint8_t size() const;
  vector<ArithmeticConstraint> constraints() const;
  const vector<vector<uint8_t>> &board() const;

  friend QDebug operator<<(QDebug dbg, const KenKenBoard &board);

private:
  void check_cell(const Cell &cell) const;
  void check_value(uint8_t value) const;
  void check_constraint(const ArithmeticConstraint &constraint) const;
  bool valid_constraint(const ArithmeticConstraint &constraint) const;
  bool satisfy_constraint(const ArithmeticConstraint &constraint) const;
  bool check_rows_and_cols() const;
  bool missed_cells() const;

  vector<uint8_t> get_values(const ArithmeticConstraint &constraint) const;

private:
  uint8_t m_size;
  vector<vector<uint8_t>> m_board;
  vector<ArithmeticConstraint> m_constraints;

private:
  static constexpr uint8_t MIN_BOARD_SIZE = 2;
  static constexpr uint8_t MAX_BOARD_SIZE = 16;
};

#endif // KENKENBOARD_H
