#include "rod2d.h"
#include "diagram2d.h"
#include "label.h"

#include <algorithm>
#include <cassert>
#include <sstream>
using std::ostringstream;

#include "string_trims.h"

extern bool optRodsInitialValue;
extern bool optShowEvaluatingRods;
extern bool optWarnings;

Rod2D::Rod2D(Diagram2D &diagram, P2D const &pStart, Directions d) :
    SetOfP2Ds(),
    label(),
    id(),
    isComplete(false),
    isALockRod(false),
    headAt(),
    tailAt(),
    direction(d),
    expression(),
    tickFirstSet(-1),
    lastEvaluatedValue(optRodsInitialValue)
{
  char pStartChar = diagram.at(pStart);
  bool sawHeadOrTailChar = false;
  switch (pStartChar) {
    case '>':
    case '<':
    case 'v':
    case '^':
      sawHeadOrTailChar = true;
  }

  assert(direction == E || direction == W ||
	 direction == S || direction == N
	 /* || direction == D || direction == U */
	);

  Directions scanDirection =
      (direction == E || direction == W) ? E :
      (direction == S || direction == N) ? S :
      // (direction == D || direction == U) ? D :
      eoDirections;
  bool sawHead = scanDirection != direction;
  if (sawHead) {
    headAt = pStart;
  } else {
    tailAt = pStart;
  }

  insert(pStart);

  bool reachedTheEnd = false;
  for (P2D p = pStart.offsetBy(scanDirection);
       !reachedTheEnd && diagram.isInBounds(p);
       p.move(scanDirection)
      )
  {
    switch (char c = diagram.at(p)) {
      case '-':
	if (scanDirection == E) {
	  diagram.saw(p);
	  insert(p);
	} else {
	  reachedTheEnd = true;
	}
	break;

      case '|':
	if (scanDirection == E) {
          diagram.rodSharedAt(this, p);
          shared.insert(p);
        }
	diagram.saw(p);
	insert(p);
	break;

      case '.':
	diagram.saw(p);
	insert(p);
	break;

      case '0':
      case '1':
      case 'X':
	diagram.rodSharedAt(this, p);
	shared.insert(p);
	diagram.saw(p);
	insert(p);
	break;

      case 'I':
	inputs.insert(p);
	diagram.saw(p);
	insert(p);
	break;

      case 'O':
	outputs.insert(p);
	diagram.saw(p);
	insert(p);
	break;

      case 'D':
	debugOutputs.insert(p);
	diagram.saw(p);
	insert(p);
	break;

      default:
	if (sawHeadOrTailChar && c == pStartChar) {
	  diagram.saw(p);
	  insert(p);

	  isComplete = true;
	}

	reachedTheEnd = true;
	if (!sawHead) {
	  headAt = p;
	} else {
	  tailAt = p;
	}
	break;
    }
  }

  char buffer[1024];
  char *bPtr = buffer;
  int bCnt = 0;
  if (findHeadLabel(diagram) || findTailLabel(diagram)) {
    sprintf(bPtr, "<\"%s\":%s:%n", label.ToString().c_str(), c_str(direction), &bCnt);
  } else {
    sprintf(bPtr, "<%s:%n", c_str(direction), &bCnt);
  }
  bPtr += bCnt;
  if (direction == E || direction == W) {
    sprintf(bPtr, "%d:%d-%d>", headAt.y, headAt.x, tailAt.x);
  } else if (direction == S || direction == N) {
    sprintf(bPtr, "%d-%d:%d>", headAt.y, tailAt.y, tailAt.x);
  } else {
    sprintf(bPtr, "%d-%d:%d-%d>", headAt.y, headAt.x, tailAt.y, tailAt.x);
  }
  id = buffer;

  if (0 < inputs.size()) {
    if (1 < inputs.size()) {
      fprintf(stderr,
	      "cam.r: Warning: Rod2D(%s) has more than 1 (%lu) inputs!",
	      rodsId().c_str(),
	      inputs.size()
	     );
      fprintf(stdout,
	      "cam.r: Warning: Rod2D(%s) has more than 1 (%lu) inputs!",
	      rodsId().c_str(),
	      inputs.size()
	     );
    }
  }
  if (0 < outputs.size()) {
    if (1 < outputs.size()) {
      fprintf(stderr,
	      "cam.r: Warning: Rod2D(%s) has more than 1 (%lu) outputs!",
	      rodsId().c_str(),
	      inputs.size()
	     );
      fprintf(stdout,
	      "cam.r: Warning: Rod2D(%s) has more than 1 (%lu) outputs!",
	      rodsId().c_str(),
	      inputs.size()
	     );
    }
  }
  if (0 < debugOutputs.size()) {
    if (1 < debugOutputs.size()) {
      fprintf(stderr,
	      "cam.r: Warning: Rod2D(%s) has more than 1 (%lu) debugOutputs!",
	      rodsId().c_str(),
	      inputs.size()
	     );
      fprintf(stdout,
	      "cam.r: Warning: Rod2D(%s) has more than 1 (%lu) debugOutputs!",
	      rodsId().c_str(),
	      inputs.size()
	     );
    }
  }
  if ((hasInputs() || hasOutputs() || hasDebugOutputs()) && !label.IsDefined()) {
    label = rodsId();
    fprintf(stderr,
            "cam.r: Warning: %s has inputs/[debug]outputs, but no label!\n",
            label.ToString().c_str()
           );
    fprintf(stdout,
            "cam.r: Warning: %s has inputs/[debug] outputs, but no label!\n",
            label.ToString().c_str()
           );
  }
}

