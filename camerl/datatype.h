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
  dtInpt,
  dtOutp,

  eoDataType
};

char const *toConstCharPointer(DataType t);

#endif // DATATYPE_H
