#ifndef LOCKROD_H
#define LOCKROD_H

#include "fwdorbwd.h"
#include "item.h"
#include "lockstate.h"
#include "rodtype.h"
#include "voxel.h"

class Volume;

class LockRod: public Item {
 public:
  LockRod(RodType t): Item(t), lockState(lsUnkn) {}

  bool IsLocked() const { return lockState == lsLckd; }

  bool CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd);
  bool CheckForFreedomOfMovement(Volume *volume);
  // bool AttemptToMove(Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes);
  bool IsValid(Volume const *volume);
  virtual LockState GetCurrentLockState() const { return lockState; }
  void Dump(Volume const *volume = 0) const;
  char const *TypeName() const { return "LockRod"; }

 private:
  LockState lockState;
};

#endif // LOCKROD_H
