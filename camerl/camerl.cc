// ┃┇━┅▲△▶▷▼▽◀◁⬛
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <array>
using std::array;
#include <map>
using std::map;
#include <set>
using std::set;
#include <string>
using std::string;
#include <deque>
using std::deque;
#include <initializer_list>
using std::initializer_list;

bool optVerbose = true;

#include "direction.h"
#include "fwdorbwd.h"
#include "phasepercycle.h"
#include "tickperphase.h"
#include "tickpercycle.h"
#include "voxelcoordinant.h"
#include "fblkstate.h"
#include "rblkstate.h"
#include "voxeltype.h"
#include "locktype.h"
#include "lockstate.h"
#include "datatype.h"
#include "datastate.h"
#include "blockstate.h"
#include "rodtype.h"
#include "voxel.h"
#include "scenario.h"
#include "rule.h"

typedef map<string, Voxel> StringToVoxel;
StringToVoxel stringToVoxel;

class Item;
typedef set<Item *> SetOfItems;
class LockRod;
class DataRod;

size_t const NLvls = 7;
size_t const NRows = 28;
size_t const NCols = 25;

typedef array<array<array<Voxel, NCols>, NRows>, NLvls> VolArray;
typedef map<VoxelCoordinant, Voxel> Changes;
typedef map<Scenario, Voxel> Rules;
typedef array<array<char, NCols>, NRows> ViewLvlArray;

class Volume: public VolArray
{
 public:
  Volume(VolArray const &initial);
  Volume(Voxel initialVoxel = Unkn);
  void AddRule(Rule const &rule) { rules[rule.scenario] = rule.newVoxel; }
  void AddRule(Scenario const &scenario, Voxel newVoxel) {
    rules[scenario] = newVoxel;
  }
  Rules &GetRules() { return rules; }
  void AddInput(VoxelCoordinant const &vc, deque<DataState> const &values) {
    inputs[vc] = values;
  }
  deque<DataState> &GetInputsFor(VoxelCoordinant const &vc) { return inputs[vc]; }
  void ProceedOneCycle();
  void ProceedOnePhase();
  void ProceedOneTick();
  void PrintViewFlat() const;
  bool isVoxelCoordinantInBounds(VoxelCoordinant const &vc) const {
    return 0 <= vc.L() && vc.L() < NLvls &&
        0 <= vc.R() && vc.R() < NRows &&
        0 <= vc.C() && vc.C() < NCols;
  }
  bool isPartOf(RodType r, VoxelCoordinant const &vc, bool &reachedEnd) const;
  bool isPartOf(RodType r, VoxelCoordinant const &vc) const;
  Voxel voxelAt(VoxelCoordinant const &vc) const {
    if (isVoxelCoordinantInBounds(vc)) {
      return (*this)[vc.L()][vc.R()][vc.C()];
    }
    return Unkn;
  }
  int CurrentClock() const { return clock; }
  int CurrentCycle() const { return clock / NTicksPerCycle; }
  PhasePerCycle CurrentPhasePerCycle() const {
    return PhasePerCycle((CurrentClock() % NTicksPerCycle) / NTicksPerPhase);
  }
  TickPerCycle CurrentTickPerCycle() const {
    return TickPerCycle(CurrentClock() % NTicksPerCycle);
  }
  TickPerPhase CurrentTickPerPhase() const {
    return TickPerPhase(CurrentClock() % NTicksPerPhase);
  }

 private:
  // Voxel &voxelAt(VoxelCoordinant vc) {
  //   assert(isVoxelCoordinantInBounds(vc));
  //   return (*this)[vc.L()][vc.R()][vc.C()];
  // }
  void ViewFlat(ViewLvlArray &view) const;
  void AddToRod(RodType rodType,
                Direction fwd,
                Direction bwd,
                Item *item,
                VoxelCoordinant const &vc
               );
  Item *FormRodContaining(set<VoxelCoordinant> &seenSofar, VoxelCoordinant const &vc);
  void FindItems();

  Rules rules;
  int clock;
  array<SetOfItems, eoRodType> itemsByRodType;
  map<VoxelCoordinant, deque<DataState>> inputs;
};

