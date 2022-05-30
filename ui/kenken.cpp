#include "kenken.h"

#include <QtWidgets>

#include "ui/board-area.h"
#include "ui/solver-thread.h"
#include "lib/board/boardgenerator.h"

const int IdRole = Qt::UserRole;

KenKen::KenKen()
{
    boardArea = new BoardArea;

    boardSizeSpinBox = new QSpinBox;
    boardSizeSpinBox->setRange(3, 9);

    boardSizeLabel = new QLabel(tr("Board &Size:"));
    boardSizeLabel->setBuddy(boardSizeSpinBox);

    generatePushButton = new QPushButton("&Generate");
    connect(generatePushButton, &QAbstractButton::clicked, this, &KenKen::generatePushed);

    solverAlgorithmComboBox = new QComboBox;
    solverAlgorithmComboBox->addItem(tr("Backtracking"), static_cast<int>(SolverThread::SolverAlgorithm::Backtracking));
    solverAlgorithmComboBox->addItem(tr("ForwardChecking"), static_cast<int>(SolverThread::SolverAlgorithm::ForwardChecker));
    solverAlgorithmComboBox->addItem(tr("ArcConsistency"), static_cast<int>(SolverThread::SolverAlgorithm::ArcConsistency));

    solverAlgorithmLabel = new QLabel(tr("Solver &Algorithm:"));
    solverAlgorithmLabel->setBuddy(solverAlgorithmComboBox);

    solvePushButton = new QPushButton("So&lve");
    connect(solvePushButton, &QAbstractButton::clicked, this, &KenKen::solvePushed);

    takenTimeLabel = new QLabel("0");

    solvingTimeLabel = new QLabel(tr("Solving Time: "));
    solvingTimeLabel->setBuddy(takenTimeLabel);

    QGridLayout *controlsLayout = new QGridLayout;
    controlsLayout->addWidget(boardSizeLabel, 0, 0, Qt::AlignRight);
    controlsLayout->addWidget(boardSizeSpinBox, 0, 1);
    controlsLayout->addWidget(generatePushButton, 1, 0, 1, 2);
    controlsLayout->addWidget(solverAlgorithmLabel, 2, 0, Qt::AlignRight);
    controlsLayout->addWidget(solverAlgorithmComboBox, 2, 1);
    controlsLayout->addWidget(solvePushButton, 3, 0, 1, 2);
    controlsLayout->addWidget(solvingTimeLabel, 4, 0);
    controlsLayout->addWidget(takenTimeLabel, 4, 1);
    controlsLayout->setRowStretch(5, 1);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(controlsLayout);
    mainLayout->addWidget(boardArea, 1);

    setLayout(mainLayout);

    setWindowTitle(tr("KenKen Solver"));
}

void KenKen::generatePushed()
{
    int size = boardSizeSpinBox->value();
    BoardGenerator generator;
    KenKenBoard board = generator.generate_random(size);

    boardArea->setSolved(false);
    boardArea->setBoard(board);
}

void KenKen::solvePushed()
{
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    qint32 algorithmOrd = solverAlgorithmComboBox->itemData(
                solverAlgorithmComboBox->currentIndex(), IdRole).toInt();

    SolverThread *solverThread = new SolverThread(boardArea->getBoard(), static_cast<SolverThread::SolverAlgorithm>(algorithmOrd));
    connect(solverThread, &SolverThread::solvingFinished, this, &KenKen::handleSolved);
    connect(solverThread, &SolverThread::finished, solverThread, &QObject::deleteLater);
    solverThread->start();
}

void KenKen::handleSolved(KenKenBoard *board, qint64 solvingTime)
{
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

    boardArea->setSolvedBoard(*board);
    boardArea->setSolved(true);
    takenTimeLabel->setText(QString::number(solvingTime));

    delete board;
}
