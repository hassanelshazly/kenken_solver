#include "lib/board/boardgenerator.h"
#include "lib/benchmarking/bmbacktrackingsolver.h"
#include "lib/benchmarking/bmforwardcheckingsolver.h"
#include "lib/benchmarking/bmarcconsistencysolver.h"
#include "lib/benchmarking/bmheuristicfcsolver.h"

#include "ui/kenken.h"

#include <QApplication>
#include <cassert>

int64_t benchmarking_solver(KenKenSolver *solver, int simulation_rounds = 1) {
  int64_t time = 0;
  for (int i = 0; i < simulation_rounds; i++) {
    solver->board().clear();
    solver->solve();
    time += dynamic_cast<Benchmarking *>(solver)->measured_usecs();
  }
  return time / simulation_rounds;
}

void random_test() {
  BoardGenerator generator;

  for(int size = 2; size <= 9; size++) {
    KenKenBoard board = generator.generate_random(size);
//    QString file_path((to_string(size) + ".txt").c_str());
//    board.save(file_path);
//    board = generator.generate_from_file(file_path);

    qDebug() << "ForwardCheckingSolver";
    BMForwardCheckingSolver solver_a(board);
    qDebug() << size << "solved in:" << benchmarking_solver(&solver_a)
           << "microseconds";
//    qDebug() << solver_a.board();
    assert(solver_a.board().valid_solution());

    solver_a.board().save_solution((to_string(size) + "_sol.txt").c_str());

    qDebug() << "ArcConsistencySolver";
    BMArcConsistencySolver solver_b(board);
    qDebug() << size << "solved in:" << benchmarking_solver(&solver_b)
           << "microseconds";
    assert(solver_b.board().valid_solution());
//    qDebug() << solver_b.board();

    qDebug() << "HeuristicArcConsistencySolver";
    BMHeuristicFCSolver solver_c(board);
    qDebug() << size << "solved in:" << benchmarking_solver(&solver_c)
           << "microseconds";
    assert(solver_c.board().valid_solution());
//      qDebug() << solver_c.board();
    }
}

void expamles_test() {
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

    if (board.size() <= 9) {
      qDebug() << "BacktrackSolver";
      BMBacktrackingSolver solver(board);

      qDebug() << board_name << "solved in:" << benchmarking_solver(&solver)
               << "microseconds";
      //      qDebug() << solver.board();
      assert(solver.board().valid_solution());
    }

    if (board.size() <= 9) {
      qDebug() << "ForwardCheckingSolver";
      BMForwardCheckingSolver solver(board);

      qDebug() << board_name << "solved in:" << benchmarking_solver(&solver)
               << "microseconds";
      //      qDebug() << solver.board();
      assert(solver.board().valid_solution());
    }


    if (board.size() <= 9) {
      qDebug() << "ArcConsistencySolver";
      board.clear();
      BMArcConsistencySolver solver(board);

      qDebug() << board_name << "solved in:" << benchmarking_solver(&solver)
               << "microseconds";
      //      qDebug() << solver.board();
      assert(solver.board().valid_solution());
    }

    if (board.size() <= 9) {
      qDebug() << "HeuristicArcConsistencySolver";
      board.clear();
      BMHeuristicFCSolver solver(board);

      qDebug() << board_name << "solved in:" << benchmarking_solver(&solver)
               << "microseconds";
      //      qDebug() << solver.board();
      assert(solver.board().valid_solution());
    }

    qDebug() << "\n";
  }
}

int main(int argc, char *argv[]) {
//  random_test();
  expamles_test();
  return 0;

  QApplication a(argc, argv);
  KenKen w;
  w.show();

  return a.exec();
}
