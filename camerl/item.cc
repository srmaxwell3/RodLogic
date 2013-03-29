#include "item.h"
#include "tickpercycleproperties.h"
#include "volume.h"

bool Item::AttemptToMove(Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes) {
  TickPerCycle tick = volume->CurrentTickPerCycle();
  TickPerCycleProperties const &tProperties = tickPerCycleProperties[tick];
  assert(rodType == tProperties.rodType);

  if (optVerbose) {
    fprintf(stdout,
            "(%s *)(%p)->AttemptToMove(): fwdOrBwd=%s\n",
            TypeName(),
            this,
            c_str(fwdOrBwd)
            );
    Dump(volume);
    fprintf(stdout, "\n");
  }

  if (bool isMovable = CheckForFreedomOfMovement(volume, fwdOrBwd)) {
    RodTypeProperties const &rProperties = rodTypeProperties[rodType];
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
        if (tvProperties.voxelType == vtData &&
            (tvProperties.dataType == dtInpt ||
             tvProperties.dataType == dtOutp
            )
           )
        {
          nextVoxel = tvProperties.motion[fwdOrBwd].nextVoxel;
        } else {
          prevVoxel = scenario.From(antiDirectionOfMotion);
          VoxelProperties const &pvProperties = voxelProperties[prevVoxel];
          if (pvProperties.rodType[0] == rodType) {
            if (pvProperties.voxelType == vtData &&
                (pvProperties.dataType == dtInpt ||
                 pvProperties.dataType == dtOutp
                )
               )
            {
	      static Voxel const RB[eoDirection][eoDataState] = {
		{ Unkn, DBER, DBE0, DBEX, DBE1 }, // E
		{ Unkn, DBSR, DBS0, DBSX, DBS1 }, // S
		{ Unkn, DBDR, DBD0, DBDX, DBD1 }, // D
		{ Unkn, DBWR, DBW0, DBWX, DBW1 }, // W
		{ Unkn, DBNR, DBN0, DBNX, DBN1 }, // N
		{ Unkn, DBUR, DBU0, DBUX, DBU1 }  // U
	      };
              nextVoxel = RB[tvProperties.direction][voxelProperties[tvProperties.motion[fwdOrBwd].nextVoxel].dataState];
            } else {
              nextVoxel = pvProperties.motion[fwdOrBwd].nextVoxel;
            }
          } else {
            nextVoxel = Slot;
          }
        }
        if (nextVoxel != Unkn) {
          // scenario.Dump();
          // fprintf(stdout, ", thisVoxel=%s", c_str(thisVoxel));
          // fprintf(stdout, ", prevVoxel=%s", c_str(prevVoxel));
          // fprintf(stdout, ", nextVoxel=%s\n", c_str(nextVoxel));

          changes[c] = nextVoxel;
          madeChanges = true;
        } else {
          scenario.Dump();
	  fprintf(stdout, "\n");
          fprintf(stdout, "thisVoxel=%s\n", c_str(thisVoxel));
          fprintf(stdout, "prevVoxel=%s\n", c_str(prevVoxel));
          fprintf(stdout,
                  "nextVoxel(%s) %s Unkn\n",
                  c_str(nextVoxel),
                  nextVoxel == Unkn ? "==" : "!="
                 );
          assert(nextVoxel != Unkn);
        }
      }
    }

    if (madeChanges) {
      lastMovedAt = volume->CurrentClock();
    }
    return true;
  } else {
    RodTypeProperties const &rProperties = rodTypeProperties[rodType];
    LockState lockState = GetCurrentLockState();
    DataState dataState = GetCurrentDataState();
    bool rodIsInMoveFromState = false;

    switch (rProperties.voxelType) {
      case vtLock:
        rodIsInMoveFromState = lockState == tProperties.moveFromLockState;
        break;
      case vtData:
        rodIsInMoveFromState = dataState == tProperties.moveFromDataState;
        break;
      default:
        rodIsInMoveFromState = false;
    }

    // if (rodIsInMoveFromState && tProperties.mustBeFreeToMove) {
    //   fprintf(stdout,
    //           "(%s *)(%p)->AttemptToMove(): Unable to move %s during %s: ",
    //           TypeName(),
    //           this,
    //           c_str(fwdOrBwd),
    //           c_str(tick)
    //           );
    //   Dump(volume);
    //   fprintf(stdout, "\n");
    //   assert(isMovable || !tProperties.mustBeFreeToMove);
    // }
  }
  return false;
}

bool Item::IsValid(Volume const *volume) {
  array<size_t, eoRodType> rodTypeCounts;

  rodTypeCounts.fill(0);
  bool foundMoreThan1RodType = false;
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];
    if (vProperties.voxelType != vtSlot) {
      foundMoreThan1RodType |= !vProperties.IsOneOf(rodType);
      rodTypeCounts[vProperties.rodType[0]] += 1;
    }
  }
  if (foundMoreThan1RodType) {
    fprintf(stdout,
            "(%s *)(%p)->IsValid(volume=%p): foundMoreThan1RodType!",
            TypeName(),
	    this,
            volume
           );
    fprintf(stdout,
            "  rodTypeCounts={ [%s]%lu",
            c_str(RodType(0)),
            rodTypeCounts[0]
           );
    for (size_t t = 1; t < eoRodType; t += 1) {
      fprintf(stdout,
              ", [%s]%lu",
              c_str(RodType(t)),
              rodTypeCounts[t]
              );
    }
    fprintf(stdout, " }\n");
  }
  return !foundMoreThan1RodType;
}

void Item::Dump(Volume const *volume) const {
  fprintf(stdout, "(Item *)(%p)->{", this);
  fprintf(stdout, " rodType=%s, ", c_str(rodType));
  fprintf(stdout, " fBlkState=%s, ", c_str(fBlkState));
  fprintf(stdout, " rBlkState=%s, {", c_str(rBlkState));
  char const *comma = "";
  for (auto const &c : *this) {
    fprintf(stdout, "%s ", comma);
    c.Dump(volume);
    comma = ",";
  }
  fprintf(stdout, " }");
  fprintf(stdout, " }");
}
