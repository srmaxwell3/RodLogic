#include "fblkstate.h"

char const *toConstCharPointer(FBlkState s) {
  switch (s) {
    case FBUnkn: return "FBUnkn";
    case FBBlkd: return "FBBlkd";
    case FBUnbk: return "FBUnbk";
    case eoFBlkState:
      return "eoFBlkState";
  }
  return "FBlkState(?)";
}
