#ifndef FBLKSTATE_H
#define FBLKSTATE_H

enum FBlkState {
  fbUnkn,

  fbBlkd,
  fbUnbk,

  eoFBlkState
};

char const *toConstCharPointer(FBlkState s);

#endif // FBLKSTATE_H
