#include "lockstate.h"

char const *toConstCharPointer(LockState t) {
  switch (t) {
    case lsUnkn: return "lsUnkn";
    case lsLckd: return "lsLckd";
    case lsUnlk: return "lsUnlk";
    case eoLockState:
      return "eoLockState";
  }
  return "LockState(?)";
}
