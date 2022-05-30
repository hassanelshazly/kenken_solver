#ifndef ARCCONSISTENCYSOLVER_H
#define ARCCONSISTENCYSOLVER_H

#include "kenkensolver.h"

template <typename value_t>
QDebug operator<<(QDebug dbg, const set<value_t> &st) {
  for (const value_t &val : st)
    dbg << val;
  return dbg;
}

class ArcConsistencySolver : public KenKenSolver {
public:
  ArcConsistencySolver();

  ArcConsistencySolver(KenKenBoard board) : KenKenSolver(board) {}
  ~ArcConsistencySolver() {}

  virtual void solve() override {
    set<uint8_t> initial_domain = m_board.total_domain();
    vector<vector<set<uint8_t>>> domains(
        m_board.size(), vector<set<uint8_t>>(m_board.size(), initial_domain));
    initialize_arcs();
    ac_solve(make_optional(make_pair(0, 0)), domains);
    clear_arcs();
  }

private:
  bool ac_solve(const optional<Cell> &cell,
                vector<vector<set<uint8_t>>> domains) {
    if (m_board.valid_solution())
      return true;

    if (!cell || m_board.complete_solution())
      return false;

    if (!ac3(*cell, domains)) {
      return false;
    }

    for (uint8_t i : domains[cell->first][cell->second]) {
      m_board.assign(*cell, i);

      if (m_board.vaild_assignment(*cell))
        if (ac_solve(next_cell(*cell), domains))
          return true;

      m_board.unassign(*cell);
    }

    return false;
  }

  bool ac3(Cell cell, vector<vector<set<uint8_t>>> &domains) {
    vector<Cell> cells = m_board.related_cells(cell);
    set<pair<Cell, Cell>> remaining;
    for (const Cell &related_cell : cells) {
      remaining.insert({cell, related_cell});
      remaining.insert({related_cell, cell});
    }

    while (remaining.size()) {
      auto front_pair = *remaining.begin();
      remaining.erase(front_pair);

      if (remove_inconsistent_values(front_pair.first, front_pair.second,
                                     domains)) {
        Cell curr_cell = front_pair.first;
        if (domains[curr_cell.first][curr_cell.second].empty())
          return false;
        for (const Cell &related_cell : m_board.related_cells(curr_cell)) {
          remaining.insert({related_cell, curr_cell});
        }
      }
    }
    return true;
  }

  bool remove_inconsistent_values(Cell xi, Cell xj,
                                  vector<vector<set<uint8_t>>> &domains) {
    set<uint8_t> domain_i = domains[xi.first][xi.second];
    set<uint8_t> domain_j = domains[xj.first][xj.second];

    set<uint8_t> reduced_domain_i;
    ArithmeticConstraint constraint = m_board.get_constraint(xi);
    if (constraint.cells_size() == 1) {
      domain_i = {constraint.result()};
    }

    for (uint8_t val_i : domain_i) {
      for (const uint8_t &val_j : domain_j) {
        if (can_satisfy(xi, val_i, xj, val_j)) {
          reduced_domain_i.insert(val_i);
          break;
        }
      }
    }

    domains[xi.first][xi.second] = reduced_domain_i;
    return reduced_domain_i.size() != domain_i.size();
  }

  bool can_satisfy(Cell xi, uint8_t val_i, Cell xj, uint8_t val_j) {
    for (const auto fun : arcs[xi][xj])
      if (!fun(val_i, val_j))
        return false;
    return true;
  }

  void initialize_arcs() {
    if (arcs.size())
      return;
    for (uint8_t i = 0; i < m_board.size(); i++) {
      for (uint8_t j = 0; j < m_board.size(); j++) {
        Cell cell(i, j);
        add_not_equal_arcs(cell);
        add_arithmetic_arcs(cell);
      }
    }
  }

  void add_not_equal_arcs(Cell cell) {
    static auto not_equal_fun = [](uint8_t val_i, uint8_t val_j) {
      return val_i != val_j;
    };

    for (uint8_t i = 0; i < m_board.size(); i++) {
      if (i != cell.second) {
        Cell related_cell(cell.first, i);
        arcs[cell][related_cell].push_back(not_equal_fun);
      }

      if (i != cell.first) {
        Cell related_cell(i, cell.second);
        arcs[cell][related_cell].push_back(not_equal_fun);
      }
    }
  }

  void add_arithmetic_arcs(Cell cell) {
    static const auto equal_func =
        [](int64_t result) -> function<bool(uint8_t, uint8_t)> {
      return [result](uint8_t val_i, uint8_t val_j) { return val_i == result; };
    };

    static const auto subtract_func =
        [](int64_t result) -> function<bool(uint8_t, uint8_t)> {
      return [result](uint8_t val_i, uint8_t val_j) {
        return abs(val_i - val_j) == result;
      };
    };

    static const auto divide_func =
        [](int64_t result) -> function<bool(uint8_t, uint8_t)> {
      return [result](uint8_t val_i, uint8_t val_j) {
        return val_i / val_j == result || val_j / val_i == result;
      };
    };

    static const auto add_func =
        [](int64_t result, bool binary_op) -> function<bool(uint8_t, uint8_t)> {
      return [result, binary_op](uint8_t val_i, uint8_t val_j) {
        if (binary_op)
          return val_i + val_j == result;
        else
          return val_i + val_j < result;
      };
    };

    static const auto multiply_func =
        [](int64_t result, bool binary_op) -> function<bool(uint8_t, uint8_t)> {
      return [result, binary_op](uint8_t val_i, uint8_t val_j) {
        if (binary_op)
          return val_i * val_j == result;
        else
          return val_i * val_j <= result;
      };
    };

    ArithmeticConstraint constraint = m_board.get_constraint(cell);
    bool binary_op = constraint.cells_size() == 2;
    function<bool(uint8_t, uint8_t)> fun =
        constraint.operation() == ArithmeticConstraint::ADD
            ? add_func(constraint.result(), binary_op)
        : constraint.operation() == ArithmeticConstraint::MULTIPLY
            ? multiply_func(constraint.result(), binary_op)
        : constraint.operation() == ArithmeticConstraint::SUBTRACT
            ? subtract_func(constraint.result())
        : constraint.operation() == ArithmeticConstraint::DIVIDE
            ? divide_func(constraint.result())
            : equal_func(constraint.result());

    for (const auto &related_cell : constraint.cells()) {
      if (cell != related_cell) {
        arcs[cell][related_cell].push_back(fun);
      }
    }
  }

  void clear_arcs() { arcs.clear(); }

  void print_domains(const vector<vector<set<uint8_t>>> &domains) {
    for (const auto &row : domains) {
      QDebug deb = qDebug();
      for (const auto &domain : row)
        deb << domain << "\t\t";
    }
  }

protected:
  map<Cell, map<Cell, vector<function<bool(uint8_t, uint8_t)>>>> arcs;
};

#endif // ARCCONSISTENCYSOLVER_H
