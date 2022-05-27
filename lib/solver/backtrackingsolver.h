#ifndef BACKTRACKINGSOLVER_H
#define BACKTRACKINGSOLVER_H

#include "kenkensolver.h"

class BacktrackingSolver : public KenKenSolver
{
public:
  BacktrackingSolver();
  BacktrackingSolver(KenKenBoard board) : KenKenSolver(board) {}
  virtual ~BacktrackingSolver() {}
};

#endif // BACKTRACKINGSOLVER_H
