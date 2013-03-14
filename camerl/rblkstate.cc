#include "rblkstate.h"

char const *toConstCharPointer(RBlkState s) {
  switch (s) {
    case rbUnkn: return "rbUnkn";
    case rbBlkd: return "rbBlkd";
    case rbUnbk: return "rbUnbk";
    case eoRBlkState:
      return "eoRBlkState";
  }
  return "FBlkState(?)";
}
