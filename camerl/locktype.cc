#include "locktype.h"

char const *toConstCharPointer(LockType t) {
  switch (t) {
    case LTUnkn: return "LTUnkn";
    case LTBody: return "LTBody";
    case LTHead: return "LTHead";
    case LTTail: return "LTTail";
    case LTPost: return "LTPost";
    case LTLock: return "LTLock";
    case eoLockType:
      return "eoLockType";
  }
  return "LockType(?)";
}
