#include "diagram2d.h"
#include "rod2d.h"
#include "voxel.h"

#include <cctype>
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
#if 1
#define CHRONO_CLOCK high_resolution_clock
#else
#define CHRONO_CLOCK steady_clock
#endif
using std::chrono::CHRONO_CLOCK;
#include <tuple>
using std::tuple;
using std::make_tuple;

extern bool optEchoInput;
extern bool optShowChangedStateEveryTick;
extern bool optShowDebugOutput;
extern bool optShowPerformance;
extern bool optShowRods;

PlateOfInt::PlateOfInt(size_t _yMax, size_t _xMax) :
  vector<vector<int>>(_yMax),
  scale(1),
  yMax(_yMax),
  xMax(_xMax)
{
  for (size_t w = 10; w <= xMax; w *= 10) {
    scale += 1;
  }

  for (size_t y = 0; y < yMax; y += 1) {
    (*this)[y].resize(xMax, ' ');
  }
}

PlateOfInt::PlateOfInt(PlateOfInt const &that) :
  vector<vector<int>>(that.yMax),
  scale(1),
  yMax(that.yMax),
  xMax(that.xMax)
{
  for (size_t w = 10; w <= xMax; w *= 10) {
    scale += 1;
  }

  for (size_t y = 0; y < yMax; y += 1) {
    (*this)[y].resize(xMax, ' ');
  }
}

void PlateOfInt::insertRow(size_t atY) {
  auto insertAt = atY < yMax ? begin() + atY : end();
  insert(insertAt, vector<int>(xMax, ' '));
  yMax = size();
}

void PlateOfInt::insertCol(size_t atX) {
  for (auto &r : *this) {
    auto insertAt = atX < xMax ? r.begin() + atX : r.end();
    r.insert(insertAt, ' ');
    xMax = r.size();
  }
}

void PlateOfInt::deleteRow(size_t atY) {
  if (atY < yMax) {
    erase(begin() + atY);
    yMax = size();
  }
}

void PlateOfInt::deleteCol(size_t atX) {
  if (atX < xMax) {
     for (auto &r : *this) {
       r.erase(r.begin() + atX);
       xMax = r.size();
     }
   }
 }

 bool PlateOfInt::compareRowAndRowBelow(size_t atY) const {
   if ((atY + 1) < yMax) {
     return (*this)[atY] == (*this)[atY + 1];
   }
   return false;
 }

 bool PlateOfInt::compareColAndColToRight(size_t atX) const {
   if ((atX + 1) < xMax) {
     for (auto const &r : *this) {
       if (r[atX] != r[atX + 1]) {
         return false;
       }
     }
     return true;
   }
   return false;
 }

bool PlateOfInt::isRowEmpty(size_t atY) const {
  if (atY < yMax) {
    auto const &r = (*this)[atY];
    for (auto const &c : r) {
      if (c != ' ' && c != '.') {
        return false;
      }
    }
  }
  return true;
}

bool PlateOfInt::isColEmpty(size_t atX) const {
  if (atX < xMax) {
    for (auto const &r : *this) {
      auto const &c = r[atX];
      if (c != ' ' && c != '.') {
        return false;
      }
    }
  }
  return true;
}

bool PlateOfInt::isRowSqueezable(size_t atY) const {
  if (atY < yMax) {
    auto const &r = (*this)[atY];
    for (auto const &c : r) {
      if (c != ' ' && c != '.' && c != '|') {
        return false;
      }
    }
  }
  return true;
}

bool PlateOfInt::isColSqueezable(size_t atX) const {
  if (atX < xMax) {
    for (auto const &r : *this) {
      auto const &c = r[atX];
      if (c != ' ' && c != '.' && c != '-') {
        return false;
      }
    }
  }
  return true;
}

void PlateOfInt::Dump() const {
  if (100 < xMax) {
    fprintf(stdout, "%*lu", scale + 1, 0lu);
    for (size_t x = 1; x < xMax; x += 1) {
      if (x % 10 == 0) {
        fprintf(stdout, "%1lu", (x / 100) % 10);
      } else {
        fprintf(stdout, " ");
      }
    }
    fprintf(stdout, "\n");
  }
  if (10 < xMax) {
    fprintf(stdout, "%*lu", scale + 1, 0lu);
    for (size_t x = 1; x < xMax; x += 1) {
      if (x % 10 == 0) {
        fprintf(stdout, "%1lu", (x / 10) % 10);
      } else {
        fprintf(stdout, " ");
      }
    }
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "%*lu", scale + 1, 0lu);
  for (size_t x = 1; x < xMax; x += 1) {
    if (x % 10 == 0) {
      fprintf(stdout, "%1lu", x % 10);
    } else {
      fprintf(stdout, " ");
    }
  }
  fprintf(stdout, "\n");
  fprintf(stdout, "%*s", scale + 1, ".");
  for (size_t x = 0; x < xMax + 1; x += 1) {
    fprintf(stdout, ".");
  }
  fprintf(stdout, "\n");

  for (size_t y = 0; y < yMax; y += 1) {
    if (y % 10 == 0) {
      fprintf(stdout, "%*lu.", scale, y);
    } else {
      fprintf(stdout, "%*s.", scale, "");
    }
    for (size_t x = 0; x < xMax; x += 1) {
      fprintf(stdout, "%c", (*this)[y][x]);
    }
    if (y % 10 == 0) {
      fprintf(stdout, ".%*lu\n", scale, y);
    } else {
      fprintf(stdout, ".\n");
    }
  }

  fprintf(stdout, "%*s", scale + 1, ".");
  for (size_t x = 0; x < xMax + 1; x += 1) {
    fprintf(stdout, ".");
  }
  fprintf(stdout, "\n");
  if (100 < xMax) {
    fprintf(stdout, "%*lu", scale + 1, 0lu);
    for (size_t x = 1; x < xMax; x += 1) {
      if (x % 10 == 0) {
        fprintf(stdout, "%1lu", (x / 100) % 10);
      } else {
        fprintf(stdout, " ");
      }
    }
    fprintf(stdout, "\n");
  }
  if (10 < xMax) {
    fprintf(stdout, "%*lu", scale + 1, 0lu);
    for (size_t x = 1; x < xMax; x += 1) {
      if (x % 10 == 0) {
        fprintf(stdout, "%1lu", (x / 10) % 10);
      } else {
        fprintf(stdout, " ");
      }
    }
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "%*lu", scale + 1, 0lu);
  for (size_t x = 1; x < xMax; x += 1) {
    if (x % 10 == 0) {
      fprintf(stdout, "%1lu", x % 10);
    } else {
      fprintf(stdout, " ");
    }
  }
  fprintf(stdout, "\n\n");
}

BrickOfInt::BrickOfInt(size_t _zMax, size_t _yMax, size_t _xMax) :
  vector<PlateOfInt>(_zMax),
  scale(1),
  zMax(_zMax),
  yMax(_yMax),
  xMax(_xMax)
{
  for (size_t w = 10; w <= xMax; w *= 10) {
    scale += 1;
  }

  for (size_t z = 0; z < zMax; z += 1) {
    (*this)[z].scale = scale;
    (*this)[z].yMax = yMax;
    (*this)[z].xMax = xMax;
    (*this)[z].resize(yMax);
    for (size_t y = 0; y < yMax; y += 1) {
      (*this)[z][y].resize(xMax, ' ');
    }
  }
}

BrickOfInt::BrickOfInt(BrickOfInt const &that) :
  vector<PlateOfInt>(that.zMax),
  scale(1),
  zMax(that.zMax),
  yMax(that.yMax),
  xMax(that.xMax)
{
  for (size_t w = 10; w <= xMax; w *= 10) {
    scale += 1;
  }

  for (size_t z = 0; z < zMax; z += 1) {
    (*this)[z].scale = scale;
    (*this)[z].yMax = yMax;
    (*this)[z].xMax = xMax;
    (*this)[z].resize(yMax);
    for (size_t y = 0; y < yMax; y += 1) {
      (*this)[z][y].resize(xMax);
      for (size_t x = 0; x < xMax; x += 1) {
        (*this)[z][y][z] = that[z][y][x];
      }
    }
  }
}

void BrickOfInt::Dump() const {
  for (size_t z = 0; z < zMax; z += 1) {
    int length;
    fprintf(stdout, "# %lu %n", z, &length);
    while (length++ < xMax + scale + 2) {
      fprintf(stdout, "-");
    }
    fprintf(stdout, "\n");

    (*this)[z].Dump();
  }
  fprintf(stdout, "\n");
}

Diagram2D::Diagram2D(PlateOfInt const &p) :
    PlateOfInt(p),
    earliestInput(eoDirection),
    earliestOutput(eoDirection),
    earliestDebugOutput(eoDirection),
    totalEvaluatedUSecs(0),
    totalEvaluatedTicks(0),
    lastEvaluatedTick(-1),
    lastEvaluatedTickNChangedRods(0),
    lastEvaluatedTickNInputsRead(0),
    lastEvaluatedTickNUnreadInputsRead(0),
    lastEvaluatedTickNOutputsWritten(0),
    lastEvaluatedTickNDebugOutputsWritten(0)
{
  for (size_t y = 0; y < yMax; y += 1) {
    for (size_t x = 0; x < xMax; x += 1) {
      (*this)[y][x] = p[y][x];
    }
  }

  totalEvaluatedUSecsPerDirection.fill(0);
}

