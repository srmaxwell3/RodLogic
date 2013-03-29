#ifndef VOLUME_H
#define VOLUME_H

#include <cassert>
#include <cstddef>
using std::size_t;
#include <array>
using std::array;
#include <deque>
using std::deque;
#include <fstream>
using std::ifstream;
#include <iostream>
using std::istream;
#include <map>
using std::map;
#include <set>
using std::set;
#include <vector>
using std::vector;

#include "rule.h"
#include "scenario.h"
#include "tickpercycle.h"
#include "tickperphase.h"
#include "voxel.h"

extern bool optVerbose;

class Item;
typedef set<Item *> SetOfItems;
class LockRod;
class DataRod;

// size_t const NLvls = 7;
// size_t const NRows = 29;
// size_t const NCols = 32;
//
// typedef array<array<array<Voxel, NCols>, NRows>, NLvls> VolArray;
// typedef array<array<wchar_t, NCols>, NRows> ViewLvlArray;
typedef vector<Voxel> VoxelRank;
typedef vector<VoxelRank> VoxelPlate;
typedef vector<VoxelPlate> VoxelBrick;
typedef vector<vector<wchar_t>> ViewLvlArray;

class Volume: public VoxelBrick
{
 public:
  Volume(VoxelBrick const &initial);
  Volume(istream &in);

  char parseBrick(istream &in, char c = ' ');
  char parsePlate(VoxelPlate &plate, istream &in, char c);
  char parseRank(VoxelRank &rank, istream &in, char c);
  char parseVoxel(VoxelRank &rank, istream &in, char c);
  char parseIOD(istream &in, char c);
  char parseLabel(string &label, istream &in, char c);
  char parseExpectingA(istream &in, char c, char expecting);
  char skipWhitespaceAndComments(istream &in, char c);

  void AddRule(Scenario const &scenario, Voxel newVoxel) {
    rules[scenario] = newVoxel;
  }
  void AddRule(TickPerCycle t,
       Voxel n, Voxel u, Voxel w, Voxel o, Voxel e, Voxel d, Voxel s,
       Voxel v
      )
  {
    Scenario scenario(t, n, u, w, o, e, d, s);
    rules[scenario] = v;
    if (optVerbose) {
      fprintf(stdout,
              "(Volume *)(%p)->AddRule(%s, %s, %s, %s, %s, %s, %s, %s, %s): rules[",
              this,
              c_str(t),
              c_str(n),
              c_str(u),
              c_str(w),
              c_str(o),
              c_str(e),
              c_str(d),
              c_str(s),
              c_str(v)
             );
      scenario.Show();
      fprintf(stdout, "] = %s\n", c_str(v));
    }
  }
  Rules &GetRules() { return rules; }
  RuleCounts &GetRuleCounts() { return ruleCounts; }
  bool FoundNewRules() const {
    bool foundNewRules = false;
    for (auto const &r : rules) {
      if (r.second == Unkn) {
        fprintf(stdout, "  volume.Add");
        Rule rule(r.first, r.second);
        rule.Show();
        foundNewRules = true;
      }
    }
    return foundNewRules;
  }
  void DumpUnusedRules() const {
    for (auto const &r : ruleCounts) {
      if (r.second == 0) {
        fprintf(stdout, "Unused rule; scenario ");
        r.first.Show();
      }
    }
  }

  void AddInput(string const &label, deque<DataState> const &values) {
    for (size_t l = 0; l < inputLabels.size(); l += 1) {
      if (inputLabels[l] == label) {
        AddInput(inputCoordinates[l], values);
        return;
      }
    }
    fprintf(stderr,
            "AddInput(label=\"%s\", values): "
            "%s is not a valid input label!\n",
            label.c_str(),
            label.c_str()
            );
  }
  void AddInput(VoxelCoordinant const &vc, deque<DataState> const &values) {
    GetInputsFor(vc) = values;
  }
  deque<DataState> &GetInputsFor(VoxelCoordinant const &vc) {
    Voxel v = voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];
    assert(vProperties.voxelType == vtData);
    assert(vProperties.dataType == dtIPut);
    Direction d = rodTypeProperties[vProperties.rodType[0]].fwd;
    return inputs[d][vc];
  }
  void ProceedOneCycle();
  void ProceedOnePhase();
  void ProceedOneTick();
  void PrintViewFlat() const;
  void DumpPerformance() const;
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
    return Wall;
  }
  int CurrentClock() const { return clock; }
  int CurrentPhase() const { return (clock / NTicksPerPhase) % NPhasesPerCycle; }
  int CurrentCycle() const { return clock / NTicksPerCycle; }
  Direction CurrentPhasePerCycle() const {
    return Direction(CurrentPhase());
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

  size_t NLvls;
  size_t NRows;
  size_t NCols;

  Rules rules;
  RuleCounts ruleCounts;
  int clock;
  array<SetOfItems, eoRodType> itemsByRodType;
  array<map<VoxelCoordinant, deque<DataState>>, eoDirection> inputs;
  array<long, eoDirection> totalEvaluatedUSecsPerDirection;
  array<long, eoTickPerCycle> totalEvaluatedUSecsPerTick;
  long totalEvaluatedUSecs;
  long totalEvaluatedTicks;

  int lineNumber;
  int charNumber;
  vector<string> inputLabels;
  vector<string> outputLabels;
  vector<string> debugOutputLabels;
  vector<VoxelCoordinant> inputCoordinates;
  vector<VoxelCoordinant> outputCoordinates;
  vector<VoxelCoordinant> debugOutputCoordinates;
};

#endif // VOLUME_H