class Item: public set<VoxelCoordinant> {
 public:
  Item(RodType t):
      set<VoxelCoordinant>(),
      rodType(t),
      lastMovedAt(-1),
      fBlkState(FBUnkn),
      rBlkState(RBUnkn)
  {}
  virtual ~Item() {}

  virtual bool IsFBlked() const { return fBlkState == FBBlkd; }
  virtual bool IsRBlked() const { return rBlkState == RBBlkd; }
  virtual bool IsBlocked() const { return IsFBlked() || IsRBlked(); };
  virtual bool CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd) = 0;
  virtual bool CheckForFreedomOfMovement(Volume *volume) = 0;
  virtual bool AttemptToMove
      (Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes) = 0;
  virtual bool IsValid(Volume const *volume);
  virtual RodType GetRodType() const { return rodType; }
  virtual void Dump(Volume const *volume = 0) const;
  virtual char const *TypeName() const { return "Item"; }

 protected:
  RodType rodType;
  int lastMovedAt;
  FBlkState fBlkState;
  RBlkState rBlkState;
};

class LockRod: public Item {
 public:
  LockRod(RodType t): Item(t), lockState(LSUnkn) {}

  bool IsLocked() const { return lockState == LSLckd; }

  bool CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd);
  bool CheckForFreedomOfMovement(Volume *volume);
  bool AttemptToMove(Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes);
  bool IsValid(Volume const *volume);
  void Dump(Volume const *volume = 0) const;
  char const *TypeName() const { return "LockRod"; }

 private:
  LockState lockState;
};

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

Volume::Volume(VolArray const &initial) :
    VolArray(initial),
    clock(0)
{
  FindItems();
}

Volume::Volume(Voxel initialVoxel) : clock(0) {
  for (int l = 0; l < NLvls; l += 1) {
    for (int r = 0; r < NRows; r += 1) {
      for (int c = 0; c < NCols; c += 1) {
        (*this)[l][r][c] = initialVoxel;
      }
    }
  }
  FindItems();
}

void Volume::ProceedOneCycle() {
  size_t eoTick = clock + NTicksPerCycle;
  while (clock < eoTick) {
    ProceedOneTick();
  }
}

void Volume::ProceedOnePhase() {
  size_t eoTick = clock + NTicksPerPhase;
  while (clock < eoTick) {
    ProceedOneTick();
  }
}

void Volume::ProceedOneTick() {
  size_t tick = clock % NTicksPerCycle;
  TickPerCycleProperties const &tProperties = tickPerCycleProperties[tick];
  if (optVerbose) {
    fprintf(stdout,
            "(Volume *)(%p)->ProceedOneTick(): "
            "tick=%s, tProperties.rodType=%s, tProperties.fwdOrBwd=%s\n",
            this,
            toConstCharPointer(TickPerCycle(tick)),
            toConstCharPointer(tProperties.rodType),
            toConstCharPointer(tProperties.fwdOrBwd)
            );
  }

  Changes changes;
  for (Item *i : itemsByRodType[tProperties.rodType]) {
    i->AttemptToMove(this, tProperties.fwdOrBwd, changes);
  }
  for (auto const &c : changes) {
    VoxelCoordinant vc = c.first;
    assert(isVoxelCoordinantInBounds(vc));
    (*this)[vc.L()][vc.R()][vc.C()] = c.second;
  }
  clock += 1;
}

void Volume::PrintViewFlat() const {
  ViewLvlArray view;

  ViewFlat(view);

  fprintf(stdout, "   ");
  for (int c = 0; c < NCols; c += 1) {
    if (int cc = (c / 10) % 10) {
      fprintf(stdout, "%1d", cc);
    } else {
      fprintf(stdout, " ");
    }
  }
  fprintf(stdout, "\n   ");
  for (int c = 0; c < NCols; c += 1) {
    fprintf(stdout, "%1d", c % 10);
  }
  fprintf(stdout, "\n");
  for (int r = 0; r < NRows; r += 1) {
    fprintf(stdout, "%2d ", r % 100);
    for (int c = 0; c < NCols; c += 1) {
      fprintf(stdout, "%c", view[r][c]);
    }
    fprintf(stdout, " %2d\n", r % 100);
  }
  fprintf(stdout, "   ");
  for (int c = 0; c < NCols; c += 1) {
    if (int cc = (c / 10) % 10) {
      fprintf(stdout, "%1d", cc);
    } else {
      fprintf(stdout, " ");
    }
  }
  fprintf(stdout, "\n   ");
  for (int c = 0; c < NCols; c += 1) {
    fprintf(stdout, "%1d", c % 10);
  }
  fprintf(stdout, "\n");
  fprintf(stdout,
          "Clock: %d, tick %s (cycle: %d, phase: %s, minor tick: %s)\n\n",
          CurrentClock(),
          toConstCharPointer(CurrentTickPerCycle()),
          CurrentCycle(),
          toConstCharPointer(CurrentPhasePerCycle()),
          toConstCharPointer(CurrentTickPerPhase())
         );
}

