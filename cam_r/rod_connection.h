#ifndef ROD_CONNECTION_TYPE_H
#define ROD_CONNECTION_TYPE_H

#include "directions.h"

enum RodConnectionType {
  Get2,
  Get1,
  Put1,
  Put2,
  None,

  eoRodConnectionType
};

extern RodConnectionType const rodConnectionType[eoRodConnectionType];
extern RodConnectionType const rodConnectionThisToThat[eoDirections][eoDirections];

char const *c_str(RodConnectionType t);

struct Rod2D;

struct RodConnection {
  RodConnection(Rod2D *_rod, bool _isAnIdentityConnection) :
      rod(_rod),
      isAnIdentityConnection(_isAnIdentityConnection)
  {
  }
  RodConnection() :
      rod(0),
      isAnIdentityConnection(false)
  {
  }
  bool operator<(RodConnection const &that) const {
    return rod < that.rod ||
        (rod == that.rod && isAnIdentityConnection < that.isAnIdentityConnection);
  }

  Rod2D *rod;
  bool isAnIdentityConnection;
};

#endif // ROD_CONNECTION_TYPE_H