Diagram2D::Diagram2D(istream &in) :
    PlateOfInt(),
    earliestInput(eoDirection),
    earliestOutput(eoDirection),
    earliestDebugOutput(eoDirection),
    totalEvaluatedUSecs(0),
    totalEvaluatedTicks(0),
    lastEvaluatedTick(-1),
    lastEvaluatedTickNChangedRods(0),
    lastEvaluatedTickNInputsRead(0),
    lastEvaluatedTickNUnreadInputsRead(0),
    lastEvaluatedTickNOutputsWritten(0),
    lastEvaluatedTickNDebugOutputsWritten(0)
{
  do {
    string line;
    getline(in, line);
    if (in.good()) {
      push_back(vector<int>());
      for (auto const &c : line) {
        back().push_back(int(c));
      }
      if (xMax < line.size()) {
        xMax = line.size();
      }
    }
  } while (in.good() && !in.eof());

  for (auto &row : (*this)) {
    row.resize(xMax, ' ');
  }

  totalEvaluatedUSecsPerDirection.fill(0);
  yMax = size();
}

void Diagram2D::newRodAt(P2D const &p, Direction d) {
  Rod2D *r = new Rod2D(*this, p, d);
  Label const &label = r->rodsLabel();

  rods[d].insert(r);
  if (r->hasInputs()) {
    rodsWithInputs[d].insert(r);
    if (currentInputs.find(label) != currentInputs.end()) {
      fprintf(stderr,
              "cam.r: Warning: Rod2D(%s): rod reuses an input label (\"%s\")!\n",
              r->rodsId().c_str(),
              r->rodsLabel().ToString().c_str()
             );
      fprintf(stdout,
              "cam.r: Warning: Rod2D(%s): rod reuses an input label (\"%s\")!\n",
              r->rodsId().c_str(),
              r->rodsLabel().ToString().c_str()
             );
    }
    currentInputs[label].clear();
    currentInputs[label].push_back(false);

    if (0 <= label.BitNumber()) {
      currentCombinedInputs[label.Name()].Update(label);
    }
  }
  if (r->hasOutputs()) {
    rodsWithOutputs[d].insert(r);
    if (currentOutputs.find(label) != currentOutputs.end()) {
      fprintf(stderr,
              "cam.r: Warning: Rod2D(%s): rod reuses an output label (\"%s\")!\n",
              r->rodsId().c_str(),
              r->rodsLabel().ToString().c_str()
             );
      fprintf(stdout,
              "cam.r: Warning: Rod2D(%s): rod reuses an output label (\"%s\")!\n",
              r->rodsId().c_str(),
              r->rodsLabel().ToString().c_str()
             );
    }
    currentOutputs[label] = false;

    if (0 <= label.BitNumber()) {
      currentCombinedOutputs[label.Name()].Update(label);
    }
  }
  if (r->hasDebugOutputs()) {
    rodsWithDebugOutputs[d].insert(r);
    if (currentDebugOutputs.find(label) != currentDebugOutputs.end()) {
      fprintf(stderr,
              "cam.r: Warning: Rod2D(%s): rod reuses an debugOutput label (\"%s\")!\n",
              r->rodsId().c_str(),
              r->rodsLabel().ToString().c_str()
             );
      fprintf(stdout,
              "cam.r: Warning: Rod2D(%s): rod reuses an debugOutput label (\"%s\")!\n",
              r->rodsId().c_str(),
              r->rodsLabel().ToString().c_str()
             );
    }
    currentDebugOutputs[label] = false;

    if (0 <= label.BitNumber()) {
      currentCombinedDebugOutputs[label.Name()].Update(label);
    }
  }
}

void Diagram2D::newIncompleteEWRodAt(P2D const &p) {
  assert(at(p) == '-');

  SetOfP2Ds eHeads;
  SetOfP2Ds wHeads;

  SetOfP2Ds eWard;
  for (P2D e = p.offsetBy(E); isLegalEW(e); e.move(E)) {
    eWard.insert(e);
    switch (char c = at(e)) {
      case '<':
        eHeads.insert(e);
        break;
      case '>':
        wHeads.insert(e);
        break;
    }
  }

  SetOfP2Ds wWard;
  for (P2D w = p.offsetBy(W); isLegalEW(w); w.move(W)) {
    wWard.insert(w);
    switch (char c = at(w)) {
      case '<':
        eHeads.insert(w);
        break;
      case '>':
        wHeads.insert(w);
        break;
    }
  }

  size_t nEHeads = eHeads.size();
  size_t nWHeads = wHeads.size();
  Direction d =
      0 < nEHeads ?
      (0 < nWHeads ? eoDirection : E) :
      (0 < nWHeads ? W : eoDirection);

  if (d == eoDirection) {
    if (nEHeads || nWHeads) {
      fprintf(stderr,
              "cam.r: Warning: Unable to determine the direction of an EW rod, at %s!\n",
              p.ToString().c_str()
             );
      fprintf(stdout,
              "cam.r: Warning: Unable to determine the direction of an EW rod, at %s!\n",
              p.ToString().c_str()
             );
    }
    return;
  }

  newRodAt(0 < wWard.size() ? *wWard.begin() : p, d);
}

void Diagram2D::newIncompleteSNRodAt(P2D const &p) {
  assert(at(p) == '|');

  SetOfP2Ds sHeads;
  SetOfP2Ds nHeads;

  SetOfP2Ds sWard;
  for (P2D s = p.offsetBy(S); isLegalEW(s); s.move(S)) {
    sWard.insert(s);
    switch (char c = at(s)) {
      case '<':
        sHeads.insert(s);
        break;
      case '>':
        nHeads.insert(s);
        break;
    }
  }

  SetOfP2Ds nWard;
  for (P2D n = p.offsetBy(N); isLegalEW(n); n.move(N)) {
    nWard.insert(n);
    switch (char c = at(n)) {
      case '<':
        sHeads.insert(n);
        break;
      case '>':
        nHeads.insert(n);
        break;
    }
  }

  size_t nSHeads = sHeads.size();
  size_t nNHeads = nHeads.size();
  Direction d =
      0 < nSHeads ?
      (0 < nNHeads ? eoDirection : S) :
      (0 < nNHeads ? N : eoDirection);

  if (d == eoDirection) {
    if (nSHeads || nNHeads) {
      fprintf(stderr,
              "cam.r: Warning: Unable to determine the direction of an SN rod, at %s!\n",
              p.ToString().c_str()
             );
      fprintf(stdout,
              "cam.r: Warning: Unable to determine the direction of an SN rod, at %s!\n",
              p.ToString().c_str()
             );
    }
    return;
  }

  newRodAt(0 < nWard.size() ? *nWard.begin() : p, d);
}

void Diagram2D::rodSharedAt(Rod2D *r, P2D const &p) {
  pointsShared[p].insert(r);
}

void Diagram2D::scan() {
  for (int y = 0; y < yMax; y += 1) {
    for (int x = 0; x < xMax; x += 1) {
      char c = at(y, x);

      // Allow '#' to start a comment line.
      if (c == '#') {
        break;
      }

      // Allow C-style comments.
      if (c == '/' && ((x + 1) < xMax) && at(y, x + 1) == '*') {
        bool foundTheEnd = false;
        for (int yy = y; !foundTheEnd && yy < yMax; yy += 1) {
          for (int xx = x + 2; !foundTheEnd && xx < xMax; xx += 1) {
            if (at(yy, xx) == '*' && ((xx + 1) < xMax) && at(yy, xx + 1) == '/') {
              y = yy;
              x = xx + 1;
              foundTheEnd = true;
              break;
            }
          }
        }
        if (foundTheEnd) {
          continue;
        }
      }

      P2D p(y, x);

      if (hasAlreadyBeenSeen(p)) {
        continue;
      }
      saw(p);

      Rod2D *rod = 0;
      switch (c) {
        case ' ':
          break;

        case '>':
          newRodAt(p, E);
          break;

        case 'v':
          newRodAt(p, S);
          break;

        case '<':
          newRodAt(p, W);
          break;

        case '^':
          newRodAt(p, N);
          break;

        case '-':
          newIncompleteEWRodAt(p);
          break;

        case '|':
          newIncompleteSNRodAt(p);
          break;
      }
    }
  }

  for (auto &pAndRs : pointsShared) {
    auto &rods = pAndRs.second;
    auto &p = pAndRs.first;
    RodIntersectionType intersectionType = riNone;

    switch (char const &c = at(p)) {
      case '|':
      case '-':
      case '>':
      case 'v':
      case '<':
      case '^':
        intersectionType = riCrossing;
        break;

      case '0':
        intersectionType = riComplement;
        break;

      case '1':
        intersectionType = riIdentity;
        break;

      case 'X':
        intersectionType = riLocking;
        break;

      default:
        assert(intersectionType != riNone);
    }

    if (1 < rods.size()) {
      for (auto r1 = rods.begin(); r1 != rods.end(); ++r1) {
        for (auto r2 = std::next(r1); r2 != rods.end(); ++r2) {
          (*r1)->connectWith(*r2, intersectionType);
        }
      }
    }
  }

  earliestInput = eoDirection;
  for (auto const d : direction) {
    if (!rodsWithInputs[d].empty()) {
      earliestInput = d;
    }
  }

  earliestOutput = eoDirection;
  for (auto const d : direction) {
    if (!rodsWithOutputs[d].empty()) {
      earliestOutput = d;
    }
  }

  earliestDebugOutput = eoDirection;
  for (auto const d : direction) {
    if (!rodsWithDebugOutputs[d].empty()) {
      earliestDebugOutput = d;
    }
  }

  if (earliestInput != eoDirection) {
    int tick = 0;
    for (auto d = earliestInput; Last(d) != earliestInput; d = Last(d)) {
      for (auto &r : rodsWithInputs[d]) {
        r->setFirstTick(tick);
      }
      tick += 1;
    }
  }

  if (earliestOutput != eoDirection) {
    for (auto d = earliestOutput; Last(d) != earliestOutput; d = Last(d)) {
      for (auto &r : rodsWithOutputs[d]) {
        r->verifyInputDelays();
      }
    }
  }

  if (earliestDebugOutput != eoDirection) {
    for (auto d = earliestDebugOutput; Last(d) != earliestDebugOutput; d = Last(d)) {
      for (auto &r : rodsWithDebugOutputs[d]) {
        r->verifyInputDelays();
      }
    }
  }

  for (auto const d : direction) {
    for (auto r : rods[d]) {
      r->DetermineIfALockRod();
    }
  }

  // for (auto const d : direction) {
  //   for (auto r : rods[d]) {
  //     r->formExpression();
  //   }
  // }
}

