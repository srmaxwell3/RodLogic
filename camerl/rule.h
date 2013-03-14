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
    fprintf(stdout, ", newVoxel=%s }", toConstCharPointer(newVoxel));
  }

  void Show() const {
    fprintf(stdout,
            "Rule(%s,\n"
            "        %s, %s,\n"
            "     %s, %s, %s,\n"
            "        %s, %s,\n"
            "     %s\n"
            "    );\n",
            toConstCharPointer(scenario.tick),
            toConstCharPointer(scenario.area[N]),
            toConstCharPointer(scenario.area[U]),
            toConstCharPointer(scenario.area[W]),
            toConstCharPointer(scenario.self),
            toConstCharPointer(scenario.area[E]),
            toConstCharPointer(scenario.area[D]),
            toConstCharPointer(scenario.area[S]),
            toConstCharPointer(newVoxel)
           );
  }

  Scenario scenario;
  Voxel newVoxel;
};

typedef map<Scenario, Voxel> Rules;
typedef map<Scenario, size_t> RuleCounts;

#endif // RULE_H