Rod2D::Rod2D() :
    SetOfP2Ds(),
    isComplete(false),
    isALockRod(false),
    label(),
    id(),
    shared(),
    direction(eoDirections),
    tickFirstSet(-1),
    lastEvaluatedValue(optRodsInitialValue)
{
}

Label const &Rod2D::rodsLabel() const { return label; }

string const &Rod2D::rodsId() const {
  if (id.empty()) {
  }
  return id;
}

bool Rod2D::rodIsIncomplete() const {
  return !isComplete;
}

bool Rod2D::rodIsALockRod() const {
  return !isALockRod;
}

bool Rod2D::rodsDirection() const {
  return direction;
}

int Rod2D::rodsLength() const {
  int dY = headAt.y - tailAt.y;
  int dX = headAt.x - tailAt.x;

  if (dX == 0) {
    return dY < 0 ? -dY : dY;
  }

  return dX < 0 ? -dX : dX;
}

bool Rod2D::findHeadLabel(Diagram2D const &diagram) {
  return findLabel(diagram, headAt, FWard(direction));
}

bool Rod2D::findTailLabel(Diagram2D const &diagram) {
  return findLabel(diagram, tailAt, BWard(direction));
}

bool Rod2D::findLabel(Diagram2D const &diagram, P2D pStart, Directions oWard) {
  Directions lWard = LWard(oWard);
  Directions rWard = RWard(oWard);

  static Directions oWardToTWard[eoDirections][2] = {
    { E, E }, // E
    { E, W }, // S
    { W, E }, // D
    { W, W }, // W
    { W, E }, // N
    { W, E }, // U
  };
  static char oWardToSlash[eoDirections][2] = {
    { '/', '\\' }, // E
    { '\\', '/' }, // S
    { '?',  '?' }, // D
    { '/', '\\' }, // W
    { '\\', '/' }, // N
    { '?',  '?' }, // U
  };

  if (!label.IsDefined()) {
    Directions const &tWard = oWardToTWard[oWard][0];
    char const &slash = oWardToSlash[oWard][0];
    int nLSlashes = 0;
    for (P2D pOl = pStart.offsetBy(oWard, lWard);
         diagram.isInBounds(pOl);
         pOl.move(oWard, lWard)
        )
    {
      char c = diagram.at(pOl);
      if (0 < nLSlashes && c != slash &&
          (c != '/' && c != '\\')
         )
      {
	int nBlanks = 0;
	string l;
	for (P2D t = pOl; diagram.isInBounds(t); t.move(tWard)) {
	  char c = diagram.at(t);
	  if (c != ' ') {
	    if (nBlanks) {
	      l += ' ';
	      nBlanks = 0;
	    }
	    l += c;
	  } else if (nBlanks++) {
	    break;
	  }
	}
	l = trim(l);
	if (tWard == W) {
	  std::reverse(l.begin(), l.end());
	}
	label = l;
	break;
      } else if (c == slash) {
	nLSlashes += 1;
      } else {
	break;
      }
    }
  }

  if (!label.IsDefined()) {
    Directions const &tWard = oWardToTWard[oWard][1];
    char const &slash = oWardToSlash[oWard][1];
    int nRSlashes = 0;
    for (P2D pOr = pStart.offsetBy(oWard, rWard);
         diagram.isInBounds(pOr);
         pOr.move(oWard, rWard)
        )
    {
      char c = diagram.at(pOr);
      if (0 < nRSlashes && c != slash &&
          (c != '/' && c != '\\')
         )
      {
	int nBlanks = 0;
	string l;
	for (P2D t = pOr; diagram.isInBounds(t); t.move(tWard)) {
	  char c = diagram.at(t);
	  if (c != ' ') {
	    if (nBlanks) {
	      l += ' ';
	      nBlanks = 0;
	    }
	    l += c;
	  } else if (nBlanks++) {
	    break;
	  }
	}
	l = trim(l);
	if (tWard == W) {
	  std::reverse(l.begin(), l.end());
	}
	label = l;
	break;
      } else if (c == slash) {
	nRSlashes += 1;
      } else {
	break;
      }
    }
  }

  if (!label.IsDefined()) {
    if (direction == E || direction == W) {
      Directions const tWard = oWard;
      int nBlanks = 0;
      string l;
      P2D t;
      for (t = pStart.offsetBy(oWard);
           nBlanks < 2 && diagram.isInBounds(t) && diagram.at(t) == ' ';
           t.move(tWard)
           )
      {
        nBlanks += 1;
      }
      if (diagram.isInBounds(t)) {
        char c = diagram.at(t);
        if (nBlanks < 2 &&
            (c != '/' && c != '\\') &&
            (c == '0' || c == '1' || c == 'O' || c == 'I' || !Diagram2D::isLegalEWChar(c))
            )
        {
          for (; diagram.isInBounds(t); t.move(tWard)) {
            c = diagram.at(t);
            if (c != ' ') {
              if (nBlanks) {
                l += ' ';
                nBlanks = 0;
              }
              l += c;
            } else if (nBlanks++) {
              break;
            }
          }
          l = trim(l);
          if (tWard == W) {
            std::reverse(l.begin(), l.end());
          }
          label = l;
        }
      }
    } else {
      Directions const tWard = oWard;
      int nBlanks = 0;
      string l;
      P2D t;
      for (t = pStart.offsetBy(oWard);
           nBlanks < 2 && diagram.isInBounds(t) && diagram.at(t) == ' ';
           t.move(tWard)
           )
      {
        nBlanks += 1;
      }
      if (diagram.isInBounds(t)) {
        char c = diagram.at(t);
        if (nBlanks < 2 &&
            (c != '/' && c != '\\') &&
            (c == '0' || c == '1' || c == 'O' || c == 'I' || !Diagram2D::isLegalEWChar(c))
            )
        {
          for (; diagram.isInBounds(t); t.move(tWard)) {
            c = diagram.at(t);
            if (c != ' ') {
              if (nBlanks) {
                l += ' ';
                nBlanks = 0;
              }
              l += c;
            } else if (nBlanks++) {
              break;
            }
          }
          l = trim(l);
          if (tWard == N) {
            std::reverse(l.begin(), l.end());
          }
          label = l;
        }
      }
    }
  }
  return label.IsDefined();
}

