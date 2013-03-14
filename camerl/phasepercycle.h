#ifndef PHASEPERCYCLE_H
#define PHASEPERCYCLE_H

#include <cstddef>

enum PhasePerCycle {
  E, S, W, N,

  eoPhasePerCycle,
  Unknown = eoPhasePerCycle
};

size_t const NPhasesPerCycle = size_t(eoPhasePerCycle);

char const *toConstCharPointer(PhasePerCycle t);

struct PhaseProperties {
  PhasePerCycle fwd;  // this major cycle direction forward (identity)
  PhasePerCycle bwd;  // this major cycle direction backward
  PhasePerCycle nxt;  // the next major cycle's forward direction
  PhasePerCycle lst;  // the last major cycle's forward direction
};

extern PhaseProperties phaseProperties[eoPhasePerCycle];

#endif // PHASEPERCYCLE_H
