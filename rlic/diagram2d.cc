#include "diagram2d.h"
#include "rod2d.h"

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

extern bool optEchoInput;
extern bool optShowChangedStateEveryTick;
extern bool optShowPerfrmance;
extern bool optShowRods;

Diagram2D::Diagram2D(istream &in) :
    vector<string>(),
    earliestInput(eoDirections),
    earliestOutput(eoDirections),
    totalEvaluatedUSecs(0),
    totalEvaluatedTicks(0),
    lastEvaluatedTick(-1),
    lastEvaluatedTickNChangedRods(0),
    lastEvaluatedTickNInputsRead(0),
    lastEvaluatedTickNOutputsWritten(0),
    xMax(0),
    yMax(0)
{
  do {
    string line;
    getline(in, line);
    if (in.good()) {
      push_back(line);
      if (xMax < line.size()) {
        xMax = line.size();
      }
    }
  } while (in.good() && !in.eof());

  for (auto &line : (*this)) {
    line.resize(xMax, ' ');
  }

  totalEvaluatedUSecsPerDirection.fill(0);
  yMax = size();
}

void Diagram2D::newRodAt(P2D const &p, Directions d) {
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
  Directions d =
      0 < nEHeads ?
      (0 < nWHeads ? eoDirections : E) :
      (0 < nWHeads ? W : eoDirections);

  if (d == eoDirections) {
    if (nEHeads || nWHeads) {
      fprintf(stderr,
              "cam.r: Warning: Unable to determine the direction of an EW rod, at P2D(%d,%d)!\n",
              p.y,
              p.x
             );
      fprintf(stdout,
              "cam.r: Warning: Unable to determine the direction of an EW rod, at P2D(%d,%d)!\n",
              p.y,
              p.x
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
  Directions d =
      0 < nSHeads ?
      (0 < nNHeads ? eoDirections : S) :
      (0 < nNHeads ? N : eoDirections);

  if (d == eoDirections) {
    if (nSHeads || nNHeads) {
      fprintf(stderr,
              "cam.r: Warning: Unable to determine the direction of an SN rod, at P2D(%d,%d)!\n",
              p.y,
              p.x
             );
      fprintf(stdout,
              "cam.r: Warning: Unable to determine the direction of an SN rod, at P2D(%d,%d)!\n",
              p.y,
              p.x
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
    bool isAnIdentityConnection = false;

    switch (char const &c = at(p)) {
      case '0':
        isAnIdentityConnection = false;
        break;

      case '1':
        isAnIdentityConnection = true;
        break;

      default:
        assert(c == '0' || c == '1');
    }

    if (1 < rods.size()) {
      for (auto r1 = rods.begin(); r1 != rods.end(); ++r1) {
        for (auto r2 = std::next(r1); r2 != rods.end(); ++r2) {
          (*r1)->connectWith(*r2, isAnIdentityConnection);
        }
      }
    }
  }

  earliestInput = eoDirections;
  for (auto const d : directions) {
    if (!rodsWithInputs[d].empty()) {
      earliestInput = d;
    }
  }

  earliestOutput = eoDirections;
  for (auto const d : directions) {
    if (!rodsWithOutputs[d].empty()) {
      earliestOutput = d;
    }
  }

  if (earliestInput != eoDirections) {
    int tick = 0;
    for (auto d = earliestInput; Last(d) != earliestInput; d = Last(d)) {
      for (auto &r : rodsWithInputs[d]) {
        r->setFirstTick(tick);
      }
      tick += 1;
    }
  }

  if (earliestOutput != eoDirections) {
    for (auto d = earliestOutput; Last(d) != earliestOutput; d = Last(d)) {
      for (auto &r : rodsWithOutputs[d]) {
        r->verifyInputDelays();
      }
    }
  }
}

void Diagram2D::reset() {
  for (Directions d : directions) {
    auto &dRods = rods[d];

    for (auto &r : dRods) {
      r->reset();
    }
  }
  lastEvaluatedTick = -1;
}

void Diagram2D::evaluateAt(int tick) {
  Directions d =
      Directions((earliestInput != eoDirections ? tick + int(earliestInput) : tick) %
                 eoDirections
                );
  lastEvaluatedTickNChangedRods = 0;
  lastEvaluatedTickNInputsRead = 0;
  lastEvaluatedTickNOutputsWritten = 0;

  auto boEvaluation = CHRONO_CLOCK::now();
  for (auto &r : rods[d]) {
    r->evaluateAt(*this, tick);

    if (r->hasChanged()) {
      lastEvaluatedTickNChangedRods += 1;
    }
  }

  auto eoEvaluation = CHRONO_CLOCK::now();
  auto dTEvaluation = duration_cast<microseconds>(eoEvaluation - boEvaluation).count();
  totalEvaluatedUSecsPerDirection[totalEvaluatedTicks % eoDirections] += dTEvaluation;
  totalEvaluatedUSecs += dTEvaluation;

  if (optShowPerfrmance) {
    fprintf(stdout,
	    "%ld [%ld.%s]: dT = %lld us\n",
	    totalEvaluatedTicks,
	    totalEvaluatedTicks / eoDirections,
	    c_str(Directions(totalEvaluatedTicks % eoDirections)),
	    (long long) dTEvaluation
	    );
  }

  totalEvaluatedTicks += 1;
  lastEvaluatedTick = tick;
}

void Diagram2D::evaluate() {
  evaluateAt(lastEvaluatedTick + 1);
}

bool Diagram2D::dumpInputLabelState(Label const &label, char const *comma, CombinedLabel const *&lastCLabel) {
  return dumpLabelState(true, label, comma, lastCLabel);
}

bool Diagram2D::dumpOutputLabelState(Label const &label, char const *comma, CombinedLabel const *&lastCLabel) {
  return dumpLabelState(false, label, comma, lastCLabel);
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

bool Diagram2D::dumpLabelState(bool isInput, Label const &label, char const *comma, CombinedLabel const *&lastCLabel) {
  string labelToString = label.ToString();
  bool dumpedALabel = false;

  if (label.BitNumber() < 0) {
    unsigned &lastLabelState =
        isInput ? lastInputLabelStates[labelToString] : lastOutputLabelStates[labelToString];
    unsigned value = isInput ? getInputFor(label) : getOutputFor(label);

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
            bool bit = isInput ? getInputFor(name, bitNumber) : getOutputFor(name, bitNumber);
            value = (value << 1) | bit;
          }
        } else {
          for (int bitNumber = minBitNumber; bitNumber <= maxBitNumber; bitNumber += 1) {
            if (bitNumber < 0) {
              continue;
            }
            bool bit = isInput ? getInputFor(name, bitNumber) : getOutputFor(name, bitNumber);
            value = (value << 1) | bit;
          }
        }

        unsigned &lastLabelState =
            isInput ? lastInputLabelStates[labelToString] : lastOutputLabelStates[labelToString];

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
            isInput ? lastInputLabelStates[labelToString] : lastOutputLabelStates[labelToString];
        unsigned value = isInput ? getInputFor(label) : getOutputFor(label);

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
        isInput ? lastInputLabelStates[labelToString] : lastOutputLabelStates[labelToString];
    unsigned value = isInput ? getInputFor(label) : getOutputFor(label);

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
  fprintf(stdout, "%d: inputs={", lastEvaluatedTick);
  bool dumpedALabel = false;
  char const *comma = "\n    ";
  if (!currentInputs.empty()) {
    CombinedLabel const *lastCLabel = nullptr;
    for (auto const &lv : currentInputs) {
      dumpedALabel |= dumpInputLabelState(lv.first, comma, lastCLabel);
      comma = ",\n    ";
    }
  }
  if (dumpedALabel) {
    fprintf(stdout, "\n  }");
  } else {
    fprintf(stdout, " }");
  }

  fprintf(stdout, ", outputs={");
  dumpedALabel = false;
  comma = "\n    ";
  if (!currentOutputs.empty()) {
    CombinedLabel const *lastCLabel = nullptr;
    for (auto const &lv : currentOutputs) {
      dumpedALabel |= dumpOutputLabelState(lv.first, comma, lastCLabel);
      comma = ",\n    ";
    }
  }
  if (dumpedALabel) {
   fprintf(stdout, "\n  }\n");
  } else {
   fprintf(stdout, " }\n");
  }
}

void Diagram2D::dumpPerformance() const {
  if (int totalEvaluatedCycles = totalEvaluatedTicks / eoDirections) {
    double averageEvaluatedUSecsPerCycle = 0.0;
    fprintf(stdout, "Performance:\n");
    for (auto d : directions) {
      double averageEvaluatedUSecsForTick = totalEvaluatedUSecsPerDirection[d] / totalEvaluatedTicks;
      fprintf(stdout,
              "%s: %9.2f (%9.2f) uS mean total time (per rod time)/tick\n",
              c_str(d),
              averageEvaluatedUSecsForTick,
              averageEvaluatedUSecsForTick / rods[d].size()
             );
      averageEvaluatedUSecsPerCycle += averageEvaluatedUSecsForTick;
    }
    fprintf(stdout, "   %9.2f uS mean time/cycle\n", averageEvaluatedUSecsPerCycle);
    fprintf(stdout, "Total %ld ticks (%d cycles), %ld uS\n", totalEvaluatedTicks, totalEvaluatedCycles, totalEvaluatedUSecs);
  }
}

void Diagram2D::setInputFor(string const &text, vector<int> const &values) {
  for (size_t dotAt = text.find_first_of('.'); dotAt != string::npos; dotAt = text.find_first_of('.', dotAt + 1)) {
    string name = text.substr(0, dotAt);
    string tail = text.substr(dotAt + 1);
    int hiBitNumber;
    int loBitNumber;
    int nChars;
    if (sscanf(tail.c_str(), "%x..%x%n", &hiBitNumber, &loBitNumber, &nChars) == 2 && nChars == tail.size()) {
      vector<Label> labels;
      bool isBigEndian = false;
      if (optEchoInput) {
	fprintf(stdout, "name=%s, hiBitNumber=%d, loBitNumber=%d\n", name.c_str(), hiBitNumber, loBitNumber);
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
	  fprintf(stdout, "[%lu] %s<-0x%0*x", nthValue++, labelToString.c_str(), nHexDigits, value);
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
  assert(currentInputs.find(label) != currentInputs.end());
  currentInputs[label].push_back(value);
}

bool Diagram2D::hasInputFor(Label const &label) {
  assert(currentInputs.find(label) != currentInputs.end());
  return !currentInputs[label].empty();
}

bool Diagram2D::getInputFor(Label const &label) {
  assert(currentInputs.find(label) != currentInputs.end());
  auto &values = currentInputs[label];
  if (!values.empty()) {
    return values.front();
  }
  return false;
}

bool Diagram2D::getInputFor(string const &name, int bitNumber) {
  Label label(name, bitNumber);
  // assert(currentInputs.find(label) != currentInputs.end());
  if (currentInputs.find(label) != currentInputs.end()) {
    auto &values = currentInputs[label];
    if (!values.empty()) {
      return values.front();
    }
  } else {
    fprintf(stderr,
            "cam.r: Warning: Diagram2D::getInputFor(name=\"%s\", bitNumber=%d): "
            "Unable to find input for Label().ToString()=%s\n",
            name.c_str(),
            bitNumber,
            label.ToString().c_str()
           );
    fprintf(stdout,
            "cam.r: Warning: Diagram2D::getInputFor(name=\"%s\", bitNumber=%d): "
            "Unable to find input for Label().ToString()=%s\n",
            name.c_str(),
            bitNumber,
            label.ToString().c_str()
           );
  }
  return false;
}

bool Diagram2D::readInputFor(Label const &label) {
  assert(currentInputs.find(label) != currentInputs.end());
  lastEvaluatedTickNInputsRead += 1;
  auto &values = currentInputs[label];
  if (!values.empty()) {
    values.pop_front();
  }
  return getInputFor(label);
}

void Diagram2D::writeOutputFor(Label const &label, bool value) {
  assert(currentOutputs.find(label) != currentOutputs.end());
  lastEvaluatedTickNOutputsWritten += 1;
  currentOutputs[label] = value;
}

bool Diagram2D::getOutputFor(Label const &label) {
  assert(currentOutputs.find(label) != currentOutputs.end());
  return currentOutputs[label];
}

bool Diagram2D::getOutputFor(string const &name, int bitNumber) {
  Label label(name, bitNumber);
  assert(currentOutputs.find(label) != currentOutputs.end());
  return currentOutputs[label];
}

void Diagram2D::dump() const {
  struct {
    size_t nRods;
    size_t nIncompleteRods;
  } rodCounts[eoDirections + 1] = {
    { 0, 0 }, // E
    { 0, 0 }, // S
    { 0, 0 }, // D
    { 0, 0 }, // W
    { 0, 0 }, // N
    { 0, 0 }, // U
    { 0, 0 }  // eoDirections
  };

  map<int, int> rodLengthHistogram;

  for (Directions d : directions) {
    auto const &dRods = rods[d];

    rodCounts[eoDirections].nRods += dRods.size();
    rodCounts[d].nRods = dRods.size();

    for (auto const &r : dRods) {
      if (r->rodIsIncomplete()) {
        rodCounts[eoDirections].nIncompleteRods += 1;
        rodCounts[d].nIncompleteRods += 1;
      }
      rodLengthHistogram[r->rodsLength()] += 1;

      if (optShowRods) {
        r->dump(*this);
      }
    }
  }
  if (optShowRods) {
    fprintf(stdout, "\n");
  }

  fprintf(stdout, "Rod Count by Length:\n");
  for (auto const &lengthAndCount : rodLengthHistogram) {
    fprintf(stdout, "%5d: %5d\n", lengthAndCount.first, lengthAndCount.second);
  }
  fprintf(stdout, "\n");

  fprintf(stdout, "Rod Count (and # incomplete) by Direction:\n");
  for (Directions d : directions) {
    auto const &cs = rodCounts[d];
    fprintf(stdout, "%5s: %5lu (%5lu)\n", c_str(d), cs.nRods, cs.nIncompleteRods);
  }
  fprintf(stdout,
          "Total: %5lu (%5lu)\n",
          rodCounts[eoDirections].nRods,
          rodCounts[eoDirections].nIncompleteRods
         );
  fprintf(stdout, "\n");

  fprintf(stdout,
          "Earliest input: %s\n",
          earliestInput != eoDirections ? c_str(earliestInput) : "None"
         );
  fprintf(stdout,
          "Earliest output: %s\n",
          earliestOutput != eoDirections ? c_str(earliestOutput) : "None"
         );
  fprintf(stdout, "\n");
}

bool Diagram2D::isLegalEWChar(char c) {
  switch (c) {
  case '-':
  case '|':
  case '.':
  case '0':
  case '1':
  case '>':
  case '<':
  case 'I':
  case 'O':
    return true;
  default:
    return false;
  }
}

bool Diagram2D::isLegalSNChar(char c) {
  switch (c) {
  case '|':
  case '.':
  case '0':
  case '1':
  case 'v':
  case '^':
  case 'I':
  case 'O':
    return true;
  default:
    return false;
  }
}
