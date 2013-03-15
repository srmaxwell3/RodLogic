#include "rod2d.h"
#include "diagram2d.h"
#include "label.h"

#include <algorithm>
#include <cassert>

#include "string_trims.h"

extern bool optRodsInitialValue;
extern bool optShowEvaluatingRods;
extern bool optWarnings;

Rod2D::Rod2D(Diagram2D &diagram, P2D const &pStart, Directions d) :
    SetOfP2Ds(),
    label(),
    id(),
    isComplete(false),
    headAt(),
    tailAt(),
    direction(d),
    tickFirstSet(-1),
    lastEvaluatedTick(-1),
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
  if (findTailLabel(diagram) || findHeadLabel(diagram)) {
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
  if (hasInputs() && hasOutputs() && !label.IsDefined()) {
    label = rodsId();
    fprintf(stderr,
            "cam.r: Warning: %s has inputs/outputs, but no label!\n",
            label.ToString().c_str()
           );
    fprintf(stdout,
            "cam.r: Warning: %s has inputs/outputs, but no label!\n",
            label.ToString().c_str()
           );
  }
}

Rod2D::Rod2D() :
    SetOfP2Ds(),
    isComplete(false),
    label(),
    id(),
    shared(),
    direction(eoDirections),
    tickFirstSet(-1),
    lastEvaluatedTick(-1),
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
  // verifyInputDelays(seenAlready);
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
      r->verifyInputDelays();
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
      r->verifyInputDelays();
    }
  }
}

void Rod2D::connectWith(Rod2D *that, RodIntersectionType intersectionType) {
  assert(intersectionType != riNone);

  RodConnectionType toThat = rodConnectionThisToThat[direction][that->direction];
  assert(toThat != rcNone || intersectionType == riCrossing);
  connectedTo[toThat].insert(RodConnection(that, intersectionType));

  RodConnectionType toThis = rodConnectionThisToThat[that->direction][direction];
  assert(toThis != rcNone || intersectionType == riCrossing);
  that->connectedTo[toThis].insert(RodConnection(this, intersectionType));
}

bool Rod2D::isShared(P2D const &p) const {
  return shared.find(p) != shared.end();
}

bool Rod2D::hasInputs() const { return 0 < inputs.size(); }
bool Rod2D::hasOutputs() const { return 0 < outputs.size(); }
size_t Rod2D::countOfInputs() const {
  return connectedTo[rcGet1].size() + connectedTo[rcGet2].size();
}
size_t Rod2D::countOfOutputs() const {
  return connectedTo[rcPut1].size() + connectedTo[rcPut2].size();
}

void Rod2D::reset() {
  lastEvaluatedTick = -1;
}

bool Rod2D::getValue() const {
  // if (lastEvaluatedTick < 0) {
  //   return false;
  // }
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

  if (optShowEvaluatingRods) {
    fprintf(stdout, " } -> %1d\n", result);
  }

  lastEvaluatedValue = result;
  lastEvaluatedTick = tick;
  return lastEvaluatedValue;
}

bool Rod2D::hasChanged() const { return lastEvaluatedValue.hasChanged(); }

void Rod2D::dump(Diagram2D const &diagram) const {
  fprintf(stdout, "(Rod2D *)(%p)->", this);
  fprintf(stdout, "{ label=\"%s\"", label.ToString().c_str());
  fprintf(stdout, ", %sisComplete", isComplete ? "" : "!");
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
  fprintf(stdout, ", connectedTo[]={");
  for (auto const t : rodConnectionType) {
    auto const connections = connectedTo[t];
    if (size_t nConnections = connections.size()) {
      fprintf(stdout, " [%s](%lu){", c_str(t), nConnections);
      char const *comma = "";
      for (auto const &c : connections) {
	fprintf(stdout,
                "%s{%p,%s}",
                comma,
                c.rod,
                c.isACrossingConnection() ? "+" : c.isAnIdentityConnection() ? "1" : "0"
               );
	comma = ",";
      }
      fprintf(stdout, "}");
    }
  }
  fprintf(stdout, "}");
  fprintf(stdout, "}");
  fprintf(stdout, "\n");
}
