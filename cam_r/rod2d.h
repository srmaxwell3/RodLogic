#ifndef ROD2D_H
#define ROD2D_H

#include <set>
using std::set;
#include <string>
using std::string;

#include "p2d.h"

#include "directions.h"
#include "edged_bool.h"
#include "label.h"
#include "rod_connection.h"

struct Diagram2D;
struct Rod2D;
typedef set<Rod2D *> SetOfRod2Ds;

struct Rod2D : public SetOfP2Ds {
  Rod2D(Diagram2D &diagram, P2D const &pStart, Directions d);
  Rod2D();

  Label const &rodsLabel() const;
  string const &rodsId() const;

  bool rodIsIncomplete() const;
  bool rodsDirection() const;
  int rodsLength() const;

  void setFirstTick (int t);
  int tickFirstSetAt () const;
  void verifyInputDelays();
  void verifyInputDelays(SetOfRod2Ds &seenAlready);

  void connectWith(Rod2D *that, bool isAnIdentityConnection);
  bool isShared(P2D const &p) const;
  bool hasInputs() const;
  bool hasOutputs() const;

  void reset();
  bool getValue() const;
  bool evaluateAt(Diagram2D &diagram, int tick);
  bool hasChanged() const;

  void dump(Diagram2D const &diagram) const;

 private:
  bool findHeadLabel(Diagram2D const &diagram);
  bool findTailLabel(Diagram2D const &diagram);
  bool findLabel(Diagram2D const &diagram, P2D pStart, Directions oWard);

  Label label;
  string id;
  bool isComplete;
  P2D headAt;
  P2D tailAt;
  Directions direction;
  int tickFirstSet;
  int countOfValidInputs;
  SetOfP2Ds shared;
  SetOfP2Ds inputs;
  SetOfP2Ds outputs;
  set<RodConnection> connectedTo[eoRodConnectionType];
  EdgedBool lastEvaluatedValue;
  int lastEvaluatedTick;
};

#endif // ROD2D
