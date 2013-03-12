#ifndef RBLKSTATE_H
#define RBLKSTATE_H

enum RBlkState {
  RBUnkn,

  RBBlkd,
  RBUnbk,

  eoRBlkState
};

char const *toConstCharPointer(RBlkState s);

#endif // RBLKSTATE_H
