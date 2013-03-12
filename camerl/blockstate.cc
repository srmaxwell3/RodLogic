#include "blockstate.h"

char const *toConstCharPointer(BlockState s) {
  switch (s) {
    case ___: return "___";
    case F__: return "F__";
    case _R_: return "_R_";
    case FR_: return "FR_";
    case __L: return "__L";
    case F_L: return "F_L";
    case _RL: return "_RL";
    case FRL: return "FRL";
    case eoBlockState:
      return "eoBlockState";
  }
  return "BlockState(?)";
}

BlockStateProperties blockStateProperties[eoBlockState] = {
  { false, false, false }, // ___
  { true,  false, false }, // F__
  { false, true,  false }, // _R_
  { true,  true,  false }, // FR_
  { false, false, true  }, // __L
  { true,  false, true  }, // F_L
  { false, true,  true  }, // _RL
  { true,  true,  true  }  // FRL
};
