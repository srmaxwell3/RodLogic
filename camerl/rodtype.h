#ifndef RODTYPE_H
#define RODTYPE_H

#include "voxeltype.h"
#include "tickpercycle.h"
#include "direction.h"

// L[ock]{E,S,W,N}
// D[ata]{E,S,W,N}

enum RodType {
  rtUn,

  rtLE, rtLS, rtLD, rtLW, rtLN, rtLU,
  rtDE, rtDS, rtDD, rtDW, rtDN, rtDU,

  eoRodType
};

char const *c_str(RodType t);

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
