#include "tickpercycle.h"

char const *toConstCharPointer(TickPerCycle t) {
  switch (t) {
    case EUSL: return "EUSL";
    case ERS1: return "ERS1";
    case ERS0: return "ERS0";
    case ELSL: return "ELSL";
    case EUEL: return "EUEL";
    case ESE0: return "ESE0";
    case ESE1: return "ESE1";
    case ELEL: return "ELEL";
    case SUWL: return "SUWL";
    case SRW1: return "SRW1";
    case SRW0: return "SRW0";
    case SLWL: return "SLWL";
    case SUSL: return "SUSL";
    case SSS0: return "SSS0";
    case SSS1: return "SSS1";
    case SLSL: return "SLSL";
    case WUNL: return "WUNL";
    case WRN1: return "WRN1";
    case WRN0: return "WRN0";
    case WLNL: return "WLNL";
    case WUWL: return "WUWL";
    case WSW0: return "WSW0";
    case WSW1: return "WSW1";
    case WLWL: return "WLWL";
    case NUEL: return "NUEL";
    case NRE1: return "NRE1";
    case NRE0: return "NRE0";
    case NLEL: return "NLEL";
    case NUNL: return "NUNL";
    case NSN0: return "NSN0";
    case NSN1: return "NSN1";
    case NLNL: return "NLNL";
    case eoTickPerCycle:
      return "eoTickPerCycle";
  }
  return "TickPerCycle(?)";
}

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
