#include "p2d.h"
#include "diagram2d.h"

void SetOfP2Ds::dump(Diagram2D const &diagram) const {
  char const *comma = "{";
  fprintf(stdout, "(%lu)", size());
  for (auto const &p : *this) {
    fprintf(stdout, "%s%s", comma, p.ToString().c_str());
    fprintf(stdout, "(%c)", diagram.at(p));
    comma = ",";
  }
  fprintf(stdout, "}");
}
