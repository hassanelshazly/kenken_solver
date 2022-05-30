#include "solver-thread.h"

#include "lib/benchmarking/bmbacktrackingsolver.h"
#include "lib/benchmarking/bmforwardcheckingsolver.h"
#include "lib/benchmarking/bmarcconsistencysolver.h"
#include "lib/benchmarking/bmheuristicfcsolver.h"

qint64 SolverThread::benchmarking_solver(KenKenSolver *solver, int simulation_rounds) {
  qint64 time = 0;
  for (int i = 0; i < simulation_rounds; i++) {
    solver->board().clear();
    solver->solve();
    time += dynamic_cast<Benchmarking *>(solver)->measured_usecs();
  }
  return time / simulation_rounds;
}

void SolverThread::run()
{
    KenKenSolver *solver;

    switch (solverAlgorithm) {
    case Backtracking:
        solver = new BMBacktrackingSolver(board);
        break;
    case ForwardChecker:
        solver = new BMForwardCheckingSolver(board);
        break;
    case ArcConsistency:
        solver = new BMArcConsistencySolver(board);
        break;
    }

    qint64 solvingTime = benchmarking_solver(solver);
    KenKenBoard board = solver->board();
    KenKenBoard *dynamicBoard = new KenKenBoard(board);
    emit solvingFinished(dynamicBoard, solvingTime);

    delete solver;
}
