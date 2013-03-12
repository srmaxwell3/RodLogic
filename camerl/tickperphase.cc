#include "tickperphase.h"

char const *toConstCharPointer(TickPerPhase t) {
  switch (t) {
    case UnlockNextDataRods: return "UnlockNextDataRods";
    case ResetNextDataRodsFrom1: return "ResetNextDataRodsFrom1";
    case ResetNextDataRodsFrom0: return "ResetNextDataRodsFrom0";
    case RelockNextDataRods: return "RelockNextDataRods";
    case UnlockDataRods: return "UnlockDataRods";
    case SetDataRodsTo0: return "SetDataRodsTo0";
    case SetDataRodsTo1: return "SetDataRodsTo1";
    case RelockDataRods: return "RelockDataRods";
    case eoTickPerPhase:
      return "eoTickPerPhase";
  }
  return "TickPerPhase(?)";
}
