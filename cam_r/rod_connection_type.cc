#include "rod_connection_type.h"

RodConnectionType const rodConnectionType[eoRodConnectionType] = {
  Get2, Get1, Put1, Put2, None
};

RodConnectionType const rodConnectionThisToThat[eoDirections][eoDirections] = {
  //  that->direction ==
  //  E     S     D     W     N     U
  { None, Put1, Put2, None, Get2, Get1 }, // this->direction == E
  { Get1, None, Put1, Put2, None, Get2 }, // this->direction == S
  { Get2, Get1, None, Put1, Put2, None }, // this->direction == D
  { None, Get2, Get1, None, Put1, Put2 }, // this->direction == W
  { Put2, None, Get2, Get1, None, Put1 }, // this->direction == N
  { Put1, Put2, None, Get2, Get1, None }, // this->direction == U
};

char const *c_str(RodConnectionType t) {
  switch (t) {
    case Get2: return "Get2";
    case Get1: return "Get1";
    case Put1: return "Put1";
    case Put2: return "Put2";
    case None: return "None";
    case eoRodConnectionType:
      return "eoRodConnectionType";
  }
  return "RodConnectionType(?)";
}
