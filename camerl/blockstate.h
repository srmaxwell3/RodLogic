#ifndef BLOCKSTATE_H
#define BLOCKSTATE_H

enum BlockState {
  ___,  // fbUnbk, rbUnbk, lsUnlk
  F__,  // fbBlkd, rbUnbk, lsUnlk
  _R_,  // fbUnbk, rbBlkd, lsUnlk
  FR_,  // fbBlkd, rbBlkd, lsUnlk
  __L,  // fbUnbk, rbUnbk, lsLckd
  F_L,  // fbBlkd, rbUnbk, lsLckd
  _RL,  // fbUnbk, rbBlkd, lsLckd
  FRL,  // fbBlkd, rbBlkd, lsLckd

  eoBlockState
};

char const *toConstCharPointer(BlockState s);

struct BlockStateProperties {
  bool isFBlkable;
  bool isRBlkable;
  bool isLockable;
};

extern BlockStateProperties blockStateProperties[eoBlockState];

#endif // BLOCKSTATE_H
