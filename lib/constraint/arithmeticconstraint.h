#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <QDebug>
#include <cassert>
#include <exception>
#include <inttypes.h>
#include <set>
#include <vector>

using namespace std;

typedef pair<uint8_t, uint8_t> Cell;

class ArithmeticConstraint {
public:
  ArithmeticConstraint();
  ArithmeticConstraint(char operation, int64_t result, set<Cell> cells)
      : m_operation(operation), m_result(result), m_cells(cells) {}

  bool includes(const Cell &cell) const { return m_cells.count(cell); }

  bool valid() const {
    if (!check_adjacency())
      return false;
    if (check_operation())
      return false;
    if (m_operation == EQUAL)
      return m_cells.size() == 1;
    if (m_operation == SUBTRACT || m_operation == DIVIDE)
      return m_cells.size() == 2;
    if (m_operation == ADD || m_operation == MULTIPLY)
      return m_cells.size() >= 2;
    return true;
  }

  bool satisfy(const vector<uint8_t> &values) const {
    assert(m_cells.size() == values.size());
    assert(valid());
    return m_result == apply(values);
  }

  bool can_satisfy(const vector<uint8_t> &values) const {
    return m_operation == EQUAL
               ? find(values.begin(), values.end(), m_result) != values.end()
           : m_operation == ADD
               ? (values.size() == m_cells.size() ? ADD_FUNC(values) == m_result
                                                  : ADD_FUNC(values) < m_result)
           : m_operation == MULTIPLY ? (values.size() == m_cells.size()
                                            ? MULTIPLY_FUNC(values) == m_result
                                            : MULTIPLY_FUNC(values) <= m_result)
           : m_operation == SUBTRACT
               ? (values.size() == 1 || SUBTRACT_FUNC(values) == m_result)
           : m_operation == DIVIDE
               ? (values.size() == 1 || DIVIDE_FUNC(values) == m_result)
               : false;
  }

  set<uint8_t> get_domain(vector<uint8_t> values, int64_t board_size) const {
    auto can_place = [&](uint8_t value) {
      values.push_back(value);
      bool result = can_satisfy(values);
      values.pop_back();
      return result;
    };

    set<uint8_t> domain;
    for (uint8_t i = 1; i <= board_size; i++) {
      if (can_place(i))
        domain.insert(i);
    }
    return domain;
  }

  int64_t apply(const vector<uint8_t> &values) const {
    return allowed_operations.find(m_operation)->second(values);
  }

  char operation() const;
  void set_operation(char operation);

  set<Cell> cells() const;
  uint8_t cells_size() const;
  void set_cells(const set<Cell> &cells);

  int64_t result() const;
  void set_result(const int64_t &result);

  friend QDebug operator<<(QDebug dbg, const ArithmeticConstraint &constraint);

private:
  bool check_operation() const {
    return !allowed_operations.count(m_operation);
  }

  bool check_adjacency() const {
    if (m_cells.size() < 2)
      return true;

    for (const auto cell1 : m_cells) {
      bool cell_adjecent = false;
      for (const auto cell2 : m_cells) {
        if (cell1 == cell2)
          continue;
        if (adjecent(cell1, cell2)) {
          cell_adjecent = true;
          break;
        }
      }
      if (!cell_adjecent)
        return false;
    }
    return true;
  }

  bool adjecent(const Cell &c1, const Cell &c2) const {
    return (abs(c1.first - c2.first) == 1 && abs(c1.second - c2.second) == 0) ||
           (abs(c1.first - c2.first) == 0 && abs(c1.second - c2.second) == 1);
  }

public:
  static constexpr char ADD = '+';
  static constexpr char SUBTRACT = '-';
  static constexpr char MULTIPLY = '*';
  static constexpr char DIVIDE = '/';
  static constexpr char EQUAL = '=';

private:
  char m_operation;
  int64_t m_result;
  set<Cell> m_cells;

  static constexpr auto ADD_FUNC = [](const vector<uint8_t> &values) {
    int64_t result = 0;
    for (uint8_t value : values)
      result += value;
    return result;
  };

  static constexpr auto MULTIPLY_FUNC = [](const vector<uint8_t> &values) {
    int64_t result = 1;
    for (uint8_t value : values)
      result *= value;
    return result;
  };

  static constexpr auto EQUAL_FUNC = [](const vector<uint8_t> &values) {
    return values[0];
  };

  static constexpr auto SUBTRACT_FUNC = [](const vector<uint8_t> &values) {
    uint8_t v1 = max(values[0], values[1]);
    uint8_t v2 = min(values[0], values[1]);
    return v1 - v2;
  };

  static constexpr auto DIVIDE_FUNC = [](const vector<uint8_t> &values) {
    uint8_t v1 = max(values[0], values[1]);
    uint8_t v2 = min(values[0], values[1]);
    if (v1 % v2 != 0)
      return -1;
    return v1 / v2;
  };

  inline static const map<char,
                          const std::function<int64_t(const vector<uint8_t> &)>>
      allowed_operations = {{ADD, ADD_FUNC},
                            {SUBTRACT, SUBTRACT_FUNC},
                            {MULTIPLY, MULTIPLY_FUNC},
                            {DIVIDE, DIVIDE_FUNC},
                            {EQUAL, EQUAL_FUNC}};
};

#endif // CONSTRAINT_H