void Rod2D::setFirstTick (int t) {
  if (tickFirstSet != -1) {
    if (tickFirstSet != t && optWarnings) {
      fprintf(stderr,
	      "cam.r: Warning: Rod2D(%s): First tick was set to %d, now to %d?\n",
	      rodsId().c_str(),
	      tickFirstSet,
	      t
	     );
      fprintf(stdout,
	      "cam.r: Warning: Rod2D(%s): First tick was set to %d, now to %d?\n",
	      rodsId().c_str(),
	      tickFirstSet,
	      t
	     );
    }
    return;
  }

  tickFirstSet = t;
  for (auto const &c : connectedTo[rcPut1]) {
    if (Rod2D *r = c.rod) {
      r->setFirstTick(t + 1);
    }
  }
  for (auto const &c : connectedTo[rcPut2]) {
    if (Rod2D *r = c.rod) {
      r->setFirstTick(t + 2);
    }
  }
}

int Rod2D::tickFirstSetAt () const {
  return tickFirstSet;
}

void Rod2D::verifyInputDelays() {
  SetOfRod2Ds seenAlready;
  verifyInputDelays(seenAlready);
}

void Rod2D::verifyInputDelays(SetOfRod2Ds &seenAlready) {
  if (seenAlready.find(this) != seenAlready.end()) {
    return;
  }
  seenAlready.insert(this);

  countOfValidInputs = 0;

  if (tickFirstSet == -1 && optWarnings) {
    fprintf(stderr,
            "cam.r: Warning: Rod2D(%s): First tick never set!\n",
            rodsId().c_str()
           );
    fprintf(stdout,
            "cam.r: Warning: Rod2D(%s): First tick never set!\n",
            rodsId().c_str()
           );
  }

  for (auto const &c : connectedTo[rcGet2]) {
    if (Rod2D *r = c.rod) {
      if (r->tickFirstSetAt() != (tickFirstSet - 2)) {
	if (optWarnings) {
	  fprintf(stderr,
		  "cam.r: Warning: Rod2D(%s): "
                  "First tick was set to %d, but input from Rod2D(%s) was set to %d?\n",
		  rodsId().c_str(),
		  tickFirstSet,
		  r->rodsId().c_str(),
		  r->tickFirstSet
		  );
	  fprintf(stdout,
		  "cam.r: Warning: Rod2D(%s): "
                  "First tick was set to %d, but input from Rod2D(%s) was set to %d?\n",
		  rodsId().c_str(),
		  tickFirstSet,
		  r->rodsId().c_str(),
		  r->tickFirstSet
		  );
	}
	continue;
      }
      countOfValidInputs += 1;
      r->verifyInputDelays(seenAlready);
    }
  }
  for (auto const &c : connectedTo[rcGet1]) {
    if (Rod2D *r = c.rod) {
      if (r->tickFirstSetAt() != (tickFirstSet - 1)) {
	if (optWarnings) {
	  fprintf(stderr,
		  "cam.r: Warning: Rod2D(%s): "
                  "First tick was set to %d, but input from Rod2D(%s) was set to %d?\n",
		  rodsId().c_str(),
		  tickFirstSet,
		  r->rodsId().c_str(),
		  r->tickFirstSet
		  );
	  fprintf(stdout,
		  "cam.r: Warning: Rod2D(%s): "
                  "First tick was set to %d, but input from Rod2D(%s) was set to %d?\n",
		  rodsId().c_str(),
		  tickFirstSet,
		  r->rodsId().c_str(),
		  r->tickFirstSet
		  );
	}
	continue;
      }
      countOfValidInputs += 1;
      r->verifyInputDelays(seenAlready);
    }
  }
}

