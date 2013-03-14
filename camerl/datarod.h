#ifndef DATAROD_H
#define DATAROD_H

#include "datastate.h"
#include "fwdorbwd.h"
#include "item.h"
#include "lockstate.h"
#include "rodtype.h"
#include "voxel.h"

class Volume;

class DataRod: public Item {
 public:
  DataRod(RodType t): Item(t), lockState(LSUnkn), dataState(DSUnkn) {}

  bool IsLocked() const { return lockState == LSLckd; }

  bool IsFBlked() const { return Item::IsFBlked() || IsLocked(); }
  bool IsRBlked() const { return Item::IsRBlked() || IsLocked(); }
  bool IsBlocked() const { return Item::IsBlocked() || IsLocked(); };

  bool CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd);
  bool CheckForFreedomOfMovement(Volume *volume);
  bool AttemptToMove(Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes);
  bool IsValid(Volume const *volume);
  void Dump(Volume const *volume = 0) const;
  char const *TypeName() const { return "DataRod"; }

 private:
  LockState lockState;
  DataState dataState;
};

#endif // DATAROD_H
