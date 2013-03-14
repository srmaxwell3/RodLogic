#ifndef RODTYPE_H
#define RODTYPE_H

#include "voxeltype.h"
#include "tickpercycle.h"
#include "direction.h"

// L[ock]{E,S,W,N}
// D[ata]{E,S,W,N}

enum RodType {
  RTUn,

  RTLE, RTLS, RTLW, RTLN,
  RTDE, RTDS, RTDW, RTDN,

  eoRodType
};

char const *toConstCharPointer(RodType t);

struct RodTypeProperties {
  VoxelType voxelType;
  TickPerCycle activeDuring[4];
  Direction fwd;
  Direction bwd;
  Direction lwd;
  Direction rwd;
};

extern RodTypeProperties rodTypeProperties[eoRodType];

#endif // RODTYPE_H
