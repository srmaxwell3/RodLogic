#ifndef TICKPERCYCLEPROPERTIES_H
#define TICKPERCYCLEPROPERTIES_H

#include "datastate.h"
#include "fwdorbwd.h"
#include "lockstate.h"
#include "rodtype.h"

struct TickPerCycleProperties {
  RodType rodType;
  FwdOrBwd fwdOrBwd;
  LockState moveFromLockState;
  DataState moveFromDataState;
  bool mustBeFreeToMove;
};

extern TickPerCycleProperties tickPerCycleProperties[eoTickPerCycle];

#endif // TICKPERCYCLEPROPERTIES_H