void Rod2D::connectWith(Rod2D *that, RodIntersectionType intersectionType) {
  RodConnectionType toThat = rodConnectionThisToThat[direction][that->direction];
  RodConnectionType toThis = rodConnectionThisToThat[that->direction][direction];

  switch (intersectionType) {
    case riCrossing:
    case riComplement:
    case riIdentity:
    case riLocking:
      assert(toThat != rcNone ||
             intersectionType == riCrossing ||
             intersectionType == riLocking
            );
      assert(toThis != rcNone ||
             intersectionType == riCrossing ||
             intersectionType == riLocking
            );

      connectedTo[toThat].insert(RodConnection(that, intersectionType));
      that->connectedTo[toThis].insert(RodConnection(this, intersectionType));

    case riNone:
      assert(intersectionType != riNone && intersectionType != eoRodIntersectionType);
      break;
  }
}

bool Rod2D::isShared(P2D const &p) const {
  return shared.find(p) != shared.end();
}

bool Rod2D::hasInputs() const { return 0 < inputs.size(); }
bool Rod2D::hasOutputs() const { return 0 < outputs.size(); }
bool Rod2D::hasDebugOutputs() const { return 0 < debugOutputs.size(); }
size_t Rod2D::countOfInputs() const {
  return connectedTo[rcGet1].size() + connectedTo[rcGet2].size();
}
size_t Rod2D::countOfOutputs() const {
  return connectedTo[rcPut1].size() + connectedTo[rcPut2].size();
}

