#ifndef RBLKSTATE_H
#define RBLKSTATE_H

enum RBlkState {
  rbUnkn,

  rbBlkd,
  rbUnbk,

  eoRBlkState
};

char const *c_str(RBlkState s);

#endif // RBLKSTATE_H
