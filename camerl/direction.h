#ifndef DIRECTION_H
#define DIRECTION_H

// Directions of motion, in order (clockwise).

enum Direction {
  E, S, D, W, N, U,

  eoDirection,
  X = eoDirection
};

extern Direction const direction[eoDirection];

char const *c_str(Direction d);
Direction Next(Direction d);
Direction NNext(Direction d);
Direction Last(Direction d);
Direction LLast(Direction d);
Direction FWard(Direction d);
Direction BWard(Direction d);
Direction LWard(Direction d);
Direction RWard(Direction d);

#endif // DIRECTION_H
