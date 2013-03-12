#ifndef RULE_H
#define RULE_H

struct Rule {
  Rule(TickPerCycle tick, Voxel self, Voxel n, Voxel w, Voxel e, Voxel s, Voxel v) :
      scenario(tick, self, n, w, e, s),
      newVoxel(v)
  {
  }
  Rule(Scenario const &s, Voxel v) : scenario(s), newVoxel(v) { }
  void Dump() const {
    fprintf(stdout,
            "Rule(Scenario(%s, /* { */ %s, %s, %s, %s, %s /* } */), %s)",
            toConstCharPointer(scenario.tick),
            toConstCharPointer(scenario.area[DirN]),
            toConstCharPointer(scenario.area[DirW]),
            toConstCharPointer(scenario.self),
            toConstCharPointer(scenario.area[DirE]),
            toConstCharPointer(scenario.area[DirS]),
            toConstCharPointer(newVoxel)
           );
  }
  Scenario scenario;
  Voxel newVoxel;
};

#endif // RULE_H