string Rod2D::formExpression(SetOfRod2Ds &seenAlready) {
  fprintf(stderr,
	  "(Rod2D *)(%p)->formExpression(): [%s] %shasExpression()\n",
	  this,
	  rodsId().c_str(),
	  hasExpression() ? "!" : ""
	 );
  if (hasExpression()) {
    return expression;
  }

  fprintf(stderr,
	  "(Rod2D *)(%p)->formExpression(): [%s] %sseenAlready\n",
	  this,
	  rodsId().c_str(),
	  seenAlready.find(this) == seenAlready.end() ? "!" : ""
	 );
  if (seenAlready.find(this) == seenAlready.end()) {
    seenAlready.insert(this);

    fprintf(stderr,
	    "(Rod2D *)(%p)->formExpression(): [%s] countOfInputs()=%lu\n",
	    this,
	    rodsId().c_str(),
	    countOfInputs()
	    );
    if (countOfInputs()) {
      vector<string> argExpressions;
      for (auto &rc : connectedTo[rcGet2]) {
	fprintf(stderr,
		"(Rod2D *)(%p)->formExpression(): [%s] get2 rc.intersectionType=%s\n",
		this,
		rodsId().c_str(),
		c_str(rc.intersectionType)
		);
	if (rc.intersectionType == riComplement) {
	  ostringstream result;
	  result << "NOT(" << rc.rod->formExpression(seenAlready) << ")";
	  argExpressions.push_back(result.str());

	  fprintf(stderr,
		  "(Rod2D *)(%p)->formExpression(): [%s] get2 arg=%s\n",
		  this,
		  rodsId().c_str(),
		  argExpressions.back().c_str()
		  );
	} else if (rc.intersectionType == riIdentity) {
	  argExpressions.push_back(rc.rod->formExpression(seenAlready));

	  fprintf(stderr,
		  "(Rod2D *)(%p)->formExpression(): [%s] get2 arg=%s\n",
		  this,
		  rodsId().c_str(),
		  argExpressions.back().c_str()
		  );
	}
      }
      for (auto &rc : connectedTo[rcGet1]) {
	fprintf(stderr,
		"(Rod2D *)(%p)->formExpression(): [%s] get1 rc.intersectionType=%s\n",
		this,
		rodsId().c_str(),
		c_str(rc.intersectionType)
		);
	if (rc.intersectionType == riComplement) {
	  ostringstream result;
	  result << "NOT(" << rc.rod->formExpression(seenAlready) << ")";
	  argExpressions.push_back(result.str());

	  fprintf(stderr,
		  "(Rod2D *)(%p)->formExpression(): [%s] get1 arg=%s\n",
		  this,
		  rodsId().c_str(),
		  argExpressions.back().c_str()
		  );
	} else if (rc.intersectionType == riIdentity) {
	  argExpressions.push_back(rc.rod->formExpression(seenAlready));

	  fprintf(stderr,
		  "(Rod2D *)(%p)->formExpression(): [%s] get1 arg=%s\n",
		  this,
		  rodsId().c_str(),
		  argExpressions.back().c_str()
		  );
	}
      }
      if (!argExpressions.empty()) {
	std::sort(argExpressions.begin(), argExpressions.end());
	ostringstream result;
	result << "AND(" << argExpressions[0];
	for (size_t a = 1; a < argExpressions.size(); a += 1) {
	  result << "," << argExpressions[a];
	}
	result << ")";
	expression = result.str();
      } else {
	expression = !label.IsDefined() ? label.ToString() : rodsId();
      }
    } else {
      expression = !label.IsDefined() ? label.ToString() : rodsId();
    }
  } else {
    expression = !label.IsDefined() ? label.ToString() : rodsId();
  }

  // fprintf(stderr,
  // 	  "(Rod2D *)(%p)->formExpression(): [%s] expression=%s\n",
  // 	  this,
  // 	  rodsId().c_str(),
  // 	  expression.c_str()
  // 	  );
  return expression;
}

string const &Rod2D::formExpression() {
  if (!hasExpression()) {
    SetOfRod2Ds seenAlready;
    expression = formExpression(seenAlready);
  }
  return expression;
}

void Rod2D::DetermineIfALockRod() {
  isALockRod = true;
  for (auto const &rcs : connectedTo) {
    for (auto const &rc : rcs) {
      isALockRod &= (rc.intersectionType == riLocking);
    }
  }
}

void Rod2D::reset() {
  lastEvaluatedValue.reset(optRodsInitialValue);
}

EdgedBool const &Rod2D::getValue() const {
  return lastEvaluatedValue;
}

