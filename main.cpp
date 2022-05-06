#include "lib/backtracksolver.h"
#include "lib/benchmarkingsolver.h"
#include "lib/boardgenerator.h"
#include "lib/forwardcheckingsolver.h"
#include "lib/arcconsistencysolver.h"
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
      {"board7x7.txt", "board7x7_sol.txt"},
      {"board8x8.txt", "board8x8_sol.txt"},
      {"board9x9.txt", "board9x9_sol.txt"},
  };

  for (const auto &[board_name, solution_name] : borads) {
    KenKenBoard board =
        generateor.generate_from_file(examples_path + board_name);
    assert(board.valid_board());

    board.set_board(generateor.read_solution(examples_path + solution_name));
    assert(board.valid_solution());

    if (board.size() <= 3) {
      qDebug() << "BacktrackSolver";
      board.clear();
      shared_ptr<KenKenSolver> bt_solver(new BacktrackSolver(board));
      BenchmarkingSolver bm_bt_solver{shared_ptr<KenKenSolver>(bt_solver)};

      int64_t time = 0;
      int simulation_rounds = 1;
      for (int i = 0; i < simulation_rounds; i++) {
        bm_bt_solver->board().clear();
        bm_bt_solver.solve();
        time += bm_bt_solver.measured_usecs();
      }
      qDebug() << board_name << "solved in:" << time / simulation_rounds
               << "milliseconds";
      qDebug() << bm_bt_solver->board();
      assert(bm_bt_solver->board().valid_solution());
    }

    if (board.size() <= 4) {
      qDebug() << "ForwardCheckingSolver";
      board.clear();
      shared_ptr<KenKenSolver> fd_solver(new ForwardCheckingSolver(board));
      BenchmarkingSolver bm_fd_solver{shared_ptr<KenKenSolver>(fd_solver)};

      int64_t time = 0;
      int simulation_rounds = 1;
      for (int i = 0; i < simulation_rounds; i++) {
        bm_fd_solver->board().clear();
        bm_fd_solver.solve();
        time += bm_fd_solver.measured_usecs();
      }
      qDebug() << board_name << "solved in:" << time / simulation_rounds
               << "milliseconds";
      qDebug() << bm_fd_solver->board();
      assert(bm_fd_solver->board().valid_solution());
    }


    if (board.size() <= 9) {
      qDebug() << "ArcConsistencySolver";
      board.clear();
      shared_ptr<KenKenSolver> ar_solver(new ArcConsistencySolver(board));
      BenchmarkingSolver bm_ar_solver{shared_ptr<KenKenSolver>(ar_solver)};

      int64_t time = 0;
      int simulation_rounds = 1;
      for (int i = 0; i < simulation_rounds; i++) {
        bm_ar_solver->board().clear();
        bm_ar_solver.solve();
        time += bm_ar_solver.measured_usecs();
      }
      qDebug() << board_name << "solved in:" << time / simulation_rounds
               << "milliseconds";
      qDebug() << bm_ar_solver->board();
      assert(bm_ar_solver->board().valid_solution());
    }
  }

  return 0;
  return a.exec();
}
