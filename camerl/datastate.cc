#include "datastate.h"

char const *toConstCharPointer(DataState s) {
  switch (s) {
    case dsUnkn: return "dsUnkn";
    case dsRset: return "dsRset";
    case dsSet0: return "dsSet0";
    case dsSetX: return "dsSetX";
    case dsSet1: return "dsSet1";
    case eoDataState:
      return "eoDataState";
  }
  return "DataState(?)";
}
