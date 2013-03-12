#include "p2d.h"
#include "diagram2d.h"

void SetOfP2Ds::dump(Diagram2D const &diagram) const {
  char const *comma = "{";
  fprintf(stdout, "(%lu)", size());
  for (auto const &p : *this) {
    fprintf(stdout, "%sP2D(%d,%d)", comma, p.y, p.x);
    fprintf(stdout, "(%c)", diagram.at(p));
    comma = ",";
  }
  fprintf(stdout, "}");
}
