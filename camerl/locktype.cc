#include "locktype.h"

char const *c_str(LockType t) {
  switch (t) {
    case ltUnkn: return "ltUnkn";
    case ltBody: return "ltBody";
    case ltHead: return "ltHead";
    case ltTail: return "ltTail";
    case ltPost: return "ltPost";
    case ltLock: return "ltLock";
    case eoLockType:
      return "eoLockType";
  }
  return "LockType(?)";
}