void Volume::ViewFlat(ViewLvlArray &view) const {
  for (int r = 0; r < NRows; r += 1) {
    for (int c = 0; c < NCols; c += 1) {
      int lOfMaxDisplayPriority = NLvls;
      int maxDisplayPriority = -1;
      for (int l = NLvls - 1; 0 <= l; l -= 1) {
        Voxel const &voxel = (*this)[l][r][c];
        VoxelProperties const &vProperties = voxelProperties[voxel];
        if (maxDisplayPriority < vProperties.displayPriority) {
          maxDisplayPriority = vProperties.displayPriority;
          lOfMaxDisplayPriority = l;
        }
      }
      view[r][c] = voxelProperties[(*this)[lOfMaxDisplayPriority][r][c]].text;
    }
  }
}

// Given a voxel's coordinant, representing the first voxel of a
// {lock,data} rod, create an appropriate {Lock,Data}Rod item, and
// populate it with all attached voxels (and slots).

bool Volume::isPartOf(RodType r, VoxelCoordinant const &vc, bool &reachedEnd) const {
  if (isVoxelCoordinantInBounds(vc)) {
    Voxel voxel = voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[voxel];
    if (!reachedEnd) {
      if (vProperties.IsOneOf(r)) {
        return true;
      }
      reachedEnd = true;
    }
    return voxel == Slot;
  }
  reachedEnd = true;
  return false;
}

bool Volume::isPartOf(RodType r, VoxelCoordinant const &vc) const {
  if (isVoxelCoordinantInBounds(vc)) {
    Voxel voxel = voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[voxel];
    return vProperties.IsOneOf(r);
  }
  return false;
}

void Volume::AddToRod(RodType rodType, Direction fwd, Direction bwd, Item *item, VoxelCoordinant const &vc) {
  item->insert(vc);
  bool reachedFwdEnd = false;
  for (VoxelCoordinant fc = vc.To(fwd);
       isPartOf(rodType, fc, reachedFwdEnd);
       fc.Move(fwd)
      )
  {
    item->insert(fc);
    if (!reachedFwdEnd) {
      VoxelCoordinant uc = fc.ToU();
      if (isPartOf(rodType, uc) && item->find(uc) == item->end()) {
	AddToRod(rodType, fwd, bwd, item, uc);
      }
      VoxelCoordinant dc = fc.ToD();
      if (isPartOf(rodType, dc) && item->find(dc) == item->end()) {
	AddToRod(rodType, fwd, bwd, item, dc);
      }
    }
  }
  bool reachedBwdEnd = false;
  for (VoxelCoordinant bc = vc.To(bwd);
       isPartOf(rodType, bc, reachedBwdEnd);
       bc.Move(bwd)
      )
  {
    item->insert(bc);
    if (!reachedBwdEnd) {
      VoxelCoordinant uc = bc.ToU();
      if (isPartOf(rodType, uc) && item->find(uc) == item->end()) {
	AddToRod(rodType, fwd, bwd, item, uc);
      }
      VoxelCoordinant dc = bc.ToD();
      if (isPartOf(rodType, dc) && item->find(dc) == item->end()) {
	AddToRod(rodType, fwd, bwd, item, dc);
      }
    }
  }
}

