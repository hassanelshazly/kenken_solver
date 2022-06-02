#include "arithmeticconstraint.h"

ArithmeticConstraint::ArithmeticConstraint()
    : m_operation(EQUAL), m_result(0) {}

ArithmeticConstraint::ArithmeticConstraint(char operation, int64_t result,
                                           set<Cell> cells)
    : m_operation(operation), m_result(result), m_cells(cells) {}

bool ArithmeticConstraint::includes(const Cell &cell) const {
  return m_cells.count(cell);
}

bool ArithmeticConstraint::valid() const {
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

bool ArithmeticConstraint::satisfy(const vector<uint8_t> &values) const {
  assert(m_cells.size() == values.size());
  assert(valid());
  return m_result == apply(values);
}

bool ArithmeticConstraint::can_satisfy(const vector<uint8_t> &values) const {
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

set<uint8_t> ArithmeticConstraint::get_domain(vector<uint8_t> values,
                                              int64_t board_size) const {
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

int64_t ArithmeticConstraint::apply(const vector<uint8_t> &values) const {
  return allowed_operations.find(m_operation)->second(values);
}

char ArithmeticConstraint::operation() const { return m_operation; }

void ArithmeticConstraint::set_operation(char operation) {
  m_operation = operation;
}

set<Cell> ArithmeticConstraint::cells() const { return m_cells; }

uint8_t ArithmeticConstraint::cells_size() const { return m_cells.size(); }

void ArithmeticConstraint::set_cells(const set<Cell> &cells) {
  m_cells = cells;
}

int64_t ArithmeticConstraint::result() const { return m_result; }

void ArithmeticConstraint::set_result(const int64_t &result) {
  m_result = result;
}

bool ArithmeticConstraint::check_operation() const {
  return !allowed_operations.count(m_operation);
}

bool ArithmeticConstraint::check_adjacency() const {
  if (m_cells.size() < 2)
    return true;

  for (const auto cell1 : m_cells) {
    bool cell_adjecent = false;
    for (const auto cell2 : m_cells) {
      if (cell1 == cell2)
        continue;
      if (adjacent(cell1, cell2)) {
        cell_adjecent = true;
        break;
      }
    }
    if (!cell_adjecent)
      return false;
  }
  return true;
}

bool ArithmeticConstraint::adjacent(const Cell &c1, const Cell &c2) const {
  return (abs(c1.first - c2.first) == 1 && abs(c1.second - c2.second) == 0) ||
         (abs(c1.first - c2.first) == 0 && abs(c1.second - c2.second) == 1);
}

QDebug operator<<(QDebug dbg, const Cell &cell) {
  dbg.nospace() << "(" << cell.first << ", " << cell.second << ")";
  return dbg;
}

QDebug operator<<(QDebug dbg, const ArithmeticConstraint &constraint) {
  dbg.nospace() << "Operation: " << constraint.m_operation << "\n";
  dbg.nospace() << "Result: " << constraint.m_result << "\n";
  dbg.nospace() << "Cells: {";

  if (constraint.m_cells.empty()) {
    dbg.nospace() << " }\n";
    return dbg;
  }

  size_t i = 0;
  for (auto it = constraint.m_cells.begin(); i < constraint.m_cells.size() - 1;
       ++it, ++i)
    dbg.nospace() << *it << ", ";
  dbg.nospace() << *constraint.m_cells.rbegin() << "}\n";

  return dbg;
}
