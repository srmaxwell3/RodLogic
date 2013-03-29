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
extern bool optVerbose;

template<typename T> void Plate<T>::insertRow(size_t atY) {
  auto insertAt = atY < yMax ? Plate<T>::begin() + atY : Plate<T>::end();
  insert(insertAt, vector<int>(xMax, ' '));
  yMax = Plate<T>::size();
}

template<typename T> void Plate<T>::insertCol(size_t atX) {
  for (auto &r : *this) {
    auto insertAt = atX < xMax ? r.begin() + atX : r.end();
    r.insert(insertAt, ' ');
    xMax = r.size();
  }
}

template<typename T> void Plate<T>::deleteRow(size_t atY) {
  if (atY < yMax) {
    Plate<T>::erase(Plate<T>::begin() + atY);
    yMax = Plate<T>::size();
  }
}

template<typename T> void Plate<T>::deleteCol(size_t atX) {
  if (atX < xMax) {
     for (auto &r : *this) {
       r.erase(r.begin() + atX);
       xMax = r.size();
     }
   }
 }

template<typename T> bool Plate<T>::compareRowAndRowBelow(size_t atY) const {
  if ((atY + 1) < yMax) {
    return (*this)[atY] == (*this)[atY + 1];
  }
  return false;
}

template<typename T> bool Plate<T>::compareColAndColToRight(size_t atX) const {
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

template<typename T> bool Plate<T>::isRowEmpty(size_t atY) const {
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

template<typename T> bool Plate<T>::isColEmpty(size_t atX) const {
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

template<typename T> bool Plate<T>::isRowSqueezable(size_t atY) const {
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

template<typename T> bool Plate<T>::isColSqueezable(size_t atX) const {
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

template<typename T> void Plate<T>::DumpPixelAt(size_t y, size_t x) const {
  fprintf(stdout, "%c", (*this)[y][x]);
}

template<typename T> void Plate<T>::Dump() const {
  if (100 < xMax) {
    fprintf(stdout, "%*lu", nDigits + 1, 0lu);
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
    fprintf(stdout, "%*lu", nDigits + 1, 0lu);
    for (size_t x = 1; x < xMax; x += 1) {
      if (x % 10 == 0) {
        fprintf(stdout, "%1lu", (x / 10) % 10);
      } else {
        fprintf(stdout, " ");
      }
    }
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "%*lu", nDigits + 1, 0lu);
  for (size_t x = 1; x < xMax; x += 1) {
    if (x % 10 == 0) {
      fprintf(stdout, "%1lu", x % 10);
    } else {
      fprintf(stdout, " ");
    }
  }
  fprintf(stdout, "\n");
  fprintf(stdout, "%*s", nDigits + 1, ".");
  for (size_t x = 0; x < xMax + 1; x += 1) {
    fprintf(stdout, ".");
  }
  fprintf(stdout, "\n");

  for (size_t y = 0; y < yMax; y += 1) {
    if (y % 10 == 0) {
      fprintf(stdout, "%*lu.", nDigits, y);
    } else {
      fprintf(stdout, "%*s.", nDigits, "");
    }
    for (size_t x = 0; x < xMax; x += 1) {
      DumpPixelAt(y, x);
    }
    if (y % 10 == 0) {
      fprintf(stdout, ".%*lu\n", nDigits, y);
    } else {
      fprintf(stdout, ".\n");
    }
  }

  fprintf(stdout, "%*s", nDigits + 1, ".");
  for (size_t x = 0; x < xMax + 1; x += 1) {
    fprintf(stdout, ".");
  }
  fprintf(stdout, "\n");
  if (100 < xMax) {
    fprintf(stdout, "%*lu", nDigits + 1, 0lu);
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
    fprintf(stdout, "%*lu", nDigits + 1, 0lu);
    for (size_t x = 1; x < xMax; x += 1) {
      if (x % 10 == 0) {
        fprintf(stdout, "%1lu", (x / 10) % 10);
      } else {
        fprintf(stdout, " ");
      }
    }
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "%*lu", nDigits + 1, 0lu);
  for (size_t x = 1; x < xMax; x += 1) {
    if (x % 10 == 0) {
      fprintf(stdout, "%1lu", x % 10);
    } else {
      fprintf(stdout, " ");
    }
  }
  fprintf(stdout, "\n\n");
}

template<typename T> void Brick<T>::Dump() const {
  for (size_t z = 0; z < zMax; z += 1) {
    int length;
    fprintf(stdout, "# %lu %n", z, &length);
    while (length++ < xMax + nDigits + 2) {
      fprintf(stdout, "-");
    }
    fprintf(stdout, "\n");

    (*this)[z].Dump();
  }
  fprintf(stdout, "\n");
}

Diagram2D::Diagram2D(Plate const &p) :
    Plate(p),
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
    Plate(),
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
      Plate<char>::push_back(vector<char>());
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
              "rlic: Warning: Rod2D(%s): rod reuses an input label (\"%s\")!\n",
              r->rodsId().c_str(),
              r->rodsLabel().ToString().c_str()
             );
      fprintf(stdout,
              "rlic: Warning: Rod2D(%s): rod reuses an input label (\"%s\")!\n",
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
              "rlic: Warning: Rod2D(%s): rod reuses an output label (\"%s\")!\n",
              r->rodsId().c_str(),
              r->rodsLabel().ToString().c_str()
             );
      fprintf(stdout,
              "rlic: Warning: Rod2D(%s): rod reuses an output label (\"%s\")!\n",
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
              "rlic: Warning: Rod2D(%s): rod reuses an debugOutput label (\"%s\")!\n",
              r->rodsId().c_str(),
              r->rodsLabel().ToString().c_str()
             );
      fprintf(stdout,
              "rlic: Warning: Rod2D(%s): rod reuses an debugOutput label (\"%s\")!\n",
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
              "rlic: Warning: Unable to determine the direction of an EW rod, at %s!\n",
              p.ToString().c_str()
             );
      fprintf(stdout,
              "rlic: Warning: Unable to determine the direction of an EW rod, at %s!\n",
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
              "rlic: Warning: Unable to determine the direction of an SN rod, at %s!\n",
              p.ToString().c_str()
             );
      fprintf(stdout,
              "rlic: Warning: Unable to determine the direction of an SN rod, at %s!\n",
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
    auto const &l = i.first;
    auto &lv = inputsByTick[l];
    auto const &v = i.second.getAt(lastEvaluatedTick);
    lv.push_back(bool(v));
    // fprintf(stdout, "inputsByTick[\"%s\"][%lu] = %d\n", l.ToString().c_str(), lv.size() - 1, bool(v));
  }
  for (auto &o : currentOutputs) {
    auto const &l = o.first;
    auto &lv = outputsByTick[l];
    auto const &v = o.second.getAt(lastEvaluatedTick);
    lv.push_back(bool(v));
    // fprintf(stdout, "outputsByTick[\"%s\"][%lu] = %d\n", l.ToString().c_str(), lv.size() - 1, bool(v));
  }
  for (auto &d : currentDebugOutputs) {
    auto const &l = d.first;
    auto &lv = debugOutputsByTick[l];
    auto const &v = d.second.getAt(lastEvaluatedTick);
    lv.push_back(bool(v));
    // fprintf(stdout, "debugOutputsByTick[\"%s\"][%lu] = %d\n", l.ToString().c_str(), lv.size() - 1, bool(v));
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
  char const *comma = " ";
  if (!currentInputs.empty()) {
    CombinedLabel const *lastCLabel = nullptr;
    for (auto const &lv : currentInputs) {
      if ((dumpedALabel |= dumpInputLabelState(lv.first, comma, lastCLabel))) {
        comma = ", ";
      }
    }
  }
  if (dumpedALabel) {
    fprintf(stdout, " }");
  } else {
    fprintf(stdout, " }");
  }

  fprintf(stdout, ", outputs={");
  dumpedALabel = false;
  if (!currentOutputs.empty()) {
    CombinedLabel const *lastCLabel = nullptr;
    comma = " ";
    for (auto const &lv : currentOutputs) {
      if ((dumpedALabel |= dumpOutputLabelState(lv.first, comma, lastCLabel))) {
        comma = ", ";
      }
    }
  }

  if (optShowDebugOutput) {
    if (dumpedALabel) {
      fprintf(stdout, " }");
    } else {
      fprintf(stdout, " }");
    }

    fprintf(stdout, ", debugOutputs={");
    dumpedALabel = false;
    if (!currentDebugOutputs.empty()) {
      CombinedLabel const *lastCLabel = nullptr;
      comma = " ";
      for (auto const &lv : currentDebugOutputs) {
        if ((dumpedALabel |= dumpDebugOutputLabelState(lv.first, comma, lastCLabel))) {
          comma = ", ";
        }
      }
    }
  }

  if (dumpedALabel) {
   fprintf(stdout, " }\n");
  } else {
   fprintf(stdout, " }\n");
  }
}

void Diagram2D::dumpWaveforms() const {
  fprintf(stdout, "\"Tick\"");
  for (auto const &iw : inputsByTick) {
    fprintf(stdout, ", \"i.%s\"", iw.first.ToString().c_str());
  }
  for (auto const &ow : outputsByTick) {
    fprintf(stdout, ", \"o.%s\"", ow.first.ToString().c_str());
  }
  if (optShowDebugOutput) {
    for (auto const &dw : debugOutputsByTick) {
      fprintf(stdout, ", \"d.%s\"", dw.first.ToString().c_str());
    }
  }
  fprintf(stdout, "\n");
  for (size_t i = 0; i < lastEvaluatedTick; i += 1) {
    fprintf(stdout, "%lu", i);
    for (auto const &iw : inputsByTick) {
      fprintf(stdout, ", %d", bool(iw.second[i]));
    }
    for (auto const &ow : outputsByTick) {
      fprintf(stdout, ", %d", bool(ow.second[i]));
    }
    if (optShowDebugOutput) {
      for (auto const &dw : debugOutputsByTick) {
        fprintf(stdout, ", %d", bool(dw.second[i]));
      }
    }
    fprintf(stdout, "\n");
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
            "rlic: Warning: Attempting to set input for non-existing input [label] %s!  Ignoring....\n",
            label.ToString().c_str()
           );
    fprintf(stdout,
            "rlic: Warning: Attempting to set input for non-existing input [label] %s!  Ignoring....\n",
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
            "rlic: Error: Diagram2D::addInputFor(label=\"%s\", value=%d): "
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
            "rlic: Error: Diagram2D::hasInputFor(label=\"%s\"): "
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
            "rlic: Error: Diagram2D::getInputFor(label=\"%s\"): "
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
            "rlic: Error: Diagram2D::getInputFor(name=\"%s\", bitNumber=%d): "
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
            "rlic: Error: Diagram2D::readInputFor(label=\"%s\"): "
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
            "rlic: Error: "
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
            "rlic: Error: "
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
            "rlic: Error: "
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
            "rlic: Error: "
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
            "rlic: Error: "
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
            "rlic: Error: "
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
      rodLengthHistogram[make_tuple(r->rodsLength(), r->countOfGets(), r->countOfPuts())] += 1;

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

void Diagram2D::RebuildWithChar(Plate<char> &plate) const {
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

void Diagram2D::RebuildWithChar(Brick<char> &brick) const {
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

void Diagram2D::RebuildWithEnum(Brick<Voxel> &brick) const {
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
           brick.scaleBy
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
  static Voxel const LB[eoDirection] = { LBEL, LBSL, LBDL, LBWL, LBNL, LBUL };
  static Voxel const LH[eoDirection] = { LHEL, LHSL, LHDL, LHWL, LHNL, LHUL };
  static Voxel const LT[eoDirection] = { LTEL, LTSL, LTDL, LTWL, LTNL, LTUL };
  static Voxel const LP[eoDirection] = { LPEL, LPSL, LPDL, LPWL, LPNL, LPUL };
  static Voxel const LK[eoDirection] = { LKEL, LKSL, LKDL, LKWL, LKNL, LKUL };

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
  static Voxel const DB[eoDirection] = { DBER, DBSR, DBDR, DBWR, DBNR, DBUR };
  static Voxel const DH[eoDirection] = { DHER, DHSR, DHDR, DHWR, DHNR, DHUR };
  static Voxel const DT[eoDirection] = { DTER, DTSR, DTDR, DTWR, DTNR, DTUR };
  static Voxel const DP[eoDirection] = { DPER, DPSR, DPDR, DPWR, DPNR, DPUR };
  static Voxel const DS[eoDirection] = { DSER, DSSR, DSDR, DSWR, DSNR, DSUR };
  static Voxel const DL[eoDirection] = { DLER, DLSR, DLDR, DLWR, DLNR, DLUR };
  static Voxel const DQ[eoDirection] = { DQER, DQSR, DQDR, DQWR, DQNR, DQUR };
  static Voxel const D0[eoDirection] = { D0ER, D0SR, D0DR, D0WR, D0NR, D0UR };
  static Voxel const D1[eoDirection] = { D1ER, D1SR, D1DR, D1WR, D1NR, D1UR };
  static Voxel const DI[eoDirection] = { DIER, DISR, DIDR, DIWR, DINR, DIUR };
  static Voxel const DO[eoDirection] = { DOER, DOSR, DODR, DOWR, DONR, DOUR };
  static Voxel const DD[eoDirection] = { DDER, DDSR, DDDR, DDWR, DDNR, DDUR };

  static Direction const dataToLockDirection[eoDirection] = { S, W, X, N, E, X };
  static Direction const lockToDataDirection[eoDirection] = { N, W, X, S, E, X };

  // Given a lower (E/W) data rod's (forward) direction, and an
  // interacting upper (S/N rod's direction, and the fact that we're
  // looking at a dtGate on the lower rod (and on the upper rod, in
  // the same position, above), is the lower rod's dtGate supposed to
  // be a dtGate, or a dtTest?  (The opposite will be true for the
  // upper rod.)

  static DataType const gateToGateOrTest[eoDirection][eoDirection] = {
    //   Lower (E/W) data rod fwd direction             // Upper (S/N) data
    //   E       S       D       W       N       U      //   rod fdb direction
    { dtUnkn, dtGate, dtGate, dtUnkn, dtTest, dtTest }, // E
    { dtTest, dtUnkn, dtGate, dtGate, dtUnkn, dtTest }, // S
    { dtTest, dtTest, dtUnkn, dtGate, dtGate, dtUnkn }, // D
    { dtUnkn, dtTest, dtTest, dtUnkn, dtGate, dtGate }, // W
    { dtGate, dtUnkn, dtTest, dtTest, dtUnkn, dtGate }, // N
    { dtGate, dtGate, dtUnkn, dtTest, dtTest, dtUnkn }  // U
  };

  // We'll do the (S/N, E/W) lock rods, first, followed by the (E/W,
  // S/N) data rods.

  array<size_t, 4> levelProcessOrder = { 0, 6, 2, 4 };
  for (auto z : levelProcessOrder) {
    switch (z) {
      if (optVerbose) {
        brick[z].Dump();
        fflush(stdout);
      }

      // Level 0 is the S/N lock rods.  Note that dtLock's are allowed
      // here, propogated from the associated data rods.  We turn them
      // into lkLock/dtSlot pairs.

      case 0:
        for (size_t y = 0; y < brick.yMax; y += 1) {
          for (size_t x = 0; x < brick.xMax; x += 1) {
            P3D p(z, y, x);
            Voxel &v = (Voxel &) brick.at(p);
            VoxelProperties const &vp = voxelProperties[v];
            VoxelType vt = vp.voxelType;

            switch (vt) {
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
              assert(vp.dataType == dtLock);
              break;
            case eoVoxelType:
              assert(vt != eoVoxelType);
            }

            Direction lFwd =
                vt == vtLock ? vp.direction : dataToLockDirection[vp.direction];
            Direction dFwd =
                vt == vtLock ? lockToDataDirection[vp.direction] : vp.direction;

            P3D pu = p.offsetBy(U);
            Voxel &vu = (Voxel &) brick.at(pu);
            VoxelProperties vup = voxelProperties[vu];

            P3D puu = pu.offsetBy(U);
            Voxel &vuu = (Voxel &) brick.at(puu);
            VoxelProperties vuup = voxelProperties[vuu];

            // For vtData/dtLock's....

            if (vt == vtData && vp.dataType == dtLock) {

              // ...[X]--... -> ...[+]--...
              // ...[#]##... -> ...[<]< ...
              // ...[X]--...    ..-[-]+-...

              P3D puub = puu.offsetBy(BWard(lFwd));
              Voxel &vuub = (Voxel &) brick.at(puub);
              VoxelProperties vuufp = voxelProperties[vuub];

              P3D pf = p.offsetBy(FWard(lFwd));
              Voxel &vf = (Voxel &) brick.at(pf);
              VoxelProperties vfp = voxelProperties[vf];

              P3D puf = pu.offsetBy(FWard(lFwd));
              Voxel &vuf = (Voxel &) brick.at(puf);
              VoxelProperties vufp = voxelProperties[vuf];
              P3D puff = puf.offsetBy(FWard(lFwd));
              Voxel &vuff = (Voxel &) brick.at(puff);
              VoxelProperties vuffp = voxelProperties[vuff];


              // Setup a key for this lock, and change the data rod's
              // dtLock to a dtSlot....

              assert(vfp.voxelType == vtLock && vfp.lockType == ltBody);
              assert(vup.voxelType == vtWall);
              assert(vufp.voxelType == vtWall);
              assert(vuffp.voxelType == vtWall);
              assert(vuup.voxelType == vtData && vuup.dataType == dtLock);

              vuu = DP[dFwd];
              vu = DS[dFwd];
              vuf = LK[lFwd];
              if (vuff == Wall) {
                vuff = Slot;
              }
              vf = LP[lFwd];
              v = LB[lFwd];
              continue;
            }

            // For vtLock's....

            P3D pf = p.offsetBy(FWard(lFwd));
            Voxel &vf = (Voxel &) brick.at(pf);
            VoxelProperties vfp = voxelProperties[vf];

            switch (LockType lt = vp.lockType) {
              case ltLock:
                assert(vup.voxelType == vtWall);
                assert(vfp.voxelType == vtLock && vfp.lockType == ltBody);

                if (vuup.voxelType == vtWall) {

                  // ...<[#]#... -> ...+[#]#...
                  // ...#[#]#... -> ...<[<] ...
                  // ...-[<]-...    ...-[+]-...

                  P3D puuf = puu.offsetBy(FWard(lFwd));
                  Voxel &vuuf = (Voxel &) brick.at(puuf);
                  VoxelProperties vuufp = voxelProperties[vuuf];
                  P3D puf = pu.offsetBy(FWard(lFwd));
                  Voxel &vuf = (Voxel &) brick.at(puf);
                  VoxelProperties vufp = voxelProperties[vuf];

                  assert(vuufp.voxelType == vtData && vuufp.dataType == dtSlot);

                  vuuf = DP[dFwd];
                  vuf = DS[dFwd];
                  vu = LK[lFwd];
                  v = LP[lFwd];

                } else if (vuup.voxelType == vtData) {

                  // ...[<]##... -> ...[+]#...
                  // ...[#]##... -> ...[<]< ..
                  // ...[<]--...    ...[-]+-..

                  P3D puf = pu.offsetBy(FWard(lFwd));
                  Voxel &vuf = (Voxel &) brick.at(puf);
                  VoxelProperties vufp = voxelProperties[vuf];
                  P3D pf = pu.offsetBy(FWard(lFwd));
                  Voxel &vf = (Voxel &) brick.at(pf);
                  VoxelProperties vfp = voxelProperties[vf];

                  assert(vuup.dataType == dtLock);
                  vuu = DP[dFwd];
                  vu = DS[dFwd];
                  vuf = LK[lFwd];
                  vf = LP[lFwd];
                  v = LB[lFwd];
                } else {
                  assert(vuup.voxelType == vtWall || vuup.voxelType == vtData);
                }
                break;
              case ltHead:
                assert(vfp.voxelType == vtWall);
                if (vf == Wall) {
                  vf = Slot;
                }
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
        break;

      // Level 2 is the E/W data rods.

      case 2:
        for (size_t y = 0; y < brick.yMax; y += 1) {
          for (size_t x = 0; x < brick.xMax; x += 1) {
            P3D p(z, y, x);
            Voxel &lv = (Voxel &) brick.at(p);
            VoxelProperties const &lvp = voxelProperties[lv];

            switch (VoxelType vt = lvp.voxelType) {
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
            case eoVoxelType:
              assert(vt != eoVoxelType);
            }

            Direction lFwd = lvp.direction;

            P3D pd = p.offsetBy(D);
            Voxel &lvd = (Voxel &) brick.at(pd);
            VoxelProperties lvdp = voxelProperties[lvd];

            P3D pdf = pd.offsetBy(FWard(lFwd));
            Voxel &lvdf = (Voxel &) brick.at(pdf);
            VoxelProperties lvdfp = voxelProperties[lvdf];
            P3D pdff = pdf.offsetBy(FWard(lFwd));
            Voxel &lvdff = (Voxel &) brick.at(pdff);
            VoxelProperties lvdffp = voxelProperties[lvdff];
            P3D pdfff = pdff.offsetBy(FWard(lFwd));
            Voxel &lvdfff = (Voxel &) brick.at(pdfff);
            VoxelProperties lvdfffp = voxelProperties[lvdfff];

            P3D pdd = pd.offsetBy(D);
            Voxel &lvdd = (Voxel &) brick.at(pdd);
            VoxelProperties lvddp = voxelProperties[lvdd];

            P3D pu = p.offsetBy(U);
            Voxel &lvu = (Voxel &) brick.at(pu);
            VoxelProperties lvup = voxelProperties[lvu];

            P3D up = pu.offsetBy(U);
            Voxel &uv = (Voxel &) brick.at(up);
            VoxelProperties uvp = voxelProperties[uv];

            Direction uFwd = X;
            if (uvp.voxelType == vtData) {
              uFwd = uvp.direction;
            }

            P3D pf = p.offsetBy(FWard(lFwd));
            Voxel &lvf = (Voxel &) brick.at(pf);
            VoxelProperties lvfp = voxelProperties[lvf];
            P3D pff = pf.offsetBy(FWard(lFwd));
            Voxel &lvff = (Voxel &) brick.at(pff);
            VoxelProperties lvffp = voxelProperties[lvff];
            P3D pfff = pff.offsetBy(FWard(lFwd));
            Voxel &lvfff = (Voxel &) brick.at(pfff);
            VoxelProperties lvfffp = voxelProperties[lvfff];

            P3D puf = pu.offsetBy(FWard(lFwd));
            Voxel &lvuf = (Voxel &) brick.at(puf);
            VoxelProperties lvufp = voxelProperties[lvuf];
            P3D puff = puf.offsetBy(FWard(lFwd));
            Voxel &lvuff = (Voxel &) brick.at(puff);
            VoxelProperties lvuffp = voxelProperties[lvuff];
            P3D pufff = puff.offsetBy(FWard(lFwd));
            Voxel &lvufff = (Voxel &) brick.at(pufff);
            VoxelProperties lvufffp = voxelProperties[lvufff];

            P3D pb = p.offsetBy(BWard(lFwd));
            Voxel &lvb = (Voxel &) brick.at(pb);
            VoxelProperties lvbp = voxelProperties[lvb];
            P3D pbb = pb.offsetBy(BWard(lFwd));
            Voxel &lvbb = (Voxel &) brick.at(pbb);
            VoxelProperties lvbbp = voxelProperties[lvbb];
            P3D pbbb = pbb.offsetBy(BWard(lFwd));
            Voxel &lvbbb = (Voxel &) brick.at(pbbb);
            VoxelProperties lvbbbp = voxelProperties[lvbbb];

            P3D pub = pu.offsetBy(BWard(lFwd));
            Voxel &lvub = (Voxel &) brick.at(pub);
            VoxelProperties lvubp = voxelProperties[lvub];
            P3D pubb = pub.offsetBy(BWard(lFwd));
            Voxel &lvubb = (Voxel &) brick.at(pubb);
            VoxelProperties lvubbp = voxelProperties[lvubb];
            P3D pubbb = pubb.offsetBy(BWard(lFwd));
            Voxel &lvubbb = (Voxel &) brick.at(pubbb);
            VoxelProperties lvubbbp = voxelProperties[lvubbb];

            P3D pdb = pd.offsetBy(BWard(lFwd));
            Voxel &lvdb = (Voxel &) brick.at(pdb);
            VoxelProperties lvdbp = voxelProperties[lvdb];
            P3D pdbb = pdb.offsetBy(BWard(lFwd));
            Voxel &lvdbb = (Voxel &) brick.at(pdbb);
            VoxelProperties lvdbbp = voxelProperties[lvdbb];
            P3D pdbbb = pdbb.offsetBy(BWard(lFwd));
            Voxel &lvdbbb = (Voxel &) brick.at(pdbbb);
            VoxelProperties lvdbbbp = voxelProperties[lvdbbb];

            switch (DataType dt = lvp.dataType) {
              case dtBody:
                break;
              case dtHead:
                assert(lvfp.voxelType == vtWall);
                assert(lvffp.voxelType == vtWall);
                if (lvf == Wall) {
                  lvf = Slot;
                }
                if (lvff == Wall) {
                  lvff = Slot;
                }
                if (lvfff == Wall) {
                  lvfff = Slot;
                }
                break;
              case dtTail:
                break;
              case dtPost:
                if (lvd == DS[lFwd]) {
                  if (lvf != DP[lFwd] &&
                      lvff != DP[lFwd] &&
                      lvbbb == DB[lFwd] &&
                      lvbb == DB[lFwd] &&
                      lvb == DB[lFwd]
                     )
                  {
                    lvdbbb = DS[lFwd];
                    lvbbb = DP[lFwd];
                    lvdb = DS[lFwd];
                    lvb = DP[lFwd];
                    if (lvdbb == Wall) {
                      lvdbb = Slot;
                    }
                    if (lvdf == Wall) {
                      lvdf = Slot;
                    }
                    if (lvdff == Wall) {
                      lvdff = Slot;
                    }
                    if (lvdfff == Wall) {
                      lvdfff = Slot;
                    }
                  }
                }
                break;
              case dtSlot:
                assert(dt != dtSlot);
                break;
              case dtLock:
                assert(dt != dtLock);
                break;
              case dtTest:
                assert(dt != dtTest);
                break;
              case dtGate:
                assert(uvp.voxelType == vtData && uvp.dataType == dtGate);

                assert(lvbbbp.voxelType == vtData && lvbbbp.dataType == dtBody);
                assert(lvbbp.voxelType == vtData && lvbbp.dataType == dtBody);
                assert(lvbp.voxelType == vtData && lvbp.dataType == dtBody);
                assert(lvubbbp.voxelType == vtWall || lvubbbp.voxelType == vtSlot);
                assert(lvubbp.voxelType == vtWall || lvubbp.voxelType == vtSlot);
                assert(lvubp.voxelType == vtWall || lvubp.voxelType == vtSlot);
                assert(lvup.voxelType == vtWall || lvup.voxelType == vtSlot);
                // assert(lvufp.voxelType == vtWall || lvufp.voxelType == vtSlot);
                // assert(lvuffp.voxelType == vtWall || lvuffp.voxelType == vtSlot);

                if (gateToGateOrTest[lFwd][uFwd] == dtGate) {
                  if (lvp.isComplementing) {

                    // ...###[0]...    ...###[0]...
                    // ...###[#]... -> ...0  [ ]...
                    // ...---[0]...    ...+--[-]...

                    lvbbb = DP[lFwd];
                    lvbb = DB[lFwd];
                    lvb = DB[lFwd];
                    lv = DB[lFwd];

                    lvubbb = D0[lFwd];
                    if (lvubb == Wall) {
                      lvubb = Slot;
                    }
                    if (lvub == Wall) {
                      lvub = Slot;
                    }
                    lvu = Slot;
                  } else {

                    // ...#[1]##...    ...#[1]##...
                    // ...#[#]##... -> ...1[ ]  ...
                    // ...-[1]--...    ...+[-]--...

                    lvb = DP[lFwd];
                    lv = DB[lFwd];
                    lvf = DB[lFwd];
                    lvff = DB[lFwd];

                    lvub = D1[lFwd];
                    lvu = Slot;
                    if (lvuf == Wall) {
                      lvuf = Slot;
                    }
                    if (lvuff == Wall) {
                      lvuff = Slot;
                    }
                  }
                } else {

                  // ...##[G]#...    ...##[G]#...
                  // ...##[#]#... -> ...> [ ] ...
                  // ...--[G]-...    ...+-[-]-...

                  lvbb = DP[lFwd];
                  lvb = DB[lFwd];
                  lv = DB[lFwd];
                  lvf = DB[lFwd];

                  lvubb = DQ[lFwd];
                  if (lvub == Wall) {
                    lvub = Slot;
                  }
                  lvu = Slot;
                  if (lvuf == Wall) {
                    lvuf = Slot;
                  }
                }
                break;
              case dtIPut:
              case dtOPut:
              case dtDPut:
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
        break;

      // Level 4 is the S/N data rods.

      case 4:
        for (size_t y = 0; y < brick.yMax; y += 1) {
          for (size_t x = 0; x < brick.xMax; x += 1) {
            P3D p(z, y, x);
            Voxel &uv = (Voxel &) brick.at(p);
            VoxelProperties const &uvp = voxelProperties[uv];

            switch (VoxelType vt = uvp.voxelType) {
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
            case eoVoxelType:
              assert(vt != eoVoxelType);
            }

            Direction uFwd = uvp.direction;

            P3D pu = p.offsetBy(U);
            Voxel &uvu = (Voxel &) brick.at(pu);
            VoxelProperties uvup = voxelProperties[uvu];

            P3D puf = pu.offsetBy(FWard(uFwd));
            Voxel &uvuf = (Voxel &) brick.at(puf);
            VoxelProperties uvufp = voxelProperties[uvuf];
            P3D puff = puf.offsetBy(FWard(uFwd));
            Voxel &uvuff = (Voxel &) brick.at(puff);
            VoxelProperties uvuffp = voxelProperties[uvuff];
            P3D pufff = puff.offsetBy(FWard(uFwd));
            Voxel &uvufff = (Voxel &) brick.at(pufff);
            VoxelProperties uvufffp = voxelProperties[uvufff];

            P3D puu = pu.offsetBy(U);
            Voxel &uvuu = (Voxel &) brick.at(puu);
            VoxelProperties uvuup = voxelProperties[uvuu];

            P3D pd = p.offsetBy(D);
            Voxel &uvd = (Voxel &) brick.at(pd);
            VoxelProperties uvdp = voxelProperties[uvd];

            P3D dp = pd.offsetBy(D);
            Voxel &dv = (Voxel &) brick.at(dp);
            VoxelProperties dvp = voxelProperties[dv];

            Direction lFwd = X;
            if (dvp.voxelType == vtData) {
              lFwd = dvp.direction;
            }

            P3D pf = p.offsetBy(FWard(uFwd));
            Voxel &uvf = (Voxel &) brick.at(pf);
            VoxelProperties uvfp = voxelProperties[uvf];
            P3D pff = pf.offsetBy(FWard(uFwd));
            Voxel &uvff = (Voxel &) brick.at(pff);
            VoxelProperties uvffp = voxelProperties[uvff];
            P3D pfff = pff.offsetBy(FWard(uFwd));
            Voxel &uvfff = (Voxel &) brick.at(pfff);
            VoxelProperties uvfffp = voxelProperties[uvfff];

            P3D pdf = pd.offsetBy(FWard(uFwd));
            Voxel &uvdf = (Voxel &) brick.at(pdf);
            VoxelProperties uvdfp = voxelProperties[uvdf];
            P3D pdff = pdf.offsetBy(FWard(uFwd));
            Voxel &uvdff = (Voxel &) brick.at(pdff);
            VoxelProperties uvdffp = voxelProperties[uvdff];
            P3D pdfff = pdff.offsetBy(FWard(uFwd));
            Voxel &uvdfff = (Voxel &) brick.at(pdfff);
            VoxelProperties uvdfffp = voxelProperties[uvdfff];

            P3D pb = p.offsetBy(BWard(uFwd));
            Voxel &uvb = (Voxel &) brick.at(pb);
            VoxelProperties uvbp = voxelProperties[uvb];
            P3D pbb = pb.offsetBy(BWard(uFwd));
            Voxel &uvbb = (Voxel &) brick.at(pbb);
            VoxelProperties uvbbp = voxelProperties[uvbb];
            P3D pbbb = pbb.offsetBy(BWard(uFwd));
            Voxel &uvbbb = (Voxel &) brick.at(pbbb);
            VoxelProperties uvbbbp = voxelProperties[uvbbb];

            P3D pdb = pd.offsetBy(BWard(uFwd));
            Voxel &uvdb = (Voxel &) brick.at(pdb);
            VoxelProperties uvdbp = voxelProperties[uvdb];
            P3D pdbb = pdb.offsetBy(BWard(uFwd));
            Voxel &uvdbb = (Voxel &) brick.at(pdbb);
            VoxelProperties uvdbbp = voxelProperties[uvdbb];
            P3D pdbbb = pdbb.offsetBy(BWard(uFwd));
            Voxel &uvdbbb = (Voxel &) brick.at(pdbbb);
            VoxelProperties uvdbbbp = voxelProperties[uvdbbb];

            P3D pub = pu.offsetBy(BWard(uFwd));
            Voxel &uvub = (Voxel &) brick.at(pub);
            VoxelProperties uvubp = voxelProperties[uvub];
            P3D pubb = pub.offsetBy(BWard(uFwd));
            Voxel &uvubb = (Voxel &) brick.at(pubb);
            VoxelProperties uvubbp = voxelProperties[uvubb];
            P3D pubbb = pubb.offsetBy(BWard(uFwd));
            Voxel &uvubbb = (Voxel &) brick.at(pubbb);
            VoxelProperties uvubbbp = voxelProperties[uvubbb];

            switch (DataType dt = uvp.dataType) {
              case dtHead:
                assert(uvfp.voxelType == vtWall);
                assert(uvffp.voxelType == vtWall);
                if (uvf == Wall) {
                  uvf = Slot;
                }
                if (uvff == Wall) {
                  uvff = Slot;
                }
                if (uvfff == Wall) {
                  uvfff = Slot;
                }
                break;
              case dtTail:
                break;
              case dtBody:
                break;
              case dtPost:
                if (uvu == DS[uFwd]) {
                  if (uvf != DP[uFwd] &&
                      uvff != DP[uFwd] &&
                      uvbbb == DB[uFwd] &&
                      uvbb == DB[uFwd] &&
                      uvb == DB[uFwd]
                     )
                  {
                    uvubbb = DS[uFwd];
                    uvbbb = DP[uFwd];
                    uvub = DS[uFwd];
                    uvb = DP[uFwd];
                    if (uvubb == Wall) {
                      uvubb = Slot;
                    }
                    if (uvuf == Wall) {
                      uvuf = Slot;
                    }
                    if (uvuff == Wall) {
                      uvuff = Slot;
                    }
                    if (uvufff == Wall) {
                      uvufff = Slot;
                    }
                  }
                }
                break;
              case dtSlot:
                assert(dt != dtSlot);
                break;
              case dtLock:
                assert(dt != dtLock);
                break;
              case dtTest:
                assert(dt != dtTest);
                break;
              case dtGate:
                assert(dvp.voxelType == vtData && dvp.dataType == dtBody);

                assert(uvbbbp.voxelType == vtData && uvbbbp.dataType == dtBody);
                assert(uvbbp.voxelType == vtData && uvbbp.dataType == dtBody);
                assert(uvbp.voxelType == vtData && uvbp.dataType == dtBody);
                assert(uvdbbbp.voxelType == vtWall || uvdbbbp.voxelType == vtSlot);
                assert(uvdbbp.voxelType == vtWall || uvdbbp.voxelType == vtSlot);
                assert(uvdbp.voxelType == vtWall || uvdbp.voxelType == vtSlot);
                assert(uvdp.voxelType == vtWall || uvdp.voxelType == vtSlot);
                // assert(uvdfp.voxelType == vtWall || uvdfp.voxelType == vtSlot);
                // assert(uvdffp.voxelType == vtWall || uvdffp.voxelType == vtSlot);

                if (gateToGateOrTest[uFwd][lFwd] == dtGate) {
                  if (uvp.isComplementing) {

                    // ...###[0]...    ...###[0]...
                    // ...###[#]... -> ...   [ ]...
                    // ...---[0]...    ...+--[-]...

                    uvbbb = DP[uFwd];
                    uvbb = DB[uFwd];
                    uvb = DB[uFwd];
                    uv = DB[uFwd];

                    uvdbbb = D0[uFwd];
                    if (uvdbb == Wall) {
                      uvdbb = Slot;
                    }
                    if (uvdb == Wall) {
                      uvdb = Slot;
                    }
                    uvd = Slot;
                  } else {

                    // ...#[1]##...    ...#[1]##...
                    // ...#[#]##... -> ...@[ ]  ...
                    // ...-[1]--...    ...+[-]--...

                    uvb = DP[uFwd];
                    uv = DB[uFwd];
                    uvf = DB[uFwd];
                    uvff = DB[uFwd];

                    uvdb = D1[uFwd];
                    uvd = Slot;
                    if (uvdf == Wall) {
                      uvdf = Slot;
                    }
                    if (uvdff == Wall) {
                      uvdff = Slot;
                    }
                  }
                } else {

                  // ...##[?]#...    ...##[?]#...
                  // ...##[#]#... -> ...> [ ] ...
                  // ...--[?]-...    ...+-[-]-...

                  uvbb = DP[uFwd];
                  uvb = DB[uFwd];
                  uv = DB[uFwd];
                  uvf = DB[uFwd];

                  uvdbb = DQ[uFwd];
                  if (uvdb == Wall) {
                    uvdb = Slot;
                  }
                  uvd = Slot;
                  if (uvdf == Wall) {
                    uvdf = Slot;
                  }
                }
                break;
              case dtIPut:
              case dtOPut:
              case dtDPut:
                break;
              default:
                assert(dt != eoDataType);
                break;
            }
          }
        }
        break;

      // Level 6 is the E/W lock rods.

      case 6:
        for (size_t y = 0; y < brick.yMax; y += 1) {
          for (size_t x = 0; x < brick.xMax; x += 1) {
            P3D p(z, y, x);
            Voxel &v = (Voxel &) brick.at(p);
            VoxelProperties const &vp = voxelProperties[v];
            VoxelType vt = vp.voxelType;

            switch (vt) {
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
              assert(vp.dataType == dtLock);
              break;
            case eoVoxelType:
              assert(vt != eoVoxelType);
            }

            Direction lFwd =
                vt == vtLock ? vp.direction : dataToLockDirection[vp.direction];
            Direction dFwd =
                vt == vtLock ? lockToDataDirection[vp.direction] : vp.direction;

            P3D pd = p.offsetBy(D);
            Voxel &vd = (Voxel &) brick.at(pd);
            VoxelProperties vdp = voxelProperties[vd];

            P3D pdd = pd.offsetBy(D);
            Voxel &vdd = (Voxel &) brick.at(pdd);
            VoxelProperties vddp = voxelProperties[vdd];

            // For vtData/dtLock's....

            if (vt == vtData && vp.dataType == dtLock) {
              P3D pddf = pdd.offsetBy(FWard(lFwd));
              Voxel &vddf = (Voxel &) brick.at(pddf);
              VoxelProperties vddfp = voxelProperties[vddf];

              P3D pf = p.offsetBy(FWard(lFwd));
              Voxel &vf = (Voxel &) brick.at(pf);
              VoxelProperties vfp = voxelProperties[vf];

              P3D pdf = pd.offsetBy(FWard(lFwd));
              Voxel &vdf = (Voxel &) brick.at(pdf);
              VoxelProperties vdfp = voxelProperties[vdf];
              P3D pdff = pdf.offsetBy(FWard(lFwd));
              Voxel &vdff = (Voxel &) brick.at(pdff);
              VoxelProperties vdffp = voxelProperties[vdff];


              // Setup a key for this lock, and change the data rod's
              // dtLock to a dtSlot....

              assert(vfp.voxelType == vtLock && vfp.lockType == ltBody);
              assert(vdp.voxelType == vtWall);
              assert(vdfp.voxelType == vtWall);
              assert(vddp.voxelType == vtData && vddp.dataType == dtLock);

              vdd = DP[dFwd];
              vd = DS[dFwd];
              vdf = LK[lFwd];
              if (vdff == Wall) {
                vdff = Slot;
              }
              vf = LP[lFwd];
              v = LB[lFwd];
              continue;
            }

            // For vtLock's....

            P3D pf = p.offsetBy(FWard(lFwd));
            Voxel &vf = (Voxel &) brick.at(pf);
            VoxelProperties vfp = voxelProperties[vf];

            switch (LockType lt = vp.lockType) {
              case ltLock:
                assert(vdp.voxelType == vtWall);
                assert(vfp.voxelType == vtLock && vfp.lockType == ltBody);

                if (vddp.voxelType == vtWall) {
                  P3D pddf = pdd.offsetBy(FWard(lFwd));
                  Voxel &vddf = (Voxel &) brick.at(pddf);
                  VoxelProperties vddfp = voxelProperties[vddf];
                  P3D pdf = pd.offsetBy(FWard(lFwd));
                  Voxel &vdf = (Voxel &) brick.at(pdf);
                  VoxelProperties vdfp = voxelProperties[vdf];

                  assert(vddfp.voxelType == vtData && vddfp.dataType == dtSlot);

                  vddf = DP[dFwd];
                  vdf = DS[dFwd];
                  vd = LK[lFwd];
                  v = LP[lFwd];

                } else if (vddp.voxelType == vtData) {
                  P3D pdf = pd.offsetBy(FWard(lFwd));
                  Voxel &vdf = (Voxel &) brick.at(pdf);
                  VoxelProperties vdfp = voxelProperties[vdf];
                  P3D pf = p.offsetBy(FWard(lFwd));
                  Voxel &vf = (Voxel &) brick.at(pf);
                  VoxelProperties vfp = voxelProperties[vf];

                  assert(vddp.dataType == dtLock);
                  vdd = DP[dFwd];
                  vd = DS[dFwd];
                  vdf = LK[lFwd];
                  vf = LP[lFwd];
                  v = LB[lFwd];
                } else {
                  assert(vddp.voxelType == vtWall || vddp.voxelType == vtData);
                }
                break;
              case ltHead:
                assert(vfp.voxelType == vtWall);
                if (vf == Wall) {
                  vf = Slot;
                }
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
    }
  }

  if (optVerbose) {
    brick.Dump();
  }
}

void Diagram2D::getIODPointsAndLabels(MapP3dToLabel (&p3dToLabel)[3]) const {
  static size_t const dToZ[eoDirection] = { 2, 4, 7, 2, 4, 7 };
  for (auto d : direction) {
    for (auto const &rod : rodsWithInputs[d]) {
      Label const &label = rod->rodsLabel();
      string labelText(label.ToString());
      SetOfP2Ds const &p2ds = rod->getInputs();
      for (auto const &p2d : p2ds) {
        P3D p3d(dToZ[d], p2d.y, p2d.x);
        p3dToLabel[0][p3d] = labelText;
      }
    }

    for (auto const &rod : rodsWithOutputs[d]) {
      Label const &label = rod->rodsLabel();
      string labelText(label.ToString());
      SetOfP2Ds const &p2ds = rod->getOutputs();
      for (auto const &p2d : p2ds) {
        P3D p3d(dToZ[d], p2d.y, p2d.x);
        p3dToLabel[1][p3d] = labelText;
      }
    }

    for (auto const &rod : rodsWithDebugOutputs[d]) {
      Label const &label = rod->rodsLabel();
      string labelText(label.ToString());
      SetOfP2Ds const &p2ds = rod->getDebugOutputs();
      for (auto const &p2d : p2ds) {
        P3D p3d(dToZ[d], p2d.y, p2d.x);
        p3dToLabel[2][p3d] = labelText;
      }
    }
  }
}
