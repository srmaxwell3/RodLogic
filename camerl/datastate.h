#ifndef DATASTATE_H
#define DATASTATE_H

#include <string>
using std::string;

enum DataState {
  dsUnkn,

  dsRset,
  dsSet0,
  dsSetX,
  dsSet1,

  eoDataState
};

char const *c_str(DataState s);
DataState toDataState(string const &state);

extern DataState const dataState[eoDataState];

#endif // DATASTATE_H
