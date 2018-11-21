#include <cassert>
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
#if 1
#define CHRONO_CLOCK high_resolution_clock
#else
#define CHRONO_CLOCK steady_clock
#endif
using std::chrono::CHRONO_CLOCK;

#include "ncurses.h"

#include "datarod.h"
#include "item.h"
#include "lockrod.h"
#include "tickpercycleproperties.h"
#include "volume.h"

extern bool optDelayMSecSet;
extern bool optShowPerformance;
extern bool optVerbose;

Volume::Volume(VoxelBrick const &initial) :
    VoxelBrick(initial),
    NLvls(initial.size()),
    NRows(0),
    NCols(0),
    clock(0),
    totalEvaluatedUSecs(0),
    totalEvaluatedTicks(0),
    lineNumber(1),
    charNumber(0)
{
  for (auto const &lvl : initial) {
    if (NRows < lvl.size()) {
      NRows = lvl.size();
    }
    for (auto const &row : lvl) {
      if (NCols < row.size()) {
        NCols = row.size();
      }
    }
  }

  totalEvaluatedUSecsPerDirection.fill(0);
  totalEvaluatedUSecsPerTick.fill(0);

  FindItems();
}

Volume::Volume(istream &in) :
    VoxelBrick(),
    NLvls(0),
    NRows(0),
    NCols(0),
    clock(0),
    totalEvaluatedUSecs(0),
    totalEvaluatedTicks(0),
    lineNumber(1),
    charNumber(0)
{
  // Read and parse the input.
  char c = parseBrick(in);

  // Determine sizes.
  NLvls = size();
  for (auto const &lvl : *this) {
    if (NRows < lvl.size()) {
      NRows = lvl.size();
    }
    for (auto const &row : lvl) {
      if (NCols < row.size()) {
        NCols = row.size();
      }
    }
  }
  for (auto &lvl : *this) {
    lvl.resize(NRows);
    for (auto &row : lvl) {
      row.resize(NCols, Wall);
    }
  }

  fprintf(stdout, "Read a %lu x %lu x %lu brick....\n", NLvls, NRows, NCols);

  for (size_t z = 0; z < NLvls; z += 1) {
    for (size_t y = 0; y < NRows; y += 1) {
      for (size_t x = 0; x < NCols; x += 1) {
        Voxel const &v = (*this)[z][y][x];
        VoxelProperties const &vp = voxelProperties[v];
        if (vp.voxelType == vtData) {
          VoxelCoordinant vc(z, y, x);
          switch (vp.dataType) {
            case dtIPut:
              inputCoordinates.push_back(vc);
              break;
            case dtOPut:
              outputCoordinates.push_back(vc);
              break;
            case dtDPut:
              debugOutputCoordinates.push_back(vc);
              break;
            default:
              break;
          }
        }
      }
    }
  }

  if (c != ';') {
    parseIOD(in, c);
  }

  int iodErrors = 0;
  if (inputCoordinates.size() != inputLabels.size()) {
    fflush(stdout);
    fprintf(stderr,
            "Volume(): Found %lu input voxels, but have %lu input labels!\n",
            inputCoordinates.size(),
            inputLabels.size()
           );
    iodErrors += 1;
  }
  if (outputCoordinates.size() != outputLabels.size()) {
    fflush(stdout);
    fprintf(stderr,
            "Volume(): Found %lu output voxels, but have %lu output labels!\n",
            outputCoordinates.size(),
            outputLabels.size()
           );
    iodErrors += 1;
  }
  if (debugOutputCoordinates.size() != debugOutputLabels.size()) {
    fflush(stdout);
    fprintf(stderr,
            "\nVolume(): Found %lu debugOutput voxels, but have %lu debugOutput labels!\n",
            debugOutputCoordinates.size(),
            debugOutputLabels.size()
           );
    iodErrors += 1;
  }
  if (iodErrors) {
    exit(1);
  }

  char const *comma;
  fprintf(stdout, "Inputs={ ");
  comma = "";
  for (size_t i = 0; i < inputLabels.size(); i += 1) {
    fprintf(stdout, "%s%s @ ", comma, inputLabels[i].c_str());
    inputCoordinates[i].Dump(this);
    comma = ", ";
  }
  fprintf(stdout, " }\nOutputs={ ");
  comma = "";
  for (size_t i = 0; i < outputLabels.size(); i += 1) {
    fprintf(stdout, "%s%s @ ", comma, outputLabels[i].c_str());
    outputCoordinates[i].Dump(this);
    comma = ", ";
  }
  fprintf(stdout, " }\nDebugOutputs={ ");
  comma = "";
  for (size_t i = 0; i < debugOutputLabels.size(); i += 1) {
    fprintf(stdout, "%s%s @ ", comma, debugOutputLabels[i].c_str());
    debugOutputCoordinates[i].Dump(this);
    comma = ", ";
  }
  fprintf(stdout, " }\n");

  totalEvaluatedUSecsPerDirection.fill(0);
  totalEvaluatedUSecsPerTick.fill(0);

  FindItems();
}