void Diagram2D::refactor() {
  fprintf(stdout, "Refactoring....\n");

  // Find the extents of the current rods.
  array<int, eoDirection>  limitTowards;
  limitTowards.fill(0);

  for (auto const d : direction) {
    for (auto const r : rods[d]) {
      P2D const &headAt = r->getHeadAt();
      P2D const &tailAt = r->getTailAt();
      Direction bwd = BWard(d);
      Direction fwd = FWard(d);
      Direction lwd = LWard(d);
      Direction rwd = RWard(d);

      switch (d) {
      case E:
        limitTowards[bwd] = std::min(limitTowards[bwd], tailAt.limitTowards(bwd));
        limitTowards[fwd] = std::max(limitTowards[fwd], tailAt.limitTowards(fwd));
        limitTowards[lwd] = std::min(limitTowards[lwd], tailAt.limitTowards(lwd));
        limitTowards[rwd] = std::max(limitTowards[rwd], tailAt.limitTowards(rwd));
        break;
      case W:
        limitTowards[bwd] = std::max(limitTowards[bwd], tailAt.limitTowards(bwd));
        limitTowards[fwd] = std::min(limitTowards[fwd], tailAt.limitTowards(fwd));
        limitTowards[lwd] = std::min(limitTowards[lwd], tailAt.limitTowards(lwd));
        limitTowards[rwd] = std::max(limitTowards[rwd], tailAt.limitTowards(rwd));
        break;
      case S:
        limitTowards[bwd] = std::min(limitTowards[bwd], tailAt.limitTowards(bwd));
        limitTowards[fwd] = std::max(limitTowards[fwd], tailAt.limitTowards(fwd));
        limitTowards[lwd] = std::max(limitTowards[lwd], tailAt.limitTowards(lwd));
        limitTowards[rwd] = std::min(limitTowards[rwd], tailAt.limitTowards(rwd));
        break;
      case N:
        limitTowards[bwd] = std::max(limitTowards[bwd], tailAt.limitTowards(bwd));
        limitTowards[fwd] = std::min(limitTowards[fwd], tailAt.limitTowards(fwd));
        limitTowards[lwd] = std::max(limitTowards[lwd], tailAt.limitTowards(lwd));
        limitTowards[rwd] = std::min(limitTowards[rwd], tailAt.limitTowards(rwd));
        break;
      default:
        break;
      }
    }
  }
  for (auto const d : direction) {
    fprintf(stdout, "limitTowards[%s] = %4d\n", c_str(d), limitTowards[d]);
  }
  fprintf(stdout, "\n");

  // Find the separations between the rods.
  array<map<int, int>, 2> rowAndColumnCounts;
  for (auto const d : direction) {
    for (auto const r : rods[d]) {
      P2D const &headAt = r->getHeadAt();

      switch (d) {
        // E/W rods are on a row.
      case E:
      case W:
        rowAndColumnCounts[0][headAt.y] += 1;
        break;
        // S/N rods are in a column.
      case S:
      case N:
        rowAndColumnCounts[1][headAt.x] += 1;
        break;
      default:
        break;
      }
    }
  }
  array<map<int, int>, 2> rowAndColumnSeparations;
  for (size_t rc = 0; rc < 2; rc += 1) {
    auto const &rcCounts = rowAndColumnCounts[rc];
    auto rcCount = rcCounts.begin();
    int rcLast = rcCount->first;
    for (++rcCount; rcCount != rcCounts.end(); ++rcCount) {
      rowAndColumnSeparations[rc][rcCount->first - rcLast] += 1;
      rcLast = rcCount->first;
    }
  }
  for (size_t rc = 0; rc < 2; rc += 1) {
    auto const &rcSeparations = rowAndColumnSeparations[rc];
    fprintf(stdout, "%s separations:\n", rc == 0 ? "Row" : "Column");
    for (auto const &rcSeparation : rcSeparations) {
      fprintf(stdout, "separations[%2d] = %4d\n", rcSeparation.first, rcSeparation.second);
    }
    fprintf(stdout, "\n");
  }
}

void Diagram2D::reset() {
  for (Direction d : direction) {
    auto &dRods = rods[d];

    for (auto &r : dRods) {
      r->reset();
    }
  }
  lastEvaluatedTick = -1;
}

void Diagram2D::evaluateAt(int tick) {
  lastEvaluatedTick = tick;

  Direction d =
      Direction((earliestInput != eoDirection ? tick + int(earliestInput) : tick) %
                 eoDirection
                );
  lastEvaluatedTickNChangedRods = 0;
  lastEvaluatedTickNInputsRead = 0;
  lastEvaluatedTickNUnreadInputsRead = 0;
  lastEvaluatedTickNOutputsWritten = 0;
  lastEvaluatedTickNDebugOutputsWritten = 0;

  auto boEvaluation = CHRONO_CLOCK::now();
  for (auto &r : rods[d]) {
    r->evaluateAt(*this, tick);

    if (r->hasChanged()) {
      lastEvaluatedTickNChangedRods += 1;
    }
  }

  auto eoEvaluation = CHRONO_CLOCK::now();
  auto dTEvaluation = duration_cast<microseconds>(eoEvaluation - boEvaluation).count();
  totalEvaluatedUSecsPerDirection[totalEvaluatedTicks % eoDirection] += dTEvaluation;
  totalEvaluatedUSecs += dTEvaluation;

  if (optShowPerformance) {
    fprintf(stdout,
            "%ld [%ld.%s]: dT = %lld us\n",
            totalEvaluatedTicks,
            totalEvaluatedTicks / eoDirection,
            c_str(Direction(totalEvaluatedTicks % eoDirection)),
            (long long) dTEvaluation
            );
  }

  for (auto &i : currentInputs) {
    i.second.getAt(lastEvaluatedTick);
  }
  for (auto &o : currentOutputs) {
    o.second.getAt(lastEvaluatedTick);
  }
  for (auto &o : currentDebugOutputs) {
    o.second.getAt(lastEvaluatedTick);
  }

  totalEvaluatedTicks += 1;
}

void Diagram2D::evaluate() {
  evaluateAt(lastEvaluatedTick + 1);
}

bool Diagram2D::dumpInputLabelState
    (Label const &label, char const *comma, CombinedLabel const *&lastCLabel)
{
  return dumpLabelState(0, label, comma, lastCLabel);
}

bool Diagram2D::dumpOutputLabelState
    (Label const &label, char const *comma, CombinedLabel const *&lastCLabel)
{
  return dumpLabelState(1, label, comma, lastCLabel);
}

bool Diagram2D::dumpDebugOutputLabelState
    (Label const &label, char const *comma, CombinedLabel const *&lastCLabel)
{
  return dumpLabelState(2, label, comma, lastCLabel);
}

string intToBinary(int value, int span) {
  string result;
  for (unsigned mask = 1 << (span - 1); 0 < mask; mask >>= 1) {
    if (value & mask) {
      result += "1";
    } else {
      result += "0";
    }
  }
  return result;
}

map<string, unsigned> lastInputLabelStates;
map<string, unsigned> lastOutputLabelStates;
map<string, unsigned> lastDebugOutputLabelStates;

