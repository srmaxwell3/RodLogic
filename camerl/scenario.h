#ifndef SCENARIO_H
#define SCENARIO_H

#include <cstdio>
#include <array>
#include "tickpercycle.h"
#include "voxel.h"
#include "direction.h"
#include "voxelcoordinant.h"

class Scenario {
  friend class Rule;
 public:
  Scenario(TickPerCycle _tick, Voxel _n, Voxel _w, Voxel _self, Voxel _e, Voxel _s) :
      tick(_tick),
      self(_self)
  {
    area[DirE] = _e;
    area[DirS] = _s;
    area[DirD] = Unkn;
    area[DirW] = _w;
    area[DirN] = _n;
    area[DirU] = Unkn;
  }
  Scenario(TickPerCycle _tick, Voxel _self, array<Voxel, eoDirection> _area) :
      tick(_tick),
      self(_self),
      area(_area)
  {
  }
  Scenario(Volume const *volume, TickPerCycle t, VoxelCoordinant const &vc);
  Scenario(Scenario const &that) :
      tick(that.tick),
      self(that.self),
      area(that.area)
  {
  }
  Scenario &operator=(Scenario const &that) {
    if (this != &that) {
      tick = that.tick;
      self = that.self;
      area = that.area;
    }
    return *this;
  }

  bool operator==(Scenario const &that) const {
    return tick == that.tick && self == that.self && area == that.area;
  }
  bool operator<(Scenario const &that) const {
    return tick < that.tick ||
        (tick == that.tick && (self < that.self ||
                               (self == that.self && (area < that.area))
                              )
        );
  }
  Voxel From(Direction d) const { return area[d]; }
  Voxel FromE() const { return area[DirE]; }
  Voxel FromS() const { return area[DirS]; }
  Voxel FromD() const { return area[DirD]; }
  Voxel FromW() const { return area[DirW]; }
  Voxel FromN() const { return area[DirN]; }
  Voxel FromU() const { return area[DirU]; }
  Voxel Self() const { return self; }
  void Dump() const {
    fprintf(stdout,
            "(Scenario *)(%p)->{ %s, %s, { %s, %s, %s, %s } }\n",
            this,
            toConstCharPointer(tick),
            toConstCharPointer(self),
            toConstCharPointer(area[DirE]),
            toConstCharPointer(area[DirS]),
            toConstCharPointer(area[DirW]),
            toConstCharPointer(area[DirN])
           );
  }

 private:
  TickPerCycle tick;
  Voxel self;
  array<Voxel, eoDirection> area;
};

#endif // SCENARIO_H