char Volume::skipWhitespaceAndComments(istream &in, char c) {
  int state = 0;
  for (/* c = c */;
       in.good() && !in.eof();
       charNumber += 1, c = in.get()
      )
  {
    switch (state) {
      case 0:
        if (isspace(c)) {
          if (c == '\n') {
            lineNumber += 1;
            charNumber = 0;
          }
          continue;
        } else if (c == '/') {
          state = 1;
          continue;
        }
        return c;
      case 1:
        if (c == '/') {
          state = 2;
          continue;
        } else if (c == '*') {
          state = 3;
          continue;
        }
        return c;
      case 2:
        if (c == '\n') {
          lineNumber += 1;
          charNumber = 0;
          state = 0;
        }
        continue;
      case 3:
        if (c == '*') {
          state = 4;
        } else if (c == '\n') {
          lineNumber += 1;
          charNumber = 0;
        }
        continue;
      case 4:
        state = (c == '/') ? 0 : 3;
        continue;
    }
  }
  return c;
}

char Volume::parseExpectingA(istream &in, char c, char expecting) {
  if ((c = skipWhitespaceAndComments(in, c)) != expecting) {
    fflush(stdout);
    fprintf(stderr,
            "\nparseExpectingA(): %d:%d: Missing '%c' (at '%c')!\n",
            lineNumber,
            charNumber,
            expecting,
            c
           );
    exit(1);
  }
  charNumber += 1, c = in.get();
  return c;
}

char Volume::parseBrick(istream &in, char c) {
  c = parseExpectingA(in, c, '{');

  push_back(VoxelPlate());
  for (c = parsePlate(back(), in, c);
       (c = skipWhitespaceAndComments(in, c)) == ',';
       c = parsePlate(back(), in, (c = skipWhitespaceAndComments(in, c)))
      )
  {
    // fprintf(stdout, ",\n");
    charNumber += 1, c = in.get();
    push_back(VoxelPlate());
  }

  c = parseExpectingA(in, c, '}');
  return c;
}

char Volume::parsePlate(VoxelPlate &plate, istream &in, char c) {
  c = parseExpectingA(in, c, '{');

  plate.push_back(VoxelRank());
  for (c = parseRank(plate.back(), in, c);
       (c = skipWhitespaceAndComments(in, c)) == ',';
       c = parseRank(plate.back(), in, (c = skipWhitespaceAndComments(in, c)))
      )
  {
    // fprintf(stdout, ",\n");
    charNumber += 1, c = in.get();
    plate.push_back(VoxelRank());
  }

  c = parseExpectingA(in, c, '}');
  return c;
}

char Volume::parseRank(VoxelRank &rank, istream &in, char c) {
  c = parseExpectingA(in, c, '{');

  for (c = parseVoxel(rank, in, c);
       (c = skipWhitespaceAndComments(in, c)) == ',';
       c = parseVoxel(rank, in, (c = skipWhitespaceAndComments(in, c)))
      )
  {
    // fprintf(stdout, ",");
    charNumber += 1, c = in.get();
  }

  c = parseExpectingA(in, c, '}');
  return c;
}

char Volume::parseVoxel(VoxelRank &rank, istream &in, char c) {
  string symbol;

  c = parseLabel(symbol, in, c);

  Voxel voxel = StringToVoxel(symbol);
  if (voxel == eoVoxel) {
    fflush(stdout);
    fprintf(stderr,
            "\nparseVoxel(): %d:%d: %s is not a Voxel!\n",
            lineNumber,
            charNumber,
            symbol.c_str()
           );
    exit(1);
  }

  // fprintf(stdout, "%s", symbol.c_str());
  rank.push_back(voxel);
  return c;
}

