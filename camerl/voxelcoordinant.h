#ifndef VOXELCOORDINANT_H
#define VOXELCOORDINANT_H

#include "direction.h"

class Volume;

class VoxelCoordinant {
 public:
  VoxelCoordinant(int _l, int _r, int _c) : l(_l), r(_r), c(_c) { }
  VoxelCoordinant(VoxelCoordinant const &that) {
    l = that.l;
    r = that.r;
    c = that.c;
  }
  VoxelCoordinant &operator=(VoxelCoordinant const &that) {
    if (this != &that) {
      l = that.l;
      r = that.r;
      c = that.c;
    }
    return *this;
  }
  bool operator==(VoxelCoordinant const &that) const {
    return l == that.l && r == that.r && c == that.c;
  }
  bool operator<(VoxelCoordinant const &that) const {
    return l < that.l || (l == that.l && (r < that.r || (r == that.r && c < that.c)));
  }
  VoxelCoordinant &Move(Direction d) {
    switch (d) {
      case E: return MoveE();
      case S: return MoveS();
      case D: return MoveD();
      case W: return MoveW();
      case N: return MoveN();
      case U: return MoveU();
    default:
      return *this;
    }
    return *this;
  }
  VoxelCoordinant &MoveE() { c += 1; return *this; }
  VoxelCoordinant &MoveS() { r += 1; return *this; }
  VoxelCoordinant &MoveD() { l -= 1; return *this; }
  VoxelCoordinant &MoveW() { c -= 1; return *this; }
  VoxelCoordinant &MoveN() { r -= 1; return *this; }
  VoxelCoordinant &MoveU() { l += 1; return *this; }

  VoxelCoordinant To(Direction d) const {
    switch (d) {
      case E: return ToE();
      case S: return ToS();
      case D: return ToD();
      case W: return ToW();
      case N: return ToN();
      case U: return ToU();
    default:
      return *this;
    }
    return *this;
  }
  VoxelCoordinant ToE() const { return VoxelCoordinant(l, r, c + 1); }
  VoxelCoordinant ToS() const { return VoxelCoordinant(l, r + 1, c); }
  VoxelCoordinant ToD() const { return VoxelCoordinant(l - 1, r, c); }
  VoxelCoordinant ToW() const { return VoxelCoordinant(l, r, c - 1); }
  VoxelCoordinant ToN() const { return VoxelCoordinant(l, r - 1, c); }
  VoxelCoordinant ToU() const { return VoxelCoordinant(l + 1, r, c); }

  VoxelCoordinant &EFrom(VoxelCoordinant const &vc) {
    l = vc.l;
    r = vc.r;
    c = vc.c + 1;
    return *this;
  }
  VoxelCoordinant &SFrom(VoxelCoordinant const &vc) {
    l = vc.l;
    r = vc.r + 1;
    c = vc.c;
    return *this;
  }
  VoxelCoordinant &DFrom(VoxelCoordinant const &vc) {
    l = vc.l - 1;
    r = vc.r;
    c = vc.c;
    return *this;
  }
  VoxelCoordinant &WFrom(VoxelCoordinant const &vc) {
    l = vc.l;
    r = vc.r;
    c = vc.c - 1;
    return *this;
  }
  VoxelCoordinant &NFrom(VoxelCoordinant const &vc) {
    l = vc.l;
    r = vc.r - 1;
    c = vc.c;
    return *this;
  }
  VoxelCoordinant &UFrom(VoxelCoordinant const &vc) {
    l = vc.l + 1;
    r = vc.r;
    c = vc.c;
    return *this;
  }

  int L() const { return l; }
  int R() const { return r; }
  int C() const { return c; }

  void Dump(Volume const *volume = 0) const;

 private:
  int l;
  int r;
  int c;
};

#endif // VOXELCOORDINANT_H