bool Diagram2D::dumpLabelState
    (int isIOD, Label const &label, char const *comma, CombinedLabel const *&lastCLabel)
{
  string labelToString = label.ToString();
  bool dumpedALabel = false;

  if (label.BitNumber() < 0) {
    unsigned &lastLabelState =
        isIOD == 0 ? lastInputLabelStates[labelToString] :
        isIOD == 1 ? lastOutputLabelStates[labelToString] :
        lastDebugOutputLabelStates[labelToString];
    unsigned value =
        isIOD == 0 ? getInputFor(label) :
        isIOD == 1 ? getOutputFor(label) :
        getDebugOutputFor(label);

    if (!optShowChangedStateEveryTick || lastLabelState != value) {
      if (optShowChangedStateEveryTick) {
        fprintf(stdout,
                "%s %s (%u) -> %u",
                comma,
                labelToString.c_str(),
                lastLabelState,
                value
                );
      } else {
        fprintf(stdout,
                "%s %s -> %u",
                comma,
                labelToString.c_str(),
                value
                );
      }
      lastLabelState = value;
      dumpedALabel = true;
    }
    return dumpedALabel;
  }

  if (CombinedLabel const *cLabel = CombinedLabel::LookupCombinedLabelFor(label)) {
    if (lastCLabel != cLabel) {
      string const &name = cLabel->Name();
      int minBitNumber = cLabel->MinBitNumber();
      int maxBitNumber = cLabel->MaxBitNumber();

      if (cLabel->isMultipleBits()) {
        labelToString = cLabel->ToString();

        // Collect the multi-bit value.

        unsigned value = 0;
        if (cLabel->IsLittleEndian()) {
          for (int bitNumber = maxBitNumber; minBitNumber <= bitNumber; bitNumber -= 1) {
            if (bitNumber < 0) {
              continue;
            }
            bool bit =
                isIOD == 0 ? getInputFor(name, bitNumber) :
                isIOD == 1 ? getOutputFor(name, bitNumber) :
                getDebugOutputFor(name, bitNumber);
            value = (value << 1) | bit;
          }
        } else {
          for (int bitNumber = minBitNumber; bitNumber <= maxBitNumber; bitNumber += 1) {
            if (bitNumber < 0) {
              continue;
            }
            bool bit =
                isIOD == 0 ? getInputFor(name, bitNumber) :
                isIOD == 1 ? getOutputFor(name, bitNumber) :
                getDebugOutputFor(name, bitNumber);
            value = (value << 1) | bit;
          }
        }

        unsigned &lastLabelState =
            isIOD == 0 ? lastInputLabelStates[labelToString] :
            isIOD == 1 ? lastOutputLabelStates[labelToString] :
            lastDebugOutputLabelStates[labelToString];

        if (!optShowChangedStateEveryTick || lastLabelState != value) {
          // Write the multi-bit value.
          // {1,2,3,4} -> 1, {5,6,7,8} -> 2, etc.
          int nBinaryDigits = cLabel->Span();
          int nHexDigits = ((nBinaryDigits + 1) / 4);
          if (optShowChangedStateEveryTick) {
            fprintf(stdout,
                    "%s %s (%s [%0*x]) -> %s [%0*x]",
                    comma,
                    labelToString.c_str(),
                    intToBinary(lastLabelState, nBinaryDigits).c_str(),
                    nHexDigits,
                    lastLabelState,
                    intToBinary(value, nBinaryDigits).c_str(),
                    nHexDigits,
                    value
                    );
          } else {
            fprintf(stdout,
                    "%s %s -> %s [%0*x]",
                    comma,
                    labelToString.c_str(),
                    intToBinary(value, nBinaryDigits).c_str(),
                    nHexDigits,
                    value
                    );
          }
          lastLabelState = value;
          dumpedALabel = true;
        }
      } else {
        unsigned &lastLabelState =
            isIOD == 0 ? lastInputLabelStates[labelToString] :
            isIOD == 1 ? lastOutputLabelStates[labelToString] :
            lastDebugOutputLabelStates[labelToString];
        unsigned value =
            isIOD == 0 ? getInputFor(label) :
            isIOD == 1 ? getOutputFor(label) :
            getDebugOutputFor(label);

        if (!optShowChangedStateEveryTick || lastLabelState != value) {
          if (optShowChangedStateEveryTick) {
            fprintf(stdout,
                    "%s %s (%u) -> %u",
                    comma,
                    labelToString.c_str(),
                    lastLabelState,
                    value
                    );
          } else {
            fprintf(stdout,
                    "%s %s -> %u",
                    comma,
                    labelToString.c_str(),
                    value
                    );
          }
          lastLabelState = value;
          dumpedALabel = true;
        }
      }
    }
    lastCLabel = cLabel;
  } else {
    unsigned &lastLabelState =
        isIOD == 0 ? lastInputLabelStates[labelToString] :
        isIOD == 1 ? lastOutputLabelStates[labelToString] :
        lastDebugOutputLabelStates[labelToString];
    unsigned value =
        isIOD == 0 ? getInputFor(label) :
        isIOD == 1 ? getOutputFor(label) :
        getDebugOutputFor(label);

    if (!optShowChangedStateEveryTick || lastLabelState != value) {
      if (optShowChangedStateEveryTick) {
        fprintf(stdout,
                "%s %s (%u) -> %u",
                comma,
                labelToString.c_str(),
                lastLabelState,
                value
                );
      } else {
        fprintf(stdout,
                "%s %s -> %u",
                comma,
                labelToString.c_str(),
                value
                );
      }
      lastLabelState = value;
      dumpedALabel = true;
    }
  }
  return dumpedALabel;
}

void Diagram2D::dumpState() {
  fprintf(stdout,
          "%d [%d.%s]: inputs={",
          lastEvaluatedTick,
          lastEvaluatedTick / eoDirection,
          c_str(Direction(lastEvaluatedTick % eoDirection))
         );
  bool dumpedALabel = false;
  char const *comma = "\n    ";
  if (!currentInputs.empty()) {
    CombinedLabel const *lastCLabel = nullptr;
    for (auto const &lv : currentInputs) {
      if ((dumpedALabel |= dumpInputLabelState(lv.first, comma, lastCLabel))) {
        comma = ",\n    ";
      }
    }
  }
  if (dumpedALabel) {
    fprintf(stdout, "\n  }");
  } else {
    fprintf(stdout, " }");
  }

  fprintf(stdout, ", outputs={");
  dumpedALabel = false;
  if (!currentOutputs.empty()) {
    CombinedLabel const *lastCLabel = nullptr;
    comma = "\n    ";
    for (auto const &lv : currentOutputs) {
      if ((dumpedALabel |= dumpOutputLabelState(lv.first, comma, lastCLabel))) {
        comma = ",\n    ";
      }
    }
  }

  if (optShowDebugOutput) {
    if (dumpedALabel) {
      fprintf(stdout, "\n  }");
    } else {
      fprintf(stdout, " }");
    }

    fprintf(stdout, ", debugOutputs={");
    dumpedALabel = false;
    if (!currentDebugOutputs.empty()) {
      CombinedLabel const *lastCLabel = nullptr;
      comma = "\n    ";
      for (auto const &lv : currentDebugOutputs) {
        if ((dumpedALabel |= dumpDebugOutputLabelState(lv.first, comma, lastCLabel))) {
          comma = ",\n    ";
        }
      }
    }
  }

  if (dumpedALabel) {
   fprintf(stdout, "\n  }\n");
  } else {
   fprintf(stdout, " }\n");
  }
}

void Diagram2D::dumpPerformance() const {
  if (int totalEvaluatedCycles = totalEvaluatedTicks / eoDirection) {
    double averageEvaluatedUSecsPerCycle = 0.0;
    fprintf(stdout, "Performance:\n");
    for (auto d : direction) {
      double averageEvaluatedUSecsForTick =
          totalEvaluatedUSecsPerDirection[d] / totalEvaluatedTicks;
      fprintf(stdout,
              "%s: %9.2f (%9.2f) uS mean total time (per rod time)/tick\n",
              c_str(d),
              averageEvaluatedUSecsForTick,
              averageEvaluatedUSecsForTick / rods[d].size()
             );
      averageEvaluatedUSecsPerCycle += averageEvaluatedUSecsForTick;
    }
    fprintf(stdout,
            "   %9.2f uS mean time/cycle\n",
            averageEvaluatedUSecsPerCycle
           );
    fprintf(stdout,
            "Total %ld ticks (%d cycles), %ld uS\n",
            totalEvaluatedTicks,
            totalEvaluatedCycles,
            totalEvaluatedUSecs
           );
  }
}

