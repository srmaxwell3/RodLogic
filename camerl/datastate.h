#ifndef DATASTATE_H
#define DATASTATE_H

enum DataState {
  DSUnkn,

  DSRset,
  DSSet0,
  DSSet1,

  eoDataState
};

char const *toConstCharPointer(DataState s);

#endif // DATASTATE_H
