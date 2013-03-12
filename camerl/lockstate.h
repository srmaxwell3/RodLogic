#ifndef LOCKSTATE_H
#define LOCKSTATE_H

enum LockState {
  LSUnkn,

  LSLckd,
  LSUnlk,

  eoLockState
};

char const *toConstCharPointer(LockState t);

#endif // LOCKSTATE
