#include "lockstate.h"

char const *toConstCharPointer(LockState t) {
  switch (t) {
    case LSUnkn: return "LSUnkn";
    case LSLckd: return "LSLckd";
    case LSUnlk: return "LSUnlk";
    case eoLockState:
      return "eoLockState";
  }
  return "LockState(?)";
}
