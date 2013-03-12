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

struct Diagram2D : public vector<string> {
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

  char const &at(P2D const &p) const {
    return at(p.y, p.x);
  }

  char const &at(int y, int x) const {
    return vector<string>::at(y).at(x);
  }

  void newRodAt(P2D const &p, Directions d);
  void newIncompleteEWRodAt(P2D const &p);
  void newIncompleteSNRodAt(P2D const &p);

  void rodSharedAt(Rod2D *r, P2D const &p);

  void scan();

  void setInputFor(Label const &label, vector<int> const &values);
  void setInputFor(string const &label, vector<int> const &values);
  void addInputFor(Label const &label, bool value);
  bool getInputFor(Label const &label);
  bool getInputFor(string const &name, int bitNumber);

  void reset();
  void evaluateAt(int tick);
  void evaluate();
  bool rodsWereChangedDuringLastEvaluation() const {
    return 0 < lastEvaluatedTickNChangedRods;
  }
  bool inputsWereReadDuringLastEvaluation() const {
    return 0 < lastEvaluatedTickNInputsRead;
  }
  bool outputsWereWrittenDuringLastEvaluation() const {
    return 0 < lastEvaluatedTickNOutputsWritten;
  }
  bool hasInputFor(Label const &label);
  bool readInputFor(Label const &label);
  void writeOutputFor(Label const &label, bool value);
  bool getOutputFor(Label const &label);
  bool getOutputFor(string const &name, int bitNumber);
  int CurrentTick() const { return lastEvaluatedTick; }

  void dumpInputLabelState(Label const &label, char const *comma, CombinedLabel const *&lastCLabel);
  void dumpOutputLabelState(Label const &label, char const *comma, CombinedLabel const *&lastCLabel);
  void dumpLabelState(bool isInput, Label const &label, char const *comma, CombinedLabel const *&lastCLabel);
  void dumpState();
  void dump() const;

  static bool isLegalEWChar(char c);
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
  map<Label, bool> currentOutputs;
  map<string, CombinedLabel> currentCombinedOutputs;
  int lastEvaluatedTick;
  int lastEvaluatedTickNChangedRods;
  int lastEvaluatedTickNInputsRead;
  int lastEvaluatedTickNOutputsWritten;
  int xMax;
  int yMax;
};

#endif // DIAGRAM2D
