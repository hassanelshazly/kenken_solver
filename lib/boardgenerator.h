#ifndef BOARDGENERATOR_H
#define BOARDGENERATOR_H

#include <qfile.h>
#include <unordered_map>
#include <vector>

#include "kenkenboard.h"

class BoardGenerator {
public:
  BoardGenerator();
  KenKenBoard generate_random(uint8_t size) {
    m_size = size;
    KenKenBoard board(size);
    vector<Constraint> constraints = process();
    for(const auto&constraint: constraints){
        board.add_constraint(constraint);
        qDebug() << constraint;
    }
    return board;
  }

  KenKenBoard generate_from_file(QString file_path) const {
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      throw FileNotFoundException();

    QTextStream in(&file);
    QString line = in.readLine();

    uint8_t borad_size = line.toInt();
    KenKenBoard board(borad_size);

    while (!in.atEnd()) {
      QString line = in.readLine();
      if(line.isEmpty())
        continue;
      Constraint constraint = parse_constraint(line);
      board.add_constraint(constraint);
    }
    return board;
  }

  vector<vector<uint8_t>> read_solution(QString file_path) const {
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      throw FileNotFoundException();

    vector<vector<uint8_t>> board;
    QTextStream in(&file);
    while (!in.atEnd()) {
      QString line = in.readLine();
      if(line.isEmpty())
        continue;

      QStringList row = line.split(" ", QString::SkipEmptyParts);
      vector<uint8_t> int_row(row.size());
      for(int i = 0; i < row.size(); i++)
        int_row[i] = row.at(i).toInt();
      board.push_back(int_row);
    }
    return board;
  }

  class FileNotFoundException : exception {};
  class UndefinedOpeartionException : exception {};

private:
  Constraint parse_constraint(const QString& line) const {
    QStringList list = line.split(" ", QString::SkipEmptyParts);
    char opration = parse_operation(list.at(0));
    int64_t result = list.at(1).toLongLong();
    set<Cell> cells;
    for(int i = 2; i < list.size(); i++)
      cells.insert(parse_cell(list.at(i)));

    Constraint constraint(opration, result, cells);
    if(!constraint.valid())
      throw KenKenBoard::InvalidConstraintException();

    return constraint;
  }

  char parse_operation(const QString& op) const {
    if(op == "+" || op == "a" || op == "add" )
      return Constraint::ADD;
    if(op == "-" || op == "s" || op == "sub" || op == "subtract")
      return Constraint::SUBTRACT;
    if(op == "*" || op == "m" || op == "mul" || op == "multiply")
      return Constraint::MULTIPLY;
    if(op == "/" || op == "d" || op == "div" || op == "divide")
      return Constraint::DIVIDE;
    if(op == "=" || op == "e" || op == "eq" || op == "equal" )
      return Constraint::EQUAL;

    throw UndefinedOpeartionException();
  }

  Cell parse_cell(const QString& cell) const {
    QRegExp rx("[(]([0-9]+),([0-9]+)[)]");
    rx.indexIn(cell);
    QStringList list = rx.capturedTexts();
    if(list.size() != 3)
      throw KenKenBoard::InvalidCellException();
    return {list.at(1).toInt(), list.at(2).toInt()};
  }

