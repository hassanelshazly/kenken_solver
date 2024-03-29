#include "kenken.h"

#include <QtWidgets>

#include "ui/board-area.h"
#include "ui/solver-thread.h"
#include "ui/benchmarking-thread.h"
#include "lib/board/boardgenerator.h"

const int IdRole = Qt::UserRole;

KenKen::KenKen()
{
    boardArea = new BoardArea;

    boardSizeSpinBox = new QSpinBox;
    boardSizeSpinBox->setRange(2, 9);

    boardSizeLabel = new QLabel(tr("Board Si&ze:"));
    boardSizeLabel->setBuddy(boardSizeSpinBox);

    generatePushButton = new QPushButton("&Generate");
    connect(generatePushButton, &QAbstractButton::clicked, this, &KenKen::generatePushed);

    solverAlgorithmComboBox = new QComboBox;
    solverAlgorithmComboBox->addItem(tr("Backtracking"), static_cast<int>(SolverThread::SolverAlgorithm::Backtracking));
    solverAlgorithmComboBox->addItem(tr("ForwardChecking"), static_cast<int>(SolverThread::SolverAlgorithm::ForwardChecker));
    solverAlgorithmComboBox->addItem(tr("ArcConsistency"), static_cast<int>(SolverThread::SolverAlgorithm::ArcConsistency));
    solverAlgorithmComboBox->addItem(tr("Heuristic"), static_cast<int>(SolverThread::SolverAlgorithm::Heuristic));

    solverAlgorithmLabel = new QLabel(tr("Solver &Algorithm:"));
    solverAlgorithmLabel->setBuddy(solverAlgorithmComboBox);

    solvePushButton = new QPushButton("Sol&ve");
    connect(solvePushButton, &QAbstractButton::clicked, this, &KenKen::solvePushed);

    takenTimeLabel = new QLabel("0");

    solvingTimeLabel = new QLabel(tr("Solving Time: "));
    solvingTimeLabel->setBuddy(takenTimeLabel);

    loadBoardFromFileButton = new QPushButton("&Load Board from file...");
    connect(loadBoardFromFileButton, &QAbstractButton::clicked, this, &KenKen::loadBoardFromFilePushed);

    saveBoardFromFileButton = new QPushButton("&Save Board to file...");
    connect(saveBoardFromFileButton, &QAbstractButton::clicked, this, &KenKen::saveBoardFromFilePushed);

    saveBoardSolutionFromFileButton = new QPushButton("&Save Board Solution to file...");
    connect(saveBoardSolutionFromFileButton, &QAbstractButton::clicked, this, &KenKen::saveBoardSolutionFromFilePushed);

    statsTableWidget = new QTableWidget(4, 2);
    QStringList statsTableHeaderLabels;
    statsTableHeaderLabels << "Algorithm" << "Time (us)";
    statsTableWidget->setHorizontalHeaderLabels(statsTableHeaderLabels);
    QHeaderView *statsHeader = statsTableWidget->horizontalHeader();
    statsHeader->setSectionResizeMode(QHeaderView::Stretch);
    statsTableWidget->verticalHeader()->setVisible(false);
    QTableWidgetItem *backtrackingItem = new QTableWidgetItem("Backtracking");
    statsTableWidget->setItem(0, 0, backtrackingItem);
    QTableWidgetItem *forwardCheckingItem = new QTableWidgetItem("ForwardChecking");
    statsTableWidget->setItem(1, 0, forwardCheckingItem);
    QTableWidgetItem *arcConsistencyItem = new QTableWidgetItem("ArcConsistency");
    statsTableWidget->setItem(2, 0, arcConsistencyItem);
    QTableWidgetItem *heuristicItem = new QTableWidgetItem("Heuristic");
    statsTableWidget->setItem(3, 0, heuristicItem);

    benchmarkPushButton = new QPushButton("&Benchmark...");
    connect(benchmarkPushButton, &QAbstractButton::clicked, this, &KenKen::benchmarkPushed);

    QGridLayout *controlsLayout = new QGridLayout;
    controlsLayout->addWidget(boardSizeLabel, 0, 0, Qt::AlignRight);
    controlsLayout->addWidget(boardSizeSpinBox, 0, 1);
    controlsLayout->addWidget(generatePushButton, 1, 0, 1, 2);
    controlsLayout->addWidget(solverAlgorithmLabel, 2, 0, Qt::AlignRight);
    controlsLayout->addWidget(solverAlgorithmComboBox, 2, 1);
    controlsLayout->addWidget(solvePushButton, 3, 0, 1, 2);
    controlsLayout->addWidget(solvingTimeLabel, 4, 0);
    controlsLayout->addWidget(takenTimeLabel, 4, 1);
    controlsLayout->addWidget(loadBoardFromFileButton, 5, 0, 1, 2);
    controlsLayout->addWidget(saveBoardFromFileButton, 6, 0, 1, 2);
    controlsLayout->addWidget(saveBoardSolutionFromFileButton, 7, 0, 1, 2);
    controlsLayout->addWidget(statsTableWidget, 8, 0, 1, 2);
    controlsLayout->addWidget(benchmarkPushButton, 9, 0, 1, 2);
    controlsLayout->setRowStretch(10, 1);

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
    takenTimeLabel->setText(QString::number(solvingTime));

    delete board;
}

void KenKen::loadBoardFromFilePushed()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open Board"),
        "../kenken_solver/examples",
        "Board files (*.txt)"
    );

    if (fileName.isEmpty())
        return;

    BoardGenerator generator;
    KenKenBoard board = generator.generate_from_file(fileName);

    boardArea->setBoard(board);
}

void KenKen::saveBoardFromFilePushed()
{
    QFileDialog dialog(
        this, tr("Save Board As"),
        "../kenken_solver/examples",
        "Board files (*.txt)"
    );

    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    if (dialog.exec() != QDialog::Accepted)
        return;

    boardArea->getSolvedBoard().save(dialog.selectedFiles().constFirst().toStdString().c_str());
}

void KenKen::saveBoardSolutionFromFilePushed()
{
    QFileDialog dialog(
        this, tr("Save Board Solution As"),
        "../kenken_solver/examples",
        "Board files (*.txt)"
    );

    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    if (dialog.exec() != QDialog::Accepted)
        return;

    boardArea->getSolvedBoard().save_solution(dialog.selectedFiles().constFirst().toStdString().c_str());
}


void KenKen::benchmarkPushed()
{
    BenchmarkingThread *benchmarkingThread = new BenchmarkingThread(boardArea->getBoard());
    connect(benchmarkingThread, &BenchmarkingThread::benchmarkingFinished, this, &KenKen::handleBenchmarked);
    connect(benchmarkingThread, &BenchmarkingThread::finished, benchmarkingThread, &QObject::deleteLater);
    benchmarkingThread->start();
}

void KenKen::handleBenchmarked(QList<qint64> solvingTimes) {
    QTableWidgetItem *backtrackingItem = new QTableWidgetItem(QString::number(solvingTimes[0]));
    statsTableWidget->setItem(0, 1, backtrackingItem);
    QTableWidgetItem *forwardCheckingItem = new QTableWidgetItem(QString::number(solvingTimes[1]));
    statsTableWidget->setItem(1, 1, forwardCheckingItem);
    QTableWidgetItem *arcConsistencyItem = new QTableWidgetItem(QString::number(solvingTimes[2]));
    statsTableWidget->setItem(2, 1, arcConsistencyItem);
    QTableWidgetItem *heuristicItem = new QTableWidgetItem(QString::number(solvingTimes[3]));
    statsTableWidget->setItem(3, 1, heuristicItem);
}
