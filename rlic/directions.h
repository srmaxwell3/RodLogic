#ifndef DIRECTIONS_H
#define DIRECTIONS_H

enum Directions {
  E,
  S,
  D,
  W,
  N,
  U,

  eoDirections
};

Directions directions[eoDirections];

char const *c_str(Directions d);
Directions Next(Directions d);
Directions NNext(Directions d);
Directions Last(Directions d);
Directions LLast(Directions d);
Directions FWard(Directions d);
Directions BWard(Directions d);
Directions LWard(Directions d);
Directions RWard(Directions d);

#endif // DIRECTIONS_H
