#include "datatype.h"

char const *toConstCharPointer(DataType t) {
  switch (t) {
    case dtUnkn: return "dtUnkn";
    case dtBody: return "dtBody";
    case dtHead: return "dtHead";
    case dtTail: return "dtTail";
    case dtPost: return "dtPost";
    case dtSlot: return "dtSlot";
    case dtLock: return "dtLock";
    case dtTest: return "dtTest";
    case dtGate: return "dtGate";
      // case dtJoin: return "dtJoin";
    case dtInpt: return "dtInpt";
    case dtOutp: return "dtOutp";
    case eoDataType:
      return "eoDataType";
  }
  return "DataType(?)";
}
