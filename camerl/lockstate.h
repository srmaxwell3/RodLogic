#ifndef LOCKSTATE_H
#define LOCKSTATE_H

enum LockState {
  lsUnkn,

  lsLckd,
  lsUnlk,

  eoLockState
};

char const *toConstCharPointer(LockState t);

#endif // LOCKSTATE
