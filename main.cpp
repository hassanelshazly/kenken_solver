#include "lib/kenkenboard.h"
#include "lib/boardgenerator.h"
#include "lib/backtracksolver.h"
#include "lib/forwardcheckingsolver.h"
#include "lib/benchmarkingsolver.h"
#include "ui/kenken.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  //  KenKen w;
  //  w.show();

  BoardGenerator generateor;
  KenKenBoard board_3 = generateor.generate_from_file("../kenken_solver/examples/ex1.txt");
  assert(board_3.vaild_board());

  board_3.set_board(
        generateor.read_board("../kenken_solver/examples/ex1_sol.txt"));
  qDebug() << board_3 << "\n";
  assert(board_3.vaild_solution());

  KenKenBoard board_4(4);
  vector<Constraint> constraints = {
    Constraint(Constraint::SUBTRACT, 1, {{0, 0}, {0, 1}}),
    Constraint(Constraint::SUBTRACT, 1, {{0, 2}, {0, 3}}),
    Constraint(Constraint::ADD, 6, {{1, 0}, {2, 0}, {3, 0}}),
    Constraint(Constraint::MULTIPLY, 24, {{1, 1}, {1, 2}, {1, 3}}),
    Constraint(Constraint::ADD, 8, {{2, 1}, {2, 2}, {3, 1}}),
    Constraint(Constraint::DIVIDE, 2, {{2, 3}, {3, 3}}),
    Constraint(Constraint::EQUAL, 1, {{3, 2}})};

  board_4.set_board({
                      {4, 3, 2, 1},
                      {1, 2, 4, 3},
                      {2, 1, 3, 4},
                      {3, 4, 1, 2},
                    });

  for (const auto &constraint : constraints)
    board_4.add_constraint(constraint);
  qDebug() << board_4 << "\n";
  assert(board_4.vaild_solution());

  assert(!Constraint('+', 5, {{1, 1}, {2, 2}, {3, 2}}).check_adjacency());
  assert(Constraint('+', 1, {{0, 2}, {0, 3}}).check_adjacency());
  assert(Constraint('+', 6, {{1, 0}, {2, 0}, {3, 0}}).check_adjacency());
  assert(Constraint('+', 24, {{1, 1}, {1, 2}, {1, 3}}).check_adjacency());
  assert(Constraint('+', 8, {{2, 1}, {2, 2}, {3, 1}}).check_adjacency());
  assert(Constraint('+', 2, {{2, 3}, {3, 3}}).check_adjacency());
  assert(Constraint('+', 1, {{3, 2}}).check_adjacency());




  board_3 = generateor.generate_from_file("../kenken_solver/examples/ex1.txt");

  for(int i = 0; i < board_3.size(); i++)
    for(int j = 0; j < board_3.size(); j++)
        qDebug() << board_3.cell_domain({i, j});

  qDebug() << "\n\nBacktrackSolver\n";
  shared_ptr<KenKenSolver> bt_solver(new BacktrackSolver(board_3));
  BenchmarkingSolver bm_bt_solver{shared_ptr<KenKenSolver>(bt_solver)};
  bm_bt_solver.solve();
  qDebug() << "3x3 Solved In:" << bm_bt_solver.measured_msecs() << "milliseconds";
  qDebug() << bm_bt_solver->board();

  qDebug() << "\n\nForwardCheckingSolver\n";
  board_4.clear();
  shared_ptr<KenKenSolver> fd_solver(new ForwardCheckingSolver(board_4));
  BenchmarkingSolver bm_fd_solver{shared_ptr<KenKenSolver>(fd_solver)};
  bm_fd_solver.solve();
  qDebug() << "4x4 Solved In:" << bm_fd_solver.measured_msecs() << "milliseconds";
  qDebug() << bm_fd_solver->board();


  return 0;
  return a.exec();
}
