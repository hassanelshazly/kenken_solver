#ifndef BENCHMARKINGTHREAD_H
#define BENCHMARKINGTHREAD_H

#include <QThread>
#include <QList>
#include <QMetaType>

#include "lib/board/kenkenboard.h"
#include "lib/solver/kenkensolver.h"

class BenchmarkingThread : public QThread
{
    Q_OBJECT
public:
    explicit BenchmarkingThread(KenKenBoard board): board(board) {
        qRegisterMetaType<QList<quint64>>("QList<qint64>");
    }

signals:
    void benchmarkingFinished(QList<qint64> solvingTimes);

private:
    KenKenBoard board;

    qint64 benchmarking_solver(KenKenSolver *solver, int simulation_rounds = 1);

    void run() override;
};

#endif // BENCHMARKINGTHREAD_H
