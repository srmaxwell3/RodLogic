#include "tickpercycle.h"

char const *c_str(TickPerCycle t) {
  switch (t) {
    case tcEUSL: return "tcEUSL";
    case tcERS1: return "tcERS1";
    case tcERSX: return "tcERSX";
    case tcERS0: return "tcERS0";
    case tcELSL: return "tcELSL";
    case tcEUEL: return "tcEUEL";
    case tcESE0: return "tcESE0";
    case tcESEX: return "tcESEX";
    case tcESE1: return "tcESE1";
    case tcELEL: return "tcELEL";
    case tcSUWL: return "tcSUWL";
    case tcSRW1: return "tcSRW1";
    case tcSRWX: return "tcSRWX";
    case tcSRW0: return "tcSRW0";
    case tcSLWL: return "tcSLWL";
    case tcSUSL: return "tcSUSL";
    case tcSSS0: return "tcSSS0";
    case tcSSSX: return "tcSSSX";
    case tcSSS1: return "tcSSS1";
    case tcSLSL: return "tcSLSL";
    case tcDUWL: return "tcDUWL";
    case tcDRW1: return "tcDRW1";
    case tcDRWX: return "tcDRWX";
    case tcDRW0: return "tcDRW0";
    case tcDLWL: return "tcDLWL";
    case tcDUSL: return "tcDUSL";
    case tcDSS0: return "tcDSS0";
    case tcDSSX: return "tcDSSX";
    case tcDSS1: return "tcDSS1";
    case tcDLSL: return "tcDLSL";
    case tcWUNL: return "tcWUNL";
    case tcWRN1: return "tcWRN1";
    case tcWRNX: return "tcWRNX";
    case tcWRN0: return "tcWRN0";
    case tcWLNL: return "tcWLNL";
    case tcWUWL: return "tcWUWL";
    case tcWSW0: return "tcWSW0";
    case tcWSWX: return "tcWSWX";
    case tcWSW1: return "tcWSW1";
    case tcWLWL: return "tcWLWL";
    case tcNUEL: return "tcNUEL";
    case tcNRE1: return "tcNRE1";
    case tcNREX: return "tcNREX";
    case tcNRE0: return "tcNRE0";
    case tcNLEL: return "tcNLEL";
    case tcNUNL: return "tcNUNL";
    case tcNSN0: return "tcNSN0";
    case tcNSNX: return "tcNSNX";
    case tcNSN1: return "tcNSN1";
    case tcNLNL: return "tcNLNL";
    case tcUUEL: return "tcUUEL";
    case tcURE1: return "tcURE1";
    case tcUREX: return "tcUREX";
    case tcURE0: return "tcURE0";
    case tcULEL: return "tcULEL";
    case tcUUNL: return "tcUUNL";
    case tcUSN0: return "tcUSN0";
    case tcUSNX: return "tcUSNX";
    case tcUSN1: return "tcUSN1";
    case tcULNL: return "tcULNL";
    case eoTickPerCycle:
      return "eoTickPerCycle";
  }
  return "TickPerCycle(?)";
}

TickPerCycle const tickPerCycle[eoTickPerCycle] = {
  tcEUSL,tcERS1,tcERSX,tcERS0,tcELSL,tcEUEL,tcESE0,tcESEX,tcESE1,tcELEL,
  tcSUWL,tcSRW1,tcSRWX,tcSRW0,tcSLWL,tcSUSL,tcSSS0,tcSSSX,tcSSS1,tcSLSL,
  tcDUWL,tcDRW1,tcDRWX,tcDRW0,tcDLWL,tcDUSL,tcDSS0,tcDSSX,tcDSS1,tcDLSL,
  tcWUNL,tcWRN1,tcWRNX,tcWRN0,tcWLNL,tcWUWL,tcWSW0,tcWSWX,tcWSW1,tcWLWL,
  tcNUEL,tcNRE1,tcNREX,tcNRE0,tcNLEL,tcNUNL,tcNSN0,tcNSNX,tcNSN1,tcNLNL,
  tcUUEL,tcURE1,tcUREX,tcURE0,tcULEL,tcUUNL,tcUSN0,tcUSNX,tcUSN1,tcULNL
};
