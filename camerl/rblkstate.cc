#include "rblkstate.h"

char const *toConstCharPointer(RBlkState s) {
  switch (s) {
    case RBUnkn: return "RBUnkn";
    case RBBlkd: return "RBBlkd";
    case RBUnbk: return "RBUnbk";
    case eoRBlkState:
      return "eoRBlkState";
  }
  return "FBlkState(?)";
}
