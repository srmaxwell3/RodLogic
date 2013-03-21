#ifndef ROD_CONNECTION_TYPE_H
#define ROD_CONNECTION_TYPE_H

enum RodConnectionType {
  Get2,
  Get1,
  Put1,
  Put2,
  Lock,
  None,

  eoRodConnectionType
};

extern RodConnectionType const rodConnectionType[eoRodConnectionType];
extern RodConnectionType const rodConnectionThisToThat[eoDirections][eoDirections];

char const *c_str(RodConnectionType t);

#endif // ROD_CONNECTION_TYPE_H
