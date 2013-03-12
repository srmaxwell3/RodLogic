#ifndef LOCKTYPE_H
#define LOCKTYPE_H

enum LockType {
  LTUnkn,

  LTBody,
  LTHead,
  LTTail,
  LTPost,
  LTLock,

  eoLockType
};

char const *toConstCharPointer(LockType t);

#endif // LOCKTYPE_H
