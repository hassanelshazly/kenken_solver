#include "lib/boardgenerator.h"
#include "lib/bmbacktrackingsolver.h"
#include "lib/bmforwardcheckingsolver.h"
#include "lib/bmarcconsistencysolver.h"
#include "lib/bmheuristicarcconsistencysolver.h"

#include "ui/kenken.h"

#include <QApplication>

int64_t benchmarking_solver(KenKenSolver *solver, int simulation_rounds = 1) {
  int64_t time = 0;
  for (int i = 0; i < simulation_rounds; i++) {
    solver->board().clear();
    solver->solve();
    time += dynamic_cast<Benchmarking *>(solver)->measured_usecs();
  }
  return time / simulation_rounds;
}

int main(int argc, char *argv[]) {
    int size = 4;
    BoardGenerator generator;
    KenKenBoard board = generator.generate_random(size);

//  QString examples_path = "../kenken_solver/examples/";
//  BoardGenerator generateor;

//  vector<pair<QString, QString>> borads = {
//      {"board3x3.txt", "board3x3_sol.txt"},
//      {"board4x4.txt", "board4x4_sol.txt"},
//      {"board5x5.txt", "board5x5_sol.txt"},
//      {"board6x6.txt", "board6x6_sol.txt"},
//      {"board7x7.txt", "board7x7_sol.txt"},
//      {"board8x8.txt", "board8x8_sol.txt"},
//      {"board9x9.txt", "board9x9_sol.txt"},
//  };

//  for (const auto &[board_name, solution_name] : borads) {
//    KenKenBoard board =
//        generateor.generate_from_file(examples_path + board_name);
//    assert(board.valid_board());

//    board.set_board(generateor.read_solution(examples_path + solution_name));
//    assert(board.valid_solution());

//    if (board.size() <= 0) {
//      qDebug() << "BacktrackSolver";
//      BMBacktrackingSolver solver(board);

//      qDebug() << board_name << "solved in:" << benchmarking_solver(&solver)
//               << "microseconds";
//      qDebug() << solver.board();
//      assert(solver.board().valid_solution());
//    }

//    if (board.size() <= 0) {
//      qDebug() << "ForwardCheckingSolver";
//      BMForwardCheckingSolver solver(board);

//      qDebug() << board_name << "solved in:" << benchmarking_solver(&solver)
//               << "microseconds";
//      qDebug() << solver.board();
//      assert(solver.board().valid_solution());
//    }


//    if (board.size() <= 0) {
//      qDebug() << "ArcConsistencySolver";
//      board.clear();
//      BMArcConsistencySolver solver(board);

//      qDebug() << board_name << "solved in:" << benchmarking_solver(&solver)
//               << "microseconds";
//      qDebug() << solver.board();
//      assert(solver.board().valid_solution());
//    }

//    if (board.size() <= 9) {
//      qDebug() << "HeuristicArcConsistencySolver";
//      board.clear();
//      BMHeuristicArcConsistencySolver solver(board);

//      qDebug() << board_name << "solved in:" << benchmarking_solver(&solver)
//               << "microseconds";
//      qDebug() << solver.board();
//      assert(solver.board().valid_solution());
//    }
//  }

  return 0;

  QApplication a(argc, argv);
  KenKen w;
  w.show();

  return a.exec();
}
