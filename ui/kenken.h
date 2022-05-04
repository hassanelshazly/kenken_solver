#ifndef KENKEN_H
#define KENKEN_H

#include <QMainWindow>

class KenKen : public QMainWindow {
  Q_OBJECT

public:
  KenKen(QWidget *parent = nullptr);
  ~KenKen();
};
#endif // KENKEN_H
