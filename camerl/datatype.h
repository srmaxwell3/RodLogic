#ifndef DATATYPE_H
#define DATATYPE_H

enum DataType {
  dtUnkn,

  dtBody,
  dtHead,
  dtTail,
  dtPost,
  dtSlot,
  dtLock,
  dtTest,
  dtGate,
  // dtJoin,
  dtIPut,
  dtOPut,
  dtDPut,

  eoDataType
};

char const *c_str(DataType t);

#endif // DATATYPE_H
