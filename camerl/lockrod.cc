#include <cassert>

#include "lockrod.h"
#include "tickpercycleproperties.h"
#include "volume.h"
#include "voxel.h"

extern bool optVerbose;

bool LockRod::CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd) {
  return CheckForFreedomOfMovement(volume) ||
      fwdOrBwd == Fwd ? !IsFBlked() : !IsRBlked();
}

bool LockRod::CheckForFreedomOfMovement(Volume *volume) {
  size_t lockStateCounts[eoLockState] = { 0 };
  bool foundMoreThan1LockState = false;
  lockState = LSUnkn;

  fBlkState = FBUnkn;
  rBlkState = RBUnkn;

  size_t fBlkCounts = 0;
  size_t rBlkCounts = 0;
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];
    if (vProperties.lockState != LSUnkn) {
      lockStateCounts[vProperties.lockState] += 1;
      foundMoreThan1LockState |=
	lockState != LSUnkn &&
	lockState != vProperties.lockState;
      lockState = vProperties.lockState;
    }

    if (blockStateProperties[vProperties.blockableStates].isFBlkable) {
      VoxelCoordinant fc = vc.To(rodTypeProperties[rodType].fwd);
      if (volume->isVoxelCoordinantInBounds(fc)) {
        Voxel f = volume->voxelAt(fc);
        if (find(fc) == end() && f != Slot) {
          if (optVerbose) {
            fprintf(stdout,
                    "(LockRod *)(%p) fBlked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
                    this,
                    toConstCharPointer(v),
                    vc.L(),
                    vc.R(),
                    vc.C(),
                    toConstCharPointer(f),
                    fc.L(),
                    fc.R(),
                    fc.C()
                    );
          }
          fBlkCounts += 1;
        }
      } else {
        if (optVerbose) {
          fprintf(stdout,
                  "(LockRod *)(%p) fBlked at %s(%d,%d,%d) by edge\n",
                  this,
                  toConstCharPointer(v),
                  vc.L(),
                  vc.R(),
                  vc.C()
                  );
        }
        fBlkCounts += 1;
      }
    }
    if (blockStateProperties[vProperties.blockableStates].isRBlkable) {
      VoxelCoordinant bc = vc.To(rodTypeProperties[rodType].bwd);
      if (volume->isVoxelCoordinantInBounds(bc)) {
        Voxel b = volume->voxelAt(bc);
        if (find(bc) == end() && b != Slot) {
          if (optVerbose) {
            fprintf(stdout,
                    "(LockRod *)(%p) rBlked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
                    this,
                    toConstCharPointer(v),
                    vc.L(),
                    vc.R(),
                    vc.C(),
                    toConstCharPointer(b),
                    bc.L(),
                    bc.R(),
                    bc.C()
                    );
          }
          rBlkCounts += 1;
        }
      } else {
        if (optVerbose) {
          fprintf(stdout,
                  "(LockRod *)(%p) rBlked at %s(%d,%d,%d) by edge\n",
                  this,
                  toConstCharPointer(v),
                  vc.L(),
                  vc.R(),
                  vc.C()
                  );
        }
        rBlkCounts += 1;
      }
    }
  }

  fBlkState = fBlkCounts != 0 ? FBBlkd : FBUnbk;
  rBlkState = rBlkCounts != 0 ? RBBlkd : RBUnbk;

  return !IsBlocked();
}

bool LockRod::AttemptToMove
    (Volume *volume,
     FwdOrBwd fwdOrBwd,
     Changes &changes
    )
{
  bool isMovable = CheckForFreedomOfMovement(volume, fwdOrBwd);

  if (optVerbose) {
    fprintf(stdout,
            "(LockRod *)(%p)->AttemptToMove(): fwdOrBwd=%s\n",
            this,
            toConstCharPointer(fwdOrBwd)
            );
    Dump(volume);
    fprintf(stdout, "\n");
  }

  if (isMovable) {
    TickPerCycle tick = volume->CurrentTickPerCycle();
    TickPerCycleProperties const &tProperties = tickPerCycleProperties[tick];
    assert(rodType == tProperties.rodType);
    RodTypeProperties const &rProperties = rodTypeProperties[rodType];
    VoxelType rodVoxelType = VTLock;
    Direction directionOfMotion =
        fwdOrBwd == Fwd ? rProperties.fwd : rProperties.bwd;
    Direction antiDirectionOfMotion =
        fwdOrBwd == Fwd ? rProperties.bwd : rProperties.fwd;
    bool madeChanges = false;

    for (auto const &c : *this) {
      Scenario scenario(volume, tick, c);
      Voxel thisVoxel = scenario.Self();
      Voxel prevVoxel = Unkn;
      Voxel nextVoxel = Unkn;
      VoxelProperties const &tvProperties = voxelProperties[thisVoxel];

      if (thisVoxel == Slot || rodType == tvProperties.rodType[0]) {
        prevVoxel = scenario.From(antiDirectionOfMotion);
        VoxelProperties const &pvProperties = voxelProperties[prevVoxel];
        if (pvProperties.IsOneOf(rodType)) {
          nextVoxel = pvProperties.motion[fwdOrBwd].nextVoxel;
        } else {
          nextVoxel = Slot;
        }
      } else {
        assert(tvProperties.voxelType == VTData);
        assert(tvProperties.dataType == DTSlot ||
               tvProperties.dataType == DTJoin
              );
      }

      if (nextVoxel != Unkn) {
        changes[c] = nextVoxel;
        madeChanges = true;
      } else {
        scenario.Dump();
        fprintf(stdout, "thisVoxel=%s\n", toConstCharPointer(thisVoxel));
        fprintf(stdout, "prevVoxel=%s\n", toConstCharPointer(prevVoxel));
        fprintf(stdout,
                "nextVoxel(%s) %s Unkn\n",
                toConstCharPointer(nextVoxel),
                nextVoxel == Unkn ? "==" : "!="
               );
        assert(nextVoxel != Unkn);
      }
    }
    if (madeChanges) {
      lastMovedAt = volume->CurrentClock();
    }
    return true;
  }
  return false;
}

bool LockRod::IsValid(Volume const *volume) {
  if (!Item::IsValid(volume)) {
    return false;
  }

  array<size_t, eoRodType> lockStateCounts;

  lockStateCounts.fill(0);

  bool foundMoreThan1LockState = false;
  lockState = eoLockState;
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];
    if (vProperties.voxelType != VTSlot) {
      lockStateCounts[vProperties.lockState] += 1;
      foundMoreThan1LockState |=
          lockState != eoLockState &&
          lockState != vProperties.lockState;
      lockState = vProperties.lockState;
    }
  }
  if (foundMoreThan1LockState) {
    fprintf(stdout,
            "(LockRod *)(%p)->IsValid(volume=%p): foundMoreThan1LockState!",
            this,
            volume
           );
    fprintf(stdout,
            "  lockStateCounts={ [%s]%lu",
            toConstCharPointer(LockState(0)),
            lockStateCounts[0]
           );
    for (size_t t = 1; t < eoLockState; t += 1) {
      fprintf(stdout,
              ", [%s]%lu",
              toConstCharPointer(LockState(t)),
              lockStateCounts[t]
              );
    }
    fprintf(stdout, " }\n");
  }
  return !foundMoreThan1LockState;
}

void LockRod::Dump(Volume const *volume) const {
  fprintf(stdout, "(LockRod *)(%p)->{ ", this);
  fprintf(stdout, " lockState=%s, ", toConstCharPointer(lockState));
  Item::Dump(volume);
  fprintf(stdout, " }");
}

