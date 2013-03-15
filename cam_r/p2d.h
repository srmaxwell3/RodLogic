#ifndef P2D_H
#define P2D_H

#include <cassert>
#include <set>
using std::set;
#include <string>
using std::string;

#include "directions.h"

struct P2D {
  P2D() : y(0), x(0) {}
  P2D(int _y, int _x) : y(_y), x(_x) { }

  bool operator<(P2D const &that) const { return y < that.y || (y == that.y && (x < that.x)); }

  P2D offsetBy(int dY, int dX) const {
    return P2D(y + dY, x + dX);
  }

  P2D offsetBy(Directions d) const {
    switch (d) {
      case E:
        return P2D(y + 0, x + 1);

      case S:
        return P2D(y + 1, x + 0);

      case W:
        return P2D(y + 0, x - 1);

      case N:
        return P2D(y - 1, x + 0);

      default:
        assert(d == E || d == S || d == W || d == N);
    }
    return P2D(y, x);
  }

  P2D offsetBy(Directions d1, Directions d2) const {
    return offsetBy(d1).offsetBy(d2);
  }

  P2D &move(Directions d) {
    switch (d) {
      case E: x += 1; break;
      case S: y += 1; break;
      case W: x -= 1; break;
      case N: y -= 1; break;
      default:
        assert(d == E || d == S || d == W || d == N);
    }
    return *this;
  }

  int limitTowards(Directions d) const {
    switch (d) {
      case E:
      case W:
        return x;
      case S:
      case N:
        return y;
      default:
        assert(d == E || d == S || d == W || d == N);
    }
    return eoDirections;
  }

  P2D &move(Directions d1, Directions d2) {
    return move(d1).move(d2);
  }

  void decY() { y -= 1; }
  void incY() { y += 1; }
  void decX() { x -= 1; }
  void incX() { x += 1; }

  string ToString() const {
    char buffer[32];
    sprintf(buffer, "P2D(%d,%d)", y + 1, x + 1);
    return buffer;
  }

  int y;
  int x;
};

struct Diagram2D;

struct SetOfP2Ds : set<P2D> {
  void dump(Diagram2D const &diagram) const;
};

#endif // P2D_H