bool Rod2D::evaluateAt(Diagram2D &diagram, int tick) {
  bool result = true;

  if (optShowEvaluatingRods) {
    fprintf(stdout, "Rod2D(%s)->evaluateAt(diagram, tick=%d): { [Get2]{", rodsId().c_str(), tick);
  }
  char const *comma = "";
  for (auto const &rc : connectedTo[rcGet2]) {
    if (!rc.isALogicalConnection()) {
      continue;
    }

    bool rValue = rc.rod->getValue();
    if (optShowEvaluatingRods) {
      fprintf(stdout,
              "%s %s -> %s%1d",
              comma,
              rc.rod->rodsId().c_str(),
              rc.isAnIdentityConnection() ? "" : "!", rValue
             );
      comma = ",";
    }
    if (!rc.isAnIdentityConnection()) {
      result &= !rValue;
    } else {
      result &= rValue;
    }
  }
  if (optShowEvaluatingRods) {
    fprintf(stdout, "}, [Get1]{");
  }
  comma = "";
  for (auto const &rc : connectedTo[rcGet1]) {
    if (!rc.isALogicalConnection()) {
      continue;
    }

    bool rValue = rc.rod->getValue();
    if (optShowEvaluatingRods) {
      fprintf(stdout,
              "%s %s -> %s%1d",
              comma,
              rc.rod->rodsId().c_str(),
              rc.isAnIdentityConnection() ? "" : "!", rValue
             );
      comma = ",";
    }
    if (!rc.isAnIdentityConnection()) {
      result &= !rValue;
    } else {
      result &= rValue;
    }
  }
  if (optShowEvaluatingRods) {
    fprintf(stdout, "}}");
  }

  if (hasInputs()) {
    if (result) {
      bool value = diagram.readInputFor(label);

      if (optShowEvaluatingRods) {
	fprintf(stdout, ", Input[%s] -> %1d", rodsLabel().ToString().c_str(), value);
      }
      result &= value;
    }
  }

  if (hasOutputs()) {
    diagram.writeOutputFor(label, result);

    if (optShowEvaluatingRods) {
      if (optShowEvaluatingRods) {
	fprintf(stdout, ", %1d -> Output[%s]", result, rodsLabel().ToString().c_str());
      }
    }
  }

  if (hasDebugOutputs()) {
    diagram.writeDebugOutputFor(label, result);

    if (optShowEvaluatingRods) {
      if (optShowEvaluatingRods) {
	fprintf(stdout, ", %1d -> DebugOutput[%s]", result, rodsLabel().ToString().c_str());
      }
    }
  }

  lastEvaluatedValue.updateAt(tick, result);

  if (optShowEvaluatingRods) {
    fprintf(stdout,
            " } -> %1d%s\n",
            result,
            lastEvaluatedValue.hasChanged() ? "*" : ""
           );
  }
  return lastEvaluatedValue;
}

bool Rod2D::hasChanged() const { return lastEvaluatedValue.hasChanged(); }

void Rod2D::dump(Diagram2D const &diagram) const {
  fprintf(stdout, "(Rod2D *)(%p)->", this);
  fprintf(stdout, "{ label=\"%s\"", label.ToString().c_str());
  fprintf(stdout, ", %sisComplete", isComplete ? "" : "!");
  fprintf(stdout, ", %sisALockRod", isALockRod ? "" : "!");
  fprintf(stdout, ", headAt=%s", headAt.ToString().c_str());
  fprintf(stdout, ", tailAt=%s", tailAt.ToString().c_str());
  fprintf(stdout, ", direction=%s", c_str(direction));
  fprintf(stdout, ", tickFirstSet=%d", tickFirstSet);
  fprintf(stdout, ", countOfValidInputs=%d", countOfValidInputs);
  if (int countOfInvalidInputs =
          (connectedTo[rcGet1].size() + connectedTo[rcGet2].size()) -
          countOfValidInputs
     )
  {
    fprintf(stdout, ", countOfInvalidInputs=%d", countOfInvalidInputs);
  }

  fprintf(stdout, ", SetOfP2Ds=");
  SetOfP2Ds::dump(diagram);
  if (0 < shared.size()) {
    fprintf(stdout, ", shared=");
    shared.dump(diagram);
  }
  if (0 < inputs.size()) {
    fprintf(stdout, ", inputs=");
    inputs.dump(diagram);
  }
  if (0 < outputs.size()) {
    fprintf(stdout, ", outputs=");
    outputs.dump(diagram);
  }
  if (0 < debugOutputs.size()) {
    fprintf(stdout, ", debugOutputs=");
    debugOutputs.dump(diagram);
  }
  fprintf(stdout, ", connectedTo[]={");
  for (auto const t : rodConnectionType) {
    auto const connections = connectedTo[t];
    if (size_t nConnections = connections.size()) {
      fprintf(stdout, " [%s](%lu){", c_str(t), nConnections);
      char const *comma = "";
      for (auto const &c : connections) {
        static char const *intersectGraphic[eoRodIntersectionType] = {
          "?", // riNone
          "|", // riCrossing
          "0", // riComplement
          "1", // riIdentity
          "X", // riLocking
        };
	fprintf(stdout,
                "%s{%s,%s}",
                comma,
                c.rod->rodsId().c_str(),
                intersectGraphic[c.intersectionType]
               );
	comma = ",";
      }
      fprintf(stdout, "}");
    }
  }
  fprintf(stdout, "}");
  fprintf(stdout, ", expression=\"%s\"", getExpression().c_str());
  fprintf(stdout, "}");
  fprintf(stdout, "\n");
}