char Volume::parseIOD(istream &in, char c) {
  for (c = skipWhitespaceAndComments(in, c);
       !in.eof();
       c = skipWhitespaceAndComments(in, c)
      )
  {
    string keyword;
    c = parseLabel(keyword, in, c);

    if (keyword != "Inputs" && keyword != "Outputs" && keyword != "DebugOutputs") {
      fflush(stdout);
      fprintf(stderr,
              "\nparseIOD(): %d:%d: "
              "Found \"%s\", expecting {Inputs,Outputs,DebugOutputs} (at '%c')!\n",
              lineNumber,
              charNumber,
              keyword.c_str(),
              c
              );
      exit(1);
    }

    c = parseExpectingA(in, c, '=');
    c = parseExpectingA(in, c, '{');

    if ((c = skipWhitespaceAndComments(in, c)) != '}') {
      in.unget();

      string label;
      do {
        charNumber += 1, c = in.get();

        c = parseLabel(label, in, c);

        if (keyword == "Inputs") {
          inputLabels.push_back(label);
        } else if (keyword == "Outputs") {
          outputLabels.push_back(label);
        } else {
          debugOutputLabels.push_back(label);
        }
      } while ((c = skipWhitespaceAndComments(in, c)) == ',');
    }

    c = parseExpectingA(in, c, '}');
  }

  return c;
}

char Volume::parseLabel(string &label, istream &in, char c) {
  label.clear();
  for (c = skipWhitespaceAndComments(in, c);
       isalnum(c) || c == '!' || c == '.' || c == '_' || c == '-' || c == '+' || c == '&' || c == '|';
       charNumber += 1, c = in.get()
      )
  {
    label.push_back(c);
  }
  if (label.empty()) {
    fflush(stdout);
    fprintf(stderr,
            "\nparseLabel(): %d:%d: Missing <label> (at '%c')!\n",
            lineNumber,
            charNumber,
            c
           );
    exit(1);
  }
  return c;
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
  if (clock % NTicksPerPhase == 0) {
    Direction d = Direction((clock / NTicksPerPhase) % NPhasesPerCycle);
    for (auto &vci : inputs[d]) {
      if (!vci.second.empty()) {
        vci.second.pop_front();
      }
    }
  }

  size_t tick = clock % NTicksPerCycle;
  TickPerCycleProperties const &tProperties = tickPerCycleProperties[tick];
  if (optVerbose) {
    fprintf(stdout,
            "(Volume *)(%p)->ProceedOneTick(): "
            "tick=%s, tProperties.rodType=%s, tProperties.fwdOrBwd=%s\n",
            this,
            c_str(TickPerCycle(tick)),
            c_str(tProperties.rodType),
            c_str(tProperties.fwdOrBwd)
            );
  }

  auto boEvaluation = CHRONO_CLOCK::now();

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

  auto eoEvaluation = CHRONO_CLOCK::now();
  auto dTEvaluation = duration_cast<microseconds>(eoEvaluation - boEvaluation).count();
  totalEvaluatedUSecsPerDirection[totalEvaluatedTicks % eoDirection] += dTEvaluation;
  totalEvaluatedUSecsPerTick[totalEvaluatedTicks % eoTickPerCycle] += dTEvaluation;
  totalEvaluatedUSecs += dTEvaluation;

  if (optShowPerformance) {
    fprintf(stdout,
            "%ld [%ld.%s]: dT = %lld us\n",
            totalEvaluatedTicks,
            totalEvaluatedTicks / eoTickPerCycle,
            c_str(TickPerCycle(totalEvaluatedTicks % eoTickPerCycle)),
            (long long) dTEvaluation
            );
  }
  totalEvaluatedTicks += 1;
}

void Volume::PrintViewFlat() const {
  static ViewLvlArray views[2];
  static int whichView = 0;

  int v = whichView % 2;
  ViewFlat(views[v]);

  if (optDelayMSecSet) {
    mvprintw(0, 0,
             "Clock: %d, tick %s (cycle: %d, phase: %s, minor tick: %s)\n",
             CurrentClock(),
             c_str(CurrentTickPerCycle()),
             CurrentCycle(),
             c_str(CurrentPhasePerCycle()),
             c_str(CurrentTickPerPhase())
             );
    printw("   ");
    for (int c = 0; c < NCols; c += 1) {
      if (int cc = (c / 10) % 10) {
        printw("%1d", cc);
      } else {
        printw(" ");
      }
    }
    printw("\n   ");
    for (int c = 0; c < NCols; c += 1) {
      printw("%1d", c % 10);
    }
    printw("\n");
    for (int r = 0; r < NRows; r += 1) {
      printw("%2d ", r % 100);
      for (int c = 0; c < NCols; c += 1) {
        if (0 < whichView && views[v][r][c] != views[v ^ 1][r][c]) {
          addch(views[v][r][c] | A_BOLD);
        } else {
          printw("%c", views[v][r][c]);
        }
      }
      printw(" %2d\n", r % 100);
    }
    printw("   ");
    for (int c = 0; c < NCols; c += 1) {
      if (int cc = (c / 10) % 10) {
        printw("%1d", cc);
      } else {
        printw(" ");
      }
    }
    printw("\n   ");
    for (int c = 0; c < NCols; c += 1) {
      printw("%1d", c % 10);
    }
    printw("\n");
    refresh();
    whichView += 1;
  } else {
    fprintf(stdout,
            "Clock: %d, tick %s (cycle: %d, phase: %s, minor tick: %s)\n",
            CurrentClock(),
            c_str(CurrentTickPerCycle()),
            CurrentCycle(),
            c_str(CurrentPhasePerCycle()),
            c_str(CurrentTickPerPhase())
            );
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
        fprintf(stdout, "%c", views[v][r][c]);
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
    fprintf(stdout, "\n\n");
  }
}

