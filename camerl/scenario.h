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
  Scenario(TickPerCycle t,
           Voxel n, Voxel u, Voxel w, Voxel o, Voxel e, Voxel d, Voxel s
          ) :
      tick(t),
      self(o)
  {
    area[E] = e;
    area[S] = s;
    area[D] = d;
    area[W] = w;
    area[N] = n;
    area[U] = u;
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
  Voxel FromE() const { return area[E]; }
  Voxel FromS() const { return area[S]; }
  Voxel FromD() const { return area[D]; }
  Voxel FromW() const { return area[W]; }
  Voxel FromN() const { return area[N]; }
  Voxel FromU() const { return area[U]; }
  Voxel Self() const { return self; }
  void Dump() const {
    fprintf(stdout,
            "(Scenario *)(%p)->{ tick=%s, self=%s, area={ %s, %s, %s, %s, %s, %s } }",
            this,
            c_str(tick),
            c_str(self),
            c_str(area[E]),
            c_str(area[S]),
            c_str(area[D]),
            c_str(area[W]),
            c_str(area[N]),
            c_str(area[U])
           );
  }
  void Show() const {
    fprintf(stdout,
            "{ %s, %s, %s, %s, %s, %s, %s, %s }",
            c_str(tick),
            c_str(area[N]),
            c_str(area[U]),
            c_str(area[W]),
            c_str(self),
            c_str(area[E]),
            c_str(area[D]),
            c_str(area[S])
           );
  }

 private:
  TickPerCycle tick;
  Voxel self;
  array<Voxel, eoDirection> area;
};

#endif // SCENARIO_H
