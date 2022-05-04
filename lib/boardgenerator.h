#ifndef KENKENBOARDGENERATOR_H
#define KENKENBOARDGENERATOR_H

#include "kenkenboard.h"
#include <qfile.h>

class BoardGenerator
{
public:
  BoardGenerator();

  KenKenBoard generate_random(uint8_t size) {
    KenKenBoard board(size);
    return board;
  }

  KenKenBoard generate_from_file(QString file_path) {
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      throw InvaildFileException();

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

  vector<vector<uint8_t>> read_board(QString file_path) {
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      throw InvaildFileException();

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

  class InvaildFileException : exception {};
  class UndefinedOpeartionException : exception {};

private:
  Constraint parse_constraint(const QString& line) {
    QStringList list = line.split(" ", QString::SkipEmptyParts);
    char opration = parse_operation(list.at(0));
    int64_t result = list.at(1).toLongLong();
    set<Cell> cells;
    for(int i = 2; i < list.size(); i++)
      cells.insert(parse_cell(list.at(i)));

    Constraint constraint(opration, result, cells);
    if(!constraint.vaild())
      throw KenKenBoard::InvaildConstraintException();

    return constraint;
  }

  char parse_operation(const QString& op) {
    if(op == "+" || op == "a" || op == "add" )
      return Constraint::ADD;
    if(op == "-" || op == "s" || op == "sub" || op == "subtract")
      return Constraint::SUBTRACT;
    if(op == "*" || op == "m" || op == "mul" || op == "multiply")
      return Constraint::MULTIPLY;
    if(op == "/" || op == "d" || op == "div" || op == "divide")
      return Constraint::DIVIDE;
    if(op == "e" || op == "eq" || op == "equal" )
      return Constraint::EQUAL;

    throw UndefinedOpeartionException();
  }

  Cell parse_cell(const QString& cell) {
    QRegExp rx("[(]([0-9]+),([0-9]+)[)]");
    rx.indexIn(cell);
    QStringList list = rx.capturedTexts();
    if(list.size() != 3)
      throw KenKenBoard::InvaildCellException();
    return {list.at(1).toInt(), list.at(2).toInt()};
  }

};

#endif // KENKENBOARDGENERATOR_H