void Diagram2D::setInputFor(string const &text, vector<int> const &values) {
  for (size_t dotAt = text.find_first_of('.');
       dotAt != string::npos;
       dotAt = text.find_first_of('.', dotAt + 1)
      )
  {
    string name = text.substr(0, dotAt);
    string tail = text.substr(dotAt + 1);
    int hiBitNumber;
    int loBitNumber;
    int nChars;
    if (sscanf(tail.c_str(), "%x..%x%n", &hiBitNumber, &loBitNumber, &nChars) == 2 &&
        nChars == tail.size()
       )
    {
      vector<Label> labels;
      bool isBigEndian = false;
      if (optEchoInput) {
        fprintf(stdout,
                "name=%s, hiBitNumber=%d, loBitNumber=%d\n",
                name.c_str(),
                hiBitNumber,
                loBitNumber
               );
      }
      if (loBitNumber < hiBitNumber) {
        for (int bitNumber = loBitNumber; bitNumber <= hiBitNumber; bitNumber += 1) {
          if (optEchoInput) {
            fprintf(stdout, "Creating [LE] Label(\"%s\", %d)\n", name.c_str(), bitNumber);
          }
          labels.push_back(Label(name, bitNumber));
        }
      } else {
        for (int bitNumber = loBitNumber; hiBitNumber <= bitNumber; bitNumber -= 1) {
          if (optEchoInput) {
            fprintf(stdout, "Creating [BE] Label(\"%s\", %d)\n", name.c_str(), bitNumber);
          }
          labels.push_back(Label(name, bitNumber));
        }
        isBigEndian = true;
      }
      CombinedLabel::SetEndianness(name, isBigEndian);
      CombinedLabel const *cLabel = CombinedLabel::LookupCombinedLabelFor(name);
      string const &labelToString = cLabel->ToString();
      int nHexDigits = ((cLabel->Span() + 1) / 4);
      size_t nthValue = 0;
      for (auto value : values) {
        if (optEchoInput) {
          fprintf(stdout,
                  "[%lu] %s<-0x%0*x",
                  nthValue++,
                  labelToString.c_str(),
                  nHexDigits,
                  value
                 );
        }
        char const *comma = "; ";
        for (auto const &label : labels) {
          if (optEchoInput) {
            fprintf(stdout, "%s.%x<-%1d", comma, label.BitNumber(), value & 1);
            comma = ", ";
          }
          currentInputs[label].push_back(value & 1);
          value >>= 1;
        }
        if (optEchoInput) {
          fprintf(stdout, "\n");
        }
      }
      return;
    }
  }

  setInputFor(Label(text), values);
}

void Diagram2D::setInputFor(Label const &label, vector<int> const &values) {
  if (currentInputs.find(label) == currentInputs.end()) {
    fprintf(stderr,
            "cam.r: Warning: Attempting to set input for non-existing input [label] %s!  Ignoring....\n",
            label.ToString().c_str()
           );
    fprintf(stdout,
            "cam.r: Warning: Attempting to set input for non-existing input [label] %s!  Ignoring....\n",
            label.ToString().c_str()
           );
  }

  size_t nthValue = 0;
  for (auto const &value : values) {
    if (optEchoInput) {
      fprintf(stdout, "[%lu] %s<-%1d\n", nthValue++, label.ToString().c_str(), value);
    }
    currentInputs[label].push_back(value);
  }
}

void Diagram2D::addInputFor(Label const &label, bool value) {
  if (currentInputs.find(label) == currentInputs.end()) {
    fprintf(stderr,
            "cam.r: Error: Diagram2D::addInputFor(label=\"%s\", value=%d): "
            "Unable to find input for label!\n",
            label.ToString().c_str(),
            value
           );
    assert(currentInputs.find(label) != currentInputs.end());
  }
  currentInputs[label].push_back(value);
}

size_t Diagram2D::maxUnreadInput()const {
  size_t result = 0;
  for (auto const &i : currentInputs) {
    // fprintf(stderr,
    //         "%s has %lu remaining inputs.\n",
    //         i.first.ToString().c_str(),
    //         i.second.size()
    //        );

    result = std::max(result, i.second.size());
  }
  return result;
}

bool Diagram2D::hasUnreadInput() const {
  return 0 < maxUnreadInput();
}

bool Diagram2D::hasInputFor(Label const &label) {
  if (currentInputs.find(label) == currentInputs.end()) {
    fprintf(stderr,
            "cam.r: Error: Diagram2D::hasInputFor(label=\"%s\"): "
            "Unable to find input for label!\n",
            label.ToString().c_str()
           );
    assert(currentInputs.find(label) != currentInputs.end());
  }
  return !currentInputs[label].empty();
}

EdgedBool const &Diagram2D::getInputFor(Label const &label) {
  if (currentInputs.find(label) == currentInputs.end()) {
    fprintf(stderr,
            "cam.r: Error: Diagram2D::getInputFor(label=\"%s\"): "
            "Unable to find input for label!\n",
            label.ToString().c_str()
           );
    assert(currentInputs.find(label) != currentInputs.end());
  }
  return currentInputs[label].get();
}

EdgedBool const &Diagram2D::getInputFor(string const &name, int bitNumber) {
  Label label(name, bitNumber);
  if (currentInputs.find(label) == currentInputs.end()) {
    fprintf(stderr,
            "cam.r: Error: Diagram2D::getInputFor(name=\"%s\", bitNumber=%d): "
            "Unable to find input for Label().ToString()=%s!\n",
            name.c_str(),
            bitNumber,
            label.ToString().c_str()
           );
    assert(currentInputs.find(label) != currentInputs.end());
  }
  return currentInputs[label].get();
}

EdgedBool const &Diagram2D::readInputFor(Label const &label) {
  if (currentInputs.find(label) == currentInputs.end()) {
    fprintf(stderr,
            "cam.r: Error: Diagram2D::readInputFor(label=\"%s\"): "
            "Unable to find input for label!\n",
            label.ToString().c_str()
           );
    assert(currentInputs.find(label) != currentInputs.end());
  }
  lastEvaluatedTickNInputsRead += 1;
  return currentInputs[label].readAt(CurrentTick());
}

void Diagram2D::writeOutputFor(Label const &label, bool value) {
  if (currentOutputs.find(label) == currentOutputs.end()) {
    fprintf(stderr,
            "cam.r: Error: "
            "Diagram2D::getOutputFor(label=\"%s\", value=%d): "
            "Unable to find input for label!\n",
            label.ToString().c_str(),
            value
           );
    assert(currentOutputs.find(label) != currentOutputs.end());
  }
  lastEvaluatedTickNOutputsWritten += 1;
  currentOutputs[label].updateAt(CurrentTick(), value);
}

EdgedBool const &Diagram2D::getOutputFor(Label const &label) {
  if (currentOutputs.find(label) == currentOutputs.end()) {
    fprintf(stderr,
            "cam.r: Error: "
            "Diagram2D::getOutputFor(label=\"%s\"): "
            "Unable to find input for label!\n",
            label.ToString().c_str()
           );
    assert(currentOutputs.find(label) != currentOutputs.end());
  }
  return currentOutputs[label];
}

EdgedBool const &Diagram2D::getOutputFor(string const &name, int bitNumber) {
  Label label(name, bitNumber);
  if (currentOutputs.find(label) == currentOutputs.end()) {
    fprintf(stderr,
            "cam.r: Error: "
            "Diagram2D::getOutputFor(name=\"%s\", bitNumber=%d): "
            "Unable to find input for Label().ToString()=%s\n",
            name.c_str(),
            bitNumber,
            label.ToString().c_str()
           );
    assert(currentOutputs.find(label) != currentOutputs.end());
  }
  return currentOutputs[label];
}

void Diagram2D::writeDebugOutputFor(Label const &label, bool value) {
  if (currentDebugOutputs.find(label) == currentDebugOutputs.end()) {
    fprintf(stderr,
            "cam.r: Error: "
            "Diagram2D::getDebugOutputFor(label=\"%s\", value=%d): "
            "Unable to find input for label!\n",
            label.ToString().c_str(),
            value
           );
    assert(currentDebugOutputs.find(label) != currentDebugOutputs.end());
  }
  lastEvaluatedTickNDebugOutputsWritten += 1;
  currentDebugOutputs[label] = value;
}

EdgedBool const &Diagram2D::getDebugOutputFor(Label const &label) {
  if (currentDebugOutputs.find(label) == currentDebugOutputs.end()) {
    fprintf(stderr,
            "cam.r: Error: "
            "Diagram2D::getDebugOutputFor(label=\"%s\"): "
            "Unable to find input for label!\n",
            label.ToString().c_str()
           );
    assert(currentDebugOutputs.find(label) != currentDebugOutputs.end());
  }
  return currentDebugOutputs[label];
}

EdgedBool const &Diagram2D::getDebugOutputFor(string const &name, int bitNumber) {
  Label label(name, bitNumber);
  if (currentDebugOutputs.find(label) == currentDebugOutputs.end()) {
    fprintf(stderr,
            "cam.r: Error: "
            "Diagram2D::getDebugOutputFor(name=\"%s\", bitNumber=%d): "
            "Unable to find input for Label().ToString()=%s\n",
            name.c_str(),
            bitNumber,
            label.ToString().c_str()
           );
    assert(currentDebugOutputs.find(label) != currentDebugOutputs.end());
  }
  return currentDebugOutputs[label];
}

