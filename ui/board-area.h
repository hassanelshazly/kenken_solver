#ifndef BOARDAREA_H
#define BOARDAREA_H

#include <QWidget>

#include "lib/board/kenkenboard.h"

class BoardArea : public QWidget
{
    Q_OBJECT

public:
    explicit BoardArea(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    const KenKenBoard &getBoard() const;
    void setBoard(const KenKenBoard &newBoard);
    const KenKenBoard &getSolvedBoard() const;
    void setSolvedBoard(const KenKenBoard &newSolvedBoard);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    KenKenBoard board;
    KenKenBoard solvedBoard;
    bool solved;

    inline static const QColor cells_colors[] = {
        QColor(255, 0, 0),     QColor(0, 255, 0),
        QColor(255, 255, 0),   QColor(255, 0, 255),   QColor(0, 255, 255),
        QColor(255, 127, 0),   QColor(255, 0, 127),   QColor(0, 255, 127),
        QColor(127, 255, 0),   QColor(127, 0, 255),   QColor(0, 127, 255),
        QColor(255, 255, 255)
    };
};

#endif // BOARDAREA_H
