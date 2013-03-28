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

Rod2D::Rod2D(Diagram2D &diagram, P2D const &pStart, Direction d) :
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
      break;
    default:
      sawHeadOrTailChar = false;
      break;
  }

  assert(direction == E || direction == W ||
	 direction == S || direction == N
	 /* || direction == D || direction == U */
	);

  Direction scanDirection =
      (direction == E || direction == W) ? E :
      (direction == S || direction == N) ? S :
      // (direction == D || direction == U) ? D :
      eoDirection;
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
      case 'x':
	diagram.rodSharedAt(this, p);
	shared.insert(p);
	diagram.saw(p);
	insert(p);
	break;

      case 'I':
      case 'i':
	inputs.insert(p);
	diagram.saw(p);
	insert(p);
	break;

      case 'O':
      case 'o':
	outputs.insert(p);
	diagram.saw(p);
	insert(p);
	break;

      case 'D':
      case 'd':
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
	      "rlic: Warning: Rod2D(%s) has more than 1 (%lu) inputs!",
	      rodsId().c_str(),
	      inputs.size()
	     );
      fprintf(stdout,
	      "rlic: Warning: Rod2D(%s) has more than 1 (%lu) inputs!",
	      rodsId().c_str(),
	      inputs.size()
	     );
    }
  }
  if (0 < outputs.size()) {
    if (1 < outputs.size()) {
      fprintf(stderr,
	      "rlic: Warning: Rod2D(%s) has more than 1 (%lu) outputs!",
	      rodsId().c_str(),
	      inputs.size()
	     );
      fprintf(stdout,
	      "rlic: Warning: Rod2D(%s) has more than 1 (%lu) outputs!",
	      rodsId().c_str(),
	      inputs.size()
	     );
    }
  }
  if (0 < debugOutputs.size()) {
    if (1 < debugOutputs.size()) {
      fprintf(stderr,
	      "rlic: Warning: Rod2D(%s) has more than 1 (%lu) debugOutputs!",
	      rodsId().c_str(),
	      inputs.size()
	     );
      fprintf(stdout,
	      "rlic: Warning: Rod2D(%s) has more than 1 (%lu) debugOutputs!",
	      rodsId().c_str(),
	      inputs.size()
	     );
    }
  }
  if ((hasInputs() || hasOutputs() || hasDebugOutputs()) && !label.IsDefined()) {
    label = rodsId();
    fprintf(stderr,
            "rlic: Warning: %s has inputs/[debug]outputs, but no label!\n",
            label.ToString().c_str()
           );
    fprintf(stdout,
            "rlic: Warning: %s has inputs/[debug] outputs, but no label!\n",
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
    direction(eoDirection),
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

bool Rod2D::findLabel(Diagram2D const &diagram, P2D pStart, Direction oWard) {
  Direction lWard = LWard(oWard);
  Direction rWard = RWard(oWard);

  static Direction oWardToTWard[eoDirection][2] = {
    { E, E }, // E
    { E, W }, // S
    { W, E }, // D
    { W, W }, // W
    { W, E }, // N
    { W, E }, // U
  };
  static char oWardToSlash[eoDirection][2] = {
    { '/', '\\' }, // E
    { '\\', '/' }, // S
    { '?',  '?' }, // D
    { '/', '\\' }, // W
    { '\\', '/' }, // N
    { '?',  '?' }, // U
  };

  if (!label.IsDefined()) {
    Direction const &tWard = oWardToTWard[oWard][0];
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
    Direction const &tWard = oWardToTWard[oWard][1];
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
      Direction const tWard = oWard;
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
      Direction const tWard = oWard;
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
	      "rlic: Warning: Rod2D(%s): First tick was set to %d, now to %d?\n",
	      rodsId().c_str(),
	      tickFirstSet,
	      t
	     );
      fprintf(stdout,
	      "rlic: Warning: Rod2D(%s): First tick was set to %d, now to %d?\n",
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
            "rlic: Warning: Rod2D(%s): First tick never set!\n",
            rodsId().c_str()
           );
    fprintf(stdout,
            "rlic: Warning: Rod2D(%s): First tick never set!\n",
            rodsId().c_str()
           );
  }

  for (auto const &c : connectedTo[rcGet2]) {
    if (Rod2D *r = c.rod) {
      if (r->tickFirstSetAt() != (tickFirstSet - 2)) {
	if (optWarnings) {
	  fprintf(stderr,
		  "rlic: Warning: Rod2D(%s): "
                  "First tick was set to %d, but input from Rod2D(%s) was set to %d?\n",
		  rodsId().c_str(),
		  tickFirstSet,
		  r->rodsId().c_str(),
		  r->tickFirstSet
		  );
	  fprintf(stdout,
		  "rlic: Warning: Rod2D(%s): "
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
		  "rlic: Warning: Rod2D(%s): "
                  "First tick was set to %d, but input from Rod2D(%s) was set to %d?\n",
		  rodsId().c_str(),
		  tickFirstSet,
		  r->rodsId().c_str(),
		  r->tickFirstSet
		  );
	  fprintf(stdout,
		  "rlic: Warning: Rod2D(%s): "
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
    assert(intersectionType != riNone);
    break;
  case eoRodIntersectionType:
    assert(intersectionType != eoRodIntersectionType);
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

void Rod2D::RebuildWithChar(Diagram2D const &diagram, Plate<char> &plate) const {
  for (auto const &p : *this) {
    plate[p.y][p.x] = diagram.at(p);
  }
}

void Rod2D::RebuildWithEnum
    (Diagram2D const &diagram,
     Plate<Voxel> &plate,
     size_t scaleBy
    ) const
{
  if (isALockRod) {

    //   LBEL, ...,  LBSL, ...,  LBDL, ...,  LBWL, ...,  LBNL, ...,  LBUL, ...,
    //   LHEL, ...,  LHSL, ...,  LHDL, ...,  LHWL, ...,  LHNL, ...,  LHUL, ...,
    //   LTEL, ...,  LTSL, ...,  LTDL, ...,  LTWL, ...,  LTNL, ...,  LTUL, ...,
    //   lpel, ...,  lpsl, ...,  lpdl, ...,  lpwl, ...,  lpnl, ...,  lpul, ...,
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
    static Voxel const lk[eoDirection] = { LKEL, LKSL, LKDL, LKWL, LKNL, LKUL };

    for (auto const &p : *this) {
      Voxel l = Unkn;
      switch (char c = diagram.at(p)) {
      case '>':
      case 'v':
      case '<':
      case '^':
	if (p == headAt) {
	  l = lh[direction];
	} else if (p == tailAt) {
	  l = lt[direction];
	} else {
	  l = lk[direction];
	}
	break;
      case '-':
      case '|':
	l = lb[direction];
	break;
      default:
	assert(l != Unkn);
      }

      assert(l != Unkn);

      P2D dst(p.y * scaleBy + scaleBy, p.x * scaleBy + scaleBy);
      plate.at(dst) = l;
      if (voxelProperties[l].lockType != ltHead) {
	l = lb[direction];
	for (int i = 1; i < scaleBy; i += 1) {
	  dst.move(FWard(direction));
	  plate.at(dst) = l;
	}
      }
    }
  } else {
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
    static Voxel const ds[eoDirection] = { DSER, DSSR, DSDR, DSWR, DSNR, DSUR };
    static Voxel const dl[eoDirection] = { DLER, DLSR, DLDR, DLWR, DLNR, DLUR };
    static Voxel const dq[eoDirection] = { DQER, DQSR, DQDR, DQWR, DQNR, DQUR };
    static Voxel const d0[eoDirection] = { D0ER, D0SR, D0DR, D0WR, D0NR, D0UR };
    static Voxel const d1[eoDirection] = { D1ER, D1SR, D1DR, D1WR, D1NR, D1UR };
    static Voxel const di[eoDirection] = { DIER, DISR, DIDR, DIWR, DINR, DIUR };
    static Voxel const dO[eoDirection] = { DOER, DOSR, DODR, DOWR, DONR, DOUR };

    static char const htq[eoDirection] = { '>',  'v',  '?',  '<',  '^',  '?'  };

    for (auto const &p : *this) {
      Voxel d = Unkn;
      switch (char c = diagram.at(p)) {
        case '>':
        case 'v':
        case '<':
        case '^':
          if (c == htq[direction]) {
            if (p == headAt) {
              d = dh[direction];
            } else if (p == tailAt) {
              d = dt[direction];
            } else {
              d = dq[direction];
            }
          } else {
            d = ds[direction];
          }
          break;
        case 'X':
        case 'x':
          d = dl[direction];
          break;
        case '-':
        case '|':
        case 'D':
        case 'd':
          d = db[direction];
          break;
        case '0':
          d = d0[direction];
          break;
        case '1':
          d = d1[direction];
          break;
        case 'I':
        case 'i':
          d = di[direction];
          break;
        case 'O':
        case 'o':
          d = dO[direction];
          break;
        default:
          assert(d != Unkn);
      }

      assert(d != Unkn);

      P2D dst(p.y * scaleBy + scaleBy, p.x * scaleBy + scaleBy);
      plate.at(dst) = d;
      if (voxelProperties[d].dataType != dtHead) {
	d = db[direction];
	for (int i = 1; i < scaleBy; i += 1) {
	  dst.move(FWard(direction));
	  plate.at(dst) = d;
	}
      }
    }
  }
}