void Diagram2D::dump() const {
  struct {
    size_t nRods;
    size_t nIncompleteRods;
  } rodCounts[eoDirection + 1] = {
    { 0, 0 }, // E
    { 0, 0 }, // S
    { 0, 0 }, // D
    { 0, 0 }, // W
    { 0, 0 }, // N
    { 0, 0 }, // U
    { 0, 0 }  // eoDirection
  };

  typedef tuple<int, size_t, size_t> LenInsOuts;
  map<LenInsOuts, int> rodLengthHistogram;

  for (Direction d : direction) {
    auto const &dRods = rods[d];

    rodCounts[eoDirection].nRods += dRods.size();
    rodCounts[d].nRods = dRods.size();

    for (auto const &r : dRods) {
      if (r->rodIsIncomplete()) {
        rodCounts[eoDirection].nIncompleteRods += 1;
        rodCounts[d].nIncompleteRods += 1;
      }
      rodLengthHistogram[make_tuple(r->rodsLength(), r->countOfInputs(), r->countOfOutputs())] += 1;

      if (optShowRods) {
        r->dump(*this);
      }
    }
  }
  if (optShowRods) {
    fprintf(stdout, "\n");
  }

  fprintf(stdout, "Rod Count by (Length, Fan-In, Fan-Out):\n");
  for (auto const &lengthAndCount : rodLengthHistogram) {
    fprintf(stdout,
            "(%5d, %3lu, %3lu): %5d\n",
            std::get<0>(lengthAndCount.first),
            std::get<1>(lengthAndCount.first),
            std::get<2>(lengthAndCount.first),
            lengthAndCount.second
           );
  }
  fprintf(stdout, "\n");

  fprintf(stdout, "Rod Count (and # incomplete) by Direction:\n");
  for (Direction d : direction) {
    auto const &cs = rodCounts[d];
    fprintf(stdout, "%5s: %5lu (%5lu)\n", c_str(d), cs.nRods, cs.nIncompleteRods);
  }
  fprintf(stdout,
          "Total: %5lu (%5lu)\n",
          rodCounts[eoDirection].nRods,
          rodCounts[eoDirection].nIncompleteRods
         );
  fprintf(stdout, "\n");

  fprintf(stdout,
          "Earliest input: %s\n",
          earliestInput != eoDirection ? c_str(earliestInput) : "None"
         );
  fprintf(stdout,
          "Earliest output: %s\n",
          earliestOutput != eoDirection ? c_str(earliestOutput) : "None"
         );
  fprintf(stdout,
          "Earliest debugOutput: %s\n",
          earliestDebugOutput != eoDirection ? c_str(earliestDebugOutput) : "None"
         );
  fprintf(stdout, "\n");
}

bool Diagram2D::isLegalEWCharNotLabel1st(char c) {
  switch (c) {
  case '-':
  case '|':
  case '.':
  case '>':
  case '<':
    return true;
  default:
    return false;
  }
}

bool Diagram2D::isLegalEWChar(char c) {
  switch (c) {
  case '0':
  case '1':
  case 'X':
  case 'I':
  case 'O':
    return true;
  default:
    return isLegalEWCharNotLabel1st(c);
  }
}

bool Diagram2D::isLegalSNCharNotLabel1st(char c) {
  switch (c) {
  case '-':
  case '|':
  case '.':
  case 'v':
  case '^':
    return true;
  default:
    return false;
  }
}

bool Diagram2D::isLegalSNChar(char c) {
  switch (c) {
  case '0':
  case '1':
  case 'X':
  case 'I':
  case 'O':
    return true;
  default:
    return isLegalSNCharNotLabel1st(c);
  }
}

void Diagram2D::RebuildWithChar(PlateOfInt &plate) const {
  size_t deleted = 0;
  for (auto const d : direction) {
    for (auto const &r : rods[d]) {
      r->RebuildWithChar(*this, plate);
    }
  }
  deleted = 0;
  for (size_t y = 0; y < plate.yMax; /* empty */) {
    if (plate.isRowEmpty(y)) {
      plate.deleteRow(y);
      deleted += 1;
    } else {
      y += 1;
    }
  }
  if (deleted) {
    fprintf(stdout, "Removed %lu empty row(s).\n", deleted);
  }
  deleted = 0;
  for (size_t x = 0; x < plate.xMax; /* empty */) {
    if (plate.isColEmpty(x)) {
      plate.deleteCol(x);
      deleted += 1;
    } else {
      x += 1;
    }
  }
  if (deleted) {
    fprintf(stdout, "Removed %lu empty col(s).\n", deleted);
  }
  deleted = 0;
  for (size_t y = 0; y < plate.yMax; /* empty */) {
    if (plate.isRowSqueezable(y)) {
      plate.deleteRow(y);
      deleted += 1;
    } else {
      y += 1;
    }
  }
  if (deleted) {
    fprintf(stdout, "Removed %lu squeezable row(s).\n", deleted);
  }
  deleted = 0;
  for (size_t x = 0; x < plate.xMax; /* empty */) {
    if (plate.isColSqueezable(x)) {
      plate.deleteCol(x);
      deleted += 1;
    } else {
      x += 1;
    }
  }
  if (deleted) {
    fprintf(stdout, "Removed %lu squeezable col(s).\n", deleted);
  }
  plate.Dump();
}

// Diagram2D::Compress() {
//   auto lessCol = [] (Rod2d *l, Rod2D *r) {
//     return l->headAt.x < r->headAt.x || ();
//   }
//   auto lessRow = [] (Rod2d *l, Rod2D *r) {
//     return l->headAt.y < r->headAt.y;
//   }

//   vector<vector<Rod2D *>> rodsInRows;
//   vector<vector<Rod2D *>> rodsInCols;

// }

void Diagram2D::RebuildWithChar(BrickOfInt &brick) const {
  static size_t const rodDirectionAndIsLockRodToLayer[eoDirection][2] = {

    //   [?][!isALockRod]
    //  /  [?][isALockRod]
    // /  /
    { 6, 2 }, // [E][?]
    { 0, 4 }, // [S][?]
    { 7, 7 }, // [D][?]
    { 6, 2 }, // [W][?]
    { 0, 4 }, // [S][?]
    { 7, 7 }, // [U][?]
  };
  for (auto const d : direction) {
    for (auto const &r : rods[d]) {
      r->RebuildWithChar(*this, brick[rodDirectionAndIsLockRodToLayer[d][r->rodIsALockRod()]]);
    }
  }

  for (size_t z = 0; z < brick.zMax; z += 1) {
    switch (z) {
      case 0:
        for (size_t y = 0; y < brick.yMax; y += 1) {
          for (size_t x = 0; x < brick.xMax; x += 1) {
            switch (brick[z][y][x]) {
              case 'X':
                assert(brick[z + 1][y][x] == ' ');
                assert(brick[z + 2][y][x] == 'X');

                brick[z + 0][y][x] = '+';
                brick[z + 1][y][x] = 'X';
                brick[z + 2][y][x] = '+';
                break;
              case '|':
              case 'v':
              case '^':
              case ' ':
                break;
              default:
                break; // assert(false);
            }
          }
        }
        break;
      case 1: case 5:
        for (size_t y = 0; y < brick.yMax; y += 1) {
          for (size_t x = 0; x < brick.xMax; x += 1) {
            switch (brick[z][y][x]) {
              case 'X':
                assert(brick[z - 1][y][x] == '+');
                assert(brick[z + 1][y][x] == '+');
                break;
              case ' ':
                break;
              default:
                break; // assert(false);
            }
          }
        }
        break;
      case 2:
        for (size_t y = 0; y < brick.yMax; y += 1) {
          for (size_t x = 0; x < brick.xMax; x += 1) {
            switch (brick[z][y][x]) {
              case '0':
                assert(brick[z + 1][y][x] == ' ');
                assert(brick[z + 2][y][x] == '0');

                brick[z + 0][y][x] = '+';
                brick[z + 1][y][x] = '0';
                brick[z + 2][y][x] = '+';
                break;
              case '1':
                assert(brick[z + 1][y][x] == ' ');
                assert(brick[z + 2][y][x] == '1');

                brick[z + 0][y][x] = '+';
                brick[z + 1][y][x] = '1';
                brick[z + 2][y][x] = '+';
                break;
              case '+':
                assert(brick[z - 1][y][x] == 'X');
                break;
              case '-':
              case '>':
              case '<':
              case ' ':
                break;
              default:
                break; // assert(false);
            }
          }
        }
        break;
      case 3:
        for (size_t y = 0; y < brick.yMax; y += 1) {
          for (size_t x = 0; x < brick.xMax; x += 1) {
            switch (brick[z][y][x]) {
              case '0': case '1':
                assert(brick[z - 1][y][x] == '+');
                assert(brick[z + 1][y][x] == '+');
                break;
              case ' ':
                break;
              default:
                break; // assert(false);
            }
          }
        }
        break;
      case 4:
        for (size_t y = 0; y < brick.yMax; y += 1) {
          for (size_t x = 0; x < brick.xMax; x += 1) {
            switch (brick[z][y][x]) {
              case 'X':
                assert(brick[z + 1][y][x] == ' ');
                assert(brick[z + 2][y][x] == 'X');

                brick[z + 0][y][x] = '+';
                brick[z + 1][y][x] = 'X';
                brick[z + 2][y][x] = '+';
                break;
              case '+':
                assert(brick[z - 1][y][x] == '0' || brick[z - 1][y][x] == '1');
                break;
              case '|':
              case 'v':
              case '^':
              case ' ':
                break;
              default:
                break; // assert(false);
            }
          }
        }
        break;
      case 6:
        for (size_t y = 0; y < brick.yMax; y += 1) {
          for (size_t x = 0; x < brick.xMax; x += 1) {
            switch (brick[z][y][x]) {
              case '+':
                assert(brick[z - 1][y][x] == 'X');
                break;
              case '-':
              case '>':
              case '<':
              case ' ':
                break;
              default:
                break; // assert(false);
            }
          }
        }
        break;
    }
  }

  brick.Dump();
}

