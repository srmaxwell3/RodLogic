#include "datatype.h"

char const *toConstCharPointer(DataType t) {
  switch (t) {
    case DTUnkn: return "DTUnkn";
    case DTBody: return "DTBody";
    case DTHead: return "DTHead";
    case DTTail: return "DTTail";
    case DTPost: return "DTPost";
    case DTSlot: return "DTSlot";
    case DTLock: return "DTLock";
    case DTTest: return "DTTest";
    case DTGate: return "DTGate";
    case DTJoin: return "DTJoin";
    case DTInpt: return "DTInpt";
    case DTOutp: return "DTOutp";
    case eoDataType:
      return "eoDataType";
  }
  return "DataType(?)";
}
