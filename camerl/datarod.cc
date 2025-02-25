#include <cassert>

#include "datarod.h"
#include "tickpercycleproperties.h"
#include "volume.h"
#include "voxel.h"

extern bool optVerbose;

bool DataRod::CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd) {
  return CheckForFreedomOfMovement(volume) ||
      fwdOrBwd == Fwd ? !IsFBlked() : !IsRBlked();
}

bool DataRod::CheckForFreedomOfMovement(Volume *volume) {
  size_t dataStateCounts[eoDataState] = { 0 };
  bool foundMoreThan1DataState = false;
  dataState = dsUnkn;

  fBlkState = fbUnkn;
  rBlkState = rbUnkn;
  lockState = lsUnkn;

  size_t fBlkCounts = 0;
  size_t rBlkCounts = 0;
  size_t lockCounts = 0;
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];

    if (v == Slot) {
      continue;
    }

    // if (rodType != vProperties.rodType[0]) {
    //   if (vProperties.voxelType == vtData &&
    //       vProperties.dataType == dtJoin
    //      )
    //   {
    //     if (optVerbose) {
    //       fprintf(stdout,
    //     	  "(LockRod *)(%p) fBlked by %s(%d,%d,%d)\n",
    //     	  this,
    //     	  c_str(v),
    //     	  vc.L(),
    //     	  vc.R(),
    //     	  vc.C()
    //     	  );
    //     }
    //     fBlkCounts += 1;
    //   }
    //   continue;
    // }
    if (vProperties.dataState != dsUnkn) {
      dataStateCounts[vProperties.dataState] += 1;
      foundMoreThan1DataState |=
	dataState != dsUnkn &&
	dataState != vProperties.dataState;
      dataState = vProperties.dataState;
    }

    if (blockStateProperties[vProperties.blockableStates].isFBlkable) {
      if (vProperties.dataType == dtIPut) {
        TickPerPhase tickPerPhase = volume->CurrentTickPerPhase();
        deque<DataState> &values = volume->GetInputsFor(vc);
        DataState inputState = values.empty() ? dsSet0 : values.front();
        // bool fBlockedByInput = false;
        // switch (tickPerPhase) {
        //   case SetDataRodsTo0:
        //     fBlockedByInput = inputState < dsSet0;
        //     break;
        //   case SetDataRodsTo1:
        //     fBlockedByInput = inputState < dsSet1;
        //   default:
        //     break;
        // }
        bool fBlockedByInput = inputState <= dataState;
        if (fBlockedByInput) {
          fBlkCounts += 1;
          if (optVerbose) {
            fprintf(stdout,
                    "(DataRod *)(%p) fBlked at %s(%d,%d,%d) by input (%s)\n",
                    this,
                    c_str(v),
                    vc.L(),
                    vc.R(),
                    vc.C(),
                    c_str(inputState)
                    );
          }
        }
      } else {
        VoxelCoordinant fc = vc.To(rodTypeProperties[rodType].fwd);
        if (volume->isVoxelCoordinantInBounds(fc)) {
          Voxel f = volume->voxelAt(fc);
          VoxelProperties const &fvProperties = voxelProperties[f];
          if (!fvProperties.IsOneOf(rodType) && f != Slot) {
            if (optVerbose) {
              fprintf(stdout,
                      "(DataRod *)(%p) fBlked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
                      this,
                      c_str(v),
                      vc.L(),
                      vc.R(),
                      vc.C(),
                      c_str(f),
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
                    "(DataRod *)(%p) fBlked at %s(%d,%d,%d) by edge\n",
                    this,
                    c_str(v),
                    vc.L(),
                    vc.R(),
                    vc.C()
                    );
          }
          fBlkCounts += 1;
        }
      }
    }
    if (blockStateProperties[vProperties.blockableStates].isRBlkable) {
      VoxelCoordinant bc = vc.To(rodTypeProperties[rodType].bwd);
      if (volume->isVoxelCoordinantInBounds(bc)) {
        Voxel b = volume->voxelAt(bc);
        VoxelProperties const &bvProperties = voxelProperties[b];
        if (!bvProperties.IsOneOf(rodType) && b != Slot) {
          if (optVerbose) {
            fprintf(stdout,
                    "(DataRod *)(%p) rBlked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
                    this,
                    c_str(v),
                    vc.L(),
                    vc.R(),
                    vc.C(),
                    c_str(b),
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
                  "(DataRod *)(%p) rBlked at %s(%d,%d,%d) by edge\n",
                  this,
                  c_str(v),
                  vc.L(),
                  vc.R(),
                  vc.C()
                  );
        }
        rBlkCounts += 1;
      }
    }
    if (blockStateProperties[vProperties.blockableStates].isLockable) {
      if (vProperties.dataType == dtLock) {
        if (optVerbose) {
          fprintf(stdout,
                  "(DataRod *)(%p) locked at %s(%d,%d,%d)\n",
                  this,
                  c_str(v),
                  vc.L(),
                  vc.R(),
                  vc.C()
                  );
        }
        lockCounts += 1;
      }
    }
  }

  fBlkState = fBlkCounts != 0 ? fbBlkd : fbUnbk;
  rBlkState = rBlkCounts != 0 ? rbBlkd : rbUnbk;
  lockState = lockCounts != 0 ? lsLckd : lsUnlk;
  return !IsBlocked();
}

// bool DataRod::AttemptToMove
//     (Volume *volume,
//      FwdOrBwd fwdOrBwd,
//      Changes &changes
//     )
// {
//   bool isMovable = CheckForFreedomOfMovement(volume, fwdOrBwd);

