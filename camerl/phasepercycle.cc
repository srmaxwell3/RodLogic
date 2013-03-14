#include "phasepercycle.h"

char const *toConstCharPointer(PhasePerCycle t) {
  switch (t) {
    case E: return "E";
    case S: return "S";
    case W: return "W";
    case N: return "N";
    case eoPhasePerCycle:
      return "eoPhasePerCycle";
  }
  return "PhasePerCycle(?)";
}

PhaseProperties phaseProperties[eoPhasePerCycle] = {
  { E, W, S, N },  // E
  { S, N, D, E },  // S
  { D, U, W, S },  // D
  { W, E, N, D },  // W
  { N, S, U, W },  // N
  { U, D, E, N }   // U
};
