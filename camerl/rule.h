#ifndef RULE_H
#define RULE_H

#include <map>
using std::map;

#include "scenario.h"
#include "tickpercycle.h"
#include "voxel.h"

struct Rule {
  Rule(TickPerCycle t,
       Voxel n, Voxel u, Voxel w, Voxel o, Voxel e, Voxel d, Voxel s,
       Voxel v
      ) :
      scenario(t, n, u, w, o, e, d, s),
      newVoxel(v)
  {
  }
  Rule(Scenario const &s, Voxel v) : scenario(s), newVoxel(v) { }

  void Dump() const {
    fprintf(stdout, "(Rule *)(%p)->{ scenario=", this);
    scenario.Dump();
    fprintf(stdout, ", newVoxel=%s }", c_str(newVoxel));
  }

  void Show() const {
    fprintf(stdout,
            "Rule(%s,\n"
            "        %s, %s,\n"
            "     %s, %s, %s,\n"
            "        %s, %s,\n"
            "     %s\n"
            "    );\n",
            c_str(scenario.tick),
            c_str(scenario.area[N]),
            c_str(scenario.area[U]),
            c_str(scenario.area[W]),
            c_str(scenario.self),
            c_str(scenario.area[E]),
            c_str(scenario.area[D]),
            c_str(scenario.area[S]),
            c_str(newVoxel)
           );
  }

  Scenario scenario;
  Voxel newVoxel;
};

typedef map<Scenario, Voxel> Rules;
typedef map<Scenario, size_t> RuleCounts;

#endif // RULE_H
