#include "direction.h"

char const *toConstCharPointer(Direction d) {
  switch (d) {
    case DirE: return "DirE";
    case DirS: return "DirS";
    case DirD: return "DirD";
    case DirW: return "DirW";
    case DirN: return "DirN";
    case DirU: return "DirU";
    case eoDirection:
      return "eoDirection";
  }
  return "Direction(?)";
}
