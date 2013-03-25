#ifndef TICKPERPHASE_H
#define TICKPERPHASE_H

#include <cstddef>

enum TickPerPhase {
  UnlockNextDataRods,
  ResetNextDataRodsFrom1,
  ResetNextDataRodsFrom0,
  RelockNextDataRods,

  UnlockDataRods,
  SetDataRodsTo0,
  SetDataRodsTo1,
  RelockDataRods,

  eoTickPerPhase
};

size_t const NTicksPerPhase = size_t(eoTickPerPhase);

char const *c_str(TickPerPhase t);

#endif // TICKPERPHASE_H
