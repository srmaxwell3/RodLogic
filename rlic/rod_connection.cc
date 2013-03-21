#include "rod_connection.h"

RodIntersectionType const rodIntersectionType[eoRodIntersectionType] = {
  riNone, riCrossing, riComplement, riIdentity, riLocking
};

char const *c_str(RodIntersectionType t) {
  switch (t) {
    case riNone: return "riNone";
    case riCrossing: return "riCrossing";
    case riComplement: return "riComplement";
    case riIdentity: return "riIdentity";
    case riLocking: return "riLocking";
    case eoRodIntersectionType:
      return "eoRodIntersectionType";
  }
  return "RodIntersectionType(?)";
}

RodConnectionType const rodConnectionType[eoRodConnectionType] = {
  rcGet2, rcGet1, rcPut1, rcPut2, rcNone
};

RodConnectionType const rodConnectionThisToThat[eoDirections][eoDirections] = {
  //  that->direction ==
  //   E       S       D       W       N       U
  { rcNone, rcPut1, rcPut2, rcNone, rcGet2, rcGet1 }, // this->direction == E
  { rcGet1, rcNone, rcPut1, rcPut2, rcNone, rcGet2 }, // this->direction == S
  { rcGet2, rcGet1, rcNone, rcPut1, rcPut2, rcNone }, // this->direction == D
  { rcNone, rcGet2, rcGet1, rcNone, rcPut1, rcPut2 }, // this->direction == W
  { rcPut2, rcNone, rcGet2, rcGet1, rcNone, rcPut1 }, // this->direction == N
  { rcPut1, rcPut2, rcNone, rcGet2, rcGet1, rcNone }, // this->direction == U
};

char const *c_str(RodConnectionType t) {
  switch (t) {
    case rcGet2: return "rcGet2";
    case rcGet1: return "rcGet1";
    case rcPut1: return "rcPut1";
    case rcPut2: return "rcPut2";
    case rcNone: return "rcNone";
    case eoRodConnectionType:
      return "eoRodConnectionType";
  }
  return "RodConnectionType(?)";
}
