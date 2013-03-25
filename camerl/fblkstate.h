#ifndef FBLKSTATE_H
#define FBLKSTATE_H

enum FBlkState {
  fbUnkn,

  fbBlkd,
  fbUnbk,

  eoFBlkState
};

char const *c_str(FBlkState s);

#endif // FBLKSTATE_H
