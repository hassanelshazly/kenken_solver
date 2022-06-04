#ifndef KENKEN_H
#define KENKEN_H

#include <QWidget>

#include "lib/board/kenkenboard.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QPushButton;
class QSpinBox;
QT_END_NAMESPACE
class BoardArea;

class KenKen : public QWidget {
    Q_OBJECT

public:
    KenKen();

private slots:
    void generatePushed();
    void solvePushed();

    void loadBoardFromFilePushed();
    void saveBoardFromFilePushed();

    void handleSolved(KenKenBoard *board, qint64 solvingTime);

private:
    BoardArea *boardArea;
    QLabel *boardSizeLabel;
    QLabel *solverAlgorithmLabel;
    QLabel *solvingTimeLabel;
    QLabel *takenTimeLabel;
    QSpinBox *boardSizeSpinBox;
    QComboBox *solverAlgorithmComboBox;
    QPushButton *generatePushButton;
    QPushButton *solvePushButton;
    QPushButton *loadBoardFromFileButton;
    QPushButton *saveBoardFromFileButton;
};
#endif // KENKEN_H
