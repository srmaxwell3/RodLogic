#ifndef TICKPERCYCLE_H
#define TICKPERCYCLE_H

#include <cstddef>
#include "direction.h"
#include "fwdorbwd.h"

// {E,S,W,N}
// {U{S,W,N,E}L,R{S,W,N,E}1,R{S,W,N,E}X,R{S,W,N,E}0,L{S,W,N,E}L,
//  U{E,S,W,N}L,S{E,S,W,N}0,S{E,S,W,N}X,S{E,S,W,N}1,L{E,S,W,N}L
// }

enum TickPerCycle {
  tcEUSL,tcERS1,tcERSX,tcERS0,tcELSL,tcEUEL,tcESE0,tcESEX,tcESE1,tcELEL,
  tcSUWL,tcSRW1,tcSRWX,tcSRW0,tcSLWL,tcSUSL,tcSSS0,tcSSSX,tcSSS1,tcSLSL,
  tcDUWL,tcDRW1,tcDRWX,tcDRW0,tcDLWL,tcDUSL,tcDSS0,tcDSSX,tcDSS1,tcDLSL,
  tcWUNL,tcWRN1,tcWRNX,tcWRN0,tcWLNL,tcWUWL,tcWSW0,tcWSWX,tcWSW1,tcWLWL,
  tcNUEL,tcNRE1,tcNREX,tcNRE0,tcNLEL,tcNUNL,tcNSN0,tcNSNX,tcNSN1,tcNLNL,
  tcUUEL,tcURE1,tcUREX,tcURE0,tcULEL,tcUUNL,tcUSN0,tcUSNX,tcUSN1,tcULNL,

  eoTickPerCycle
};

size_t const NPhasesPerCycle = size_t(eoDirection);
size_t const NTicksPerCycle = size_t(eoTickPerCycle);

char const *toConstCharPointer(TickPerCycle t);

#endif // TICKPERCYCLE_H
