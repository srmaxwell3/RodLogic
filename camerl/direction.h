#ifndef DIRECTION_H
#define DIRECTION_H

// Directions of motion, in order (clockwise).

enum Direction {
  E, S, D, W, N, U,

  eoDirection,
  X = eoDirection
};

char const *toConstCharPointer(Direction d);

#endif // DIRECTION_H
