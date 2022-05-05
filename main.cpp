#include "lib/backtracksolver.h"
#include "lib/benchmarkingsolver.h"
#include "lib/boardgenerator.h"
#include "lib/forwardcheckingsolver.h"
#include "lib/kenkenboard.h"
#include "ui/kenken.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  //  KenKen w;
  //  w.show();

  QString examples_path = "../kenken_solver/examples/";
  BoardGenerator generateor;

  vector<pair<QString, QString>> borads = {
      {"board3x3.txt", "board3x3_sol.txt"},
      {"board4x4.txt", "board4x4_sol.txt"},
      {"board5x5.txt", "board5x5_sol.txt"},
      {"board6x6.txt", "board6x6_sol.txt"},
      {"board7x7.txt", "board7x7_sol.txt"}};

  for (const auto &[board_name, solution_name] : borads) {
    KenKenBoard board =
        generateor.generate_from_file(examples_path + board_name);
    assert(board.vaild_board());

    board.set_board(generateor.read_solution(examples_path + solution_name));
    assert(board.vaild_solution());

    if (board.size() <= 3) {
      qDebug() << "BacktrackSolver";
      board.clear();
      shared_ptr<KenKenSolver> bt_solver(new BacktrackSolver(board));
      BenchmarkingSolver bm_bt_solver{shared_ptr<KenKenSolver>(bt_solver)};
      bm_bt_solver.solve();
      qDebug() << board_name << "solved in:" << bm_bt_solver.measured_msecs()
               << "milliseconds";
      qDebug() << bm_bt_solver->board();
      assert(bm_bt_solver->board().vaild_solution());
    }

    if (board.size() <= 7) {
      qDebug() << "ForwardCheckingSolver";
      board.clear();
      shared_ptr<KenKenSolver> fd_solver(new ForwardCheckingSolver(board));
      BenchmarkingSolver bm_fd_solver{shared_ptr<KenKenSolver>(fd_solver)};
      bm_fd_solver.solve();
      qDebug() << board_name << "solved in:" << bm_fd_solver.measured_msecs()
               << "milliseconds";
      qDebug() << bm_fd_solver->board();
      assert(bm_fd_solver->board().vaild_solution());
    }
  }

  //  assert(!Constraint('+', 5, {{1, 1}, {2, 2}, {3, 2}}).check_adjacency());
  //  assert(Constraint('+', 1, {{0, 2}, {0, 3}}).check_adjacency());
  //  assert(Constraint('+', 6, {{1, 0}, {2, 0}, {3, 0}}).check_adjacency());
  //  assert(Constraint('+', 24, {{1, 1}, {1, 2}, {1, 3}}).check_adjacency());
  //  assert(Constraint('+', 8, {{2, 1}, {2, 2}, {3, 1}}).check_adjacency());
  //  assert(Constraint('+', 2, {{2, 3}, {3, 3}}).check_adjacency());
  //  assert(Constraint('+', 1, {{3, 2}}).check_adjacency());

  //  for (int i = 0; i < board_3.size(); i++)
  //    for (int j = 0; j < board_3.size(); j++)
  //      qDebug() << board_3.cell_domain({i, j});

  return 0;
  return a.exec();
}
