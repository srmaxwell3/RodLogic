#include "datastate.h"

char const *toConstCharPointer(DataState s) {
  switch (s) {
    case DSUnkn: return "DSUnkn";
    case DSRset: return "DSRset";
    case DSSet0: return "DSSet0";
    case DSSet1: return "DSSet1";
    case eoDataState:
      return "eoDataState";
  }
  return "DataState(?)";
}
