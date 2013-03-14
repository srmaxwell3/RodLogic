#ifndef ITEM_H
#define ITEM_H

#include <set>
using std::set;

#include "blockstate.h"
#include "fblkstate.h"
#include "rblkstate.h"
#include "fwdorbwd.h"
#include "rodtype.h"
#include "voxel.h"
#include "voxelcoordinant.h"

class Volume;

class Item: public set<VoxelCoordinant> {
 public:
  Item(RodType t):
      set<VoxelCoordinant>(),
      rodType(t),
      lastMovedAt(-1),
      fBlkState(fbUnkn),
      rBlkState(rbUnkn)
  {}
  virtual ~Item() {}

  virtual bool IsFBlked() const { return fBlkState == fbBlkd; }
  virtual bool IsRBlked() const { return rBlkState == rbBlkd; }
  virtual bool IsBlocked() const { return IsFBlked() || IsRBlked(); };
  virtual bool CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd) = 0;
  virtual bool CheckForFreedomOfMovement(Volume *volume) = 0;
  virtual bool AttemptToMove
      (Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes);
  virtual bool IsValid(Volume const *volume);
  virtual RodType GetRodType() const { return rodType; }
  virtual LockState GetCurrentLockState() const { return lsUnkn; }
  virtual DataState GetCurrentDataState() const { return dsUnkn; }
  virtual void Dump(Volume const *volume = 0) const;
  virtual char const *TypeName() const { return "Item"; }

 protected:
  RodType rodType;
  int lastMovedAt;
  FBlkState fBlkState;
  RBlkState rBlkState;
};

#endif // ITEM_H
