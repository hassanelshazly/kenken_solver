#include "lib/kenkenboard.h"
#include "lib/boardgenerator.h"
#include "lib/backtracksolver.h"
#include "lib/benchmarkingsolver.h"
#include "ui/kenken.h"

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
  qDebug() << board << "\n";
  assert(board.vaild_solution());

  assert(!Constraint('+', 5, {{1, 1}, {2, 2}, {3, 2}}).check_adjacency());
  assert(Constraint('+', 1, {{0, 2}, {0, 3}}).check_adjacency());
  assert(Constraint('+', 6, {{1, 0}, {2, 0}, {3, 0}}).check_adjacency());
  assert(Constraint('+', 24, {{1, 1}, {1, 2}, {1, 3}}).check_adjacency());
  assert(Constraint('+', 8, {{2, 1}, {2, 2}, {3, 1}}).check_adjacency());
  assert(Constraint('+', 2, {{2, 3}, {3, 3}}).check_adjacency());
  assert(Constraint('+', 1, {{3, 2}}).check_adjacency());

  BoardGenerator generateor;
  board = generateor.generate_from_file("../kenken_solver/examples/ex1.txt");
  assert(board.vaild_board());

  board.set_board(
        generateor.read_board("../kenken_solver/examples/ex1_sol.txt"));
  qDebug() << board << "\n";
  assert(board.vaild_solution());


  board = generateor.generate_from_file("../kenken_solver/examples/ex1.txt");
  shared_ptr<KenKenSolver> solver(new BacktrackSolver(board));
  BenchmarkingSolver bm_solver{shared_ptr<KenKenSolver>(solver)};

  qDebug() << "\n\nBacktrackSolver\n";
  bm_solver.solve();
  qDebug() << "3x3 Solved In:" << bm_solver.measured_msecs() << "milliseconds";
  qDebug() << bm_solver->board();

  return a.exec();
}
