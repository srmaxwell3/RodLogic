#include "rodtype.h"

char const *c_str(RodType t) {
  switch (t) {
    case rtUn: return "rtUn";
    case rtLE: return "rtLE";
    case rtLS: return "rtLS";
    case rtLD: return "rtLD";
    case rtLW: return "rtLW";
    case rtLN: return "rtLN";
    case rtLU: return "rtLU";
    case rtDE: return "rtDE";
    case rtDS: return "rtDS";
    case rtDD: return "rtDD";
    case rtDW: return "rtDW";
    case rtDN: return "rtDN";
    case rtDU: return "rtDU";
    case eoRodType:
      return "eoRodType";
  }
  return "RodType(?)";
}

RodTypeProperties rodTypeProperties[eoRodType] = {
#undef ______
#define ______ eoTickPerCycle
  { vtUnkn, { ______, ______, ______, ______ }, X, X, X, X }, // rtUn
  { vtLock, { tcWUNL, tcWLNL, tcNUEL, tcNLEL }, E, W, N, S }, // rtLE
  { vtLock, { tcNUEL, tcNLEL, tcEUEL, tcELEL }, S, N, W, E }, // rtLS
  { vtLock, { ______, ______, ______, ______ }, X, X, X, X }, // rtLD
  { vtLock, { tcEUSL, tcELSL, tcSUSL, tcSLSL }, W, E, S, N }, // rtLW
  { vtLock, { tcSUWL, tcSLWL, tcWUWL, tcWLWL }, N, S, E, W }, // rtLN
  { vtLock, { ______, ______, ______, ______ }, X, X, X, X }, // rtLU
  { vtData, { tcNRE1, tcNRE0, tcESE0, tcESE1 }, E, W, N, S }, // rtDE
  { vtData, { tcERS1, tcERS0, tcSSS0, tcSSS1 }, S, N, W, E }, // rtDS
  { vtLock, { ______, ______, ______, ______ }, X, X, X, X }, // rtDD
  { vtData, { tcSRW1, tcSRW0, tcWSW0, tcWSW1 }, W, E, S, N }, // rtDW
  { vtData, { tcWRN1, tcWRN0, tcNSN0, tcNSN1 }, N, S, E, W }, // rtDN
  { vtLock, { ______, ______, ______, ______ }, X, X, X, X }, // rtDU
#undef ______
};
