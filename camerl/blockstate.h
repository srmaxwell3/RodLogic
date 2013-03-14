#ifndef BLOCKSTATE_H
#define BLOCKSTATE_H

enum BlockState {
  ___,  // FBUnbk, RBUnbk, LSUnlk
  F__,  // FBBlkd, RBUnbk, LSUnlk
  _R_,  // FBUnbk, RBBlkd, LSUnlk
  FR_,  // FBBlkd, RBBlkd, LSUnlk
  __L,  // FBUnbk, RBUnbk, LSLckd
  F_L,  // FBBlkd, RBUnbk, LSLckd
  _RL,  // FBUnbk, RBBlkd, LSLckd
  FRL,  // FBBlkd, RBBlkd, LSLckd

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
