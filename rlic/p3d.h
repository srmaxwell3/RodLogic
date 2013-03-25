#ifndef P3D_H
#define P3D_H

struct P3D {
  P3D() : z(0), y(0), x(0) {}
  P3D(int _z, int _y, int _x) : z(_z), y(_y), x(_x) { }

  bool operator==(P3D const &that) const {
    return z == that.z && y == that.y && x == that.x;
  }
  bool operator<(P3D const &that) const {
    return z < that.z || (z == that.z && (y < that.y || (y == that.y && (x < that.x))));
  }

  P3D offsetBy(int dZ, int dY, int dX) const {
    return P3D(z + dZ, y + dY, x + dX);
  }

  P3D offsetBy(Direction d) const {
    switch (d) {
      case E:
        return P3D(z + 0, y + 0, x + 1);

      case S:
        return P3D(z + 0, y + 1, x + 0);

      case D:
        return P3D(z - 1, y + 0, x + 0);

      case W:
        return P3D(z + 0, y + 0, x - 1);

      case N:
        return P3D(z + 0, y - 1, x + 0);

      case U:
        return P3D(z + 1, y + 0, x + 0);

      default:
        assert(d == E || d == S || d == D || d == W || d == N || d == U);
    }
    return P3D(z, y, x);
  }

  void move(Direction d) {
    switch (d) {
      case E: x += 1; break;
      case S: y += 1; break;
      case D: z -= 1; break;
      case W: x -= 1; break;
      case N: y -= 1; break;
      case U: z += 1; break;
      default:
        assert(d == E || d == S || d == D || d == W || d == N || d == U);
    }
  }

  void decZ() { z -= 1; }
  void incZ() { z += 1; }
  void decY() { y -= 1; }
  void incY() { y += 1; }
  void decX() { x -= 1; }
  void incX() { x += 1; }

  int z;
  int y;
  int x;
};

#endif // P3D
