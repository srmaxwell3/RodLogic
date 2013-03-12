#ifndef DATATYPE_H
#define DATATYPE_H

enum DataType {
  DTUnkn,

  DTBody,
  DTHead,
  DTTail,
  DTPost,
  DTSlot,
  DTLock,
  DTTest,
  DTGate,
  DTJoin,
  DTInpt,
  DTOutp,

  eoDataType
};

char const *toConstCharPointer(DataType t);

#endif // DATATYPE_H
