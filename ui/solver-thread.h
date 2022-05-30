#ifndef SOLVERTHREAD_H
#define SOLVERTHREAD_H

#include <QThread>

#include "lib/board/kenkenboard.h"
#include "lib/solver/kenkensolver.h"

class SolverThread : public QThread
{
    Q_OBJECT
public:
    enum SolverAlgorithm { Backtracking, ForwardChecker, ArcConsistency };

    explicit SolverThread(KenKenBoard board, SolverAlgorithm solverAlgorithm):
        board(board), solverAlgorithm(solverAlgorithm) {}

signals:
    void solvingFinished(KenKenBoard *board, qint64 solvingTime);

private:
    KenKenBoard board;
    SolverAlgorithm solverAlgorithm;

    qint64 benchmarking_solver(KenKenSolver *solver, int simulation_rounds = 1);

    void run() override;
};

#endif // SOLVERTHREAD_H
