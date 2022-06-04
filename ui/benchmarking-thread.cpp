#include "benchmarking-thread.h"

#include "lib/benchmarking/bmbacktrackingsolver.h"
#include "lib/benchmarking/bmforwardcheckingsolver.h"
#include "lib/benchmarking/bmarcconsistencysolver.h"
#include "lib/benchmarking/bmheuristicfcsolver.h"

qint64 BenchmarkingThread::benchmarking_solver(KenKenSolver *solver, int simulation_rounds) {
  qint64 time = 0;
  for (int i = 0; i < simulation_rounds; i++) {
    solver->board().clear();
    solver->solve();
    time += dynamic_cast<Benchmarking *>(solver)->measured_usecs();
  }
  return time / simulation_rounds;
}

void BenchmarkingThread::run()
{
    QList<qint64> res;
    KenKenSolver *solver;

    solver = new BMBacktrackingSolver(board);
    res << benchmarking_solver(solver);
    delete solver;

    solver = new BMForwardCheckingSolver(board);
    res << benchmarking_solver(solver);
    delete solver;

    solver = new BMArcConsistencySolver(board);
    res << benchmarking_solver(solver);
    delete solver;

    emit benchmarkingFinished(res);
}
