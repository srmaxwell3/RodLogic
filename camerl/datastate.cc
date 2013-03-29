#include "datastate.h"
#include <string>
using std::string;

char const *c_str(DataState s) {
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

static char const *dataStateSymbols[eoDataState] = {
  "dsUnkn",
  "dsRset",
  "dsSet0",
  "dsSetX",
  "dsSet1"
};

DataState toDataState(string const &state) {
  for (DataState s : dataState) {
    if (state == dataStateSymbols[s]) {
      return s;
    }
  }
  return eoDataState;
}

DataState const dataState[eoDataState] = {
  dsUnkn,
  dsRset,
  dsSet0,
  dsSetX,
  dsSet1
};
