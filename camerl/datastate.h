#ifndef DATASTATE_H
#define DATASTATE_H

enum DataState {
  dsUnkn,

  dsRset,
  dsSet0,
  dsSetX,
  dsSet1,

  eoDataState
};

char const *c_str(DataState s);

#endif // DATASTATE_H
