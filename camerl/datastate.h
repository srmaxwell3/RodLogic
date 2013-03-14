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

char const *toConstCharPointer(DataState s);

#endif // DATASTATE_H
