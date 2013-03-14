#ifndef LOCKTYPE_H
#define LOCKTYPE_H

enum LockType {
  ltUnkn,

  ltBody,
  ltHead,
  ltTail,
  ltPost,
  ltLock,

  eoLockType
};

char const *toConstCharPointer(LockType t);

#endif // LOCKTYPE_H
