#include "direction.h"

char const *toConstCharPointer(Direction d) {
  switch (d) {
    case E: return "E";
    case S: return "S";
    case D: return "D";
    case W: return "W";
    case N: return "N";
    case U: return "U";
    case eoDirection:
      return "eoDirection";
  }
  return "Direction(?)";
}

Direction const direction[eoDirection] = {
  E, S, D, W, N, U
};
