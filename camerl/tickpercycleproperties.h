#ifndef TICKPERCYCLEPROPERTIES_H
#define TICKPERCYCLEPROPERTIES_H

#include "rodtype.h"
#include "fwdorbwd.h"

struct TickPerCycleProperties {
  RodType rodType;
  FwdOrBwd fwdOrBwd;
};

extern TickPerCycleProperties tickPerCycleProperties[eoTickPerCycle];

#endif // TICKPERCYCLEPROPERTIES_H