Item *Volume::FormRodContaining
  (set<VoxelCoordinant> &seenSofar, VoxelCoordinant const &vc)
{
  Voxel v = voxelAt(vc);
  VoxelProperties const &vProperties = voxelProperties[v];
  RodType rodType = vProperties.rodType[0];
  RodTypeProperties const &rProperties = rodTypeProperties[rodType];
  Item *item = 0;
  switch (rodType) {
    case RTLE: case RTLS: case RTLW: case RTLN:
      item = new LockRod(rodType);
      break;
    case RTDE: case RTDS: case RTDW: case RTDN:
      item = new DataRod(rodType);
      break;
    default:
      return 0;
  }
  AddToRod(rodType, rProperties.fwd,rProperties.bwd, item, vc);
  seenSofar.insert(item->begin(), item->end());
  if (item->IsValid(this)) {
    itemsByRodType[rodType].insert(item);
    item->CheckForFreedomOfMovement(this);
  }

  return item;
}

void Volume::FindItems() {
  for (size_t i = 0; i < eoRodType; i += 1) {
    SetOfItems &items = itemsByRodType[i];
    for (auto i : items) {
      delete i;
    }
    items.clear();
  }
  set<VoxelCoordinant> seenSofar;

  for (int l = 0; l < NLvls; l += 1) {
    for (int r = 0; r < NRows; r += 1) {
      for (int c = 0; c < NCols; c += 1) {
        VoxelCoordinant vc(l, r, c);
        Voxel v = voxelAt(vc);
	if (voxelProperties[v].isRodBody) {
	  if (seenSofar.find(vc) == seenSofar.end()) {
            Item *item = FormRodContaining(seenSofar, vc);
            if (optVerbose) {
              item->Dump(this);
              fprintf(stdout, "\n");
            }
            break;
          }
        }
      }
    }
  }
}

bool Item::IsValid(Volume const *volume) {
  array<size_t, eoRodType> rodTypeCounts;

  rodTypeCounts.fill(0);
  bool foundMoreThan1RodType = false;
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];
    if (vProperties.voxelType != VTSlot) {
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

bool DataRod::CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd) {
  return CheckForFreedomOfMovement(volume) ||
      fwdOrBwd == Fwd ? !IsFBlked() : !IsRBlked();
}

