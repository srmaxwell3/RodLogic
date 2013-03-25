#include "fblkstate.h"

char const *c_str(FBlkState s) {
  switch (s) {
    case fbUnkn: return "fbUnkn";
    case fbBlkd: return "fbBlkd";
    case fbUnbk: return "fbUnbk";
    case eoFBlkState:
      return "eoFBlkState";
  }
  return "FBlkState(?)";
}