void Volume::ViewFlat(ViewLvlArray &view) const {
  view.resize(NRows);
  for (auto &col : view) {
    col.resize(NCols);
  }

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

      VoxelProperties const &vProperties =
          voxelProperties[(*this)[lOfMaxDisplayPriority][r][c]];

      wchar_t text = vProperties.text;
      // ┃┇━┅▲△▶▷▼▽◀◁⬛
      // switch (vProperties.voxelType) {
      //   case vtLock:
      //     switch (text) {
      //       case '|': text = L'┇'; break;
      //       case '-': text = L'┅'; break;
      //       case '>': text = L'▷'; break;
      //       case 'v': text = L'▽'; break;
      //       case '<': text = L'◁'; break;
      //       case '^': text = L'△'; break;
      //       default:
      //         break;
      //     }
      //     break;
      //   case vtData:
      //     switch (text) {
      //       case '|': text = L'┃'; break;
      //       case '-': text = L'━'; break;
      //       case '>': text = vProperties.dataType == dtSlot ? L'▷' : L'▶'; break;
      //       case 'v': text = vProperties.dataType == dtSlot ? L'▽' : L'▼'; break;
      //       case '<': text = vProperties.dataType == dtSlot ? L'◁' : L'◀'; break;
      //       case '^': text = vProperties.dataType == dtSlot ? L'△' : L'▲'; break;
      //       case '#': text = L'⬛'; break;
      //       default:
      //         break;
      //     }
      //     break;
      // }
      view[r][c] = text;
    }
  }
}

void Volume::DumpPerformance() const {
  if (int totalEvaluatedCycles = totalEvaluatedTicks / eoDirection) {
    double averageEvaluatedUSecsPerCycle = 0.0;
    fprintf(stdout, "Performance:\n");
    for (auto d : direction) {
      double averageEvaluatedUSecsForDirection =
          totalEvaluatedUSecsPerDirection[d] / totalEvaluatedTicks;
      // fprintf(stdout,
      //         "%s: %9.2f (%9.2f) uS mean total time (per rod time)/tick\n",
      //         c_str(d),
      //         averageEvaluatedUSecsForDirection,
      //         averageEvaluatedUSecsForDirection / rods[d].size()
      //        );
      fprintf(stdout,
              "%s: %9.2f uS mean total time/direction\n",
              c_str(d),
              averageEvaluatedUSecsForDirection
             );
      averageEvaluatedUSecsPerCycle += averageEvaluatedUSecsForDirection;
    }
    fprintf(stdout,
            "   %9.2f uS mean time/cycle\n",
            averageEvaluatedUSecsPerCycle
           );

    for (auto t : tickPerCycle) {
      double averageEvaluatedUSecsForTick =
          totalEvaluatedUSecsPerTick[t] / totalEvaluatedTicks;
      // fprintf(stdout,
      //         "%s: %9.2f (%9.2f) uS mean total time (per rod time)/tick\n",
      //         c_str(d),
      //         averageEvaluatedUSecsForTick,
      //         averageEvaluatedUSecsForTick / rods[d].size()
      //        );
      fprintf(stdout,
              "%s: %9.2f uS mean total time/tick\n",
              c_str(t),
              averageEvaluatedUSecsForTick
             );
    }

    fprintf(stdout,
            "Total %ld ticks (%d cycles), %ld uS\n",
            totalEvaluatedTicks,
            totalEvaluatedCycles,
            totalEvaluatedUSecs
           );
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
    case rtLE: case rtLS: case rtLW: case rtLN:
      item = new LockRod(rodType);
      break;
    case rtDE: case rtDS: case rtDW: case rtDN:
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
