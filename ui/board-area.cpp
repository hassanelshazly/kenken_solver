#include "board-area.h"

#include <string>

#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QRandomGenerator>
#include <QString>

#include "lib/board/boardgenerator.h"

BoardArea::BoardArea(QWidget *parent)
    : QWidget(parent)
{
    solved = false;

    BoardGenerator generator;
    board = generator.generate_random(3);

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

QSize BoardArea::minimumSizeHint() const
{
    return QSize(114, 114);
}

QSize BoardArea::sizeHint() const
{
    return QSize(480, 480);
}

const KenKenBoard &BoardArea::getBoard() const
{
    return board;
}

void BoardArea::setBoard(const KenKenBoard &newBoard)
{
    this->board = newBoard;
    update();
}

const KenKenBoard &BoardArea::getSolvedBoard() const
{
    return solvedBoard;
}

void BoardArea::setSolvedBoard(const KenKenBoard &newSolvedBoard)
{
    this->solvedBoard = newSolvedBoard;
}

bool BoardArea::getSolved() const
{
    return solved;
}

void BoardArea::setSolved(bool newSolved)
{
    solved = newSolved;
    update();
}

void BoardArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setPen(QPen(Qt::blue, 0, Qt::SolidLine));
    painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));

    const int boardSize = board.size();
    const int margin = 16;
    const int superScriptMargin = 4;
    const int W = width() - (margin * 2);
    const int H = height() - (margin * 2);
    const int size = qMin(W, H);
    const int cellSize = qRound(static_cast<double>(size) / static_cast<double>(boardSize));

    QRect rect(0, 0, cellSize, cellSize);

    painter.save();
    painter.translate(margin + qRound((W - size) / 2.0), margin + qRound((H - size) / 2.0));

    QRandomGenerator randomGenerator;
    for (const auto &constraint: board.constraints()) {
        QColor color(randomGenerator.bounded(256), randomGenerator.bounded(256), randomGenerator.bounded(256));
        painter.setBrush(QBrush(color, Qt::SolidPattern));

        auto cells = constraint.cells();
        auto it = cells.begin();

        if (it != cells.end()) {
            painter.save();
            painter.translate((*it).first * cellSize, (*it).second * cellSize);

            painter.drawRect(rect);

            painter.save();
            painter.translate(superScriptMargin, superScriptMargin);
            painter.drawText(rect,
                             Qt::AlignTop | Qt::AlignLeft,
                             QString(QString(constraint.operation()) + QString::number(constraint.result())));
            painter.restore();

            painter.restore();
        }

        while (++it != cells.end()) {
            painter.save();
            painter.translate((*it).first * cellSize, (*it).second * cellSize);

            painter.drawRect(rect);

            painter.restore();
        }
    }

    if (solved) {
        auto &boardCells = solvedBoard.board();

        for (auto i = 0; i < boardCells.size(); ++i) {
            for (auto j = 0; j < boardCells[i].size(); ++j) {
                painter.save();
                painter.translate(i * cellSize, j * cellSize);

                painter.drawText(rect,
                                 Qt::AlignCenter,
                                 QString::number(boardCells[i][j]));

                painter.restore();
            }
        }
    }

    painter.restore();
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}