//   if (optVerbose) {
//     fprintf(stdout,
//             "(DataRod *)(%p)->AttemptToMove(): fwdOrBwd=%s\n",
//             this,
//             c_str(fwdOrBwd)
//             );
//     Dump(volume);
//     fprintf(stdout, "\n");
//   }

//   if (isMovable) {
//     TickPerCycle tick = volume->CurrentTickPerCycle();
//     TickPerCycleProperties const &tProperties = tickPerCycleProperties[tick];
//     assert(rodType == tProperties.rodType);
//     RodTypeProperties const &rProperties = rodTypeProperties[rodType];
//     VoxelType rodVoxelType = rProperties.voxelType;
//     Direction directionOfMotion =
//         fwdOrBwd == Fwd ? rProperties.fwd : rProperties.bwd;
//     Direction antiDirectionOfMotion =
//         fwdOrBwd == Fwd ? rProperties.bwd : rProperties.fwd;
//     bool madeChanges = false;

//     for (auto const &c : *this) {
//       Scenario scenario(volume, tick, c);
//       Voxel thisVoxel = scenario.Self();
//       Voxel prevVoxel = Unkn;
//       Voxel nextVoxel = Unkn;
//       VoxelProperties const &tvProperties = voxelProperties[thisVoxel];

//       if (thisVoxel == Slot || rodType == tvProperties.rodType[0]) {
//         prevVoxel = scenario.From(antiDirectionOfMotion);
//         VoxelProperties const &pvProperties = voxelProperties[prevVoxel];
//         if (pvProperties.IsOneOf(rodType)) {
//           if (pvProperties.voxelType == VTData &&
//               (pvProperties.dataType == DTInpt ||
//                pvProperties.dataType == DTOutp
//                )
//               )
//           {
//             nextVoxel = tvProperties.motion[fwdOrBwd].nextVoxel;
//           } else {
//             nextVoxel = pvProperties.motion[fwdOrBwd].nextVoxel;
//           }
//         } else {
//           nextVoxel = Slot;
//         }
//         if (nextVoxel != Unkn) {
//           changes[c] = nextVoxel;
//           madeChanges = true;
//         } else {
//           scenario.Dump();
//           fprintf(stdout, "thisVoxel=%s\n", c_str(thisVoxel));
//           fprintf(stdout, "prevVoxel=%s\n", c_str(prevVoxel));
//           fprintf(stdout,
//                   "nextVoxel(%s) %s Unkn\n",
//                   c_str(nextVoxel),
//                   nextVoxel == Unkn ? "==" : "!="
//                  );
//           assert(nextVoxel != Unkn);
//         }
//       }
//     }
//     if (madeChanges) {
//       lastMovedAt = volume->CurrentClock();
//     }
//     return true;
//   }
//   return false;
// }

bool DataRod::IsValid(Volume const *volume) {
  if (!Item::IsValid(volume)) {
    return false;
  }

  array<size_t, eoRodType> lockStateCounts;
  array<size_t, eoRodType> dataStateCounts;

  lockStateCounts.fill(0);
  dataStateCounts.fill(0);

  bool foundMoreThan1LockState = false;
  lockState = eoLockState;
  bool foundMoreThan1DataState = false;
  dataState = eoDataState;
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];
    LockState lockState = vProperties.lockState;
    DataState dataState = vProperties.dataState;
    if (vProperties.voxelType != vtSlot) {
      lockStateCounts[vProperties.lockState] += 1;
      foundMoreThan1LockState |=
          lockState != eoLockState &&
          lockState != vProperties.lockState;
      lockState = vProperties.lockState;
      dataStateCounts[vProperties.dataState] += 1;
      foundMoreThan1DataState |=
          dataState != eoDataState &&
          dataState != vProperties.dataState;
      dataState = vProperties.dataState;
    }
  }
  if (foundMoreThan1LockState) {
    fprintf(stdout,
            "(DataRod *)(%p)->IsValid(volume=%p): foundMoreThan1LockState!",
            this,
            volume
           );
    fprintf(stdout,
            "  lockStateCounts={ [%s]%lu",
            c_str(LockState(0)),
            lockStateCounts[0]
           );
    for (size_t t = 1; t < eoLockState; t += 1) {
      fprintf(stdout,
              ", [%s]%lu",
              c_str(LockState(t)),
              lockStateCounts[t]
              );
    }
    fprintf(stdout, " }\n");
  }
  if (foundMoreThan1DataState) {
    fprintf(stdout,
            "(DataRod *)(%p)->IsValid(volume=%p): foundMoreThan1DataState!",
            this,
            volume
           );
    fprintf(stdout,
            "  dataStateCounts={ [%s]%lu",
            c_str(DataState(0)),
            dataStateCounts[0]
           );
    for (size_t t = 1; t < eoDataState; t += 1) {
      fprintf(stdout,
              ", [%s]%lu",
              c_str(DataState(t)),
              dataStateCounts[t]
              );
    }
    fprintf(stdout, " }\n");
  }
  return !foundMoreThan1LockState && !foundMoreThan1DataState;
}

void DataRod::Dump(Volume const *volume) const {
  fprintf(stdout, "(DataRod *)(%p)->{", this);
  fprintf(stdout, " lockState=%s, ", c_str(lockState));
  fprintf(stdout, " dataState=%s, ", c_str(dataState));
  Item::Dump(volume);
  fprintf(stdout, " }");
}