  vector<Constraint> process() const {
    vector<vector<int64_t>> solution;
    vector<int64_t> board (m_size * m_size, -1);
    vector<char> directions;
    unordered_map<int64_t, set<Cell>> cages;
    vector<Constraint> constraints;
    uint64_t seed;

    solution = get_filled_board();

    // Fill vector with possible directions
    directions.push_back('N');
    directions.push_back('S');
    directions.push_back('E');
    directions.push_back('W');

    int64_t curCageID = 0,
        curX = -1,
        curY = -1,
        nextX = -1,
        nextY = -1,
        cageSize,
        maxCageSize = -1,
        boardFull,
        growable;

    // Each iteration generate new cage
    while (true) {
        boardFull = true;
        for (int64_t i = 0; i < m_size ; ++ i ) {
          for (int64_t j = 0; j < m_size ; ++ j ) {
              if ( board[i * m_size + j] < 0) {
                  curX = j;
                  curY = i;
                  boardFull = false;
                  break;
              }
          }
          if (!boardFull)
              break;
        }
        if (boardFull)
          break;

        // Mark this cell as visited
        board[curY * m_size + curX] = curCageID;

        // Randomly select max cage size of this iteration
        seed = chrono::system_clock::now().time_since_epoch().count();
        srand(seed);
        maxCageSize = 1 + rand() % MAX_CAGE_SIZE;
        cageSize = 1;
        cages[curCageID].emplace(curY, curX);

        // Start add cells to current cage
        while (true) {
            if (cageSize >= maxCageSize )
              break ;
            growable = false;

            // Randomly select the direction of growth
            seed = chrono::system_clock::now().time_since_epoch().count();
            shuffle(directions.begin(),directions.end(), default_random_engine(seed));
            for ( char s : directions ) {
              switch (s) {
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
              if ( nextX >= 0 && nextX < m_size && nextY >= 0 && nextY < m_size ) {
                  if (board[nextY * m_size + nextX ] == -1) {
                      growable = true ;
                      break ;
                  }
              }
            }
            if (growable && cageSize < maxCageSize) {
              board[nextY * m_size + nextX] = curCageID;
              curX = nextX ;
              curY = nextY ;
              cageSize += 1;
              cages[curCageID].emplace(nextY, nextX);
            }
            else
                break ;
        }
        curCageID++;
    }

    // Add operation for each cage
    for (auto cage : cages) {
        int64_t result;
        switch (cage.second.size()) {
            case 1:{
                set<Cell> c = cage.second;
                auto it = c.begin();
                result = solution[it->first][it->second];
                Constraint cons('=', result, cage.second);
                constraints.push_back(cons);
                break ;
            }
            case 2:{
                int64_t smaller, larger;
                set<Cell> c = cage.second;
                auto it = c.begin();

                smaller = solution[it->first][it->second];
                it++;
                larger = solution[it->first][it->second];

                larger = max(smaller, larger);
                smaller = min(smaller, larger);


                seed = chrono::system_clock::now().time_since_epoch().count();
                srand(seed);
                int op = 1 + rand() % 3;

                if(op == 1) {
                    result = larger + smaller;
                    Constraint cons('+', result, cage.second);
                    constraints.push_back(cons);
                }
                else if(op == 2) {
                    result = larger * smaller;
                    Constraint cons('*', result, cage.second);
                    constraints.push_back(cons);
                }
                else {
                    if(larger % smaller == 0) {
                        result = larger / smaller;
                        Constraint cons('/', result, cage.second);
                        constraints.push_back(cons);
                    }
                    else {
                        result = larger - smaller;
                        Constraint cons('-', result, cage.second);
                        constraints.push_back(cons);
                    }
                }
                break;
            }
            default:{
                seed = chrono::system_clock::now().time_since_epoch().count();
                srand(seed);
                int64_t op = 1 + rand() % 2;
                if(op == 1) {
                  result = 1;
                  set<Cell> c = cage.second;
                  for(auto it = c.begin(); it!=c.end(); ++it)
                      result *= solution[it->first][it->second];
                  Constraint cons('*', result, cage.second);
                  constraints.push_back(cons);
                }
                else{
                  result = 0;
                  set<Cell> c = cage.second;
                  for(auto it = c.begin(); it!=c.end(); ++it)
                      result += solution[it->first][it->second];
                  Constraint cons('+', result, cage.second);
                  constraints.push_back(cons);
                }
                break;
            }
        }
    }
    return constraints;
  }


  vector<vector<int64_t>> get_filled_board() const {
    vector<vector<int64_t>> solution(m_size, vector<int64_t> (m_size, 0));
    for (int64_t i = 0; i < m_size; i++) {
      for (int64_t j = 0; j < m_size; j++)
          solution[i][j] = (i + j) % m_size + 1;
    }

    // shuffle columns
    unsigned seed;
    seed = chrono::system_clock::now().time_since_epoch().count();
    for (auto &inner : solution)
       shuffle(inner.begin(),inner.end(), default_random_engine(seed));

    // Transpose matrix
    for(int64_t i = 0; i < m_size ; ++ i) {
       for(int64_t j = 0; j < i ; ++ j)
            swap(solution[i][j], solution[j][i]);
    }

    // shuffle columns
    seed = chrono::system_clock::now().time_since_epoch().count();
    for (auto &inner : solution)
       shuffle(inner.begin(),inner.end(), default_random_engine(seed));
    return solution;
  }
private:
    int64_t m_size;
     int64_t MAX_CAGE_SIZE = 4;
};

#endif // BOARDGENERATOR_H
