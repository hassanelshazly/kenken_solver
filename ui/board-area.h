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
    bool getSolved() const;
    void setSolved(bool newSolved);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    KenKenBoard board;
    KenKenBoard solvedBoard;
    bool solved;
};

#endif // BOARDAREA_H
