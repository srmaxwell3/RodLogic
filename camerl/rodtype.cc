#include "rodtype.h"

char const *toConstCharPointer(RodType t) {
  switch (t) {
    case RTUn: return "RTUn";
    case RTLE: return "RTLE";
    case RTLS: return "RTLS";
    case RTLW: return "RTLW";
    case RTLN: return "RTLN";
    case RTDE: return "RTDE";
    case RTDS: return "RTDS";
    case RTDW: return "RTDW";
    case RTDN: return "RTDN";
    case eoRodType:
      return "eoRodType";
  }
  return "RodType(?)";
}

RodTypeProperties rodTypeProperties[eoRodType] = {
#undef ____
#define ____ eoTickPerCycle
  { VTUnkn, { ____, ____, ____, ____ }, DirX, DirX, DirX, DirX }, // RTUn
  { VTLock, { WUNL, WLNL, NUEL, NLEL }, DirE, DirW, DirN, DirS }, // RTLE
  { VTLock, { NUEL, NLEL, EUEL, ELEL }, DirS, DirN, DirW, DirE }, // RTLS
  { VTLock, { EUSL, ELSL, SUSL, SLSL }, DirW, DirE, DirS, DirN }, // RTLW
  { VTLock, { SUWL, SLWL, WUWL, WLWL }, DirN, DirS, DirE, DirW }, // RTLN
  { VTData, { NRE1, NRE0, ESE0, ESE1 }, DirE, DirW, DirN, DirS }, // RTDE
  { VTData, { ERS1, ERS0, SSS0, SSS1 }, DirS, DirN, DirW, DirE }, // RTDS
  { VTData, { SRW1, SRW0, WSW0, WSW1 }, DirW, DirE, DirS, DirN }, // RTDW
  { VTData, { WRN1, WRN0, NSN0, NSN1 }, DirN, DirS, DirE, DirW }, // RTDN
#undef ____
};
