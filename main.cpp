#include "ui/kenken.h"
#include "lib/kenkenboard.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  KenKen w;
  w.show();

  KenKenBoard board(4);
  vector<Constraint> constraints = {
      Constraint(Constraint::SUBTRACT, 1, {{0, 0}, {0, 1}}),
      Constraint(Constraint::SUBTRACT, 1, {{0, 2}, {0, 3}}),
      Constraint(Constraint::ADD, 6, {{1, 0}, {2, 0}, {3, 0}}),
      Constraint(Constraint::MULTIPLY, 24, {{1, 1}, {1, 2}, {1, 3}}),
      Constraint(Constraint::ADD, 8, {{2, 1}, {2, 2}, {3, 1}}),
      Constraint(Constraint::DIVIDE, 2, {{2, 3}, {3, 3}}),
      Constraint(Constraint::EQUAL, 1, {{3, 2}})};

  board.set_board({
      {4, 3, 2, 1},
      {1, 2, 4, 3},
      {2, 1, 3, 4},
      {3, 4, 1, 2},
  });

  for (const auto &constraint : constraints)
    board.add_constraint(constraint);
  qDebug() << board << "\n" << board.vaild_solution() << "\n";

  qDebug() << Constraint(Constraint::ADD, 5, {{1, 1}, {2, 2}, {3, 2}})
                  .check_adjacency();
  qDebug() << Constraint(Constraint::SUBTRACT, 1, {{0, 2}, {0, 3}})
                  .check_adjacency();
  qDebug() << Constraint(Constraint::ADD, 6, {{1, 0}, {2, 0}, {3, 0}})
                  .check_adjacency();
  qDebug() << Constraint(Constraint::MULTIPLY, 24, {{1, 1}, {1, 2}, {1, 3}})
                  .check_adjacency();
  qDebug() << Constraint(Constraint::ADD, 8, {{2, 1}, {2, 2}, {3, 1}})
                  .check_adjacency();
  qDebug()
      << Constraint(Constraint::DIVIDE, 2, {{2, 3}, {3, 3}}).check_adjacency();
  qDebug() << Constraint(Constraint::EQUAL, 1, {{3, 2}}).check_adjacency();
  return a.exec();
}
