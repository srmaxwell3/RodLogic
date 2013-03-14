#ifndef RBLKSTATE_H
#define RBLKSTATE_H

enum RBlkState {
  rbUnkn,

  rbBlkd,
  rbUnbk,

  eoRBlkState
};

char const *toConstCharPointer(RBlkState s);

#endif // RBLKSTATE_H
