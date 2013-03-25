#ifndef ROD_CONNECTION_TYPE_H
#define ROD_CONNECTION_TYPE_H

#include "direction.h"

enum RodIntersectionType {
  riNone,
  riCrossing,
  riComplement,
  riIdentity,
  riLocking,

  eoRodIntersectionType
};

extern RodIntersectionType const rodIntersectionType[eoRodIntersectionType];

char const *c_str(RodIntersectionType t);

enum RodConnectionType {
  rcGet2,
  rcGet1,
  rcPut1,
  rcPut2,
  rcNone,

  eoRodConnectionType
};

extern RodConnectionType const rodConnectionType[eoRodConnectionType];
extern RodConnectionType const rodConnectionThisToThat[eoDirection][eoDirection];

char const *c_str(RodConnectionType t);

struct Rod2D;

struct RodConnection {
  RodConnection(Rod2D *_rod, RodIntersectionType _intersectionType) :
      rod(_rod),
      intersectionType(_intersectionType)
  {
  }
  RodConnection() :
      rod(0),
      intersectionType(riNone)
  {
  }
  bool operator<(RodConnection const &that) const {
    return rod < that.rod ||
        (rod == that.rod && intersectionType < that.intersectionType);
  }
  bool isACrossingConnection() const {
    return intersectionType == riCrossing;
  };
  bool isALogicalConnection() const {
    return intersectionType == riComplement || intersectionType == riIdentity;
  };
  bool isAnIdentityConnection() const {
    return intersectionType == riIdentity;
  };
  bool isAComplementConnection() const {
    return intersectionType == riComplement;
  };
  bool isALockingConnection() const {
    return intersectionType == riLocking;
  }

  Rod2D *rod;
  RodIntersectionType intersectionType;
};

#endif // ROD_CONNECTION_TYPE_H