void Rod2D::Rebuild(Diagram2D const &diagram, PlateOfInt &plane) const {
  // static Voxel const voxelFromcCharAndDirection[128 - 32 - 1][eoDirections] = {

  //   //          E
  //   //         /     S
  //   //        /     /     D
  //   //       /     /     /     W
  //   //      /     /     /     /     N
  //   //     /     /     /     /     /     U
  //   //    /     /     /     /     /     /
  //   { Wall, Wall, Wall, Wall, Wall, Wall }, //  32, 040, 20, ' ' , Space
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  33, 041, 21, '!', Exclamation mark
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  34, 042, 22, '"', Double quotes (or speech marks)
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  35, 043, 23, '#', Number
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  36, 044, 24, '$', Dollar
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  37, 045, 25, '%', Procenttecken
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  38, 046, 26, '&', Ampersand
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  39, 047, 27, ''', Single quote
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  40, 050, 28, '(', Open parenthesis (or open bracket)
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  41, 051, 29, ')', Close parenthesis (or close bracket)
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  42, 052, 2A, '*', Asterisk
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  43, 053, 2B, '+', Plus
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  44, 054, 2C, ',', Comma
  //   { DBER, DBSR, DBDR, DBWR, DBNR, DBUR }, //  45, 055, 2D, '-', Hyphen
  //   { DBER, DBSR, DBDR, DBWR, DBNR, DBUR }, //  46, 056, 2E, '.', Period, dot or full stop
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  47, 057, 2F, '/', Slash or divide
  //   { D0ER, D0SR, D0DR, D0WR, D0NR, D0UR }, //  48, 060, 30, '0', Zero
  //   { D1ER, D1SR, D1DR, D1WR, D1NR, D1UR }, //  49, 061, 31, '1', One
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  50, 062, 32, '2', Two
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  51, 063, 33, '3', Three
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  52, 064, 34, '4', Four
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  53, 065, 35, '5', Five
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  54, 066, 36, '6', Six
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  55, 067, 37, '7', Seven
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  56, 070, 38, '8', Eight
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  57, 071, 39, '9', Nine
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  58, 072, 3A, ':', Colon
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  59, 073, 3B, ';', Semicolon
  //   { DQER, DQSR, DQDR, DQWR, DQNR, DQUR }, //  60, 074, 3C, '<', Less than (or open angled bracket)
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  61, 075, 3D, '=', Equals
  //   { DQER, DQSR, DQDR, DQWR, DQNR, DQUR }, //  62, 076, 3E, '>', Greater than (or close angled bracket)
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  63, 077, 3F, '?', Question mark
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  64, 100, 40, '@', At symbol
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  65, 101, 41, 'A', Uppercase A
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  66, 102, 42, 'B', Uppercase B
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  67, 103, 43, 'C', Uppercase C
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  68, 104, 44, 'D', Uppercase D
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  69, 105, 45, 'E', Uppercase E
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  70, 106, 46, 'F', Uppercase F
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  71, 107, 47, 'G', Uppercase G
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  72, 110, 48, 'H', Uppercase H
  //   { DIER, DISR, DIDR, DIWR, DINR, DIUR }, //  73, 111, 49, 'I', Uppercase I
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  74, 112, 4A, 'J', Uppercase J
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  75, 113, 4B, 'K', Uppercase K
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  76, 114, 4C, 'L', Uppercase L
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  77, 115, 4D, 'M', Uppercase M
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  78, 116, 4E, 'N', Uppercase N
  //   { DOER, DOSR, DODR, DOWR, DONR, DOUR }, //  79, 117, 4F, 'O', Uppercase O
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  80, 120, 50, 'P', Uppercase P
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  81, 121, 51, 'Q', Uppercase Q
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  82, 122, 52, 'R', Uppercase R
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  83, 123, 53, 'S', Uppercase S
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  84, 124, 54, 'T', Uppercase T
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  85, 125, 55, 'U', Uppercase U
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  86, 126, 56, 'V', Uppercase V
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  87, 127, 57, 'W', Uppercase W
  //   { DSER, DSSR, DSDR, DSWR, DSNR, DSUR }, //  88, 130, 58, 'X', Uppercase X
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  89, 131, 59, 'Y', Uppercase Y
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  90, 132, 5A, 'Z', Uppercase Z
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  91, 133, 5B, '[', Opening bracket
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  92, 134, 5C, '\', Backslash
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  93, 135, 5D, ']', Closing bracket
  //   { DQER, DQSR, DQDR, DQWR, DQNR, DQUR }, //  94, 136, 5E, '^', Caret - circumflex
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  95, 137, 5F, '_', Underscore
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  96, 140, 60, '`', Grave accent
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  97, 141, 61, 'a', Lowercase a
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  98, 142, 62, 'b', Lowercase b
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, //  99, 143, 63, 'c', Lowercase c
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 100, 144, 64, 'd', Lowercase d
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 101, 145, 65, 'e', Lowercase e
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 102, 146, 66, 'f', Lowercase f
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 103, 147, 67, 'g', Lowercase g
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 104, 150, 68, 'h', Lowercase h
  //   { DIER, DISR, DIDR, DIWR, DINR, DIUR }, // 105, 151, 69, 'i', Lowercase i
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 106, 152, 6A, 'j', Lowercase j
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 107, 153, 6B, 'k', Lowercase k
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 108, 154, 6C, 'l', Lowercase l
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 109, 155, 6D, 'm', Lowercase m
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 110, 156, 6E, 'n', Lowercase n
  //   { DOER, DOSR, DODR, DOWR, DONR, DOUR }, // 111, 157, 6F, 'o', Lowercase o
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 112, 160, 70, 'p', Lowercase p
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 113, 161, 71, 'q', Lowercase q
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 114, 162, 72, 'r', Lowercase r
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 115, 163, 73, 's', Lowercase s
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 116, 164, 74, 't', Lowercase t
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 117, 165, 75, 'u', Lowercase u
  //   { DQER, DQSR, DQDR, DQWR, DQNR, DQUR }, // 118, 166, 76, 'v', Lowercase v
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 119, 167, 77, 'w', Lowercase w
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 120, 170, 78, 'x', Lowercase x
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 121, 171, 79, 'y', Lowercase y
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 122, 172, 7A, 'z', Lowercase z
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 123, 173, 7B, '{', Opening brace
  //   { DBER, DBSR, DBDR, DBWR, DBNR, DBUR }, // 124, 174, 7C, '|', Vertical bar
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }, // 125, 175, 7D, '}', Closing brace
  //   { Unkn, Unkn, Unkn, Unkn, Unkn, Unkn }  // 126, 176, 7E, '~', Equivalency sign - tilde
  // };

  // for (auto const &p : *this) {
  //   Voxel v = voxelFromcCharAndDirection[size_t(diagram.at(p)) - 32 - 1][direction];
  //   VoxelProperties const &vp = voxelProperties[v];
  //   if (vp.dataType == dtTest) {
  //     if (p == headAt) {
  // 	static Voxel const headFromDirection[eoDirections] = { DHER, DHSR, DHDR, DHWR, DHNR, DHUR };
  // 	v = headFromDirection[direction];
  //     } else if (p == tailAt) {
  // 	static Voxel const tailFromDirection[eoDirections] = { DTER, DTSR, DTDR, DTWR, DTNR, DTUR };
  // 	v = tailFromDirection[direction];
  //     }
  //   }
  //   plane[p.y][p.x] = int(v);
  // }
  for (auto const &p : *this) {
    plane[p.y][p.x] = diagram.at(p);
  }
}