void Diagram2D::RebuildWithEnum(BrickOfInt &brick, size_t scaleBy) const {
  static size_t const rodDirectionAndIsLockRodToLayer[eoDirection][2] = {

    //   [?][!isALockRod]
    //  /  [?][isALockRod]
    // /  /
    { 6, 2 }, // [E][?]
    { 0, 4 }, // [S][?]
    { 7, 7 }, // [D][?]
    { 6, 2 }, // [W][?]
    { 0, 4 }, // [S][?]
    { 7, 7 }, // [U][?]
  };
  for (auto const d : direction) {
    for (auto const &r : rods[d]) {
      r->RebuildWithEnum
          (*this,
           brick[rodDirectionAndIsLockRodToLayer[d][r->rodIsALockRod()]],
           scaleBy
          );
    }
  }

  //   LBEL, ...,  LBSL, ...,  LBDL, ...,  LBWL, ...,  LBNL, ...,  LBUL, ...,
  //   LHEL, ...,  LHSL, ...,  LHDL, ...,  LHWL, ...,  LHNL, ...,  LHUL, ...,
  //   LTEL, ...,  LTSL, ...,  LTDL, ...,  LTWL, ...,  LTNL, ...,  LTUL, ...,
  //   LPEL, ...,  LPSL, ...,  LPDL, ...,  LPWL, ...,  LPNL, ...,  LPUL, ...,
  //   LKEL, ...,  LKSL, ...,  LKDL, ...,  LKWL, ...,  LKNL, ...,  LKUL, ...,
  //                                                E
  //                                               /     S
  //                                              /     /     D
  //                                             /     /     /     W
  //                                            /     /     /     /     N
  //                                           /     /     /     /     /     U
  //                                          /     /     /     /     /     /
  static Voxel const lb[eoDirection] = { LBEL, LBSL, LBDL, LBWL, LBNL, LBUL };
  static Voxel const lh[eoDirection] = { LHEL, LHSL, LHDL, LHWL, LHNL, LHUL };
  static Voxel const lt[eoDirection] = { LTEL, LTSL, LTDL, LTWL, LTNL, LTUL };
  static Voxel const lp[eoDirection] = { LPEL, LPSL, LPDL, LPWL, LPNL, LPUL };
  static Voxel const lk[eoDirection] = { LKEL, LKSL, LKDL, LKWL, LKNL, LKUL };

  //   DBER, ..., ..., ...,  DBSR, ..., ..., ...,  DBDR, ..., ..., ...,
  //   DBWR, ..., ..., ...,  DBNR, ..., ..., ...,  DBUR, ..., ..., ...,
  //   DHER, ..., ..., ...,  DHSR, ..., ..., ...,  DHDR, ..., ..., ...,
  //   DHWR, ..., ..., ...,  DHNR, ..., ..., ...,  DHUR, ..., ..., ...,
  //   DTER, ..., ..., ...,  DTSR, ..., ..., ...,  DTDR, ..., ..., ...,
  //   DTWR, ..., ..., ...,  DTNR, ..., ..., ...,  DTUR, ..., ..., ...,
  //   DPER, ..., ..., ...,  DPSR, ..., ..., ...,  DPDR, ..., ..., ...,
  //   DPWR, ..., ..., ...,  DPNR, ..., ..., ...,  DPUR, ..., ..., ...,
  //   DSER, ..., ..., ...,  DSSR, ..., ..., ...,  DSDR, ..., ..., ...,
  //   DSWR, ..., ..., ...,  DSNR, ..., ..., ...,  DSUR, ..., ..., ...,
  //   DLER, ..., ..., ...,  DLSR, ..., ..., ...,  DLDR, ..., ..., ...,
  //   DLWR, ..., ..., ...,  DLNR, ..., ..., ...,  DLUR, ..., ..., ...,
  //   DQER, ..., ..., ...,  DQSR, ..., ..., ...,  DQDR, ..., ..., ...,
  //   DQWR, ..., ..., ...,  DQNR, ..., ..., ...,  DQUR, ..., ..., ...,
  //   DGER, ..., ..., ...,  DGSR, ..., ..., ...,  DGDR, ..., ..., ...,
  //   DGWR, ..., ..., ...,  DGNR, ..., ..., ...,  DGUR, ..., ..., ...,
  //   D0ER, ..., ..., ...,  D0SR, ..., ..., ...,  D0DR, ..., ..., ...,
  //   D0WR, ..., ..., ...,  D0NR, ..., ..., ...,  D0UR, ..., ..., ...,
  //   D1ER, ..., ..., ...,  D1SR, ..., ..., ...,  D1DR, ..., ..., ...,
  //   D1WR, ..., ..., ...,  D1NR, ..., ..., ...,  D1UR, ..., ..., ...,
  //   DIER, ..., ..., ...,  DISR, ..., ..., ...,  DIDR, ..., ..., ...,
  //   DIWR, ..., ..., ...,  DINR, ..., ..., ...,  DIUR, ..., ..., ...,
  //   DOER, ..., ..., ...,  DOSR, ..., ..., ...,  DODR, ..., ..., ...,
  //   DOWR, ..., ..., ...,  DONR, ..., ..., ...,  DOUR, ..., ..., ...,
  //                                                E
  //                                               /     S
  //                                              /     /     D
  //                                             /     /     /     W
  //                                            /     /     /     /     N
  //                                           /     /     /     /     /     U
  //                                          /     /     /     /     /     /
  static Voxel const db[eoDirection] = { DBER, DBSR, DBDR, DBWR, DBNR, DBUR };
  static Voxel const dh[eoDirection] = { DHER, DHSR, DHDR, DHWR, DHNR, DHUR };
  static Voxel const dt[eoDirection] = { DTER, DTSR, DTDR, DTWR, DTNR, DTUR };
  static Voxel const dp[eoDirection] = { DPER, DPSR, DPDR, DPWR, DPNR, DPUR };
  static Voxel const ds[eoDirection] = { DSER, DSSR, DSDR, DSWR, DSNR, DSUR };
  static Voxel const dl[eoDirection] = { DLER, DLSR, DLDR, DLWR, DLNR, DLUR };
  static Voxel const dq[eoDirection] = { DQER, DQSR, DQDR, DQWR, DQNR, DQUR };
  static Voxel const d0[eoDirection] = { D0ER, D0SR, D0DR, D0WR, D0NR, D0UR };
  static Voxel const d1[eoDirection] = { D1ER, D1SR, D1DR, D1WR, D1NR, D1UR };
  static Voxel const di[eoDirection] = { DIER, DISR, DIDR, DIWR, DINR, DIUR };
  static Voxel const dO[eoDirection] = { DOER, DOSR, DODR, DOWR, DONR, DOUR };

  for (size_t z = 0; z < brick.zMax; z += 1) {
    switch (z) {

      // Level 0 is the S/N lock rods.

      case 0:
        for (size_t y = 0; y < brick.yMax; y += 1) {
          for (size_t x = 0; x < brick.xMax; x += 1) {
            P3D p(z, y, x);
            Voxel &v = (Voxel &) brick.at(p);
            VoxelProperties const &vp = voxelProperties[v];

            switch (VoxelType vt = vp.voxelType) {
              case vtUnkn:
                assert(vt != vtUnkn);
              case vtWall:
                continue;
              case vtSlot:
                continue;
              case vtLock:
                assert(vt == vtLock);
                break;
              case vtData:
                assert(vt != vtData);
                break;
            }

            P3D pu = p.offsetBy(U);
            Voxel &vu = (Voxel &) brick.at(pu);
            VoxelProperties vup = voxelProperties[vu];

            P3D puu = pu.offsetBy(U);
            Voxel &vuu = (Voxel &) brick.at(puu);
            VoxelProperties vuup = voxelProperties[vuu];

            P3D pf = pu.offsetBy(FWard(vp.direction));
            Voxel &vf = (Voxel &) brick.at(pf);
            VoxelProperties vfp = voxelProperties[vf];


            switch (LockType lt = vp.lockType) {
              case ltLock:
                assert(vup.voxelType == vtWall);
                if (vuup.voxelType == vtWall) {
                  P3D puuf = puu.offsetBy(FWard(vp.direction));
                  Voxel &vuuf = (Voxel &) brick.at(puuf);
                  VoxelProperties vuufp = voxelProperties[vuuf];
                  P3D puf = pu.offsetBy(FWard(vp.direction));
                  Voxel &vuf = (Voxel &) brick.at(puf);
                  VoxelProperties vufp = voxelProperties[vuf];

                  assert(vuufp.voxelType == vtData);
                  assert(vuufp.dataType == dtSlot);

                  vuuf = dp[vp.direction];
                  vuf = ds[vp.direction];
                  vu = lk[vp.direction];
                  v = lp[vp.direction];

                } else if (vuup.voxelType == vtData) {
                  P3D pub = pu.offsetBy(BWard(vp.direction));
                  Voxel &vub = (Voxel &) brick.at(pub);
                  VoxelProperties vubp = voxelProperties[vub];
                  P3D pb = pu.offsetBy(BWard(vp.direction));
                  Voxel &vb = (Voxel &) brick.at(pb);
                  VoxelProperties vbp = voxelProperties[vb];

                  assert(vuup.dataType == dtLock);
                  vuu = dp[vp.direction];
                  vu = ds[vp.direction];
                  vub = lk[vp.direction];
                  vb = lp[vp.direction];
                  v = lb[vp.direction];
                } else {
                  assert(vuup.voxelType == vtWall || vuup.voxelType == vtData);
                }
                break;
              case ltHead:
                assert(vfp.voxelType == vtWall);
                vf = Slot;
                break;
              case ltBody:
                break;
              case ltTail:
                break;
              default:
                assert(lt != eoLockType);
                break;
            }
          }
        }
        break;

      // Level 1 is the keys/slots for the S/N lock rods, and E/W data
      // rods.

      case 1: case 5:
        // for (size_t y = 0; y < brick.yMax; y += 1) {
        //   for (size_t x = 0; x < brick.xMax; x += 1) {
        //     switch (brick[z][y][x]) {
        //       case 'X':
        //         assert(brick[z - 1][y][x] == '+');
        //         assert(brick[z + 1][y][x] == '+');
        //         break;
        //       case ' ':
        //         break;
        //       default:
        //         break; // assert(false);
        //     }
        //   }
        // }
        break;

      // Level 2 is the E/W data rods.

      case 2:
        for (size_t y = 0; y < brick.yMax; y += 1) {
          for (size_t x = 0; x < brick.xMax; x += 1) {
            P3D p(z, y, x);
            Voxel &v = (Voxel &) brick.at(p);
            VoxelProperties const &vp = voxelProperties[v];

            switch (VoxelType vt = vp.voxelType) {
              case vtUnkn:
                assert(vt != vtUnkn);
              case vtWall:
                continue;
              case vtSlot:
                continue;
              case vtLock:
                assert(vt != vtLock);
                break;
              case vtData:
                assert(vt == vtData);
                break;
            }

            P3D pd = p.offsetBy(D);
            Voxel &vd = (Voxel &) brick.at(pd);
            VoxelProperties vdp = voxelProperties[vd];
            P3D pdd = pd.offsetBy(D);
            Voxel &vdd = (Voxel &) brick.at(pdd);
            VoxelProperties vddp = voxelProperties[vdd];

            P3D pu = p.offsetBy(U);
            Voxel &vu = (Voxel &) brick.at(pu);
            VoxelProperties vup = voxelProperties[vu];
            P3D puu = pu.offsetBy(U);
            Voxel &vuu = (Voxel &) brick.at(puu);
            VoxelProperties vuup = voxelProperties[vuu];

            P3D pf = p.offsetBy(FWard(vp.direction));
            Voxel &vf = (Voxel &) brick.at(pf);
            VoxelProperties vfp = voxelProperties[vf];

            switch (DataType dt = vp.dataType) {
              case dtTest:
                assert(vup.voxelType == vtWall);
                if (vuup.voxelType == vtWall) {
                  P3D puuf = puu.offsetBy(FWard(vp.direction));
                  Voxel &vuuf = (Voxel &) brick.at(puuf);
                  VoxelProperties vuufp = voxelProperties[vuuf];
                  P3D puf = pu.offsetBy(FWard(vp.direction));
                  Voxel &vuf = (Voxel &) brick.at(puf);
                  VoxelProperties vufp = voxelProperties[vuf];

                  assert(vuufp.voxelType == vtData);
                  assert(vuufp.dataType == dtSlot);

                  vuuf = dp[vp.direction];
                  vuf = ds[vp.direction];
                  vu = lk[vp.direction];
                  v = lp[vp.direction];

                } else if (vuup.voxelType == vtData) {
                  P3D pub = pu.offsetBy(BWard(vp.direction));
                  Voxel &vub = (Voxel &) brick.at(pub);
                  VoxelProperties vubp = voxelProperties[vub];
                  P3D pb = pu.offsetBy(BWard(vp.direction));
                  Voxel &vb = (Voxel &) brick.at(pb);
                  VoxelProperties vbp = voxelProperties[vb];

                  assert(vuup.dataType == dtGate);
                  vuu = dp[vp.direction];
                  vu = ds[vp.direction];
                  vub = lk[vp.direction];
                  vb = lp[vp.direction];
                  v = lb[vp.direction];
                } else {
                  assert(vuup.voxelType == vtWall || vuup.voxelType == vtData);
                }
                break;
              case ltHead:
                assert(vfp.voxelType == vtWall);
                vf = Slot;
                break;
              case ltBody:
                break;
              case ltTail:
                break;
              default:
                assert(dt != eoDataType);
                break;
            }
          }
        }
        break;

      // Level 3 is the query/gates for E/W and S/N data rods.

      case 3:
        // for (size_t y = 0; y < brick.yMax; y += 1) {
        //   for (size_t x = 0; x < brick.xMax; x += 1) {
        //     switch (brick[z][y][x]) {
        //       case '0': case '1':
        //         assert(brick[z - 1][y][x] == '+');
        //         assert(brick[z + 1][y][x] == '+');
        //         break;
        //       case ' ':
        //         break;
        //       default:
        //         break; // assert(false);
        //     }
        //   }
        // }
        break;

      // Level 4 is the S/N data rods.

      case 4:
        for (size_t y = 0; y < brick.yMax; y += 1) {
          for (size_t x = 0; x < brick.xMax; x += 1) {
            switch (brick[z][y][x]) {
              case 'X':
                assert(brick[z + 1][y][x] == ' ');
                assert(brick[z + 2][y][x] == 'X');

                brick[z + 0][y][x] = '+';
                brick[z + 1][y][x] = 'X';
                brick[z + 2][y][x] = '+';
                break;
              case '+':
                assert(brick[z - 1][y][x] == '0' || brick[z - 1][y][x] == '1');
                break;
              case '|':
              case 'v':
              case '^':
              case ' ':
                break;
              default:
                break; // assert(false);
            }
          }
        }
        break;

      // Level 6 is the E/W lock rods.

      case 6:
        for (size_t y = 0; y < brick.yMax; y += 1) {
          for (size_t x = 0; x < brick.xMax; x += 1) {
            P3D p(z, y, x);
            P3D pd = p.offsetBy(D);
            P3D pdd = pd.offsetBy(D);

            Voxel &v = (Voxel &) brick.at(p);
            Voxel &vd = (Voxel &) brick.at(pd);
            Voxel &vdd = (Voxel &) brick.at(pdd);

            VoxelProperties vp = voxelProperties[v];
            VoxelProperties vdp = voxelProperties[vd];
            VoxelProperties vddp = voxelProperties[vdd];

            P3D pf = p.offsetBy(FWard(vp.direction));
            Voxel &vf = (Voxel &) brick.at(pf);
            VoxelProperties vfp = voxelProperties[vf];

            assert(vp.voxelType == vtLock);

            switch (vp.lockType) {
              case ltLock:
                assert(vdp.voxelType == vtWall);
                if (vddp.voxelType == vtWall) {
                  P3D pddf = pdd.offsetBy(FWard(vp.direction));
                  Voxel &vddf = (Voxel &) brick.at(pddf);
                  VoxelProperties vddfp = voxelProperties[vddf];
                  P3D pdf = pd.offsetBy(FWard(vp.direction));
                  Voxel &vdf = (Voxel &) brick.at(pdf);
                  VoxelProperties vdfp = voxelProperties[vdf];

                  assert(vddfp.voxelType == vtData);
                  assert(vddfp.dataType == dtSlot);

                  vddf = dp[vp.direction];
                  vdf = ds[vp.direction];
                  vd = lk[vp.direction];
                  v = lp[vp.direction];

                } else if (vddp.voxelType == vtData) {
                  P3D pdb = pd.offsetBy(BWard(vp.direction));
                  Voxel &vdb = (Voxel &) brick.at(pdb);
                  VoxelProperties vdbp = voxelProperties[vdb];
                  P3D pb = pd.offsetBy(BWard(vp.direction));
                  Voxel &vb = (Voxel &) brick.at(pb);
                  VoxelProperties vbp = voxelProperties[vb];

                  assert(vddp.dataType == dtLock);
                  vdd = dp[vp.direction];
                  vd = ds[vp.direction];
                  vdb = lk[vp.direction];
                  vb = lp[vp.direction];
                  v = lb[vp.direction];
                } else {
                  assert(vddp.voxelType == vtWall || vddp.voxelType == vtData);
                }
                break;
              case ltHead:
                assert(vfp.voxelType == vtWall);
                vf = Slot;
                break;
              case ltBody:
                break;
              case ltTail:
                break;
              default:
                break; // assert(false);
            }
          }
        }
        break;
    }
  }

  brick.Dump();
}
