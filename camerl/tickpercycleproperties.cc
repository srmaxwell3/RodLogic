#include "tickpercycleproperties.h"

TickPerCycleProperties tickPerCycleProperties[eoTickPerCycle] = {
  { rtLW, Fwd, lsLckd, dsUnkn, true  }, // tcEUSL
  { rtDS, Bwd, lsUnkn, dsSet1, true  }, // tcERS1
  { rtDS, Bwd, lsUnkn, dsSetX, true  }, // tcERSX
  { rtDS, Bwd, lsUnkn, dsSet0, true  }, // tcERS0
  { rtLW, Bwd, lsUnlk, dsUnkn, true  }, // tcELSL
  { rtLS, Fwd, lsLckd, dsUnkn, true  }, // tcEUEL
  { rtDE, Fwd, lsUnkn, dsRset, true  }, // tcESE0
  { rtDE, Fwd, lsUnkn, dsSet0, true  }, // tcESEX
  { rtDE, Fwd, lsUnkn, dsSetX, false }, // tcESE1
  { rtLS, Bwd, lsUnlk, dsUnkn, true  }, // tcELEL
  { rtLN, Fwd, lsLckd, dsUnkn, true  }, // tcSUWL
  { rtDW, Bwd, lsUnkn, dsSet1, true  }, // tcSRW1
  { rtDW, Bwd, lsUnkn, dsSetX, true  }, // tcSRWX
  { rtDW, Bwd, lsUnkn, dsSet0, true  }, // tcSRW0
  { rtLN, Bwd, lsUnlk, dsUnkn, true  }, // tcSLWL
  { rtLW, Fwd, lsLckd, dsUnkn, true  }, // tcSUSL
  { rtDS, Fwd, lsUnkn, dsRset, true  }, // tcSSS0
  { rtDS, Fwd, lsUnkn, dsSet0, true  }, // tcSSSX
  { rtDS, Fwd, lsUnkn, dsSetX, false }, // tcSSS1
  { rtLW, Bwd, lsUnlk, dsUnkn, true  }, // tcSLSL
  { rtUn, Fwd, lsLckd, dsUnkn, true  }, // tcDUWL
  { rtUn, Bwd, lsUnkn, dsSet1, true  }, // tcDRW1
  { rtUn, Bwd, lsUnkn, dsSetX, true  }, // tcDRWX
  { rtUn, Bwd, lsUnkn, dsSet0, true  }, // tcDRW0
  { rtUn, Bwd, lsUnlk, dsUnkn, true  }, // tcDLWL
  { rtUn, Fwd, lsLckd, dsUnkn, true  }, // tcDUSL
  { rtUn, Fwd, lsUnkn, dsRset, true  }, // tcDSSX
  { rtUn, Fwd, lsUnkn, dsSetX, true  }, // tcDSS0
  { rtUn, Fwd, lsUnkn, dsSet0, false }, // tcDSS1
  { rtUn, Bwd, lsUnlk, dsUnkn, true  }, // tcDLSL
  { rtLE, Fwd, lsLckd, dsUnkn, true  }, // tcWUNL
  { rtDN, Bwd, lsUnkn, dsSet1, true  }, // tcWRN1
  { rtDN, Bwd, lsUnkn, dsSetX, true  }, // tcWRNX
  { rtDN, Bwd, lsUnkn, dsSet0, true  }, // tcWRN0
  { rtLE, Bwd, lsUnlk, dsUnkn, true  }, // tcWLNL
  { rtLN, Fwd, lsLckd, dsUnkn, true  }, // tcWUWL
  { rtDW, Fwd, lsUnkn, dsRset, true  }, // tcWSW0
  { rtDW, Fwd, lsUnkn, dsSet0, true  }, // tcWSWX
  { rtDW, Fwd, lsUnkn, dsSetX, false }, // tcWSW1
  { rtLN, Bwd, lsUnlk, dsUnkn, true  }, // tcWLWL
  { rtLS, Fwd, lsLckd, dsUnkn, true  }, // tcNUEL
  { rtDE, Bwd, lsUnkn, dsSet1, true  }, // tcNRE1
  { rtDE, Bwd, lsUnkn, dsSetX, true  }, // tcNREX
  { rtDE, Bwd, lsUnkn, dsSet0, true  }, // tcNRE0
  { rtLS, Bwd, lsUnlk, dsUnkn, true  }, // tcNLEL
  { rtLE, Fwd, lsLckd, dsUnkn, true  }, // tcNUEL
  { rtDN, Fwd, lsUnkn, dsRset, true  }, // tcNSE0
  { rtDN, Fwd, lsUnkn, dsSetX, true  }, // tcNSEX
  { rtDN, Fwd, lsUnkn, dsSet0, false }, // tcNSE1
  { rtLE, Bwd, lsUnlk, dsUnkn, true  }, // tcNLEL
  { rtUn, Fwd, lsLckd, dsUnkn, true  }, // tcUUWL
  { rtUn, Bwd, lsUnkn, dsSet1, true  }, // tcURW1
  { rtUn, Bwd, lsUnkn, dsSetX, true  }, // tcURWX
  { rtUn, Bwd, lsUnkn, dsSet0, true  }, // tcURW0
  { rtUn, Bwd, lsUnlk, dsUnkn, true  }, // tcULWL
  { rtUn, Fwd, lsLckd, dsUnkn, true  }, // tcUUSL
  { rtUn, Fwd, lsUnkn, dsRset, true  }, // tcUSS0
  { rtUn, Fwd, lsUnkn, dsSetX, true  }, // tcUSSX
  { rtUn, Fwd, lsUnkn, dsSet0, false }, // tcUSS1
  { rtUn, Bwd, lsUnlk, dsUnkn, true  }, // tcULSL
};
