#include "arithmeticconstraint.h"

ArithmeticConstraint::ArithmeticConstraint() : m_operation(EQUAL), m_result(0)
{

}

char ArithmeticConstraint::operation() const
{
  return m_operation;
}

void ArithmeticConstraint::set_operation(char operation)
{
  m_operation = operation;
}

set<Cell> ArithmeticConstraint::cells() const
{
  return m_cells;
}

uint8_t ArithmeticConstraint::cells_size() const
{
  return m_cells.size();
}

void ArithmeticConstraint::set_cells(const set<Cell> &cells)
{
  m_cells = cells;
}

int64_t ArithmeticConstraint::result() const {
  return m_result;
}

void ArithmeticConstraint::set_result(const int64_t &result)
{
  m_result = result;
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