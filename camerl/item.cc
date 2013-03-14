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
            toConstCharPointer(fwdOrBwd)
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

#if 1
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
              nextVoxel = tvProperties.motion[fwdOrBwd].nextVoxel;
            } else {
              nextVoxel = pvProperties.motion[fwdOrBwd].nextVoxel;
            }
          } else {
            nextVoxel = Slot;
          }
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
      // } else {
      //   scenario.Dump();
      //   fprintf(stdout,
      //           "thisVoxel(%s) %s Slot\n",
      //           toConstCharPointer(thisVoxel),
      //           thisVoxel == Slot ? "==" : "!="
      //          );
      //   fprintf(stdout,
      //           "rodType(%s) %s tvProperties[%s](%s)\n",
      //           toConstCharPointer(rodType),
      //           rodType == tvProperties.rodType ? "==" : "!=",
      //           toConstCharPointer(thisVoxel),
      //           toConstCharPointer(tvProperties.rodType)
      //          );
      //   assert(thisVoxel == Slot || rodType == tvProperties.rodType);
      }
    }
#else
    Rules &rules = volume->GetRules();
    RuleCounts &ruleCounts = volume->GetRuleCounts();
    for (auto const &v : *this) {
      Scenario scenario(volume, tick, v);
      if (optVerbose) {
        fprintf(stdout, "(%s *)(%p)->AttemptToMove(): At ", TypeName(), this);
        v.Dump(volume);
        fprintf(stdout, ", ");
        scenario.Show();
        fprintf(stdout, "\n");
      }

      if (rules.find(scenario) == rules.end()) {
#if 0
        TickPerCycleProperties const &tProperties =
            tickPerCycleProperties[tick];
        RodTypeProperties const &rProperties =
            rodTypeProperties[tProperties.rodType];
        Direction fwdOrBwd =
            tProperties.fwdOrBwd == Fwd ? rProperties.fwd : rProperties.bwd;
        Direction fwd = rProperties.fwd;
        char const *directionOfMotion[eoDirection][eoFwdOrBwd] = {
          { ">", "<" }, // E, W
          { "v", "^" }, // S, N
          { "d", "u" }, // D, U
          { "<", ">" }, // W, E
          { "^", "v" }, // N, S
          { "u", "d" }  // U, D
        };

        switch (fwd) {
        case E:
        case W:
          fprintf(stdout,
                  "     +------+------+------|\n"
                  "%s | ???? | %s | ???? |\n"
                  "     +------+------+------|\n"
                  "     %s %s %s %s %s %s %s\n"
                  "     +------+------+------|\n"
                  "     | ???? | %s | ???? |\n"
                  "     +------+------+------|\n",
        	  toConstCharPointer(tick),
        	  toConstCharPointer(scenario.N()),
        	  directionOfMotion[fwd][Fwd],
        	  toConstCharPointer(scenario.W()),
        	  directionOfMotion[fwd][tProperties.fwdOrBwd],
        	  toConstCharPointer(scenario.O()),
        	  directionOfMotion[fwd][tProperties.fwdOrBwd],
        	  toConstCharPointer(scenario.E()),
        	  directionOfMotion[fwd][Fwd],
        	  toConstCharPointer(scenario.S())
        	 );
          break;
        case N:
        case S:
          fprintf(stdout,
                  "     +------+--%s---+------|\n"
                  "%s | ???? | %s | ???? |\n"
                  "     +------+--%s---+------|\n"
                  "     | %s | %s | %s |\n"
                  "     +------+--%s---+------|\n"
                  "     | ???? | %s | ???? |\n"
                  "     +------+--%s---+------|\n",
        	  directionOfMotion[fwd][Fwd],
        	  toConstCharPointer(tick),
        	  toConstCharPointer(scenario.N()),
        	  directionOfMotion[fwd][tProperties.fwdOrBwd],
        	  toConstCharPointer(scenario.W()),
        	  toConstCharPointer(scenario.O()),
        	  toConstCharPointer(scenario.E()),
        	  directionOfMotion[fwd][tProperties.fwdOrBwd],
        	  toConstCharPointer(scenario.S()),
        	  directionOfMotion[fwd][Fwd]
        	 );
          break;
        default:
          break;
        }

        char newVoxelChars[128];
        do {
          fprintf(stdout, "New voxel value? ");
          fscanf(stdin, " %s", newVoxelChars);
        } while (stringToVoxel.find(newVoxelChars) == stringToVoxel.end());

        Voxel newVoxel = stringToVoxel[newVoxelChars];
        rules[scenario] = newVoxel;
#else
        Voxel newVoxel = Unkn;
        rules[scenario] = newVoxel;
        fprintf(stdout,
                "(%s *)(%p)->AttemptToMove(): No existing rule!\n",
                TypeName(),
                this
               );
        continue;
#endif
        // Rule newRule(scenario, newVoxel);
        // newRule.Dump();
        // fprintf(stdout, "\n");
      }

      ruleCounts[scenario] += 1;
      Voxel newVoxel = rules[scenario];

      // if (newVoxel == Unkn) {
      //   fprintf(stdout,
      //           "(%s *)(%p)->AttemptToMove(): Incomplete (i.e. new) rule!\n",
      //           TypeName(),
      //           this
      //          );
      //   continue;
      // }

      if (optVerbose) {
        fprintf(stdout,
                "(%s *)(%p)->AttemptToMove(): Applying rule ",
                TypeName(),
                this
               );
        scenario.Dump();
        fprintf(stdout, " = %s\n", toConstCharPointer(newVoxel));
      }

      changes[v] = newVoxel;
      madeChanges = true;
    }
#endif

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
    //           toConstCharPointer(fwdOrBwd),
    //           toConstCharPointer(tick)
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
            toConstCharPointer(RodType(0)),
            rodTypeCounts[0]
           );
    for (size_t t = 1; t < eoRodType; t += 1) {
      fprintf(stdout,
              ", [%s]%lu",
              toConstCharPointer(RodType(t)),
              rodTypeCounts[t]
              );
    }
    fprintf(stdout, " }\n");
  }
  return !foundMoreThan1RodType;
}

void Item::Dump(Volume const *volume) const {
  fprintf(stdout, "(Item *)(%p)->{", this);
  fprintf(stdout, " rodType=%s, ", toConstCharPointer(rodType));
  fprintf(stdout, " fBlkState=%s, ", toConstCharPointer(fBlkState));
  fprintf(stdout, " rBlkState=%s, {", toConstCharPointer(rBlkState));
  char const *comma = "";
  for (auto const &c : *this) {
    fprintf(stdout, "%s ", comma);
    c.Dump(volume);
    comma = ",";
  }
  fprintf(stdout, " }");
  fprintf(stdout, " }");
}
