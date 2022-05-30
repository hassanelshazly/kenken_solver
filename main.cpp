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

    qDebug() << "BacktrackSolver";
    BMBacktrackingSolver solver(board);
    qDebug() << size << "solved in:" << benchmarking_solver(&solver)
             << "microseconds";
    assert(solver.board().valid_solution());

    qDebug() << "ForwardCheckingSolver";
    BMForwardCheckingSolver solver_a(board);
    qDebug() << size << "solved in:" << benchmarking_solver(&solver_a)
             << "microseconds";
    assert(solver_a.board().valid_solution());

    solver_a.board().save_solution((to_string(size) + "_sol.txt").c_str());

    qDebug() << "ArcConsistencySolver";
    BMArcConsistencySolver solver_b(board);
    qDebug() << size << "solved in:" << benchmarking_solver(&solver_b)
             << "microseconds";
    assert(solver_b.board().valid_solution());

    qDebug() << "HeuristicFCSolver";
    BMHeuristicFCSolver solver_c(board);
    qDebug() << size << "solved in:" << benchmarking_solver(&solver_c)
             << "microseconds";
    assert(solver_c.board().valid_solution());
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
      assert(solver.board().valid_solution());
    }

    if (board.size() <= 9) {
      qDebug() << "ForwardCheckingSolver";
      BMForwardCheckingSolver solver(board);
      qDebug() << board_name << "solved in:" << benchmarking_solver(&solver)
               << "microseconds";
      assert(solver.board().valid_solution());
    }


    if (board.size() <= 9) {
      qDebug() << "ArcConsistencySolver";
      BMArcConsistencySolver solver(board);
      qDebug() << board_name << "solved in:" << benchmarking_solver(&solver)
               << "microseconds";
      assert(solver.board().valid_solution());
    }

    if (board.size() <= 9) {
      qDebug() << "HeuristicFCSolver";
      BMHeuristicFCSolver solver(board);
      qDebug() << board_name << "solved in:" << benchmarking_solver(&solver)
               << "microseconds";
      assert(solver.board().valid_solution());
    }

    qDebug() << "\n";
  }
}

void print_statistics(vector<int64_t> data) {
  sort(data.begin(), data.end());
  int64_t min = *min_element(data.begin(), data.end());
  int64_t max = *max_element(data.begin(), data.end());
  int64_t median = data.size() & 1 ? data[data.size()/2] :
                   (data[data.size()/2 - 1] + data[data.size()/2]) / 2;
  int64_t mean = accumulate(data.begin(), data.end(), 0) / data.size();

  qDebug() << "Mean:" << mean << "    \t" << "Meadian:" << median;
  qDebug() << " Min:" << min << "    \t" << "    Max:" << max;
}

void benchmarking_test() {
  BoardGenerator generator;
  int num_boards = 100;

  for(int size = 2; size <= 9; size++) {
    vector<vector<int64_t>> data(4);

    for(int i = 0; i < num_boards; i++) {
      KenKenBoard board = generator.generate_random(size);
      assert(board.valid_board());

      BMBacktrackingSolver bt_solver(board);
      data[0].push_back(benchmarking_solver(&bt_solver));
      assert(bt_solver.board().valid_solution());

      BMForwardCheckingSolver fc_solver(board);
      data[1].push_back(benchmarking_solver(&fc_solver));
      assert(fc_solver.board().valid_solution());

      BMArcConsistencySolver ac_solver(board);
      data[2].push_back(benchmarking_solver(&ac_solver));
      assert(ac_solver.board().valid_solution());

      BMHeuristicFCSolver he_solver(board);
      data[3].push_back(benchmarking_solver(&he_solver));
      assert(he_solver.board().valid_solution());
    }


    string board_name = to_string(size) + "x" + to_string(size);
    qDebug() << "Board" << board_name.c_str() << "statistics in microseconds";
    qDebug() << "BacktrackingSolver";
    print_statistics(data[0]);
    qDebug() << "ForwardCheckingSolver";
    print_statistics(data[1]);
    qDebug() << "ArcConsistencySolver";
    print_statistics(data[2]);
    qDebug() << "HeuristicFCSolver";
    print_statistics(data[3]);
    qDebug() << "\n";
  }

}

int main(int argc, char *argv[]) {
  //  random_test();
  //  expamles_test();
  benchmarking_test();
  return 0;

  QApplication a(argc, argv);
  KenKen w;
  w.show();

  return a.exec();
}
