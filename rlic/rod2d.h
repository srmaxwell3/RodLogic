#ifndef ROD2D_H
#define ROD2D_H

#include <set>
using std::set;
#include <string>
using std::string;

#include "p2d.h"

#include "direction.h"
#include "edged_bool.h"
#include "label.h"
#include "rod_connection.h"
#include "voxel.h"

template<typename T> struct Plate;
struct Diagram2D;
struct Rod2D;
typedef set<Rod2D *> SetOfRod2Ds;

class Rod2D : public SetOfP2Ds {
 public:
  Rod2D(Diagram2D &diagram, P2D const &pStart, Direction d);
  Rod2D();

  Label const &rodsLabel() const;
  string const &rodsId() const;

  bool rodIsIncomplete() const;
  bool rodIsALockRod() const;
  bool rodsDirection() const;
  int rodsLength() const;
  P2D const &getHeadAt() const { return headAt; }
  P2D const &getTailAt() const { return tailAt; }

  void setFirstTick (int t);
  int tickFirstSetAt () const;
  void verifyInputDelays();
  void verifyInputDelays(SetOfRod2Ds &seenAlready);

  void connectWith(Rod2D *that, RodIntersectionType intersectionType);
  bool isShared(P2D const &p) const;
  bool hasInputs() const;
  bool hasOutputs() const;
  bool hasDebugOutputs() const;
  size_t countOfInputs() const;
  size_t countOfOutputs() const;

  string const &formExpression();
  string const &getExpression() const { return expression; }

  void DetermineIfALockRod();

  void reset();
  EdgedBool const &getValue() const;
  bool evaluateAt(Diagram2D &diagram, int tick);
  bool hasChanged() const;

  void dump(Diagram2D const &diagram) const;

  void RebuildWithChar(Diagram2D const &diagram, Plate<char> &plate) const;
  void RebuildWithEnum
      (Diagram2D const &diagram, Plate<Voxel> &plate, size_t scaleBy = 4) const;

 private:
  bool findHeadLabel(Diagram2D const &diagram);
  bool findTailLabel(Diagram2D const &diagram);
  bool findLabel(Diagram2D const &diagram, P2D pStart, Direction oWard);

  string formExpression(SetOfRod2Ds &seenAlready);
  bool hasExpression() const { return !expression.empty(); }

  Label label;
  string id;
  bool isComplete;
  bool isALockRod;
  P2D headAt;
  P2D tailAt;
  Direction direction;
  int tickFirstSet;
  int countOfValidInputs;
  SetOfP2Ds shared;
  SetOfP2Ds inputs;
  SetOfP2Ds outputs;
  SetOfP2Ds debugOutputs;
  set<RodConnection> connectedTo[eoRodConnectionType];
  string expression;
  EdgedBool lastEvaluatedValue;
};

#endif // ROD2D
