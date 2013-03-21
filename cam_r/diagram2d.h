#ifndef DIAGRAM2D_H
#define DIAGRAM2D_H

#include <array>
using std::array;
#include <fstream>
using std::ifstream;
#include <iostream>
using std::istream;
#include <map>
using std::map;
#include <string>
using std::string;
#include <utility>
using std::pair;
#include <vector>
using std::vector;

#include "bit_stream_input.h"
#include "directions.h"
#include "label.h"
#include "p2d.h"

struct Rod2D;
typedef set<Rod2D *> SetOfRod2Ds;

struct PlateOfInt : public vector<vector<int>> {
  PlateOfInt(size_t _yMax = 0, size_t _xMax = 0);
  PlateOfInt(PlateOfInt const &that);

  void insertRow(size_t atY);
  void insertCol(size_t atX);
  void deleteRow(size_t atY);
  void deleteCol(size_t atX);
  bool compareRowAndRowBelow(size_t atY) const;
  bool compareColAndColToRight(size_t atX) const;
  bool isRowEmpty(size_t atY) const;
  bool isColEmpty(size_t atX) const;
  bool isRowSqueezable(size_t atY) const;
  bool isColSqueezable(size_t atX) const;
  void Dump() const;

  size_t yMax;
  size_t xMax;
};

struct Diagram2D : public PlateOfInt {
  Diagram2D(PlateOfInt const &p);
  Diagram2D(istream &in);

  bool isInBounds(P2D const &p) const {
    return 0 <= p.x && p.x < xMax && 0 <= p.y && p.y < yMax;
  }

  bool isLegalEW(P2D const &p) const {
    return isInBounds(p) && isLegalEWChar(at(p));
  }

  bool isLegalSN(P2D const &p) const {
    return isInBounds(p) && isLegalSNChar(at(p));
  }

  bool hasAlreadyBeenSeen(P2D const &p) {
    return pointsAlreadySeen.find(p) != pointsAlreadySeen.end();
  }

  void saw(P2D const &p) {
    pointsAlreadySeen.insert(p);
  }

  int const &at(P2D const &p) const {
    return at(p.y, p.x);
  }

  int const &at(int y, int x) const {
    return PlateOfInt::at(y).at(x);
  }

  void newRodAt(P2D const &p, Directions d);
  void newIncompleteEWRodAt(P2D const &p);
  void newIncompleteSNRodAt(P2D const &p);

  void rodSharedAt(Rod2D *r, P2D const &p);

  void scan();

  void setInputFor(Label const &label, vector<int> const &values);
  void setInputFor(string const &label, vector<int> const &values);
  void addInputFor(Label const &label, bool value);
  EdgedBool const &getInputFor(Label const &label);
  EdgedBool const &getInputFor(string const &name, int bitNumber);

  void reset();
  void evaluateAt(int tick);
  void evaluate();
  bool rodsWereChangedDuringLastEvaluation() const {
    return 0 < lastEvaluatedTickNChangedRods;
  }
  bool inputsWereReadDuringLastEvaluation() const {
    return 0 < lastEvaluatedTickNInputsRead;
  }
  bool unreadInputsWereReadDuringLastEvaluation() const {
    return 0 < lastEvaluatedTickNUnreadInputsRead;
  }
  bool outputsWereWrittenDuringLastEvaluation() const {
    return 0 < lastEvaluatedTickNOutputsWritten;
  }
  bool debugOutputsWereWrittenDuringLastEvaluation() const {
    return 0 < lastEvaluatedTickNDebugOutputsWritten;
  }
  size_t maxUnreadInput() const;
  bool hasUnreadInput() const;
  bool hasInputFor(Label const &label);
  EdgedBool const &readInputFor(Label const &label);

  void writeOutputFor(Label const &label, bool value);
  EdgedBool const &getOutputFor(Label const &label);
  EdgedBool const &getOutputFor(string const &name, int bitNumber);

  void writeDebugOutputFor(Label const &label, bool value);
  EdgedBool const &getDebugOutputFor(Label const &label);
  EdgedBool const &getDebugOutputFor(string const &name, int bitNumber);

  int CurrentTick() const { return lastEvaluatedTick; }

  void refactor();

  bool dumpInputLabelState
    (Label const &label,
     char const *comma,
     CombinedLabel const *&lastCLabel
    );
  bool dumpOutputLabelState
    (Label const &label,
     char const *comma,
     CombinedLabel const *&lastCLabel
    );
  bool dumpDebugOutputLabelState
    (Label const &label,
     char const *comma,
     CombinedLabel const *&lastCLabel
    );
  bool dumpLabelState
    (int isIOD,
     Label const &label,
     char const *comma,
     CombinedLabel const *&lastCLabel
    );
  void dumpState();
  void dumpPerformance() const;
  void dump() const;

  void Rebuild(PlateOfInt &plane) const;

  static bool isLegalEWCharNotLabel1st(char c);
  static bool isLegalEWChar(char c);
  static bool isLegalSNCharNotLabel1st(char c);
  static bool isLegalSNChar(char c);

  set<P2D> pointsAlreadySeen;
  map<P2D, SetOfRod2Ds> pointsShared;
  array<SetOfRod2Ds, eoDirections> rods;

  array<SetOfRod2Ds, eoDirections> rodsWithInputs;
  Directions earliestInput;
  map<Label, BitStreamInput> currentInputs;
  map<string, CombinedLabel> currentCombinedInputs;

  array<SetOfRod2Ds, eoDirections> rodsWithOutputs;
  Directions earliestOutput;
  map<Label, EdgedBool> currentOutputs;
  map<string, CombinedLabel> currentCombinedOutputs;

  array<SetOfRod2Ds, eoDirections> rodsWithDebugOutputs;
  Directions earliestDebugOutput;
  map<Label, EdgedBool> currentDebugOutputs;
  map<string, CombinedLabel> currentCombinedDebugOutputs;

  array<long, eoDirections> totalEvaluatedUSecsPerDirection;
  long totalEvaluatedUSecs;
  long totalEvaluatedTicks;

  int lastEvaluatedTick;
  int lastEvaluatedTickNChangedRods;
  int lastEvaluatedTickNInputsRead;
  int lastEvaluatedTickNUnreadInputsRead;
  int lastEvaluatedTickNOutputsWritten;
  int lastEvaluatedTickNDebugOutputsWritten;
};

#endif // DIAGRAM2D
