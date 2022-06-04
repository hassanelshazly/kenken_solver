#ifndef BOARDGENERATOR_H
#define BOARDGENERATOR_H

#include <qfile.h>
#include <unordered_map>
#include <vector>

#include "kenkenboard.h"

class BoardGenerator {
public:
  BoardGenerator();

  KenKenBoard generate_random(uint8_t size);
  KenKenBoard generate_from_file(const QString &file_path) const;
  vector<vector<uint8_t>> read_solution(QString file_path) const;

  class FileNotFoundException : exception {};
  class UndefinedOpeartionException : exception {};

private:
  ArithmeticConstraint parse_constraint(const QString &line) const;
  char parse_operation(const QString &op) const;
  Cell parse_cell(const QString &cell) const;

  vector<ArithmeticConstraint>
  generate_random_constraints(int64_t board_size) const;
  vector<vector<int64_t>> get_random_board(int64_t board_size) const;
};

#endif // BOARDGENERATOR_H
