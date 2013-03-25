#ifndef PHASEPERCYCLE_H
#define PHASEPERCYCLE_H

#include <cstddef>

enum PhasePerCycle {
  E, S, W, N,

  eoPhasePerCycle,
  Unknown = eoPhasePerCycle
};

size_t const NPhasesPerCycle = size_t(eoPhasePerCycle);

char const *c_str(PhasePerCycle t);

struct PhaseProperties {
  Direction fwd;  // this major cycle direction forward (identity)
  Direction bwd;  // this major cycle direction backward
  Direction nxt;  // the next major cycle's forward direction
  Direction lst;  // the last major cycle's forward direction
};

extern PhaseProperties phaseProperties[eoPhasePerCycle];

#endif // PHASEPERCYCLE_H
