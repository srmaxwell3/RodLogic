#ifndef LOCKSTATE_H
#define LOCKSTATE_H

enum LockState {
  lsUnkn,

  lsLckd,
  lsUnlk,

  eoLockState
};

char const *c_str(LockState t);

#endif // LOCKSTATE
