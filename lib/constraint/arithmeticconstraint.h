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
  ArithmeticConstraint(char operation, int64_t result, set<Cell> cells);

  bool includes(const Cell &cell) const;
  bool valid() const;
  bool satisfy(const vector<uint8_t> &values) const;
  bool can_satisfy(const vector<uint8_t> &values) const;
  set<uint8_t> get_domain(vector<uint8_t> values, int64_t board_size) const;
  int64_t apply(const vector<uint8_t> &values) const;

  char operation() const;
  void set_operation(char operation);

  set<Cell> cells() const;
  uint8_t cells_size() const;
  void set_cells(const set<Cell> &cells);

  int64_t result() const;
  void set_result(const int64_t &result);

  friend QDebug operator<<(QDebug dbg, const ArithmeticConstraint &constraint);

private:
  bool check_operation() const;
  bool check_adjacency() const;
  bool adjacent(const Cell &c1, const Cell &c2) const;

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
