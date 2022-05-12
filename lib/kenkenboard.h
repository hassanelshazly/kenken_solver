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
#include <iostream>

#include "constraint.h"

#define MAX_CAGE_SIZE 4

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
  vector<Constraint> generate_random() const{
      vector<vector<int>> solution(m_size, vector<int> (m_size, 0));
      vector<int> board (m_size * m_size, 0);
      vector<char> directions;
      unordered_map<int, set<Cell>> cages;
      vector<Constraint> constraints;

      unsigned seed;
      int curCageID = 0,
          curX = -1,
          curY = -1,
          nextX = -1,
          nextY = -1,
          cageSize,
          maxCageSize = -1,
          boardFull,
          growable;


      solution = get_filled_board();

      // Print after shuffling
      cout << "The random generated board: " << endl;
      for (size_t i = 0; i < m_size; i++)
      {
          for (size_t j = 0; j < m_size; j++)
          {
              cout << solution[i][j] << " ";
          }
          cout << "\n";
      }
      cout << "\n";

      for ( int i = 0; i < m_size * m_size ; ++i )
      {
          board[i] = -1 ;
      }

      // Fill vector with possible directions
      directions.push_back('N');
      directions.push_back('S');
      directions.push_back('E');
      directions.push_back('W');


      // Each iteration generate new cage
      while (true)
      {
          boardFull = true;

          for ( int i = 0; i < m_size ; ++ i )
          {
              for ( int j = 0; j < m_size ; ++ j )
              {
                  if ( board[i * m_size + j] < 0)
                  {
                      curX = j;
                      curY = i;
                      boardFull = false;
                      break;
                  }
              }

              if (!boardFull)
              {
                  break;
              }
          }

          if (boardFull)
          {
              break;
          }

          // Randomly select max cage size of this iteration
          seed = chrono::system_clock::now().time_since_epoch().count();
          srand(seed);
          maxCageSize = 1 + rand() % MAX_CAGE_SIZE;

          // Mark this cell as visited
          board[curY * m_size + curX] = curCageID;

          // Fill cages map with first cell
          Cell temp;
          temp.first = curY;
          temp.second = curX;
          cages[curCageID].emplace(temp);


          // Start from 1 till max cage size
          cageSize = 1;

          // Start add cells to current cage
          while (true)
          {
              if (cageSize >= maxCageSize )
              {
                  break ;
              }
              growable = false;

              // Randomly select the direction of growth
              seed = chrono::system_clock::now().time_since_epoch().count();
              shuffle(directions.begin(),directions.end(), default_random_engine(seed));

              for ( char s : directions )
              {
                  switch (s)
                  {
                      case 'N':
                          nextX = curX ;
                          nextY = curY - 1;
                          break ;
                      case 'E':
                          nextX = curX + 1;
                          nextY = curY ;
                          break ;
                      case 'S':
                          nextX = curX ;
                          nextY = curY + 1;
                          break ;
                      case 'W':
                          nextX = curX - 1;
                          nextY = curY ;
                          break ;
                  }
                  if ( nextX >= 0 && nextX < m_size && nextY >= 0 && nextY < m_size )
                  {
                      if (board[nextY * m_size + nextX ] == -1)
                      {
                          growable = true ;
                          break ;
                      }
                  }
              }

              if ( growable && cageSize < maxCageSize )
              {
                  board[nextY * m_size + nextX] = curCageID;
                  curX = nextX ;
                  curY = nextY ;
                  cageSize += 1;

                  Cell temp;
                  temp.first = nextY;
                  temp.second = nextX;
                  cages[curCageID].emplace(temp);

              } else
              {
                  break ;
              }


          }
          // Start new cage
          curCageID++;
      }

      // Add operation for each cage
      for (auto x : cages)
      {
          int result;
          switch (x.second.size())
          {
              case 1:
                  {
                      set<Cell> c = x.second;
                      auto it = c.begin();
                      result = solution[it->first][it->second];
                      Constraint cons('=', result, x.second);
                      constraints.push_back(cons);
                      break ;
                  }
              case 2:
                  {
                      int op_1, op_2;
                      set<Cell> c = x.second;
                      auto it = c.begin();

                      op_1 = solution[it->first][it->second];
                      it++;
                      op_2 = solution[it->first][it->second];

                      seed = chrono::system_clock::now().time_since_epoch().count();
                      srand(seed);
                      int op = 1 + rand() % 3;

                      if(op == 1)
                      {
                          result = op_1 + op_2;
                          Constraint cons('+', result, x.second);
                          constraints.push_back(cons);

                      }
                      else if(op == 2)
                      {
                          result = op_1 * op_2;
                          Constraint cons('*', result, x.second);
                          constraints.push_back(cons);
                      }
                      else
                      {
                          int smaller, larger;
                          smaller = op_1;
                          larger = op_2;

                          if (larger < smaller)
                          {
                              int temp = smaller ;
                              smaller = larger ;
                              larger = temp ;
                          }
                          if(larger % smaller == 0)
                          {
                              result = larger / smaller;
                              Constraint cons('/', result, x.second);
                              constraints.push_back(cons);
                          }
                           else
                          {
                              result = larger - smaller;
                              Constraint cons('-', result, x.second);
                              constraints.push_back(cons);
                          }

                      }
                      break;
                  }
              default:
                  {
                      seed = chrono::system_clock::now().time_since_epoch().count();
                      srand(seed);
                      int op = 1 + rand() % 2;

                      if(op == 1)
                      {
                          result = 1;
                          set<Cell> c = x.second;
                          for(auto it = c.begin(); it!=c.end(); ++it)
                              result *= solution[it->first][it->second];

                          Constraint cons('*', result, x.second);
                          constraints.push_back(cons);

                      }
                      else
                      {
                          result = 0;
                          set<Cell> c = x.second;
                          for(auto it = c.begin(); it!=c.end(); ++it)
                              result += solution[it->first][it->second];

                          Constraint cons('+', result, x.second);
                          constraints.push_back(cons);
                      }
                      break;
                  }
          }
      }
      return constraints;
  }


  vector<vector<int>> get_filled_board() const{
      vector<vector<int>> solution(m_size, vector<int> (m_size, 0));
      unsigned seed;

      // Fill solution board
      for (size_t i = 0; i < m_size; i++)
      {
          for (size_t j = 0; j < m_size; j++)
          {
              solution[i][j] = (i + j) % m_size + 1;
          }

      }

       // shuffle rows
       seed = chrono::system_clock::now().time_since_epoch().count();
       for (auto &inner : solution)
           shuffle(inner.begin(),inner.end(), default_random_engine(seed));


       // Transpose matrix
       for ( int i = 0; i < m_size ; ++ i)
       {
           for ( int j = 0; j < i ; ++ j)
           {
               int temp = solution[i][j];
               solution[i][j] = solution[j][i];
               solution[j][i] = temp;
           }
       }

       // shuffle rows
       seed = chrono::system_clock::now().time_since_epoch().count();
       for (auto &inner : solution)
           shuffle(inner.begin(),inner.end(), default_random_engine(seed));

       return solution;

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
      for (const Constraint &constraint : m_constraints)
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

  void set_value(const Cell &cell, uint8_t value) {
    check_cell(cell);
    check_value(value);
    m_board[cell.first][cell.second] = value;
  }

  void set_board(const vector<vector<uint8_t>> &board) {
//    assert(board.size() == m_size);
    for (const auto &row : board) {
//      assert(row.size() == m_size);
      for (const auto &value : row)
        check_value(value);
    }
    m_board = board;
  }

  void add_constraint(const Constraint &constraint) {
    check_constraint(constraint);
    m_constraints.push_back(constraint);
  }

  std::set<uint8_t> total_domain() const {
    std::set<uint8_t> domain;
    for (int i = 1; i <= m_size; i++)
      domain.insert(i);
    return domain;
  }

   std::set<uint8_t> fd_domain(const Cell &cell) const {
    Constraint constraint = get_constraint(cell);
    vector<uint8_t> values;

    std::set<uint8_t> neighbors_domain = total_domain();
    for (const auto &neighbor : neighbors(cell)) {
      uint8_t value = m_board[neighbor.first][neighbor.second];
      if (value != 0 && constraint.includes(neighbor))
        values.push_back(value);
      neighbors_domain.erase(value);
    }
    std::set<uint8_t> const_domain = constraint.get_domain(values, m_size);

    std::set<uint8_t> domain;
    for (const uint8_t value : neighbors_domain) {
      if (const_domain.count(value))
        domain.insert(value);
    }

    return domain;
  }

  std::set<uint8_t> ar_domain(const Cell &cell) const {
    check_cell(cell);
    std::set<uint8_t> const_domain = constraint_domain(cell);

    std::set<uint8_t> domain;
    for (const uint8_t value : rc_domain(cell)) {
      if (const_domain.count(value))
        domain.insert(value);
    }

    return domain;
  }

  std::set<uint8_t> constraint_domain(const Cell &cell) const {
    check_cell(cell);
    Constraint constraint = get_constraint(cell);
    vector<uint8_t> values = get_values(constraint);
    return constraint.get_domain(values, m_size);
  }

  std::set<uint8_t> rc_domain(const Cell &cell) const {
    check_cell(cell);
    std::set<uint8_t> domain = total_domain();

    for (int i = 0; i < m_size; i++) {
      domain.erase(m_board[cell.first][i]);
      domain.erase(m_board[i][cell.second]);
    }
    return domain;
  }

  vector<Cell> neighbors(const Cell &cell) const {
    static const vector<pair<int, int>> neighbors_offest = {
        {0, -1}, {-1, 0}, {0, 1}, {1, 0}};
    auto [i, j] = cell;
    vector<Cell> neighbors_cells;
    for (const auto &[x, y] : neighbors_offest) {
      if (x + i >= 0 && x + i < m_size && y + j >= 0 && y + j < m_size) {
       neighbors_cells.push_back({x + i, y + j});
      }
    }
    return neighbors_cells;
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
    } catch (const InvalidCellException &ex) {
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