bool DataRod::CheckForFreedomOfMovement(Volume *volume) {
  size_t dataStateCounts[eoDataState] = { 0 };
  bool foundMoreThan1DataState = false;
  dataState = DSUnkn;

  fBlkState = FBUnkn;
  rBlkState = RBUnkn;
  lockState = LSUnkn;

  size_t fBlkCounts = 0;
  size_t rBlkCounts = 0;
  size_t lockCounts = 0;
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];
    if (rodType != vProperties.rodType[0]) {
      continue;
    }

    if (vProperties.dataState != DSUnkn) {
      dataStateCounts[vProperties.dataState] += 1;
      foundMoreThan1DataState |=
	dataState != DSUnkn &&
	dataState != vProperties.dataState;
      dataState = vProperties.dataState;
    }

    if (blockStateProperties[vProperties.blockableStates].isFBlkable) {
      if (vProperties.dataType == DTInpt) {
        TickPerPhase tickPerPhase = volume->CurrentTickPerPhase();
        deque<DataState> &values = volume->GetInputsFor(vc);
        DataState inputState = values.empty() ? DSSet0 : values.front();
        bool fBlockedByInput = false;
        switch (tickPerPhase) {
          case SetDataRodsTo0:
            fBlockedByInput = inputState < DSSet0;
            break;
          case SetDataRodsTo1:
            fBlockedByInput = inputState < DSSet1;
            if (!values.empty()) {
              values.pop_front();
            }
          default:
            break;
        }
        if (fBlockedByInput) {
          fBlkCounts += 1;
          if (optVerbose) {
            fprintf(stdout,
                    "(DataRod *)(%p) fBlked at %s(%d,%d,%d) by input (%s)\n",
                    this,
                    toConstCharPointer(v),
                    vc.L(),
                    vc.R(),
                    vc.C(),
                    toConstCharPointer(inputState)
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
                    "(DataRod *)(%p) fBlked at %s(%d,%d,%d) by edge\n",
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
                  "(DataRod *)(%p) rBlked at %s(%d,%d,%d) by edge\n",
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
    if (blockStateProperties[vProperties.blockableStates].isLockable) {
      if (vProperties.dataType == DTLock) {
        if (optVerbose) {
          fprintf(stdout,
                  "(DataRod *)(%p) locked at %s(%d,%d,%d)\n",
                  this,
                  toConstCharPointer(v),
                  vc.L(),
                  vc.R(),
                  vc.C()
                  );
        }
        lockCounts += 1;
      }
    }
  }

  fBlkState = fBlkCounts != 0 ? FBBlkd : FBUnbk;
  rBlkState = rBlkCounts != 0 ? RBBlkd : RBUnbk;
  lockState = lockCounts != 0 ? LSLckd : LSUnlk;
  return !IsBlocked();
}

bool DataRod::AttemptToMove
    (Volume *volume,
     FwdOrBwd fwdOrBwd,
     Changes &changes
    )
{
  bool isMovable = CheckForFreedomOfMovement(volume, fwdOrBwd);

  if (optVerbose) {
    fprintf(stdout,
            "(DataRod *)(%p)->AttemptToMove(): fwdOrBwd=%s\n",
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
    VoxelType rodVoxelType = rProperties.voxelType;
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
          if (pvProperties.voxelType == VTData &&
              (pvProperties.dataType == DTInpt ||
               pvProperties.dataType == DTOutp
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
    }
    if (madeChanges) {
      lastMovedAt = volume->CurrentClock();
    }
    return true;
  }
  return false;
}

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
    if (vProperties.voxelType != VTSlot) {
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
  if (foundMoreThan1DataState) {
    fprintf(stdout,
            "(DataRod *)(%p)->IsValid(volume=%p): foundMoreThan1DataState!",
            this,
            volume
           );
    fprintf(stdout,
            "  dataStateCounts={ [%s]%lu",
            toConstCharPointer(DataState(0)),
            dataStateCounts[0]
           );
    for (size_t t = 1; t < eoDataState; t += 1) {
      fprintf(stdout,
              ", [%s]%lu",
              toConstCharPointer(DataState(t)),
              dataStateCounts[t]
              );
    }
    fprintf(stdout, " }\n");
  }
  return !foundMoreThan1LockState && !foundMoreThan1DataState;
}

void DataRod::Dump(Volume const *volume) const {
  fprintf(stdout, "(DataRod *)(%p)->{", this);
  fprintf(stdout, " lockState=%s, ", toConstCharPointer(lockState));
  fprintf(stdout, " dataState=%s, ", toConstCharPointer(dataState));
  Item::Dump(volume);
  fprintf(stdout, " }");
}

  //   abcdefghijklmnopqrstuvwxy
  // a ......v........... ...... a
  // b ......|...........^...... b
  // c ......|...........|...... c
  // d ......|...........|...... d
  // e ......|.....>..>..|...... e
  // f ......|.....>..>..|...... f
  // g  <----------X- X--------< g
  // h ......|.....|..|..|...... h
  // i ......|.....v..v..|...... i
  // j I---^^X---0-|-1|>.|.....C j
  // k ......|.....|..|..|...... k
  // l ......|.....|..v..|...... l
  // m I---^^X-----|-1|>.|.....D m
  // n ......|.....|..|..|...... n
  // o ......|..^..v..|..|...... o
  // p ....^^X->|-1|--|--------O p
  // q ......|..1..0..0..|...... q
  // r ......|..|..|..|..|...... r
  // s ......|.<|-0|<-|<-Xvv.... s
  // t ......|..^..v..v..|...... t
  // u ......|..|........|...... u
  // v >--------X------------->  v
  // w ......|..<........|...... w
  // x ......|..<........|...... x
  // y ......|...........|...... y
  // z ......|...........|...... z
  // A ......v...........|...... A
  // B ...... ...........^...... B
  //   abcdefghijklmnopqrstuvwxy

VolArray initialVolume = {
  //   abcdefghijklmnopqrstuvwxy
  // a ......v........... ...... a
  // b ......|...........^...... b
  // c ......|...........|...... c
  // d ......|...........|...... d
  // e ......|...........|...... e
  // f ......|...........|...... f
  // g ......|...........|...... g
  // h ......|...........|...... h
  // i ......|...........|...... i
  // j ......+...........|...... j
  // k ......|...........|...... k
  // l ......|...........|...... l
  // m ......+...........|...... m
  // n ......|...........|...... n
  // o ......|...........|...... o
  // p ......+...........|...... p
  // q ......|...........|...... q
  // r ......|...........|...... r
  // s ......|...........+...... s
  // t ......|...........|...... t
  // u ......|...........|...... u
  // v ......|...........|...... v
  // w ......|...........|...... w
  // x ......|...........|...... x
  // y ......|...........|...... y
  // z ......|...........|...... z
  // A ......v...........|...... A
  // B ...... ...........^...... B
  //   abcdefghijklmnopqrstuvwxy

  //        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y
  /* a */ Wall,Wall,Wall,Wall,Wall,Wall,LTSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall, /* a */
  /* b */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LHNL,Wall,Wall,Wall,Wall,Wall,Wall, /* b */
  /* c */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* c */
  /* d */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* d */
  /* e */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* e */
  /* f */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* f */
  /* g */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* g */
  /* h */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* h */
  /* i */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* i */
  /* j */ Wall,Wall,Wall,Wall,Wall,Wall,LPSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* j */
  /* k */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* k */
  /* l */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* l */
  /* m */ Wall,Wall,Wall,Wall,Wall,Wall,LPSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* m */
  /* n */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* n */
  /* o */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* o */
  /* p */ Wall,Wall,Wall,Wall,Wall,Wall,LPSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* p */
  /* q */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* q */
  /* r */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* r */
  /* s */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LPNL,Wall,Wall,Wall,Wall,Wall,Wall, /* s */
  /* t */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* t */
  /* u */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* u */
  /* v */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* v */
  /* w */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* w */
  /* x */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* x */
  /* y */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* y */
  /* z */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* z */
  /* A */ Wall,Wall,Wall,Wall,Wall,Wall,LHSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* A */
  /* B */ Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LTNL,Wall,Wall,Wall,Wall,Wall,Wall, /* B */
  //        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ......................... e
  // f ......................... f
  // g ......................... g
  // h ......................... h
  // i ......................... i
  // j ....^^X  ................ j
  // k ...... .................. k
  // l ......................... l
  // m ....^^X  ................ m
  // n ...... .................. n
  // o ......................... o
  // p ....^^X  ................ p
  // q ...... .................. q
  // r .................. ...... r
  // s ................  Xvv.... s
  // t ......................... t
  // u ......................... u
  // v ......................... v
  // w ......................... w
  // x ......................... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

  //        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y
  /* a */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* a */
  /* b */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* b */
  /* c */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* c */
  /* d */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* d */
  /* e */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* e */
  /* f */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* f */
  /* g */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* g */
  /* h */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* h */
  /* i */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* i */
  /* j */ Wall,Wall,Wall,Wall,DSER,DSER,DKER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* j */
  /* k */ Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* k */
  /* l */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* l */
  /* m */ Wall,Wall,Wall,Wall,DSER,DSER,DKER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* m */
  /* n */ Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* n */
  /* o */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* o */
  /* p */ Wall,Wall,Wall,Wall,DSER,DSER,DKER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* p */
  /* q */ Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* q */
  /* r */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall, /* r */
  /* s */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,DKWR,DSWR,DSWR,Wall,Wall,Wall,Wall, /* s */
  /* t */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* t */
  /* u */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* u */
  /* v */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* v */
  /* w */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* w */
  /* x */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* x */
  /* y */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* y */
  /* z */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* z */
  /* A */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* A */
  /* B */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* B */
  //        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ......................... e
  // f ......................... f
  // g ......................... g
  // h ......................... h
  // i ......................... i
  // j I---^^X---0---1->  ...... j
  // k ......................... k
  // l ......................... l
  // m I---^^X-------1->  ...... m
  // n ......................... n
  // o ......................... o
  // p ....^^X->--1------------O p
  // q ......................... q
  // r ......................... r
  // s ......  <--0-<--<-Xvv.... s
  // t ......................... t
  // u ......................... u
  // v ......................... v
  // w ......................... w
  // x ......................... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
DIER,DBER,DBER,DBER,DPER,DPER,DPER,DBER,DBER,DBER,DPER,DBER,DBER,DBER,DPER,DBER,DHER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
DIER,DBER,DBER,DBER,DPER,DPER,DPER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DPER,DBER,DHER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,DPER,DPER,DPER,DBER,DTER,DBER,DBER,DPER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DOER,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,DHWR,DBWR,DBWR,DPWR,DBWR,DTWR,DBWR,DBWR,DTWR,DBWR,DPWR,DPWR,DPWR,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ......................... e
  // f ......................... f
  // g ......................... g
  // h ......................... h
  // i ............v..v......... i
  // j ..........0  .1  ........ j
  // k ............ .. ......... k
  // l ...............v......... l
  // m ..............1  ........ m
  // n ............... ......... n
  // o ......... ..v............ o
  // p ........>  1  ........... p
  // q .........1..0..0......... q
  // r ......... .. .. ......... r
  // s ........   0 <  <........ s
  // t .........^.. .. ......... t
  // u ......................... u
  // v ......................... v
  // w ......................... w
  // x ......................... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQSR,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DGER,Slot,Slot,Wall,DGER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DGER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQER,Slot,Slot,DQER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQNR,Wall,Wall,DQSR,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,Slot,DGWR,Slot,DQWR,Slot,Slot,DQWR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQNR,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ............>..>......... e
  // f ............>..>......... f
  // g ............X..X......... g
  // h ............|..|......... h
  // i ............v..v......... i
  // j ............|..|......... j
  // k ............|..|......... k
  // l ............|..v......... l
  // m ......... ..|..|......... m
  // n ......... ..|..|......... n
  // o .........^..v..|......... o
  // p .........|..|..|......... p
  // q .........1..0..0......... q
  // r .........|..|..|......... r
  // s .........|..|..|......... s
  // t .........^..v..v......... t
  // u .........|.. .. ......... u
  // v .........X.. .. ......... v
  // w .........<............... w
  // x .........<............... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DHNR,Wall,Wall,DPSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,DPSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,DHSR,Wall,Wall,DHSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ............>..>......... e
  // f ............>..>......... f
  // g ........... X. X......... g
  // h ............ .. ......... h
  // i ............ .. ......... i
  // j ......................... j
  // k ......................... k
  // l ......................... l
  // m ......................... m
  // n ......................... n
  // o ......................... o
  // p ......................... p
  // q ......................... q
  // r ......................... r
  // s ......................... s
  // t ......... ............... t
  // u ......... ............... u
  // v .........X .............. v
  // w .........<............... w
  // x .........<............... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSSR,Wall,Wall,DSSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSSR,Wall,Wall,DSSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,DKSR,Wall,Slot,DKSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DKNR,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ......................... e
  // f ......................... f
  // g  <----------X--X--------< g
  // h ......................... h
  // i ......................... i
  // j ......................... j
  // k ......................... k
  // l ......................... l
  // m ......................... m
  // n ......................... n
  // o ......................... o
  // p ......................... p
  // q ......................... q
  // r ......................... r
  // s ......................... s
  // t ......................... t
  // u ......................... u
  // v >--------X------------->  v
  // w ......................... w
  // x ......................... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Slot,LHWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LPWL,LBWL,LBWL,LPWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LTWL,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
LTWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LPWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LHWL,Slot,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
};

int main(int argc, char const *argv[]) {
  for (size_t i = 0; i < eoVoxel; i += 1) {
    Voxel v = Voxel(i);

    stringToVoxel[toConstCharPointer(v)] = v;
  }

  Volume volume(initialVolume);
  volume.AddInput
      (VoxelCoordinant(2,  8, 0),
       { DSSet1, DSSet0, DSSet0, DSSet1, DSSet0, DSSet0 }
      );
  volume.AddInput
      (VoxelCoordinant(2, 12, 0),
       { DSSet0, DSSet0, DSSet1, DSSet1, DSSet0, DSSet1 }
      );

  volume.PrintViewFlat();
  for (size_t t = 0; t < (6 * NTicksPerCycle); t += 1) {
    volume.ProceedOneTick();
    volume.PrintViewFlat();
    // for (auto const &r : volume.GetRules()) {
    //   if (r.second == Unkn) {
    //     fprintf(stdout, "  volume.Add");
    //     Rule rule(r.first, r.second);
    //     rule.Dump();
    //     fprintf(stdout, ";\n");
    //   }
    // }
  }
  return 0;
}
