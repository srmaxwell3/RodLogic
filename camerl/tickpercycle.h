#ifndef TICKPERCYCLE_H
#define TICKPERCYCLE_H

#include <cstddef>
#include "fwdorbwd.h"

// {E,S,W,N}
// {U{S,W,N,E}L,R{S,W,N,E}1,R{S,W,N,E}X,R{S,W,N,E}0,L{S,W,N,E}L,
//  U{E,S,W,N}L,S{E,S,W,N}0,S{E,S,W,N}X,S{E,S,W,N}1,L{E,S,W,N}L
// }

enum TickPerCycle {
  EUSL, ERS1, ERS0, ELSL, EUEL, ESE0, ESE1, ELEL,
  SUWL, SRW1, SRW0, SLWL, SUSL, SSS0, SSS1, SLSL,
  WUNL, WRN1, WRN0, WLNL, WUWL, WSW0, WSW1, WLWL,
  NUEL, NRE1, NRE0, NLEL, NUNL, NSN0, NSN1, NLNL,

  eoTickPerCycle
};

size_t const NTicksPerCycle = size_t(eoTickPerCycle);

char const *toConstCharPointer(TickPerCycle t);

#endif // TICKPERCYCLE_H
