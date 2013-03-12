#ifndef FBLKSTATE_H
#define FBLKSTATE_H

enum FBlkState {
  FBUnkn,

  FBBlkd,
  FBUnbk,

  eoFBlkState
};

char const *toConstCharPointer(FBlkState s);

#endif // FBLKSTATE_H
