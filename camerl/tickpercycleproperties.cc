#include "tickpercycleproperties.h"

TickPerCycleProperties tickPerCycleProperties[eoTickPerCycle] = {
  { RTLW, Fwd }, // EUSL
  { RTDS, Bwd }, // ERS1
  { RTDS, Bwd }, // ERS0
  { RTLW, Bwd }, // ELSL
  { RTLS, Fwd }, // EUEL
  { RTDE, Fwd }, // ESE0
  { RTDE, Fwd }, // ESE1
  { RTLS, Bwd }, // ELEL
  { RTLN, Fwd }, // SUWL
  { RTDW, Bwd }, // SRW1
  { RTDW, Bwd }, // SRW0
  { RTLN, Bwd }, // SLWL
  { RTLW, Fwd }, // SUSL
  { RTDS, Fwd }, // SSS0
  { RTDS, Fwd }, // SSS1
  { RTLW, Bwd }, // SLSL
  { RTLE, Fwd }, // WUNL
  { RTDN, Bwd }, // WRN1
  { RTDN, Bwd }, // WRN0
  { RTLE, Bwd }, // WLNL
  { RTLN, Fwd }, // WUWL
  { RTDW, Fwd }, // WSW0
  { RTDW, Fwd }, // WSW1
  { RTLN, Bwd }, // WLWL
  { RTLS, Fwd }, // NUEL
  { RTDE, Bwd }, // NRE1
  { RTDE, Bwd }, // NRE0
  { RTLS, Bwd }, // NLEL
  { RTLE, Fwd }, // NUEL
  { RTDN, Fwd }, // NSE0
  { RTDN, Fwd }, // NSE1
  { RTLE, Bwd }  // NLEL
};
