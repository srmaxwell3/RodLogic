#ifndef DIRECTION_H
#define DIRECTION_H

// Directions of motion, in order (clockwise).

enum Direction {
  DirE, DirS, DirD, DirW, DirN, DirU,

  eoDirection,
  DirX = eoDirection
};

char const *toConstCharPointer(Direction d);

#endif // DIRECTION_H
