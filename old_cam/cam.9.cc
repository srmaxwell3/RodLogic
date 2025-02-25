bool optVerbose = false;

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <array>
using std::array;
#include <map>
using std::map;
#include <set>
using std::set;
#include <string>
using std::string;
#include <deque>
using std::deque;
#include <initializer_list>
using std::initializer_list;

// Directions of motion, in order (clockwise).

enum Direction {
  E, S, D, W, N, U,

  eoDirection,
  X = eoDirection
};

char const *toConstCharPointer(Direction d) {
  switch (d) {
    case E: return "E";
    case S: return "S";
    case D: return "D";
    case W: return "W";
    case N: return "N";
    case U: return "U";
    case eoDirection:
      return "eoDirection";
  }
  return "Direction(?)";
}

enum FwdOrBwd {
  Fwd, Bwd,

  eoFwdOrBwd
};

char const *toConstCharPointer(FwdOrBwd d) {
  switch (d) {
    case Fwd: return "Fwd";
    case Bwd: return "Bwd";
    case eoFwdOrBwd:
      return "eoFwdOrBwd";
  }
  return "FwdOrBwd(?)";
}

struct PhaseProperties {
  Direction fwd;  // this major cycle direction forward (identity)
  Direction bwd;  // this major cycle direction backward
  Direction nxt;  // the next major cycle's forward direction
  Direction lst;  // the last major cycle's forward direction
} phaseProperties[eoDirection] = {
  { E, W, S, N },  // E
  { S, N, D, E },  // S
  { D, U, W, S },  // D
  { W, E, N, D },  // W
  { N, S, U, W },  // N
  { U, D, E, N }   // U
};

enum TickPerPhase {
  UnlockNextDataRods,
  ResetNextDataRodsFrom1,
  ResetNextDataRodsFromX,
  ResetNextDataRodsFrom0,
  RelockNextDataRods,

  UnlockDataRods,
  SetDataRodsTo0,
  SetDataRodsToX,
  SetDataRodsTo1,
  RelockDataRods,

  eoTickPerPhase
};

char const *toConstCharPointer(TickPerPhase t) {
  switch (t) {
    case UnlockNextDataRods: return "UnlockNextDataRods";
    case ResetNextDataRodsFrom1: return "ResetNextDataRodsFrom1";
    case ResetNextDataRodsFromX: return "ResetNextDataRodsFromX";
    case ResetNextDataRodsFrom0: return "ResetNextDataRodsFrom0";
    case RelockNextDataRods: return "RelockNextDataRods";
    case UnlockDataRods: return "UnlockDataRods";
    case SetDataRodsTo0: return "SetDataRodsTo0";
    case SetDataRodsToX: return "SetDataRodsToX";
    case SetDataRodsTo1: return "SetDataRodsTo1";
    case RelockDataRods: return "RelockDataRods";
    case eoTickPerPhase:
      return "eoTickPerPhase";
  }
  return "TickPerPhase(?)";
}

// {E,S,W,N}
// {U{S,W,N,E}L,R{S,W,N,E}1,R{S,W,N,E}X,R{S,W,N,E}0,L{S,W,N,E}L,
//  U{E,S,W,N}L,S{E,S,W,N}0,S{E,S,W,N}X,S{E,S,W,N}1,L{E,S,W,N}L
// }

enum TickPerCycle {
  tcEUSL,tcERS1,tcERSX,tcERS0,tcELSL,tcEUEL,tcESE0,tcESEX,tcESE1,tcELEL,
  tcSUWL,tcSRW1,tcSRWX,tcSRW0,tcSLWL,tcSUSL,tcSSS0,tcSSSX,tcSSS1,tcSLSL,
  tcDUWL,tcDRW1,tcDRWX,tcDRW0,tcDLWL,tcDUSL,tcDSS0,tcDSSX,tcDSS1,tcDLSL,
  tcWUNL,tcWRN1,tcWRNX,tcWRN0,tcWLNL,tcWUWL,tcWSW0,tcWSWX,tcWSW1,tcWLWL,
  tcNUEL,tcNRE1,tcNREX,tcNRE0,tcNLEL,tcNUNL,tcNSN0,tcNSNX,tcNSN1,tcNLNL,
  tcUUEL,tcURE1,tcUREX,tcURE0,tcULEL,tcUUNL,tcUSN0,tcUSNX,tcUSN1,tcULNL,

  eoTickPerCycle
};

char const *toConstCharPointer(TickPerCycle t) {
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

size_t const NPhasesPerCycle = size_t(eoDirection);
size_t const NTicksPerPhase = size_t(eoTickPerPhase);
size_t const NTicksPerCycle = size_t(eoTickPerCycle);

class Volume;

class VoxelCoordinant {
 public:
  VoxelCoordinant(int _l, int _r, int _c) : l(_l), r(_r), c(_c) { }
  VoxelCoordinant(VoxelCoordinant const &that) {
    l = that.l;
    r = that.r;
    c = that.c;
  }
  VoxelCoordinant &operator=(VoxelCoordinant const &that) {
    if (this != &that) {
      l = that.l;
      r = that.r;
      c = that.c;
    }
    return *this;
  }
  bool operator==(VoxelCoordinant const &that) const {
    return l == that.l && r == that.r && c == that.c;
  }
  bool operator<(VoxelCoordinant const &that) const {
    return l < that.l || (l == that.l && (r < that.r || (r == that.r && c < that.c)));
  }
  VoxelCoordinant &Move(Direction d) {
    switch (d) {
      case E: return MoveE();
      case S: return MoveS();
      case D: return MoveD();
      case W: return MoveW();
      case N: return MoveN();
      case U: return MoveU();
    default:
      return *this;
    }
    return *this;
  }
  VoxelCoordinant &MoveE() { c += 1; return *this; }
  VoxelCoordinant &MoveS() { r += 1; return *this; }
  VoxelCoordinant &MoveD() { l -= 1; return *this; }
  VoxelCoordinant &MoveW() { c -= 1; return *this; }
  VoxelCoordinant &MoveN() { r -= 1; return *this; }
  VoxelCoordinant &MoveU() { l += 1; return *this; }

  VoxelCoordinant To(Direction d) const {
    switch (d) {
      case E: return ToE();
      case S: return ToS();
      case D: return ToD();
      case W: return ToW();
      case N: return ToN();
      case U: return ToU();
    default:
      return *this;
    }
    return *this;
  }
  VoxelCoordinant ToE() const { return VoxelCoordinant(l, r, c + 1); }
  VoxelCoordinant ToS() const { return VoxelCoordinant(l, r + 1, c); }
  VoxelCoordinant ToD() const { return VoxelCoordinant(l - 1, r, c); }
  VoxelCoordinant ToW() const { return VoxelCoordinant(l, r, c - 1); }
  VoxelCoordinant ToN() const { return VoxelCoordinant(l, r - 1, c); }
  VoxelCoordinant ToU() const { return VoxelCoordinant(l + 1, r, c); }

  VoxelCoordinant &EFrom(VoxelCoordinant const &vc) {
    l = vc.l;
    r = vc.r;
    c = vc.c + 1;
    return *this;
  }
  VoxelCoordinant &SFrom(VoxelCoordinant const &vc) {
    l = vc.l;
    r = vc.r + 1;
    c = vc.c;
    return *this;
  }
  VoxelCoordinant &DFrom(VoxelCoordinant const &vc) {
    l = vc.l - 1;
    r = vc.r;
    c = vc.c;
    return *this;
  }
  VoxelCoordinant &WFrom(VoxelCoordinant const &vc) {
    l = vc.l;
    r = vc.r;
    c = vc.c - 1;
    return *this;
  }
  VoxelCoordinant &NFrom(VoxelCoordinant const &vc) {
    l = vc.l;
    r = vc.r - 1;
    c = vc.c;
    return *this;
  }
  VoxelCoordinant &UFrom(VoxelCoordinant const &vc) {
    l = vc.l + 1;
    r = vc.r;
    c = vc.c;
    return *this;
  }

  int L() const { return l; }
  int R() const { return r; }
  int C() const { return c; }

  void Dump(Volume const *volume = 0) const;

 private:
  int l;
  int r;
  int c;
};

enum FBlkState {
  fbUnkn,

  fbBlkd,
  fbUnbk,

  eoFBlkState
};

char const *toConstCharPointer(FBlkState s) {
  switch (s) {
    case fbUnkn: return "fbUnkn";
    case fbBlkd: return "fbBlkd";
    case fbUnbk: return "fbUnbk";
    case eoFBlkState:
      return "eoFBlkState";
  }
  return "FBlkState(?)";
}

enum RBlkState {
  rbUnkn,

  rbBlkd,
  rbUnbk,

  eoRBlkState
};

char const *toConstCharPointer(RBlkState s) {
  switch (s) {
    case rbUnkn: return "rbUnkn";
    case rbBlkd: return "rbBlkd";
    case rbUnbk: return "rbUnbk";
    case eoRBlkState:
      return "eoRBlkState";
  }
  return "FBlkState(?)";
}

enum VoxelType {
  vtUnkn,

  vtWall,
  vtSlot,
  vtLock,
  vtData,

  eoVoxelType
};

char const *toConstCharPointer(VoxelType t) {
  switch (t) {
    case vtUnkn: return "vtUnkn";
    case vtWall: return "vtWall";
    case vtSlot: return "vtSlot";
    case vtLock: return "vtLock";
    case vtData: return "vtData";
    case eoVoxelType:
      return "eoVoxelType";
  }
  return "VoxelType(?)";
}

enum LockType {
  ltUnkn,

  ltBody,
  ltHead,
  ltTail,
  ltPost,
  ltLock,

  eoLockType
};

char const *toConstCharPointer(LockType t) {
  switch (t) {
    case ltUnkn: return "ltUnkn";
    case ltBody: return "ltBody";
    case ltHead: return "ltHead";
    case ltTail: return "ltTail";
    case ltPost: return "ltPost";
    case ltLock: return "ltLock";
    case eoLockType:
      return "eoLockType";
  }
  return "LockType(?)";
}

enum LockState {
  lsUnkn,

  lsLckd,
  lsUnlk,

  eoLockState
};

char const *toConstCharPointer(LockState t) {
  switch (t) {
    case lsUnkn: return "lsUnkn";
    case lsLckd: return "lsLckd";
    case lsUnlk: return "lsUnlk";
    case eoLockState:
      return "eoLockState";
  }
  return "LockState(?)";
}

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

enum DataState {
  dsUnkn,

  dsRset,
  dsSet0,
  dsSetX,
  dsSet1,

  eoDataState
};

char const *toConstCharPointer(DataState s) {
  switch (s) {
    case dsUnkn: return "dsUnkn";
    case dsRset: return "dsRset";
    case dsSet0: return "dsSet0";
    case dsSetX: return "dsSetX";
    case dsSet1: return "dsSet1";
    case eoDataState:
      return "eoDataState";
  }
  return "DataState(?)";
}

enum BlockState {
  ___,  // fbUnbk, rbUnbk, lsUnlk
  F__,  // fbBlkd, rbUnbk, lsUnlk
  _R_,  // fbUnbk, rbBlkd, lsUnlk
  FR_,  // fbBlkd, rbBlkd, lsUnlk
  __L,  // fbUnbk, rbUnbk, lsLckd
  F_L,  // fbBlkd, rbUnbk, lsLckd
  _RL,  // fbUnbk, rbBlkd, lsLckd
  FRL,  // fbBlkd, rbBlkd, lsLckd

  eoBlockState
};

char const *toConstCharPointer(BlockState s) {
  switch (s) {
    case ___: return "___";
    case F__: return "F__";
    case _R_: return "_R_";
    case FR_: return "FR_";
    case __L: return "__L";
    case F_L: return "F_L";
    case _RL: return "_RL";
    case FRL: return "FRL";
    case eoBlockState:
      return "eoBlockState";
  }
  return "BlockState(?)";
}

struct BlockStateProperties {
  bool isFBlkable;
  bool isRBlkable;
  bool isLockable;
} blockStateProperties[eoBlockState] = {
  { false, false, false }, // ___
  { true,  false, false }, // F__
  { false, true,  false }, // _R_
  { true,  true,  false }, // FR_
  { false, false, true  }, // __L
  { true,  false, true  }, // F_L
  { false, true,  true  }, // _RL
  { true,  true,  true  }  // FRL
};

// L[ock]{E,S,D,W,N,U}
// D[ata]{E,S,D,W,N,U}

enum RodType {
  rtUn,

  rtLE, rtLS, rtLD, rtLW, rtLN, rtLU,
  rtDE, rtDS, rtDD, rtDW, rtDN, rtDU,

  eoRodType
};

char const *toConstCharPointer(RodType t) {
  switch (t) {
    case rtUn: return "rtUn";
    case rtLE: return "rtLE";
    case rtLS: return "rtLS";
    case rtLD: return "rtLD";
    case rtLW: return "rtLW";
    case rtLN: return "rtLN";
    case rtLU: return "rtLU";
    case rtDE: return "rtDE";
    case rtDS: return "rtDS";
    case rtDD: return "rtDD";
    case rtDW: return "rtDW";
    case rtDN: return "rtDN";
    case rtDU: return "rtDU";
  case eoRodType:
      return "eoRodType";
  }
  return "RodType(?)";
}

struct RodTypeProperties {
  VoxelType voxelType;
  TickPerCycle activeDuring[4];
  Direction fwd;
  Direction bwd;
  Direction lwd;
  Direction rwd;
} rodTypeProperties[eoRodType] = {
#undef ______
#define ______ eoTickPerCycle
  { vtUnkn, { ______, ______, ______, ______ }, X, X, X, X }, // rtUn
  { vtLock, { tcWUNL, tcWLNL, tcNUEL, tcNLEL }, E, W, N, S }, // rtLE
  { vtLock, { tcNUEL, tcNLEL, tcEUEL, tcELEL }, S, N, W, E }, // rtLS
  { vtLock, { ______, ______, ______, ______ }, X, X, X, X }, // rtLD
  { vtLock, { tcEUSL, tcELSL, tcSUSL, tcSLSL }, W, E, S, N }, // rtLW
  { vtLock, { tcSUWL, tcSLWL, tcWUWL, tcWLWL }, N, S, E, W }, // rtLN
  { vtLock, { ______, ______, ______, ______ }, X, X, X, X }, // rtLU
  { vtData, { tcNRE1, tcNRE0, tcESE0, tcESE1 }, E, W, N, S }, // rtDE
  { vtData, { tcERS1, tcERS0, tcSSS0, tcSSS1 }, S, N, W, E }, // rtDS
  { vtLock, { ______, ______, ______, ______ }, X, X, X, X }, // rtDD
  { vtData, { tcSRW1, tcSRW0, tcWSW0, tcWSW1 }, W, E, S, N }, // rtDW
  { vtData, { tcWRN1, tcWRN0, tcNSN0, tcNSN1 }, N, S, E, W }, // rtDN
  { vtLock, { ______, ______, ______, ______ }, X, X, X, X }, // rtDU
#undef ______
};

struct TickPerCycleProperties {
  RodType rodType;
  FwdOrBwd fwdOrBwd;
  LockState moveFromLockState;
  DataState moveFromDataState;
  bool mustBeFreeToMove;
} tickPerCycleProperties[eoTickPerCycle] = {
  { rtLW, Fwd, lsLckd, dsUnkn, true  }, // tcEUSL
  { rtDS, Bwd, lsUnkn, dsSet1, true  }, // tcERS1
  { rtDS, Bwd, lsUnkn, dsSetX, true  }, // tcERSX
  { rtDS, Bwd, lsUnkn, dsSet0, true  }, // tcERS0
  { rtLW, Bwd, lsUnlk, dsUnkn, true  }, // tcELSL
  { rtLS, Fwd, lsLckd, dsUnkn, true  }, // tcEUEL
  { rtDE, Fwd, lsUnkn, dsRset, true  }, // tcESE0
  { rtDE, Fwd, lsUnkn, dsSet0, true  }, // tcESEX
  { rtDE, Fwd, lsUnkn, dsSetX, false }, // tcESE1
  { rtLS, Bwd, lsUnlk, dsUnkn, true  }, // tcELEL
  { rtLN, Fwd, lsLckd, dsUnkn, true  }, // tcSUWL
  { rtDW, Bwd, lsUnkn, dsSet1, true  }, // tcSRW1
  { rtDW, Bwd, lsUnkn, dsSetX, true  }, // tcSRWX
  { rtDW, Bwd, lsUnkn, dsSet0, true  }, // tcSRW0
  { rtLN, Bwd, lsUnlk, dsUnkn, true  }, // tcSLWL
  { rtLW, Fwd, lsLckd, dsUnkn, true  }, // tcSUSL
  { rtDS, Fwd, lsUnkn, dsRset, true  }, // tcSSS0
  { rtDS, Fwd, lsUnkn, dsSet0, true  }, // tcSSSX
  { rtDS, Fwd, lsUnkn, dsSetX, false }, // tcSSS1
  { rtLW, Bwd, lsUnlk, dsUnkn, true  }, // tcSLSL
  { rtUn, Fwd, lsLckd, dsUnkn, true  }, // tcDUWL
  { rtUn, Bwd, lsUnkn, dsSet1, true  }, // tcDRW1
  { rtUn, Bwd, lsUnkn, dsSetX, true  }, // tcDRWX
  { rtUn, Bwd, lsUnkn, dsSet0, true  }, // tcDRW0
  { rtUn, Bwd, lsUnlk, dsUnkn, true  }, // tcDLWL
  { rtUn, Fwd, lsLckd, dsUnkn, true  }, // tcDUSL
  { rtUn, Fwd, lsUnkn, dsRset, true  }, // tcDSSX
  { rtUn, Fwd, lsUnkn, dsSetX, true  }, // tcDSS0
  { rtUn, Fwd, lsUnkn, dsSet0, false }, // tcDSS1
  { rtUn, Bwd, lsUnlk, dsUnkn, true  }, // tcDLSL
  { rtLE, Fwd, lsLckd, dsUnkn, true  }, // tcWUNL
  { rtDN, Bwd, lsUnkn, dsSet1, true  }, // tcWRN1
  { rtDN, Bwd, lsUnkn, dsSetX, true  }, // tcWRNX
  { rtDN, Bwd, lsUnkn, dsSet0, true  }, // tcWRN0
  { rtLE, Bwd, lsUnlk, dsUnkn, true  }, // tcWLNL
  { rtLN, Fwd, lsLckd, dsUnkn, true  }, // tcWUWL
  { rtDW, Fwd, lsUnkn, dsRset, true  }, // tcWSW0
  { rtDW, Fwd, lsUnkn, dsSet0, true  }, // tcWSWX
  { rtDW, Fwd, lsUnkn, dsSetX, false }, // tcWSW1
  { rtLN, Bwd, lsUnlk, dsUnkn, true  }, // tcWLWL
  { rtLS, Fwd, lsLckd, dsUnkn, true  }, // tcNUEL
  { rtDE, Bwd, lsUnkn, dsSet1, true  }, // tcNRE1
  { rtDE, Bwd, lsUnkn, dsSetX, true  }, // tcNREX
  { rtDE, Bwd, lsUnkn, dsSet0, true  }, // tcNRE0
  { rtLS, Bwd, lsUnlk, dsUnkn, true  }, // tcNLEL
  { rtLE, Fwd, lsLckd, dsUnkn, true  }, // tcNUEL
  { rtDN, Fwd, lsUnkn, dsRset, true  }, // tcNSE0
  { rtDN, Fwd, lsUnkn, dsSetX, true  }, // tcNSEX
  { rtDN, Fwd, lsUnkn, dsSet0, false }, // tcNSE1
  { rtLE, Bwd, lsUnlk, dsUnkn, true  }, // tcNLEL
  { rtUn, Fwd, lsLckd, dsUnkn, true  }, // tcUUWL
  { rtUn, Bwd, lsUnkn, dsSet1, true  }, // tcURW1
  { rtUn, Bwd, lsUnkn, dsSetX, true  }, // tcURWX
  { rtUn, Bwd, lsUnkn, dsSet0, true  }, // tcURW0
  { rtUn, Bwd, lsUnlk, dsUnkn, true  }, // tcULWL
  { rtUn, Fwd, lsLckd, dsUnkn, true  }, // tcUUSL
  { rtUn, Fwd, lsUnkn, dsRset, true  }, // tcUSS0
  { rtUn, Fwd, lsUnkn, dsSetX, true  }, // tcUSSX
  { rtUn, Fwd, lsUnkn, dsSet0, false }, // tcUSS1
  { rtUn, Bwd, lsUnlk, dsUnkn, true  }, // tcULSL
};

// U[nknown]
// W[all]
// S[lot]({ES,SW,WN,NE}{L[ocked],U[unlocked],R[eset],[Set]0,I[llegal],[Set]1})?
// L[ock]{B[ody},H[ead],T[ail],P[ost],K[ey]}
//       {E,S,D,N,W,U}
//       {L[ocked],U[unlocked]}
// D[ata]{B[ody},H[ead],T[ail],P[ost],S[lot],[Loc]K,T[est],G[ate],J[oin],I[nput],O[utput}}
//       {E,S,D,W,N,U}
//       {R[eset],[Set]0,[Set]1}

enum Voxel {
  Unkn,

  Wall,

  Slot,

  LBEL, LBEU,  LBSL, LBSU,  LBDL, LBDU,  LBWL, LBWU,  LBNL, LBNU,  LBUL, LBUU,
  LHEL, LHEU,  LHSL, LHSU,  LHDL, LHDU,  LHWL, LHWU,  LHNL, LHNU,  LHUL, LHUU,
  LTEL, LTEU,  LTSL, LTSU,  LTDL, LTDU,  LTWL, LTWU,  LTNL, LTNU,  LTUL, LTUU,
  LPEL, LPEU,  LPSL, LPSU,  LPDL, LPDU,  LPWL, LPWU,  LPNL, LPNU,  LPUL, LPUU,
  LKEL, LKEU,  LKSL, LKSU,  LKDL, LKDU,  LKWL, LKWU,  LKNL, LKNU,  LKUL, LKUU,

  DBER, DBE0, DBEX, DBE1,  DBSR, DBS0, DBSX, DBS1,  DBDR, DBD0, DBDX, DBD1,
  DBWR, DBW0, DBWX, DBW1,  DBNR, DBN0, DBNX, DBN1,  DBUR, DBU0, DBUX, DBU1,
  DHER, DHE0, DHEX, DHE1,  DHSR, DHS0, DHSX, DHS1,  DHDR, DHD0, DHDX, DHD1,
  DHWR, DHW0, DHWX, DHW1,  DHNR, DHN0, DHNX, DHN1,  DHUR, DHU0, DHUX, DHU1,
  DTER, DTE0, DTEX, DTE1,  DTSR, DTS0, DTSX, DTS1,  DTDR, DTD0, DTDX, DTD1,
  DTWR, DTW0, DTWX, DTW1,  DTNR, DTN0, DTNX, DTN1,  DTUR, DTU0, DTUX, DTU1,
  DPER, DPE0, DPEX, DPE1,  DPSR, DPS0, DPSX, DPS1,  DPDR, DPD0, DPDX, DPD1,
  DPWR, DPW0, DPWX, DPW1,  DPNR, DPN0, DPNX, DPN1,  DPUR, DPU0, DPUX, DPU1,
  DSER, DSE0, DSEX, DSE1,  DSSR, DSS0, DSSX, DSS1,  DSDR, DSD0, DSDX, DSD1,
  DSWR, DSW0, DSWX, DSW1,  DSNR, DSN0, DSNX, DSN1,  DSUR, DSU0, DSUX, DSU1,
  DQER, DQE0, DQEX, DQE1,  DQSR, DQS0, DQSX, DQS1,  DQDR, DQD0, DQDX, DQD1,
  DQWR, DQW0, DQWX, DQW1,  DQNR, DQN0, DQNX, DQN1,  DQUR, DQU0, DQUX, DQU1,
  DGER, DGE0, DGEX, DGE1,  DGSR, DGS0, DGSX, DGS1,  DGDR, DGD0, DGDX, DGD1,
  DGWR, DGW0, DGWX, DGW1,  DGNR, DGN0, DGNX, DGN1,  DGUR, DGU0, DGUX, DGU1,
  DIER, DIE0, DIEX, DIE1,  DISR, DIS0, DISX, DIS1,  DIDR, DID0, DIDX, DID1,
  DIWR, DIW0, DIWX, DIW1,  DINR, DIN0, DINX, DIN1,  DIUR, DIU0, DIUX, DIU1,
  DOER, DOE0, DOEX, DOE1,  DOSR, DOS0, DOSX, DOS1,  DODR, DOD0, DODX, DOD1,
  DOWR, DOW0, DOWX, DOW1,  DONR, DON0, DONX, DON1,  DOUR, DOU0, DOUX, DOU1,

  eoVoxel
};

char const *toConstCharPointer(Voxel v) {
  switch (v) {
  case Unkn: return "Unkn";
  case Wall: return "Wall";
  case Slot: return "Slot";
  case LBEL: return "LBEL";
  case LBEU: return "LBEU";
  case LBSL: return "LBSL";
  case LBSU: return "LBSU";
  case LBDL: return "LBDL";
  case LBDU: return "LBDU";
  case LBWL: return "LBWL";
  case LBWU: return "LBWU";
  case LBNL: return "LBNL";
  case LBNU: return "LBNU";
  case LBUL: return "LBUL";
  case LBUU: return "LBUU";
  case LHEL: return "LHEL";
  case LHEU: return "LHEU";
  case LHSL: return "LHSL";
  case LHSU: return "LHSU";
  case LHDL: return "LHDL";
  case LHDU: return "LHDU";
  case LHWL: return "LHWL";
  case LHWU: return "LHWU";
  case LHNL: return "LHNL";
  case LHNU: return "LHNU";
  case LHUL: return "LHUL";
  case LHUU: return "LHUU";
  case LTEL: return "LTEL";
  case LTEU: return "LTEU";
  case LTSL: return "LTSL";
  case LTSU: return "LTSU";
  case LTDL: return "LTDL";
  case LTDU: return "LTDU";
  case LTWL: return "LTWL";
  case LTWU: return "LTWU";
  case LTNL: return "LTNL";
  case LTNU: return "LTNU";
  case LTUL: return "LTUL";
  case LTUU: return "LTUU";
  case LPEL: return "LPEL";
  case LPEU: return "LPEU";
  case LPSL: return "LPSL";
  case LPSU: return "LPSU";
  case LPDL: return "LPDL";
  case LPDU: return "LPDU";
  case LPWL: return "LPWL";
  case LPWU: return "LPWU";
  case LPNL: return "LPNL";
  case LPNU: return "LPNU";
  case LPUL: return "LPUL";
  case LPUU: return "LPUU";
  case LKEL: return "LKEL";
  case LKEU: return "LKEU";
  case LKSL: return "LKSL";
  case LKSU: return "LKSU";
  case LKDL: return "LKDL";
  case LKDU: return "LKDU";
  case LKWL: return "LKWL";
  case LKWU: return "LKWU";
  case LKNL: return "LKNL";
  case LKNU: return "LKNU";
  case LKUL: return "LKUL";
  case LKUU: return "LKUU";
  case DBER: return "DBER";
  case DBE0: return "DBE0";
  case DBEX: return "DBEX";
  case DBE1: return "DBE1";
  case DBSR: return "DBSR";
  case DBS0: return "DBS0";
  case DBSX: return "DBSX";
  case DBS1: return "DBS1";
  case DBDR: return "DBDR";
  case DBD0: return "DBD0";
  case DBDX: return "DBDX";
  case DBD1: return "DBD1";
  case DBWR: return "DBWR";
  case DBW0: return "DBW0";
  case DBWX: return "DBWX";
  case DBW1: return "DBW1";
  case DBNR: return "DBNR";
  case DBN0: return "DBN0";
  case DBNX: return "DBNX";
  case DBN1: return "DBN1";
  case DBUR: return "DBUR";
  case DBU0: return "DBU0";
  case DBUX: return "DBUX";
  case DBU1: return "DBU1";
  case DHER: return "DHER";
  case DHE0: return "DHE0";
  case DHEX: return "DHEX";
  case DHE1: return "DHE1";
  case DHSR: return "DHSR";
  case DHS0: return "DHS0";
  case DHSX: return "DHSX";
  case DHS1: return "DHS1";
  case DHDR: return "DHDR";
  case DHD0: return "DHD0";
  case DHDX: return "DHDX";
  case DHD1: return "DHD1";
  case DHWR: return "DHWR";
  case DHW0: return "DHW0";
  case DHWX: return "DHWX";
  case DHW1: return "DHW1";
  case DHNR: return "DHNR";
  case DHN0: return "DHN0";
  case DHNX: return "DHNX";
  case DHN1: return "DHN1";
  case DHUR: return "DHUR";
  case DHU0: return "DHU0";
  case DHUX: return "DHUX";
  case DHU1: return "DHU1";
  case DTER: return "DTER";
  case DTE0: return "DTE0";
  case DTEX: return "DTEX";
  case DTE1: return "DTE1";
  case DTSR: return "DTSR";
  case DTS0: return "DTS0";
  case DTSX: return "DTSX";
  case DTS1: return "DTS1";
  case DTDR: return "DTDR";
  case DTD0: return "DTD0";
  case DTDX: return "DTDX";
  case DTD1: return "DTD1";
  case DTWR: return "DTWR";
  case DTW0: return "DTW0";
  case DTWX: return "DTWX";
  case DTW1: return "DTW1";
  case DTNR: return "DTNR";
  case DTN0: return "DTN0";
  case DTNX: return "DTNX";
  case DTN1: return "DTN1";
  case DTUR: return "DTUR";
  case DTU0: return "DTU0";
  case DTUX: return "DTUX";
  case DTU1: return "DTU1";
  case DPER: return "DPER";
  case DPE0: return "DPE0";
  case DPEX: return "DPEX";
  case DPE1: return "DPE1";
  case DPSR: return "DPSR";
  case DPS0: return "DPS0";
  case DPSX: return "DPSX";
  case DPS1: return "DPS1";
  case DPDR: return "DPDR";
  case DPD0: return "DPD0";
  case DPDX: return "DPDX";
  case DPD1: return "DPD1";
  case DPWR: return "DPWR";
  case DPW0: return "DPW0";
  case DPWX: return "DPWX";
  case DPW1: return "DPW1";
  case DPNR: return "DPNR";
  case DPN0: return "DPN0";
  case DPNX: return "DPNX";
  case DPN1: return "DPN1";
  case DPUR: return "DPUR";
  case DPU0: return "DPU0";
  case DPUX: return "DPUX";
  case DPU1: return "DPU1";
  case DSER: return "DSER";
  case DSE0: return "DSE0";
  case DSEX: return "DSEX";
  case DSE1: return "DSE1";
  case DSSR: return "DSSR";
  case DSS0: return "DSS0";
  case DSSX: return "DSSX";
  case DSS1: return "DSS1";
  case DSDR: return "DSDR";
  case DSD0: return "DSD0";
  case DSDX: return "DSDX";
  case DSD1: return "DSD1";
  case DSWR: return "DSWR";
  case DSW0: return "DSW0";
  case DSWX: return "DSWX";
  case DSW1: return "DSW1";
  case DSNR: return "DSNR";
  case DSN0: return "DSN0";
  case DSNX: return "DSNX";
  case DSN1: return "DSN1";
  case DSUR: return "DSUR";
  case DSU0: return "DSU0";
  case DSUX: return "DSUX";
  case DSU1: return "DSU1";
  case DQER: return "DQER";
  case DQE0: return "DQE0";
  case DQEX: return "DQEX";
  case DQE1: return "DQE1";
  case DQSR: return "DQSR";
  case DQS0: return "DQS0";
  case DQSX: return "DQSX";
  case DQS1: return "DQS1";
  case DQDR: return "DQDR";
  case DQD0: return "DQD0";
  case DQDX: return "DQDX";
  case DQD1: return "DQD1";
  case DQWR: return "DQWR";
  case DQW0: return "DQW0";
  case DQWX: return "DQWX";
  case DQW1: return "DQW1";
  case DQNR: return "DQNR";
  case DQN0: return "DQN0";
  case DQNX: return "DQNX";
  case DQN1: return "DQN1";
  case DQUR: return "DQUR";
  case DQU0: return "DQU0";
  case DQUX: return "DQUX";
  case DQU1: return "DQU1";
  case DGER: return "DGER";
  case DGE0: return "DGE0";
  case DGEX: return "DGEX";
  case DGE1: return "DGE1";
  case DGSR: return "DGSR";
  case DGS0: return "DGS0";
  case DGSX: return "DGSX";
  case DGS1: return "DGS1";
  case DGDR: return "DGDR";
  case DGD0: return "DGD0";
  case DGDX: return "DGDX";
  case DGD1: return "DGD1";
  case DGWR: return "DGWR";
  case DGW0: return "DGW0";
  case DGWX: return "DGWX";
  case DGW1: return "DGW1";
  case DGNR: return "DGNR";
  case DGN0: return "DGN0";
  case DGNX: return "DGNX";
  case DGN1: return "DGN1";
  case DGUR: return "DGUR";
  case DGU0: return "DGU0";
  case DGUX: return "DGUX";
  case DGU1: return "DGU1";
  case DIER: return "DIER";
  case DIE0: return "DIE0";
  case DIEX: return "DIEX";
  case DIE1: return "DIE1";
  case DISR: return "DISR";
  case DIS0: return "DIS0";
  case DISX: return "DISX";
  case DIS1: return "DIS1";
  case DIDR: return "DIDR";
  case DID0: return "DID0";
  case DIDX: return "DIDX";
  case DID1: return "DID1";
  case DIWR: return "DIWR";
  case DIW0: return "DIW0";
  case DIWX: return "DIWX";
  case DIW1: return "DIW1";
  case DINR: return "DINR";
  case DIN0: return "DIN0";
  case DINX: return "DINX";
  case DIN1: return "DIN1";
  case DIUR: return "DIUR";
  case DIU0: return "DIU0";
  case DIUX: return "DIUX";
  case DIU1: return "DIU1";
  case DOER: return "DOER";
  case DOE0: return "DOE0";
  case DOEX: return "DOEX";
  case DOE1: return "DOE1";
  case DOSR: return "DOSR";
  case DOS0: return "DOS0";
  case DOSX: return "DOSX";
  case DOS1: return "DOS1";
  case DODR: return "DODR";
  case DOD0: return "DOD0";
  case DODX: return "DODX";
  case DOD1: return "DOD1";
  case DOWR: return "DOWR";
  case DOW0: return "DOW0";
  case DOWX: return "DOWX";
  case DOW1: return "DOW1";
  case DONR: return "DONR";
  case DON0: return "DON0";
  case DONX: return "DONX";
  case DON1: return "DON1";
  case DOUR: return "DOUR";
  case DOU0: return "DOU0";
  case DOUX: return "DOUX";
  case DOU1: return "DOU1";
  case eoVoxel:
    return "eoVoxel";
  }
  return "Voxel(?)";
}

struct VoxelProperties {
  int displayPriority;
  char const text;
  VoxelType voxelType;
  array<RodType, /* 3 */ 1> rodType;
  LockType lockType;
  LockState lockState;
  DataType dataType;
  DataState dataState;
  bool isRodBody;
  BlockState blockableStates;
  struct Changes {
    Direction direction;
    Voxel nextVoxel;
  } motion[eoFwdOrBwd];

  bool IsOneOf(RodType r) const {
    for (auto const &rt : rodType) {
      if (r == rt) {
        return true;
      }
    }
    return false;
  }
} voxelProperties[eoVoxel] = {
#undef Props
#define Props(displayPriority, \
              text, \
              voxelType, \
              rodType0, \
              rodType1, \
              rodType2, \
              lockType, \
              lockState, \
              dataType, \
              dataState, \
              isRodBody, \
              blockableStates, \
              fwd, \
              fwdVoxel, \
              bwd, \
              bwdVoxel \
             ) { \
  displayPriority, \
  text, \
  vt##voxelType, \
  rt##rodType0, /* rt##rodType1, rt##rodType2, */ \
  lt##lockType, \
  ls##lockState, \
  dt##dataType, \
  ds##dataState, \
  isRodBody, \
  blockableStates, \
  { { fwd, fwdVoxel }, \
    { bwd, bwdVoxel } \
  } \
}

#undef T
#define T true
#undef F
#define F false
#undef ____
#define ____ Unkn

  //                     displayPriority
  //                    /  text
  //                   /  /    voxelType
  //                  /  /    /   rodType[0]
  //                 /  /    /   /  rodType[1]
  //                /  /    /   /  /  rodType[2]
  //               /  /    /   /  /  /    lockType
  //              /  /    /   /  /  /    /    lockState
  //             /  /    /   /  /  /    /    /    dataType
  //            /  /    /   /  /  /    /    /    /    dataState
  //           /  /    /   /  /  /    /    /    /    / isRodBody
  //          /  /    /   /  /  /    /    /    /    / /   blockableStates
  //         /  /    /   /  /  /    /    /    /    / /   / fwd
  //        /  /    /   /  /  /    /    /    /    / /   / /    fwdVoxel
  //       /  /    /   /  /  /    /    /    /    / /   / /    / bwd
  //      /  /    /   /  /  /    /    /    /    / /   / /    / /    bwdVoxel
  //     /  /    /   /  /  /    /    /    /    / /   / /    / /    /
  Props(9,'?',Unkn,Un,Un,Un,Unkn,Unkn,Unkn,Unkn,F,___,X,____,X,____), // Unkn
  Props(0,'.',Wall,Un,Un,Un,Unkn,Unkn,Unkn,Unkn,F,___,X,____,X,____), // Wall
  Props(1,' ',Slot,Un,Un,Un,Unkn,Unkn,Unkn,Unkn,F,___,X,____,X,____), // Slot
  Props(2,'-',Lock,LE,Un,Un,Body,Lckd,Unkn,Unkn,T,___,E,LBEU,W,____), // LBEL
  Props(2,'-',Lock,LE,Un,Un,Body,Unlk,Unkn,Unkn,T,___,E,____,W,LBEL), // LBEU
  Props(2,'|',Lock,LS,Un,Un,Body,Lckd,Unkn,Unkn,T,___,S,LBSU,N,____), // LBSL
  Props(2,'|',Lock,LS,Un,Un,Body,Unlk,Unkn,Unkn,T,___,S,____,N,LBSL), // LBSU
  Props(2,'|',Lock,LS,Un,Un,Body,Lckd,Unkn,Unkn,T,___,S,LBDU,N,____), // LBDL
  Props(2,'|',Lock,LS,Un,Un,Body,Unlk,Unkn,Unkn,T,___,S,____,N,LBDL), // LBDU
  Props(2,'-',Lock,LW,Un,Un,Body,Lckd,Unkn,Unkn,T,___,W,LBWU,E,____), // LBWL
  Props(2,'-',Lock,LW,Un,Un,Body,Unlk,Unkn,Unkn,T,___,W,____,E,LBWL), // LBWU
  Props(2,'|',Lock,LN,Un,Un,Body,Lckd,Unkn,Unkn,T,___,N,LBNU,S,____), // LBNL
  Props(2,'|',Lock,LN,Un,Un,Body,Unlk,Unkn,Unkn,T,___,N,____,S,LBNL), // LBNU
  Props(2,'|',Lock,LN,Un,Un,Body,Lckd,Unkn,Unkn,T,___,N,LBUU,S,____), // LBUL
  Props(2,'|',Lock,LN,Un,Un,Body,Unlk,Unkn,Unkn,T,___,N,____,S,LBUL), // LBUU
  Props(3,'>',Lock,LE,Un,Un,Head,Lckd,Unkn,Unkn,T,___,E,LHEU,W,____), // LHEL
  Props(3,'>',Lock,LE,Un,Un,Head,Unlk,Unkn,Unkn,T,___,E,____,W,LHEL), // LHEU
  Props(3,'v',Lock,LS,Un,Un,Head,Lckd,Unkn,Unkn,T,___,S,LHSU,N,____), // LHSL
  Props(3,'v',Lock,LS,Un,Un,Head,Unlk,Unkn,Unkn,T,___,S,____,N,LHSL), // LHSU
  Props(3,'v',Lock,LS,Un,Un,Head,Lckd,Unkn,Unkn,T,___,S,LHDU,N,____), // LHDL
  Props(3,'v',Lock,LS,Un,Un,Head,Unlk,Unkn,Unkn,T,___,S,____,N,LHDL), // LHDU
  Props(3,'<',Lock,LW,Un,Un,Head,Lckd,Unkn,Unkn,T,___,W,LHWU,E,____), // LHWL
  Props(3,'<',Lock,LW,Un,Un,Head,Unlk,Unkn,Unkn,T,___,W,____,E,LHWL), // LHWU
  Props(3,'^',Lock,LN,Un,Un,Head,Lckd,Unkn,Unkn,T,___,N,LHNU,S,____), // LHNL
  Props(3,'^',Lock,LN,Un,Un,Head,Unlk,Unkn,Unkn,T,___,N,____,S,LHNL), // LHNU
  Props(3,'^',Lock,LN,Un,Un,Head,Lckd,Unkn,Unkn,T,___,N,LHUU,S,____), // LHUL
  Props(3,'^',Lock,LN,Un,Un,Head,Unlk,Unkn,Unkn,T,___,N,____,S,LHUL), // LHUU
  Props(3,'>',Lock,LE,Un,Un,Tail,Lckd,Unkn,Unkn,T,___,E,LTEU,W,____), // LTEL
  Props(3,'>',Lock,LE,Un,Un,Tail,Unlk,Unkn,Unkn,T,___,E,____,W,LTEL), // LTEU
  Props(3,'v',Lock,LS,Un,Un,Tail,Lckd,Unkn,Unkn,T,___,S,LTSU,N,____), // LTSL
  Props(3,'v',Lock,LS,Un,Un,Tail,Unlk,Unkn,Unkn,T,___,S,____,N,LTSL), // LTSU
  Props(3,'v',Lock,LS,Un,Un,Tail,Lckd,Unkn,Unkn,T,___,S,LTDU,N,____), // LTDL
  Props(3,'v',Lock,LS,Un,Un,Tail,Unlk,Unkn,Unkn,T,___,S,____,N,LTDL), // LTDU
  Props(3,'<',Lock,LW,Un,Un,Tail,Lckd,Unkn,Unkn,T,___,W,LTWU,E,____), // LTWL
  Props(3,'<',Lock,LW,Un,Un,Tail,Unlk,Unkn,Unkn,T,___,W,____,E,LTWL), // LTWU
  Props(3,'^',Lock,LN,Un,Un,Tail,Lckd,Unkn,Unkn,T,___,N,LTNU,S,____), // LTNL
  Props(3,'^',Lock,LN,Un,Un,Tail,Unlk,Unkn,Unkn,T,___,N,____,S,LTNL), // LTNU
  Props(3,'^',Lock,LN,Un,Un,Tail,Lckd,Unkn,Unkn,T,___,N,LTUU,S,____), // LTUL
  Props(3,'^',Lock,LN,Un,Un,Tail,Unlk,Unkn,Unkn,T,___,N,____,S,LTUL), // LTUU
  Props(2,'+',Lock,LE,Un,Un,Post,Lckd,Unkn,Unkn,T,___,E,LPEU,W,____), // LPEL
  Props(2,'+',Lock,LE,Un,Un,Post,Unlk,Unkn,Unkn,T,___,E,____,W,LPEL), // LPEU
  Props(2,'+',Lock,LS,Un,Un,Post,Lckd,Unkn,Unkn,T,___,S,LPSU,N,____), // LPSL
  Props(2,'+',Lock,LS,Un,Un,Post,Unlk,Unkn,Unkn,T,___,S,____,N,LPSL), // LPSU
  Props(2,'+',Lock,LS,Un,Un,Post,Lckd,Unkn,Unkn,T,___,S,LPDU,N,____), // LPDL
  Props(2,'+',Lock,LS,Un,Un,Post,Unlk,Unkn,Unkn,T,___,S,____,N,LPDL), // LPDU
  Props(2,'+',Lock,LW,Un,Un,Post,Lckd,Unkn,Unkn,T,___,W,LPWU,E,____), // LPWL
  Props(2,'+',Lock,LW,Un,Un,Post,Unlk,Unkn,Unkn,T,___,W,____,E,LPWL), // LPWU
  Props(2,'+',Lock,LN,Un,Un,Post,Lckd,Unkn,Unkn,T,___,N,LPNU,S,____), // LPNL
  Props(2,'+',Lock,LN,Un,Un,Post,Unlk,Unkn,Unkn,T,___,N,____,S,LPNL), // LPNU
  Props(2,'+',Lock,LN,Un,Un,Post,Lckd,Unkn,Unkn,T,___,N,LPUU,S,____), // LPUL
  Props(2,'+',Lock,LN,Un,Un,Post,Unlk,Unkn,Unkn,T,___,N,____,S,LPUL), // LPUU
  Props(3,'<',Lock,LE,Un,Un,Lock,Lckd,Unkn,Unkn,F,FRL,E,LKEU,W,____), // LKEL
  Props(3,'<',Lock,LE,Un,Un,Lock,Unlk,Unkn,Unkn,F,FRL,E,____,W,LKEL), // LKEU
  Props(3,'^',Lock,LS,Un,Un,Lock,Lckd,Unkn,Unkn,F,FRL,S,LKSU,N,____), // LKSL
  Props(3,'^',Lock,LS,Un,Un,Lock,Unlk,Unkn,Unkn,F,FRL,S,____,N,LKSL), // LKSU
  Props(3,'^',Lock,LS,Un,Un,Lock,Lckd,Unkn,Unkn,F,FRL,S,LKDU,N,____), // LKDL
  Props(3,'^',Lock,LS,Un,Un,Lock,Unlk,Unkn,Unkn,F,FRL,S,____,N,LKDL), // LKDU
  Props(3,'>',Lock,LW,Un,Un,Lock,Lckd,Unkn,Unkn,F,FRL,W,LKWU,E,____), // LKWL
  Props(3,'>',Lock,LW,Un,Un,Lock,Unlk,Unkn,Unkn,F,FRL,W,____,E,LKWL), // LKWU
  Props(3,'v',Lock,LN,Un,Un,Lock,Lckd,Unkn,Unkn,F,FRL,N,LKNU,S,____), // LKNL
  Props(3,'v',Lock,LN,Un,Un,Lock,Unlk,Unkn,Unkn,F,FRL,N,____,S,LKNL), // LKNU
  Props(3,'v',Lock,LN,Un,Un,Lock,Lckd,Unkn,Unkn,F,FRL,N,LKUU,S,____), // LKUL
  Props(3,'v',Lock,LN,Un,Un,Lock,Unlk,Unkn,Unkn,F,FRL,N,____,S,LKUL), // LKUU
  Props(4,'-',Data,DE,Un,Un,Unkn,Unkn,Body,Rset,T,___,E,DBE0,W,____), // DBER
  Props(4,'-',Data,DE,Un,Un,Unkn,Unkn,Body,Set0,T,___,E,DBEX,W,DBER), // DBE0
  Props(4,'-',Data,DE,Un,Un,Unkn,Unkn,Body,SetX,T,___,E,DBE1,W,DBE0), // DBEX
  Props(4,'-',Data,DE,Un,Un,Unkn,Unkn,Body,Set1,T,___,E,____,W,DBEX), // DBE1
  Props(4,'|',Data,DS,Un,Un,Unkn,Unkn,Body,Rset,T,___,S,DBS0,N,____), // DBSR
  Props(4,'|',Data,DS,Un,Un,Unkn,Unkn,Body,Set0,T,___,S,DBSX,N,DBSR), // DBS0
  Props(4,'|',Data,DS,Un,Un,Unkn,Unkn,Body,SetX,T,___,S,DBS1,N,DBS0), // DBSX
  Props(4,'|',Data,DS,Un,Un,Unkn,Unkn,Body,Set1,T,___,S,____,N,DBSX), // DBS1
  Props(4,'|',Data,DS,Un,Un,Unkn,Unkn,Body,Rset,T,___,S,DBD0,N,____), // DBDR
  Props(4,'|',Data,DS,Un,Un,Unkn,Unkn,Body,Set0,T,___,S,DBDX,N,DBDR), // DBD0
  Props(4,'|',Data,DS,Un,Un,Unkn,Unkn,Body,SetX,T,___,S,DBD1,N,DBD0), // DBDX
  Props(4,'|',Data,DS,Un,Un,Unkn,Unkn,Body,Set1,T,___,S,____,N,DBDX), // DBD1
  Props(4,'-',Data,DW,Un,Un,Unkn,Unkn,Body,Rset,T,___,W,DBW0,E,____), // DBWR
  Props(4,'-',Data,DW,Un,Un,Unkn,Unkn,Body,Set0,T,___,W,DBWX,E,DBWR), // DBW0
  Props(4,'-',Data,DW,Un,Un,Unkn,Unkn,Body,SetX,T,___,W,DBW1,E,DBW0), // DBWX
  Props(4,'-',Data,DW,Un,Un,Unkn,Unkn,Body,Set1,T,___,W,____,E,DBWX), // DBW1
  Props(4,'|',Data,DN,Un,Un,Unkn,Unkn,Body,Rset,T,___,N,DBN0,S,____), // DBNR
  Props(4,'|',Data,DN,Un,Un,Unkn,Unkn,Body,Set0,T,___,N,DBNX,S,DBNR), // DBN0
  Props(4,'|',Data,DN,Un,Un,Unkn,Unkn,Body,SetX,T,___,N,DBN1,S,DBN0), // DBNX
  Props(4,'|',Data,DN,Un,Un,Unkn,Unkn,Body,Set1,T,___,N,____,S,DBNX), // DBN1
  Props(4,'|',Data,DN,Un,Un,Unkn,Unkn,Body,Rset,T,___,N,DBU0,S,____), // DBUR
  Props(4,'|',Data,DN,Un,Un,Unkn,Unkn,Body,Set0,T,___,N,DBUX,S,DBUR), // DBU0
  Props(4,'|',Data,DN,Un,Un,Unkn,Unkn,Body,SetX,T,___,N,DBU1,S,DBU0), // DBUX
  Props(4,'|',Data,DN,Un,Un,Unkn,Unkn,Body,Set1,T,___,N,____,S,DBUX), // DBU1
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Head,Rset,T,F__,E,DHE0,W,____), // DHER
  Props(5,'0',Data,DE,Un,Un,Unkn,Unkn,Head,Set0,T,F__,E,DHEX,W,DHER), // DHE0
  Props(5,'X',Data,DE,Un,Un,Unkn,Unkn,Head,SetX,T,F__,E,DHE1,W,DHE0), // DHEX
  Props(5,'1',Data,DE,Un,Un,Unkn,Unkn,Head,Set1,T,F__,E,____,W,DHEX), // DHE1
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Head,Rset,T,F__,S,DHS0,N,____), // DHSR
  Props(5,'0',Data,DS,Un,Un,Unkn,Unkn,Head,Set0,T,F__,S,DHSX,N,DHSR), // DHS0
  Props(5,'X',Data,DS,Un,Un,Unkn,Unkn,Head,SetX,T,F__,S,DHS1,N,DHS0), // DHSX
  Props(5,'1',Data,DS,Un,Un,Unkn,Unkn,Head,Set1,T,F__,S,____,N,DHSX), // DHS1
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Head,Rset,T,F__,S,DHD0,N,____), // DHDR
  Props(5,'0',Data,DS,Un,Un,Unkn,Unkn,Head,Set0,T,F__,S,DHDX,N,DHDR), // DHD0
  Props(5,'X',Data,DS,Un,Un,Unkn,Unkn,Head,SetX,T,F__,S,DHD1,N,DHD0), // DHDX
  Props(5,'1',Data,DS,Un,Un,Unkn,Unkn,Head,Set1,T,F__,S,____,N,DHDX), // DHD1
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Head,Rset,T,F__,W,DHW0,E,____), // DHWR
  Props(5,'0',Data,DW,Un,Un,Unkn,Unkn,Head,Set0,T,F__,W,DHWX,E,DHWR), // DHW0
  Props(5,'X',Data,DW,Un,Un,Unkn,Unkn,Head,SetX,T,F__,W,DHW1,E,DHW0), // DHWX
  Props(5,'1',Data,DW,Un,Un,Unkn,Unkn,Head,Set1,T,F__,W,____,E,DHWX), // DHW1
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Head,Rset,T,F__,N,DHN0,S,____), // DHNR
  Props(5,'0',Data,DN,Un,Un,Unkn,Unkn,Head,Set0,T,F__,X,DHNX,S,DHNR), // DHN0
  Props(5,'X',Data,DN,Un,Un,Unkn,Unkn,Head,SetX,T,F__,N,DHN1,S,DHN0), // DHNX
  Props(5,'1',Data,DN,Un,Un,Unkn,Unkn,Head,Set1,T,F__,N,____,S,DHNX), // DHN1
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Head,Rset,T,F__,N,DHU0,S,____), // DHUR
  Props(5,'0',Data,DN,Un,Un,Unkn,Unkn,Head,Set0,T,F__,N,DHUX,S,DHUR), // DHU0
  Props(5,'X',Data,DN,Un,Un,Unkn,Unkn,Head,SetX,T,F__,N,DHU1,S,DHU0), // DHUX
  Props(5,'1',Data,DN,Un,Un,Unkn,Unkn,Head,Set1,T,F__,N,____,S,DHUX), // DHU1
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Tail,Rset,T,_R_,E,DTE0,W,____), // DTER
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Tail,Set0,T,_R_,E,DTEX,W,DTER), // DTE0
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Tail,SetX,T,_R_,E,DTE1,W,DTE0), // DTEX
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Tail,Set1,T,_R_,E,____,W,DTEX), // DTE1
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Tail,Rset,T,_R_,S,DTS0,N,____), // DTSR
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Tail,Set0,T,_R_,S,DTSX,N,DTSR), // DTS0
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Tail,SetX,T,_R_,S,DTS1,N,DTS0), // DTSX
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Tail,Set1,T,_R_,S,____,N,DTSX), // DTS1
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Tail,Rset,T,_R_,S,DTD0,N,____), // DTDR
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Tail,Set0,T,_R_,S,DTDX,N,DTDR), // DTD0
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Tail,SetX,T,_R_,S,DTD1,N,DTD0), // DTDX
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Tail,Set1,T,_R_,S,____,N,DTDX), // DTD1
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Tail,Rset,T,_R_,W,DTW0,E,____), // DTWR
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Tail,Set0,T,_R_,W,DTWX,E,DTWR), // DTW0
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Tail,SetX,T,_R_,W,DTW1,E,DTW0), // DTWX
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Tail,Set1,T,_R_,W,____,E,DTWX), // DTW1
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Tail,Rset,T,_R_,N,DTN0,S,____), // DTNR
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Tail,Set0,T,_R_,N,DTNX,S,DTNR), // DTN0
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Tail,SetX,T,_R_,N,DTN1,S,DTN0), // DTNX
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Tail,Set1,T,_R_,N,____,S,DTNX), // DTN1
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Tail,Rset,T,_R_,N,DTU0,S,____), // DTUR
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Tail,Set0,T,_R_,N,DTUX,S,DTUR), // DTU0
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Tail,SetX,T,_R_,N,DTU1,S,DTU0), // DTUX
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Tail,Set1,T,_R_,N,____,S,DTUX), // DTU1
  Props(4,'+',Data,DE,Un,Un,Unkn,Unkn,Post,Rset,T,FR_,E,DPE0,W,____), // DPER
  Props(4,'+',Data,DE,Un,Un,Unkn,Unkn,Post,Set0,T,FR_,E,DPEX,W,DPER), // DPE0
  Props(4,'+',Data,DE,Un,Un,Unkn,Unkn,Post,SetX,T,FR_,E,DPE1,W,DPE0), // DPEX
  Props(4,'+',Data,DE,Un,Un,Unkn,Unkn,Post,Set1,T,FR_,E,____,W,DPEX), // DPE1
  Props(4,'+',Data,DS,Un,Un,Unkn,Unkn,Post,Rset,T,FR_,S,DPS0,X,____), // DPSR
  Props(4,'+',Data,DS,Un,Un,Unkn,Unkn,Post,Set0,T,FR_,S,DPSX,N,DPSR), // DPS0
  Props(4,'+',Data,DS,Un,Un,Unkn,Unkn,Post,SetX,T,FR_,S,DPS1,N,DPS0), // DPSX
  Props(4,'+',Data,DS,Un,Un,Unkn,Unkn,Post,Set1,T,FR_,S,____,N,DPSX), // DPS1
  Props(4,'+',Data,DS,Un,Un,Unkn,Unkn,Post,Rset,T,FR_,S,DPD0,N,____), // DPDR
  Props(4,'+',Data,DS,Un,Un,Unkn,Unkn,Post,Set0,T,FR_,S,DPDX,N,DPDR), // DPD0
  Props(4,'+',Data,DS,Un,Un,Unkn,Unkn,Post,SetX,T,FR_,S,DPD1,N,DPD0), // DPDX
  Props(4,'+',Data,DS,Un,Un,Unkn,Unkn,Post,Set1,T,FR_,S,____,N,DPDX), // DPD1
  Props(4,'+',Data,DW,Un,Un,Unkn,Unkn,Post,Rset,T,FR_,W,DPW0,E,____), // DPWR
  Props(4,'+',Data,DW,Un,Un,Unkn,Unkn,Post,Set0,T,FR_,W,DPWX,E,DPWR), // DPW0
  Props(4,'+',Data,DW,Un,Un,Unkn,Unkn,Post,SetX,T,FR_,W,DPW1,E,DPW0), // DPWX
  Props(4,'+',Data,DW,Un,Un,Unkn,Unkn,Post,Set1,T,FR_,W,____,E,DPWX), // DPW1
  Props(4,'+',Data,DN,Un,Un,Unkn,Unkn,Post,Rset,T,FR_,N,DPN0,S,____), // DPNR
  Props(4,'+',Data,DN,Un,Un,Unkn,Unkn,Post,Set0,T,FR_,N,DPNX,S,DPNR), // DPN0
  Props(4,'+',Data,DN,Un,Un,Unkn,Unkn,Post,SetX,T,FR_,N,DPN1,S,DPN0), // DPNX
  Props(4,'+',Data,DN,Un,Un,Unkn,Unkn,Post,Set1,T,FR_,N,____,S,DPNX), // DPN1
  Props(4,'+',Data,DN,Un,Un,Unkn,Unkn,Post,Rset,T,FR_,N,DPU0,S,____), // DPUR
  Props(4,'+',Data,DN,Un,Un,Unkn,Unkn,Post,Set0,T,FR_,N,DPUX,S,DPUR), // DPU0
  Props(4,'+',Data,DN,Un,Un,Unkn,Unkn,Post,SetX,T,FR_,N,DPU1,S,DPU0), // DPUX
  Props(4,'+',Data,DN,Un,Un,Unkn,Unkn,Post,Set1,T,FR_,N,____,S,DPUX), // DPU1
  Props(5,'^',Data,DE,Un,Un,Unkn,Unkn,Slot,Rset,F,FRL,E,DSE0,W,____), // DSER
  Props(5,'^',Data,DE,Un,Un,Unkn,Unkn,Slot,Set0,F,FRL,E,DSEX,W,DSER), // DSE0
  Props(5,'^',Data,DE,Un,Un,Unkn,Unkn,Slot,SetX,F,FRL,E,DSE1,W,DSE0), // DSEX
  Props(5,'^',Data,DE,Un,Un,Unkn,Unkn,Slot,Set1,F,FRL,E,____,W,DSEX), // DSE1
  Props(5,'>',Data,DS,Un,Un,Unkn,Unkn,Slot,Rset,F,FRL,S,DSS0,N,____), // DSSR
  Props(5,'>',Data,DS,Un,Un,Unkn,Unkn,Slot,Set0,F,FRL,S,DSSX,N,DSSR), // DSS0
  Props(5,'>',Data,DS,Un,Un,Unkn,Unkn,Slot,SetX,F,FRL,S,DSS1,N,DSS0), // DSSX
  Props(5,'>',Data,DS,Un,Un,Unkn,Unkn,Slot,Set1,F,FRL,S,____,N,DSSX), // DSS1
  Props(5,'>',Data,DS,Un,Un,Unkn,Unkn,Slot,Rset,F,FRL,S,DSD0,N,____), // DSDR
  Props(5,'>',Data,DS,Un,Un,Unkn,Unkn,Slot,Set0,F,FRL,S,DSDX,N,DSDR), // DSD0
  Props(5,'>',Data,DS,Un,Un,Unkn,Unkn,Slot,SetX,F,FRL,S,DSD1,N,DSD0), // DSDX
  Props(5,'>',Data,DS,Un,Un,Unkn,Unkn,Slot,Set1,F,FRL,S,____,N,DSDX), // DSD1
  Props(5,'v',Data,DW,Un,Un,Unkn,Unkn,Slot,Rset,F,FRL,W,DSW0,E,____), // DSWR
  Props(5,'v',Data,DW,Un,Un,Unkn,Unkn,Slot,Set0,F,FRL,W,DSWX,E,DSWR), // DSW0
  Props(5,'v',Data,DW,Un,Un,Unkn,Unkn,Slot,SetX,F,FRL,W,DSW1,E,DSW0), // DSWX
  Props(5,'v',Data,DW,Un,Un,Unkn,Unkn,Slot,Set1,F,FRL,W,____,E,DSWX), // DSW1
  Props(5,'<',Data,DN,Un,Un,Unkn,Unkn,Slot,Rset,F,FRL,N,DSN0,S,____), // DSNR
  Props(5,'<',Data,DN,Un,Un,Unkn,Unkn,Slot,Set0,F,FRL,N,DSNX,S,DSNR), // DSN0
  Props(5,'<',Data,DN,Un,Un,Unkn,Unkn,Slot,SetX,F,FRL,N,DSN1,S,DSN0), // DSNX
  Props(5,'<',Data,DN,Un,Un,Unkn,Unkn,Slot,Set1,F,FRL,N,____,S,DSNX), // DSN1
  Props(5,'<',Data,DN,Un,Un,Unkn,Unkn,Slot,Rset,F,FRL,N,DSU0,S,____), // DSUR
  Props(5,'<',Data,DN,Un,Un,Unkn,Unkn,Slot,Set0,F,FRL,N,DSUX,S,DSUR), // DSU0
  Props(5,'<',Data,DN,Un,Un,Unkn,Unkn,Slot,SetX,F,FRL,N,DSU1,S,DSU0), // DSUX
  Props(5,'<',Data,DN,Un,Un,Unkn,Unkn,Slot,Set1,F,FRL,N,____,S,DSUX), // DSU1
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Test,Rset,F,FR_,E,DQE0,W,____), // DQER
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Test,Set0,F,FR_,E,DQEX,W,DQER), // DQE0
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Test,SetX,F,FR_,E,DQE1,W,DQE0), // DQEX
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Test,Set1,F,FR_,E,____,W,DQEX), // DQE1
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Test,Rset,F,FR_,S,DQS0,N,____), // DQSR
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Test,Set0,F,FR_,S,DQSX,N,DQSR), // DQS0
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Test,SetX,F,FR_,S,DQS1,N,DQS0), // DQSX
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Test,Set1,F,FR_,S,____,N,DQSX), // DQS1
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Test,Rset,F,FR_,S,DQD0,N,____), // DQDR
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Test,Set0,F,FR_,S,DQDX,N,DQDR), // DQD0
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Test,SetX,F,FR_,S,DQD1,N,DQD0), // DQDX
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Test,Set1,F,FR_,S,____,N,DQDX), // DQD1
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Test,Rset,F,FR_,W,DQW0,E,____), // DQWR
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Test,Set0,F,FR_,W,DQWX,E,DQWR), // DQW0
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Test,SetX,F,FR_,W,DQW1,E,DQW0), // DQWX
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Test,Set1,F,FR_,W,____,E,DQWX), // DQW1
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Test,Rset,F,FR_,N,DQN0,S,____), // DQNR
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Test,Set0,F,FR_,N,DQNX,S,DQNR), // DQN0
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Test,SetX,F,FR_,N,DQN1,S,DQN0), // DQNX
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Test,Set1,F,FR_,N,____,S,DQNX), // DQN1
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Test,Rset,F,FR_,N,DQU0,S,____), // DQUR
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Test,Set0,F,FR_,N,DQUX,S,DQUR), // DQU0
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Test,SetX,F,FR_,N,DQU1,S,DQU0), // DQUX
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Test,Set1,F,FR_,N,____,S,DQUX), // DQU1
  Props(5,'#',Data,DE,Un,Un,Unkn,Unkn,Gate,Rset,F,FR_,E,DGE0,W,____), // DGER
  Props(5,'#',Data,DE,DS,Un,Unkn,Unkn,Gate,Set0,F,FR_,E,DGEX,W,DGER), // DGE0
  Props(5,'#',Data,DE,DS,Un,Unkn,Unkn,Gate,SetX,F,FR_,E,DGE1,W,DGE0), // DGEX
  Props(5,'#',Data,DE,DS,Un,Unkn,Unkn,Gate,Set1,F,FR_,E,____,W,DGEX), // DGE1
  Props(5,'#',Data,DS,Un,Un,Unkn,Unkn,Gate,Rset,F,FR_,S,DGS0,N,____), // DGSR
  Props(5,'#',Data,DS,DW,Un,Unkn,Unkn,Gate,Set0,F,FR_,S,DGSX,N,DGSR), // DGS0
  Props(5,'#',Data,DS,DW,Un,Unkn,Unkn,Gate,SetX,F,FR_,S,DGS1,N,DGS0), // DGSX
  Props(5,'#',Data,DS,DW,Un,Unkn,Unkn,Gate,Set1,F,FR_,S,____,N,DGSX), // DGS1
  Props(5,'#',Data,DS,Un,Un,Unkn,Unkn,Gate,Rset,F,FR_,S,DGD0,N,____), // DGDR
  Props(5,'#',Data,DS,Un,Un,Unkn,Unkn,Gate,Set0,F,FR_,S,DGDX,N,DGDR), // DGD0
  Props(5,'#',Data,DS,Un,Un,Unkn,Unkn,Gate,SetX,F,FR_,S,DGD1,N,DGD0), // DGDX
  Props(5,'#',Data,DS,Un,Un,Unkn,Unkn,Gate,Set1,F,FR_,S,____,N,DGDX), // DGD1
  Props(5,'#',Data,DW,DN,Un,Unkn,Unkn,Gate,Rset,F,FR_,W,DGW0,E,____), // DGWR
  Props(5,'#',Data,DW,DN,Un,Unkn,Unkn,Gate,Set0,F,FR_,W,DGWX,E,DGWR), // DGW0
  Props(5,'#',Data,DW,DN,Un,Unkn,Unkn,Gate,SetX,F,FR_,W,DGW1,E,DGW0), // DGWX
  Props(5,'#',Data,DW,Un,Un,Unkn,Unkn,Gate,Set1,F,FR_,W,____,E,DGWX), // DGW1
  Props(5,'#',Data,DN,Un,Un,Unkn,Unkn,Gate,Rset,F,FR_,N,DGN0,S,____), // DGNR
  Props(5,'#',Data,DN,DE,Un,Unkn,Unkn,Gate,Set0,F,FR_,N,DGNX,S,DGNR), // DGN0
  Props(5,'#',Data,DN,DE,Un,Unkn,Unkn,Gate,SetX,F,FR_,N,DGN1,S,DGN0), // DGNX
  Props(5,'#',Data,DN,DE,Un,Unkn,Unkn,Gate,Set1,F,FR_,N,____,S,DGNX), // DGN1
  Props(5,'#',Data,DN,Un,Un,Unkn,Unkn,Gate,Rset,F,FR_,N,DGU0,S,____), // DGUR
  Props(5,'#',Data,DN,Un,Un,Unkn,Unkn,Gate,Set0,F,FR_,N,DGUX,S,DGUR), // DGU0
  Props(5,'#',Data,DN,Un,Un,Unkn,Unkn,Gate,SetX,F,FR_,N,DGU1,S,DGU0), // DGUX
  Props(5,'#',Data,DN,Un,Un,Unkn,Unkn,Gate,Set1,F,FR_,N,____,S,DGUX), // DGU1
  Props(6,'I',Data,DE,Un,Un,Unkn,Unkn,Inpt,Rset,T,F__,E,DIE0,W,____), // DIER
  Props(6,'0',Data,DE,Un,Un,Unkn,Unkn,Inpt,Set0,T,F__,E,DIEX,W,DIER), // DIE0
  Props(6,'X',Data,DE,Un,Un,Unkn,Unkn,Inpt,SetX,T,F__,E,DIE1,W,DIE0), // DIEX
  Props(6,'1',Data,DE,Un,Un,Unkn,Unkn,Inpt,Set1,T,F__,E,____,W,DIEX), // DIE1
  Props(6,'I',Data,DS,Un,Un,Unkn,Unkn,Inpt,Rset,T,F__,S,DIS0,N,____), // DISR
  Props(6,'0',Data,DS,Un,Un,Unkn,Unkn,Inpt,Set0,T,F__,S,DISX,N,DISR), // DIS0
  Props(6,'X',Data,DS,Un,Un,Unkn,Unkn,Inpt,SetX,T,F__,S,DIS1,N,DIS0), // DISX
  Props(6,'1',Data,DS,Un,Un,Unkn,Unkn,Inpt,Set1,T,F__,S,____,N,DISX), // DIS1
  Props(6,'I',Data,DS,Un,Un,Unkn,Unkn,Inpt,Rset,T,F__,S,DID0,N,____), // DIDR
  Props(6,'0',Data,DS,Un,Un,Unkn,Unkn,Inpt,Set0,T,F__,S,DIDX,N,DIDR), // DID0
  Props(6,'X',Data,DS,Un,Un,Unkn,Unkn,Inpt,SetX,T,F__,S,DID1,N,DID0), // DIDX
  Props(6,'1',Data,DS,Un,Un,Unkn,Unkn,Inpt,Set1,T,F__,S,____,N,DIDX), // DID1
  Props(6,'I',Data,DW,Un,Un,Unkn,Unkn,Inpt,Rset,T,F__,W,DIW0,E,____), // DIWR
  Props(6,'0',Data,DW,Un,Un,Unkn,Unkn,Inpt,Set0,T,F__,W,DIWX,E,DIWR), // DIW0
  Props(6,'X',Data,DW,Un,Un,Unkn,Unkn,Inpt,SetX,T,F__,W,DIW1,E,DIW0), // DIWX
  Props(6,'1',Data,DW,Un,Un,Unkn,Unkn,Inpt,Set1,T,F__,W,____,E,DIWX), // DIW1
  Props(6,'I',Data,DN,Un,Un,Unkn,Unkn,Inpt,Rset,T,F__,N,DIN0,S,____), // DINR
  Props(6,'0',Data,DN,Un,Un,Unkn,Unkn,Inpt,Set0,T,F__,N,DINX,S,DINR), // DIN0
  Props(6,'X',Data,DN,Un,Un,Unkn,Unkn,Inpt,SetX,T,F__,N,DIN1,S,DIN0), // DINX
  Props(6,'1',Data,DN,Un,Un,Unkn,Unkn,Inpt,Set1,T,F__,N,____,S,DINX), // DIN1
  Props(6,'I',Data,DN,Un,Un,Unkn,Unkn,Inpt,Rset,T,F__,N,DIU0,S,____), // DIUR
  Props(6,'0',Data,DN,Un,Un,Unkn,Unkn,Inpt,Set0,T,F__,N,DIUX,S,DIUR), // DIU0
  Props(6,'X',Data,DN,Un,Un,Unkn,Unkn,Inpt,SetX,T,F__,N,DIU1,S,DIU0), // DIUX
  Props(6,'1',Data,DN,Un,Un,Unkn,Unkn,Inpt,Set1,T,F__,N,____,S,DIUX), // DIU1
  Props(6,'O',Data,DE,Un,Un,Unkn,Unkn,Outp,Rset,T,___,E,DOE0,W,____), // DOER
  Props(6,'0',Data,DE,Un,Un,Unkn,Unkn,Outp,Set0,T,___,E,DOEX,W,DOER), // DOE0
  Props(6,'X',Data,DE,Un,Un,Unkn,Unkn,Outp,SetX,T,___,E,DOE1,W,DOE0), // DOEX
  Props(6,'1',Data,DE,Un,Un,Unkn,Unkn,Outp,Set1,T,___,E,____,W,DOEX), // DOE1
  Props(6,'O',Data,DS,Un,Un,Unkn,Unkn,Outp,Rset,T,___,S,DOS0,N,____), // DOSR
  Props(6,'0',Data,DS,Un,Un,Unkn,Unkn,Outp,Set0,T,___,S,DOSX,N,DOSR), // DOS0
  Props(6,'X',Data,DS,Un,Un,Unkn,Unkn,Outp,SetX,T,___,S,DOS1,N,DOS0), // DOSX
  Props(6,'1',Data,DS,Un,Un,Unkn,Unkn,Outp,Set1,T,___,S,____,N,DOSX), // DOS1
  Props(6,'O',Data,DS,Un,Un,Unkn,Unkn,Outp,Rset,T,___,S,DOD0,N,____), // DODR
  Props(6,'0',Data,DS,Un,Un,Unkn,Unkn,Outp,Set0,T,___,S,DODX,N,DODR), // DOD0
  Props(6,'X',Data,DS,Un,Un,Unkn,Unkn,Outp,SetX,T,___,S,DOD1,N,DOD0), // DODX
  Props(6,'1',Data,DS,Un,Un,Unkn,Unkn,Outp,Set1,T,___,S,____,N,DODX), // DOD1
  Props(6,'O',Data,DW,Un,Un,Unkn,Unkn,Outp,Rset,T,___,W,DOW0,E,____), // DOWR
  Props(6,'0',Data,DW,Un,Un,Unkn,Unkn,Outp,Set0,T,___,W,DOWX,E,DOWR), // DOW0
  Props(6,'X',Data,DW,Un,Un,Unkn,Unkn,Outp,SetX,T,___,W,DOW1,E,DOW0), // DOWX
  Props(6,'1',Data,DW,Un,Un,Unkn,Unkn,Outp,Set1,T,___,W,____,E,DOWX), // DOW1
  Props(6,'O',Data,DN,Un,Un,Unkn,Unkn,Outp,Rset,T,___,N,DON0,S,____), // DONR
  Props(6,'0',Data,DN,Un,Un,Unkn,Unkn,Outp,Set0,T,___,N,DONX,S,DONR), // DON0
  Props(6,'X',Data,DN,Un,Un,Unkn,Unkn,Outp,SetX,T,___,N,DON1,S,DON0), // DONX
  Props(6,'1',Data,DN,Un,Un,Unkn,Unkn,Outp,Set1,T,___,N,____,S,DONX), // DON1
  Props(6,'O',Data,DN,Un,Un,Unkn,Unkn,Outp,Rset,T,___,N,DOU0,S,____), // DOUR
  Props(6,'0',Data,DN,Un,Un,Unkn,Unkn,Outp,Set0,T,___,N,DOUX,S,DOUR), // DOU0
  Props(6,'X',Data,DN,Un,Un,Unkn,Unkn,Outp,SetX,T,___,N,DOU1,S,DOU0), // DOUX
  Props(6,'1',Data,DN,Un,Un,Unkn,Unkn,Outp,Set1,T,___,N,____,S,DOUX)  // DOU1
#undef _
#undef T
#undef F
};

typedef map<string, Voxel> StringToVoxel;
StringToVoxel stringToVoxel;

class Scenario {
  friend class Rule;
 public:
  Scenario(TickPerCycle t,
           Voxel n, Voxel u, Voxel w, Voxel o, Voxel e, Voxel d, Voxel s
          ) :
      tick(t),
      self(o)
  {
    area[E] = e;
    area[S] = s;
    area[D] = d;
    area[W] = w;
    area[N] = n;
    area[U] = u;
  }
  Scenario(TickPerCycle _tick, Voxel _self, array<Voxel, eoDirection> _area) :
      tick(_tick),
      self(_self),
      area(_area)
  {
  }
  Scenario(Volume const *volume, TickPerCycle t, VoxelCoordinant const &vc);
  Scenario(Scenario const &that) :
      tick(that.tick),
      self(that.self),
      area(that.area)
  {
  }
  Scenario &operator=(Scenario const &that) {
    if (this != &that) {
      tick = that.tick;
      self = that.self;
      area = that.area;
    }
    return *this;
  }

  bool operator==(Scenario const &that) const {
    return tick == that.tick && self == that.self && area == that.area;
  }
  bool operator<(Scenario const &that) const {
    return tick < that.tick ||
        (tick == that.tick && (self < that.self ||
                               (self == that.self && (area < that.area))
                              )
        );
  }
  Voxel From(Direction d) const { return area[d]; }
  Voxel FromE() const { return area[E]; }
  Voxel FromS() const { return area[S]; }
  Voxel FromD() const { return area[D]; }
  Voxel FromW() const { return area[W]; }
  Voxel FromN() const { return area[N]; }
  Voxel FromU() const { return area[U]; }
  Voxel Self() const { return self; }
  void Dump() const {
    fprintf(stdout,
            "(Scenario *)(%p)->{ tick=%s, self=%s, area={ %s, %s, %s, %s, %s, %s } }",
            this,
            toConstCharPointer(tick),
            toConstCharPointer(self),
            toConstCharPointer(area[E]),
            toConstCharPointer(area[S]),
            toConstCharPointer(area[D]),
            toConstCharPointer(area[W]),
            toConstCharPointer(area[N]),
            toConstCharPointer(area[U])
           );
  }
  void Show() const {
    fprintf(stdout,
            "{ %s, %s, %s, %s, %s, %s, %s, %s }",
            toConstCharPointer(tick),
            toConstCharPointer(area[N]),
            toConstCharPointer(area[U]),
            toConstCharPointer(area[W]),
            toConstCharPointer(self),
            toConstCharPointer(area[E]),
            toConstCharPointer(area[D]),
            toConstCharPointer(area[S])
           );
  }

 private:
  TickPerCycle tick;
  Voxel self;
  array<Voxel, eoDirection> area;
};

struct Rule {
  Rule(TickPerCycle t,
       Voxel n, Voxel u, Voxel w, Voxel o, Voxel e, Voxel d, Voxel s,
       Voxel v
      ) :
      scenario(t, n, u, w, o, e, d, s),
      newVoxel(v)
  {
  }
  Rule(Scenario const &s, Voxel v) : scenario(s), newVoxel(v) { }

  void Dump() const {
    fprintf(stdout, "(Rule *)(%p)->{ scenario=", this);
    scenario.Dump();
    fprintf(stdout, ", newVoxel=%s }", toConstCharPointer(newVoxel));
  }

  void Show() const {
    fprintf(stdout,
            "Rule(%s,\n"
            "        %s, %s,\n"
            "     %s, %s, %s,\n"
            "        %s, %s,\n"
            "     %s\n"
            "    );\n",
            toConstCharPointer(scenario.tick),
            toConstCharPointer(scenario.area[N]),
            toConstCharPointer(scenario.area[U]),
            toConstCharPointer(scenario.area[W]),
            toConstCharPointer(scenario.self),
            toConstCharPointer(scenario.area[E]),
            toConstCharPointer(scenario.area[D]),
            toConstCharPointer(scenario.area[S]),
            toConstCharPointer(newVoxel)
           );
  }
  Scenario scenario;
  Voxel newVoxel;
};

class Item;
typedef set<Item *> SetOfItems;
class LockRod;
class DataRod;

size_t const NLvls = 7;
size_t const NRows = 29;
size_t const NCols = 32;

typedef array<array<array<Voxel, NCols>, NRows>, NLvls> VolArray;
typedef map<VoxelCoordinant, Voxel> Changes;

typedef map<Scenario, Voxel> Rules;
typedef map<Scenario, size_t> RuleCounts;
typedef array<array<char, NCols>, NRows> ViewLvlArray;

class Volume: public VolArray
{
 public:
  Volume(VolArray const &initial);
  Volume(Voxel initialVoxel = Unkn);
  void AddRule(Rule const &rule) { rules[rule.scenario] = rule.newVoxel; }
  void AddRule(Scenario const &scenario, Voxel newVoxel) {
    rules[scenario] = newVoxel;
  }
  void AddRule(TickPerCycle t,
       Voxel n, Voxel u, Voxel w, Voxel o, Voxel e, Voxel d, Voxel s,
       Voxel v
      )
  {
    Scenario scenario(t, n, u, w, o, e, d, s);
    rules[scenario] = v;
    if (optVerbose) {
      fprintf(stdout,
              "(Volume *)(%p)->AddRule(%s, %s, %s, %s, %s, %s, %s, %s, %s): rules[",
              this,
              toConstCharPointer(t),
              toConstCharPointer(n),
              toConstCharPointer(u),
              toConstCharPointer(w),
              toConstCharPointer(o),
              toConstCharPointer(e),
              toConstCharPointer(d),
              toConstCharPointer(s),
              toConstCharPointer(v)
             );
      scenario.Show();
      fprintf(stdout, "] = %s\n", toConstCharPointer(v));
    }
  }
  Rules &GetRules() { return rules; }
  RuleCounts &GetRuleCounts() { return ruleCounts; }
  bool FoundNewRules() const {
    bool foundNewRules = false;
    for (auto const &r : rules) {
      if (r.second == Unkn) {
        fprintf(stdout, "  volume.Add");
        Rule rule(r.first, r.second);
        rule.Show();
        foundNewRules = true;
      }
    }
    return foundNewRules;
  }
  void DumpUnusedRules() const {
    for (auto const &r : ruleCounts) {
      if (r.second == 0) {
        fprintf(stdout, "Unused rule; scenario ");
        r.first.Show();
      }
    }
  }

  void AddInput(VoxelCoordinant const &vc, deque<DataState> const &values) {
    GetInputsFor(vc) = values;
  }
  deque<DataState> &GetInputsFor(VoxelCoordinant const &vc) {
    Voxel v = voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];
    assert(vProperties.voxelType == vtData);
    assert(vProperties.dataType == dtInpt);
    Direction d = rodTypeProperties[vProperties.rodType[0]].fwd;
    return inputs[d][vc];
  }
  void ProceedOneCycle();
  void ProceedOnePhase();
  void ProceedOneTick();
  void PrintViewFlat() const;
  bool isVoxelCoordinantInBounds(VoxelCoordinant const &vc) const {
    return 0 <= vc.L() && vc.L() < NLvls &&
        0 <= vc.R() && vc.R() < NRows &&
        0 <= vc.C() && vc.C() < NCols;
  }
  bool isPartOf(RodType r, VoxelCoordinant const &vc, bool &reachedEnd) const;
  bool isPartOf(RodType r, VoxelCoordinant const &vc) const;
  Voxel voxelAt(VoxelCoordinant const &vc) const {
    if (isVoxelCoordinantInBounds(vc)) {
      return (*this)[vc.L()][vc.R()][vc.C()];
    }
    return Wall;
  }
  int CurrentClock() const { return clock; }
  int CurrentCycle() const { return clock / NTicksPerCycle; }
  Direction CurrentPhasePerCycle() const {
    return Direction((CurrentClock() % NTicksPerCycle) / NTicksPerPhase);
  }
  TickPerCycle CurrentTickPerCycle() const {
    return TickPerCycle(CurrentClock() % NTicksPerCycle);
  }
  TickPerPhase CurrentTickPerPhase() const {
    return TickPerPhase(CurrentClock() % NTicksPerPhase);
  }

 private:
  // Voxel &voxelAt(VoxelCoordinant vc) {
  //   assert(isVoxelCoordinantInBounds(vc));
  //   return (*this)[vc.L()][vc.R()][vc.C()];
  // }
  void ViewFlat(ViewLvlArray &view) const;
  void AddToRod(RodType rodType,
                Direction fwd,
                Direction bwd,
                Item *item,
                VoxelCoordinant const &vc
               );
  Item *FormRodContaining(set<VoxelCoordinant> &seenSofar, VoxelCoordinant const &vc);
  void FindItems();

  Rules rules;
  RuleCounts ruleCounts;
  int clock;
  array<SetOfItems, eoRodType> itemsByRodType;
  array<map<VoxelCoordinant, deque<DataState>>, eoDirection> inputs;
};

class Item: public set<VoxelCoordinant> {
 public:
  Item(RodType t):
      set<VoxelCoordinant>(),
      rodType(t),
      lastMovedAt(-1),
      fBlkState(fbUnkn),
      rBlkState(rbUnkn)
  {}
  virtual ~Item() {}

  virtual bool IsFBlked() const { return fBlkState == fbBlkd; }
  virtual bool IsRBlked() const { return rBlkState == rbBlkd; }
  virtual bool IsBlocked() const { return IsFBlked() || IsRBlked(); };
  virtual bool CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd) = 0;
  virtual bool CheckForFreedomOfMovement(Volume *volume) = 0;
  virtual bool AttemptToMove
      (Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes);
  virtual bool IsValid(Volume const *volume);
  virtual RodType GetRodType() const { return rodType; }
  virtual LockState GetCurrentLockState() const { return lsUnkn; }
  virtual DataState GetCurrentDataState() const { return dsUnkn; }
  virtual void Dump(Volume const *volume = 0) const;
  virtual char const *TypeName() const { return "Item"; }

 protected:
  RodType rodType;
  int lastMovedAt;
  FBlkState fBlkState;
  RBlkState rBlkState;
};

class LockRod: public Item {
 public:
  LockRod(RodType t): Item(t), lockState(lsUnkn) {}

  bool IsLocked() const { return lockState == lsLckd; }

  bool CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd);
  bool CheckForFreedomOfMovement(Volume *volume);
  // bool AttemptToMove(Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes);
  bool IsValid(Volume const *volume);
  virtual LockState GetCurrentLockState() const { return lockState; }
  void Dump(Volume const *volume = 0) const;
  char const *TypeName() const { return "LockRod"; }

 private:
  LockState lockState;
};

class DataRod: public Item {
 public:
  DataRod(RodType t): Item(t), lockState(lsUnkn), dataState(dsUnkn) {}

  bool IsLocked() const { return lockState == lsLckd; }

  bool IsFBlked() const { return Item::IsFBlked() || IsLocked(); }
  bool IsRBlked() const { return Item::IsRBlked() || IsLocked(); }
  bool IsBlocked() const { return Item::IsBlocked() || IsLocked(); };

  bool CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd);
  bool CheckForFreedomOfMovement(Volume *volume);
  // bool AttemptToMove(Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes);
  bool IsValid(Volume const *volume);
  virtual LockState GetCurrentLockState() const { return lockState; }
  virtual DataState GetCurrentDataState() const { return dataState; }
  void Dump(Volume const *volume = 0) const;
  char const *TypeName() const { return "DataRod"; }

 private:
  LockState lockState;
  DataState dataState;
};

void VoxelCoordinant::Dump(Volume const *volume) const {
  if (volume) {
    Voxel v = volume->voxelAt(*this);
    fprintf(stdout,
            "%s(%d,%d,%d)",
            toConstCharPointer(v),
            l,
            r,
            c
           );
  } else {
    fprintf(stdout,
            "(VoxelCoordinant *)(%p)->{ l=%d, r=%d, c=%d }",
            this,
            l,
            r,
            c
           );
  }
}

Scenario::Scenario(Volume const *volume,
                   TickPerCycle t,
                   VoxelCoordinant const &vc
                  ) :
    tick(t),
    self(volume->voxelAt(vc))
{
  area[E] = volume->voxelAt(vc.To(E));
  area[S] = volume->voxelAt(vc.To(S));
  area[D] = volume->voxelAt(vc.To(D));
  area[W] = volume->voxelAt(vc.To(W));
  area[N] = volume->voxelAt(vc.To(N));
  area[U] = volume->voxelAt(vc.To(U));
}

Volume::Volume(VolArray const &initial) :
    VolArray(initial),
    clock(0)
{
  FindItems();
}

Volume::Volume(Voxel initialVoxel) : clock(0) {
  for (int l = 0; l < NLvls; l += 1) {
    for (int r = 0; r < NRows; r += 1) {
      for (int c = 0; c < NCols; c += 1) {
        (*this)[l][r][c] = initialVoxel;
      }
    }
  }
  FindItems();
}

void Volume::ProceedOneCycle() {
  size_t eoTick = clock + NTicksPerCycle;
  while (clock < eoTick) {
    ProceedOneTick();
  }
}

void Volume::ProceedOnePhase() {
  size_t eoTick = clock + NTicksPerPhase;
  while (clock < eoTick) {
    ProceedOneTick();
  }
}

void Volume::ProceedOneTick() {
  if (clock % NTicksPerPhase == 0) {
    Direction d = Direction((clock / NTicksPerPhase) % NPhasesPerCycle);
    for (auto &vci : inputs[d]) {
      if (!vci.second.empty()) {
        vci.second.pop_front();
      }
    }
  }

  size_t tick = clock % NTicksPerCycle;
  TickPerCycleProperties const &tProperties = tickPerCycleProperties[tick];
  if (optVerbose) {
    fprintf(stdout,
            "(Volume *)(%p)->ProceedOneTick(): "
            "tick=%s, tProperties.rodType=%s, tProperties.fwdOrBwd=%s\n",
            this,
            toConstCharPointer(TickPerCycle(tick)),
            toConstCharPointer(tProperties.rodType),
            toConstCharPointer(tProperties.fwdOrBwd)
            );
  }

  Changes changes;
  for (Item *i : itemsByRodType[tProperties.rodType]) {
    i->AttemptToMove(this, tProperties.fwdOrBwd, changes);
  }
  for (auto const &c : changes) {
    VoxelCoordinant vc = c.first;
    assert(isVoxelCoordinantInBounds(vc));
    (*this)[vc.L()][vc.R()][vc.C()] = c.second;
  }
  clock += 1;
}

void Volume::PrintViewFlat() const {
  ViewLvlArray view;

  ViewFlat(view);

  fprintf(stdout, "   ");
  for (int c = 0; c < NCols; c += 1) {
    if (int cc = (c / 10) % 10) {
      fprintf(stdout, "%1d", cc);
    } else {
      fprintf(stdout, " ");
    }
  }
  fprintf(stdout, "\n   ");
  for (int c = 0; c < NCols; c += 1) {
    fprintf(stdout, "%1d", c % 10);
  }
  fprintf(stdout, "\n");
  for (int r = 0; r < NRows; r += 1) {
    fprintf(stdout, "%2d ", r % 100);
    for (int c = 0; c < NCols; c += 1) {
      fprintf(stdout, "%c", view[r][c]);
    }
    fprintf(stdout, " %2d\n", r % 100);
  }
  fprintf(stdout, "   ");
  for (int c = 0; c < NCols; c += 1) {
    if (int cc = (c / 10) % 10) {
      fprintf(stdout, "%1d", cc);
    } else {
      fprintf(stdout, " ");
    }
  }
  fprintf(stdout, "\n   ");
  for (int c = 0; c < NCols; c += 1) {
    fprintf(stdout, "%1d", c % 10);
  }
  fprintf(stdout, "\n");
  fprintf(stdout,
          "Clock: %d, tick %s (cycle: %d, phase: %s, minor tick: %s)\n\n",
          CurrentClock(),
          toConstCharPointer(CurrentTickPerCycle()),
          CurrentCycle(),
          toConstCharPointer(CurrentPhasePerCycle()),
          toConstCharPointer(CurrentTickPerPhase())
         );
}

void Volume::ViewFlat(ViewLvlArray &view) const {
  for (int r = 0; r < NRows; r += 1) {
    for (int c = 0; c < NCols; c += 1) {
      int lOfMaxDisplayPriority = NLvls;
      int maxDisplayPriority = -1;
      for (int l = NLvls - 1; 0 <= l; l -= 1) {
        Voxel const &voxel = (*this)[l][r][c];
        VoxelProperties const &vProperties = voxelProperties[voxel];
        if (maxDisplayPriority < vProperties.displayPriority) {
          maxDisplayPriority = vProperties.displayPriority;
          lOfMaxDisplayPriority = l;
        }
      }
      view[r][c] = voxelProperties[(*this)[lOfMaxDisplayPriority][r][c]].text;
    }
  }
}

// Given a voxel's coordinant, representing the first voxel of a
// {lock,data} rod, create an appropriate {Lock,Data}Rod item, and
// populate it with all attached voxels (and slots).

bool Volume::isPartOf(RodType r, VoxelCoordinant const &vc, bool &reachedEnd) const {
  if (isVoxelCoordinantInBounds(vc)) {
    Voxel voxel = voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[voxel];
    if (!reachedEnd) {
      if (vProperties.IsOneOf(r)) {
        return true;
      }
      reachedEnd = true;
    }
    return voxel == Slot;
  }
  reachedEnd = true;
  return false;
}

bool Volume::isPartOf(RodType r, VoxelCoordinant const &vc) const {
  if (isVoxelCoordinantInBounds(vc)) {
    Voxel voxel = voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[voxel];
    return vProperties.IsOneOf(r);
  }
  return false;
}

void Volume::AddToRod(RodType rodType, Direction fwd, Direction bwd, Item *item, VoxelCoordinant const &vc) {
  item->insert(vc);
  bool reachedFwdEnd = false;
  for (VoxelCoordinant fc = vc.To(fwd);
       isPartOf(rodType, fc, reachedFwdEnd);
       fc.Move(fwd)
      )
  {
    item->insert(fc);
    if (!reachedFwdEnd) {
      VoxelCoordinant uc = fc.ToU();
      if (isPartOf(rodType, uc) && item->find(uc) == item->end()) {
	AddToRod(rodType, fwd, bwd, item, uc);
      }
      VoxelCoordinant dc = fc.ToD();
      if (isPartOf(rodType, dc) && item->find(dc) == item->end()) {
	AddToRod(rodType, fwd, bwd, item, dc);
      }
    }
  }
  bool reachedBwdEnd = false;
  for (VoxelCoordinant bc = vc.To(bwd);
       isPartOf(rodType, bc, reachedBwdEnd);
       bc.Move(bwd)
      )
  {
    item->insert(bc);
    if (!reachedBwdEnd) {
      VoxelCoordinant uc = bc.ToU();
      if (isPartOf(rodType, uc) && item->find(uc) == item->end()) {
	AddToRod(rodType, fwd, bwd, item, uc);
      }
      VoxelCoordinant dc = bc.ToD();
      if (isPartOf(rodType, dc) && item->find(dc) == item->end()) {
	AddToRod(rodType, fwd, bwd, item, dc);
      }
    }
  }
}

Item *Volume::FormRodContaining
  (set<VoxelCoordinant> &seenSofar, VoxelCoordinant const &vc)
{
  Voxel v = voxelAt(vc);
  VoxelProperties const &vProperties = voxelProperties[v];
  RodType rodType = vProperties.rodType[0];
  RodTypeProperties const &rProperties = rodTypeProperties[rodType];
  Item *item = 0;
  switch (rodType) {
    case rtLE: case rtLS: case rtLW: case rtLN:
      item = new LockRod(rodType);
      break;
    case rtDE: case rtDS: case rtDW: case rtDN:
      item = new DataRod(rodType);
      break;
    default:
      return 0;
  }
  AddToRod(rodType, rProperties.fwd,rProperties.bwd, item, vc);
  seenSofar.insert(item->begin(), item->end());
  if (item->IsValid(this)) {
    itemsByRodType[rodType].insert(item);
    item->CheckForFreedomOfMovement(this);
  }

  return item;
}

void Volume::FindItems() {
  for (size_t i = 0; i < eoRodType; i += 1) {
    SetOfItems &items = itemsByRodType[i];
    for (auto i : items) {
      delete i;
    }
    items.clear();
  }
  set<VoxelCoordinant> seenSofar;

  for (int l = 0; l < NLvls; l += 1) {
    for (int r = 0; r < NRows; r += 1) {
      for (int c = 0; c < NCols; c += 1) {
        VoxelCoordinant vc(l, r, c);
        Voxel v = voxelAt(vc);
	if (voxelProperties[v].isRodBody) {
	  if (seenSofar.find(vc) == seenSofar.end()) {
            Item *item = FormRodContaining(seenSofar, vc);
            if (optVerbose) {
              item->Dump(this);
              fprintf(stdout, "\n");
            }
            break;
          }
        }
      }
    }
  }
}

bool Item::AttemptToMove(Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes) {
  TickPerCycle tick = volume->CurrentTickPerCycle();
  TickPerCycleProperties const &tProperties = tickPerCycleProperties[tick];
  assert(rodType == tProperties.rodType);

  if (optVerbose) {
    fprintf(stdout,
            "(%s *)(%p)->AttemptToMove(): fwdOrBwd=%s\n",
            TypeName(),
            this,
            toConstCharPointer(fwdOrBwd)
            );
    Dump(volume);
    fprintf(stdout, "\n");
  }

  if (bool isMovable = CheckForFreedomOfMovement(volume, fwdOrBwd)) {
    RodTypeProperties const &rProperties = rodTypeProperties[rodType];
    Direction directionOfMotion =
        fwdOrBwd == Fwd ? rProperties.fwd : rProperties.bwd;
    Direction antiDirectionOfMotion =
        fwdOrBwd == Fwd ? rProperties.bwd : rProperties.fwd;
    bool madeChanges = false;

#if 1
    for (auto const &c : *this) {
      Scenario scenario(volume, tick, c);
      Voxel thisVoxel = scenario.Self();
      Voxel prevVoxel = Unkn;
      Voxel nextVoxel = Unkn;
      VoxelProperties const &tvProperties = voxelProperties[thisVoxel];

      if (thisVoxel == Slot || rodType == tvProperties.rodType[0]) {
        if (tvProperties.voxelType == vtData &&
            (tvProperties.dataType == dtInpt ||
             tvProperties.dataType == dtOutp
            )
           )
        {
          nextVoxel = tvProperties.motion[fwdOrBwd].nextVoxel;
        } else {
          prevVoxel = scenario.From(antiDirectionOfMotion);
          VoxelProperties const &pvProperties = voxelProperties[prevVoxel];
          if (pvProperties.rodType[0] == rodType) {
            if (pvProperties.voxelType == vtData &&
                (pvProperties.dataType == dtInpt ||
                 pvProperties.dataType == dtOutp
                )
               )
            {
              nextVoxel = tvProperties.motion[fwdOrBwd].nextVoxel;
            } else {
              nextVoxel = pvProperties.motion[fwdOrBwd].nextVoxel;
            }
          } else {
            nextVoxel = Slot;
          }
        }
        if (nextVoxel != Unkn) {
          changes[c] = nextVoxel;
          madeChanges = true;
        } else {
          scenario.Dump();
          fprintf(stdout, "thisVoxel=%s\n", toConstCharPointer(thisVoxel));
          fprintf(stdout, "prevVoxel=%s\n", toConstCharPointer(prevVoxel));
          fprintf(stdout,
                  "nextVoxel(%s) %s Unkn\n",
                  toConstCharPointer(nextVoxel),
                  nextVoxel == Unkn ? "==" : "!="
                 );
          assert(nextVoxel != Unkn);
        }
      // } else {
      //   scenario.Dump();
      //   fprintf(stdout,
      //           "thisVoxel(%s) %s Slot\n",
      //           toConstCharPointer(thisVoxel),
      //           thisVoxel == Slot ? "==" : "!="
      //          );
      //   fprintf(stdout,
      //           "rodType(%s) %s tvProperties[%s](%s)\n",
      //           toConstCharPointer(rodType),
      //           rodType == tvProperties.rodType ? "==" : "!=",
      //           toConstCharPointer(thisVoxel),
      //           toConstCharPointer(tvProperties.rodType)
      //          );
      //   assert(thisVoxel == Slot || rodType == tvProperties.rodType);
      }
    }
#else
    Rules &rules = volume->GetRules();
    RuleCounts &ruleCounts = volume->GetRuleCounts();
    for (auto const &v : *this) {
      Scenario scenario(volume, tick, v);
      if (optVerbose) {
        fprintf(stdout, "(%s *)(%p)->AttemptToMove(): At ", TypeName(), this);
        v.Dump(volume);
        fprintf(stdout, ", ");
        scenario.Show();
        fprintf(stdout, "\n");
      }

      if (rules.find(scenario) == rules.end()) {
#if 0
        TickPerCycleProperties const &tProperties =
            tickPerCycleProperties[tick];
        RodTypeProperties const &rProperties =
            rodTypeProperties[tProperties.rodType];
        Direction fwdOrBwd =
            tProperties.fwdOrBwd == Fwd ? rProperties.fwd : rProperties.bwd;
        Direction fwd = rProperties.fwd;
        char const *directionOfMotion[eoDirection][eoFwdOrBwd] = {
          { ">", "<" }, // E, W
          { "v", "^" }, // S, N
          { "d", "u" }, // D, U
          { "<", ">" }, // W, E
          { "^", "v" }, // N, S
          { "u", "d" }  // U, D
        };

        switch (fwd) {
        case E:
        case W:
          fprintf(stdout,
                  "     +------+------+------|\n"
                  "%s | ???? | %s | ???? |\n"
                  "     +------+------+------|\n"
                  "     %s %s %s %s %s %s %s\n"
                  "     +------+------+------|\n"
                  "     | ???? | %s | ???? |\n"
                  "     +------+------+------|\n",
        	  toConstCharPointer(tick),
        	  toConstCharPointer(scenario.N()),
        	  directionOfMotion[fwd][Fwd],
        	  toConstCharPointer(scenario.W()),
        	  directionOfMotion[fwd][tProperties.fwdOrBwd],
        	  toConstCharPointer(scenario.O()),
        	  directionOfMotion[fwd][tProperties.fwdOrBwd],
        	  toConstCharPointer(scenario.E()),
        	  directionOfMotion[fwd][Fwd],
        	  toConstCharPointer(scenario.S())
        	 );
          break;
        case N:
        case S:
          fprintf(stdout,
                  "     +------+--%s---+------|\n"
                  "%s | ???? | %s | ???? |\n"
                  "     +------+--%s---+------|\n"
                  "     | %s | %s | %s |\n"
                  "     +------+--%s---+------|\n"
                  "     | ???? | %s | ???? |\n"
                  "     +------+--%s---+------|\n",
        	  directionOfMotion[fwd][Fwd],
        	  toConstCharPointer(tick),
        	  toConstCharPointer(scenario.N()),
        	  directionOfMotion[fwd][tProperties.fwdOrBwd],
        	  toConstCharPointer(scenario.W()),
        	  toConstCharPointer(scenario.O()),
        	  toConstCharPointer(scenario.E()),
        	  directionOfMotion[fwd][tProperties.fwdOrBwd],
        	  toConstCharPointer(scenario.S()),
        	  directionOfMotion[fwd][Fwd]
        	 );
          break;
        default:
          break;
        }

        char newVoxelChars[128];
        do {
          fprintf(stdout, "New voxel value? ");
          fscanf(stdin, " %s", newVoxelChars);
        } while (stringToVoxel.find(newVoxelChars) == stringToVoxel.end());

        Voxel newVoxel = stringToVoxel[newVoxelChars];
        rules[scenario] = newVoxel;
#else
        Voxel newVoxel = Unkn;
        rules[scenario] = newVoxel;
        fprintf(stdout,
                "(%s *)(%p)->AttemptToMove(): No existing rule!\n",
                TypeName(),
                this
               );
        continue;
#endif
        // Rule newRule(scenario, newVoxel);
        // newRule.Dump();
        // fprintf(stdout, "\n");
      }

      ruleCounts[scenario] += 1;
      Voxel newVoxel = rules[scenario];

      // if (newVoxel == Unkn) {
      //   fprintf(stdout,
      //           "(%s *)(%p)->AttemptToMove(): Incomplete (i.e. new) rule!\n",
      //           TypeName(),
      //           this
      //          );
      //   continue;
      // }

      if (optVerbose) {
        fprintf(stdout,
                "(%s *)(%p)->AttemptToMove(): Applying rule ",
                TypeName(),
                this
               );
        scenario.Dump();
        fprintf(stdout, " = %s\n", toConstCharPointer(newVoxel));
      }

      changes[v] = newVoxel;
      madeChanges = true;
    }
#endif

    if (madeChanges) {
      lastMovedAt = volume->CurrentClock();
    }
    return true;
  } else {
    RodTypeProperties const &rProperties = rodTypeProperties[rodType];
    LockState lockState = GetCurrentLockState();
    DataState dataState = GetCurrentDataState();
    bool rodIsInMoveFromState = false;

    switch (rProperties.voxelType) {
      case vtLock:
        rodIsInMoveFromState = lockState == tProperties.moveFromLockState;
        break;
      case vtData:
        rodIsInMoveFromState = dataState == tProperties.moveFromDataState;
        break;
      default:
        rodIsInMoveFromState = false;
    }

    // if (rodIsInMoveFromState && tProperties.mustBeFreeToMove) {
    //   fprintf(stdout,
    //           "(%s *)(%p)->AttemptToMove(): Unable to move %s during %s: ",
    //           TypeName(),
    //           this,
    //           toConstCharPointer(fwdOrBwd),
    //           toConstCharPointer(tick)
    //           );
    //   Dump(volume);
    //   fprintf(stdout, "\n");
    //   assert(isMovable || !tProperties.mustBeFreeToMove);
    // }
  }
  return false;
}

bool Item::IsValid(Volume const *volume) {
  array<size_t, eoRodType> rodTypeCounts;

  rodTypeCounts.fill(0);
  bool foundMoreThan1RodType = false;
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];
    if (vProperties.voxelType != vtSlot) {
      foundMoreThan1RodType |= !vProperties.IsOneOf(rodType);
      rodTypeCounts[vProperties.rodType[0]] += 1;
    }
  }
  if (foundMoreThan1RodType) {
    fprintf(stdout,
            "(%s *)(%p)->IsValid(volume=%p): foundMoreThan1RodType!",
            TypeName(),
	    this,
            volume
           );
    fprintf(stdout,
            "  rodTypeCounts={ [%s]%lu",
            toConstCharPointer(RodType(0)),
            rodTypeCounts[0]
           );
    for (size_t t = 1; t < eoRodType; t += 1) {
      fprintf(stdout,
              ", [%s]%lu",
              toConstCharPointer(RodType(t)),
              rodTypeCounts[t]
              );
    }
    fprintf(stdout, " }\n");
  }
  return !foundMoreThan1RodType;
}

void Item::Dump(Volume const *volume) const {
  fprintf(stdout, "(Item *)(%p)->{", this);
  fprintf(stdout, " rodType=%s, ", toConstCharPointer(rodType));
  fprintf(stdout, " fBlkState=%s, ", toConstCharPointer(fBlkState));
  fprintf(stdout, " rBlkState=%s, {", toConstCharPointer(rBlkState));
  char const *comma = "";
  for (auto const &c : *this) {
    fprintf(stdout, "%s ", comma);
    c.Dump(volume);
    comma = ",";
  }
  fprintf(stdout, " }");
  fprintf(stdout, " }");
}

bool LockRod::CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd) {
  return CheckForFreedomOfMovement(volume) ||
      fwdOrBwd == Fwd ? !IsFBlked() : !IsRBlked();
}

bool LockRod::CheckForFreedomOfMovement(Volume *volume) {
  size_t lockStateCounts[eoLockState] = { 0 };
  bool foundMoreThan1LockState = false;
  lockState = lsUnkn;

  fBlkState = fbUnkn;
  rBlkState = rbUnkn;

  size_t fBlkCounts = 0;
  size_t rBlkCounts = 0;
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];

    if (v == Slot) {
      continue;
    }

    if (rodType != vProperties.rodType[0]) {
      if (vProperties.voxelType == vtData && vProperties.dataType == dtLock) {
	if (optVerbose) {
	  fprintf(stdout,
		  "(LockRod *)(%p) rBlked by %s(%d,%d,%d)\n",
		  this,
		  toConstCharPointer(v),
		  vc.L(),
		  vc.R(),
		  vc.C()
		  );
	}
	rBlkCounts += 1;
      }
      continue;
    } else if (vProperties.lockState != lsUnkn) {
      lockStateCounts[vProperties.lockState] += 1;
      foundMoreThan1LockState |=
	lockState != lsUnkn &&
	lockState != vProperties.lockState;
      lockState = vProperties.lockState;
    }

    if (blockStateProperties[vProperties.blockableStates].isFBlkable) {
      VoxelCoordinant fc = vc.To(rodTypeProperties[rodType].fwd);
      if (volume->isVoxelCoordinantInBounds(fc)) {
        Voxel f = volume->voxelAt(fc);
        if (find(fc) == end() && f != Slot) {
          if (optVerbose) {
            fprintf(stdout,
                    "(LockRod *)(%p) fBlked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
                    this,
                    toConstCharPointer(v),
                    vc.L(),
                    vc.R(),
                    vc.C(),
                    toConstCharPointer(f),
                    fc.L(),
                    fc.R(),
                    fc.C()
                    );
          }
          fBlkCounts += 1;
        }
      } else {
        if (optVerbose) {
          fprintf(stdout,
                  "(LockRod *)(%p) fBlked at %s(%d,%d,%d) by edge\n",
                  this,
                  toConstCharPointer(v),
                  vc.L(),
                  vc.R(),
                  vc.C()
                  );
        }
        fBlkCounts += 1;
      }
    }
    if (blockStateProperties[vProperties.blockableStates].isRBlkable) {
      VoxelCoordinant bc = vc.To(rodTypeProperties[rodType].bwd);
      if (volume->isVoxelCoordinantInBounds(bc)) {
        Voxel b = volume->voxelAt(bc);
        if (find(bc) == end() && b != Slot) {
          if (optVerbose) {
            fprintf(stdout,
                    "(LockRod *)(%p) rBlked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
                    this,
                    toConstCharPointer(v),
                    vc.L(),
                    vc.R(),
                    vc.C(),
                    toConstCharPointer(b),
                    bc.L(),
                    bc.R(),
                    bc.C()
                    );
          }
          rBlkCounts += 1;
        }
      } else {
        if (optVerbose) {
          fprintf(stdout,
                  "(LockRod *)(%p) rBlked at %s(%d,%d,%d) by edge\n",
                  this,
                  toConstCharPointer(v),
                  vc.L(),
                  vc.R(),
                  vc.C()
                  );
        }
        rBlkCounts += 1;
      }
    }
  }

  fBlkState = fBlkCounts != 0 ? fbBlkd : fbUnbk;
  rBlkState = rBlkCounts != 0 ? rbBlkd : rbUnbk;

  return !IsBlocked();
}

// bool LockRod::AttemptToMove
//     (Volume *volume,
//      FwdOrBwd fwdOrBwd,
//      Changes &changes
//     )
// {
//   bool isMovable = CheckForFreedomOfMovement(volume, fwdOrBwd);
//
//   if (optVerbose) {
//     fprintf(stdout,
//             "(LockRod *)(%p)->AttemptToMove(): fwdOrBwd=%s\n",
//             this,
//             toConstCharPointer(fwdOrBwd)
//             );
//     Dump(volume);
//     fprintf(stdout, "\n");
//   }
//
//   if (isMovable) {
//     TickPerCycle tick = volume->CurrentTickPerCycle();
//     TickPerCycleProperties const &tProperties = tickPerCycleProperties[tick];
//     assert(rodType == tProperties.rodType);
//     RodTypeProperties const &rProperties = rodTypeProperties[rodType];
//     VoxelType rodVoxelType = vtLock;
//     Direction directionOfMotion =
//         fwdOrBwd == Fwd ? rProperties.fwd : rProperties.bwd;
//     Direction antiDirectionOfMotion =
//         fwdOrBwd == Fwd ? rProperties.bwd : rProperties.fwd;
//     bool madeChanges = false;
//
//     for (auto const &c : *this) {
//       Scenario scenario(volume, tick, c);
//       Voxel thisVoxel = scenario.Self();
//       Voxel prevVoxel = Unkn;
//       Voxel nextVoxel = Unkn;
//       VoxelProperties const &tvProperties = voxelProperties[thisVoxel];
//
//       if (thisVoxel == Slot || rodType == tvProperties.rodType[0]) {
//         prevVoxel = scenario.From(antiDirectionOfMotion);
//         VoxelProperties const &pvProperties = voxelProperties[prevVoxel];
//         if (pvProperties.IsOneOf(rodType)) {
//           nextVoxel = pvProperties.motion[fwdOrBwd].nextVoxel;
//         } else {
//           nextVoxel = Slot;
//         }
//       } else {
//         assert(tvProperties.voxelType == vtData);
//         assert(tvProperties.dataType == dtSlot ||
//                tvProperties.dataType == dtJoin
//               );
//       }
//
//       if (nextVoxel != Unkn) {
//         changes[c] = nextVoxel;
//         madeChanges = true;
//       } else {
//         scenario.Dump();
//         fprintf(stdout, "thisVoxel=%s\n", toConstCharPointer(thisVoxel));
//         fprintf(stdout, "prevVoxel=%s\n", toConstCharPointer(prevVoxel));
//         fprintf(stdout,
//                 "nextVoxel(%s) %s Unkn\n",
//                 toConstCharPointer(nextVoxel),
//                 nextVoxel == Unkn ? "==" : "!="
//                );
//         assert(nextVoxel != Unkn);
//       }
//     }
//     if (madeChanges) {
//       lastMovedAt = volume->CurrentClock();
//     }
//     return true;
//   }
//   return false;
// }

bool LockRod::IsValid(Volume const *volume) {
  if (!Item::IsValid(volume)) {
    return false;
  }

  array<size_t, eoRodType> lockStateCounts;

  lockStateCounts.fill(0);

  bool foundMoreThan1LockState = false;
  lockState = eoLockState;
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];
    if (vProperties.voxelType != vtSlot) {
      lockStateCounts[vProperties.lockState] += 1;
      foundMoreThan1LockState |=
          lockState != eoLockState &&
          lockState != vProperties.lockState;
      lockState = vProperties.lockState;
    }
  }
  if (foundMoreThan1LockState) {
    fprintf(stdout,
            "(LockRod *)(%p)->IsValid(volume=%p): foundMoreThan1LockState!",
            this,
            volume
           );
    fprintf(stdout,
            "  lockStateCounts={ [%s]%lu",
            toConstCharPointer(LockState(0)),
            lockStateCounts[0]
           );
    for (size_t t = 1; t < eoLockState; t += 1) {
      fprintf(stdout,
              ", [%s]%lu",
              toConstCharPointer(LockState(t)),
              lockStateCounts[t]
              );
    }
    fprintf(stdout, " }\n");
  }
  return !foundMoreThan1LockState;
}

void LockRod::Dump(Volume const *volume) const {
  fprintf(stdout, "(LockRod *)(%p)->{ ", this);
  fprintf(stdout, " lockState=%s, ", toConstCharPointer(lockState));
  Item::Dump(volume);
  fprintf(stdout, " }");
}

bool DataRod::CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd) {
  return CheckForFreedomOfMovement(volume) ||
      fwdOrBwd == Fwd ? !IsFBlked() : !IsRBlked();
}

bool DataRod::CheckForFreedomOfMovement(Volume *volume) {
  size_t dataStateCounts[eoDataState] = { 0 };
  bool foundMoreThan1DataState = false;
  dataState = dsUnkn;

  fBlkState = fbUnkn;
  rBlkState = rbUnkn;
  lockState = lsUnkn;

  size_t fBlkCounts = 0;
  size_t rBlkCounts = 0;
  size_t lockCounts = 0;
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];

    if (v == Slot) {
      continue;
    }

    // if (rodType != vProperties.rodType[0]) {
    //   if (vProperties.voxelType == vtData &&
    //       vProperties.dataType == dtJoin
    //      )
    //   {
    //     if (optVerbose) {
    //       fprintf(stdout,
    //     	  "(LockRod *)(%p) fBlked by %s(%d,%d,%d)\n",
    //     	  this,
    //     	  toConstCharPointer(v),
    //     	  vc.L(),
    //     	  vc.R(),
    //     	  vc.C()
    //     	  );
    //     }
    //     fBlkCounts += 1;
    //   }
    //   continue;
    // }
    if (vProperties.dataState != dsUnkn) {
      dataStateCounts[vProperties.dataState] += 1;
      foundMoreThan1DataState |=
	dataState != dsUnkn &&
	dataState != vProperties.dataState;
      dataState = vProperties.dataState;
    }

    if (blockStateProperties[vProperties.blockableStates].isFBlkable) {
      if (vProperties.dataType == dtInpt) {
        TickPerPhase tickPerPhase = volume->CurrentTickPerPhase();
        deque<DataState> &values = volume->GetInputsFor(vc);
        DataState inputState = values.empty() ? dsSet0 : values.front();
        // bool fBlockedByInput = false;
        // switch (tickPerPhase) {
        //   case SetDataRodsTo0:
        //     fBlockedByInput = inputState < dsSet0;
        //     break;
        //   case SetDataRodsTo1:
        //     fBlockedByInput = inputState < dsSet1;
        //   default:
        //     break;
        // }
        bool fBlockedByInput = inputState <= dataState;
        if (fBlockedByInput) {
          fBlkCounts += 1;
          if (optVerbose) {
            fprintf(stdout,
                    "(DataRod *)(%p) fBlked at %s(%d,%d,%d) by input (%s)\n",
                    this,
                    toConstCharPointer(v),
                    vc.L(),
                    vc.R(),
                    vc.C(),
                    toConstCharPointer(inputState)
                    );
          }
        }
      } else {
        VoxelCoordinant fc = vc.To(rodTypeProperties[rodType].fwd);
        if (volume->isVoxelCoordinantInBounds(fc)) {
          Voxel f = volume->voxelAt(fc);
          VoxelProperties const &fvProperties = voxelProperties[f];
          if (!fvProperties.IsOneOf(rodType) && f != Slot) {
            if (optVerbose) {
              fprintf(stdout,
                      "(DataRod *)(%p) fBlked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
                      this,
                      toConstCharPointer(v),
                      vc.L(),
                      vc.R(),
                      vc.C(),
                      toConstCharPointer(f),
                      fc.L(),
                      fc.R(),
                      fc.C()
                      );
            }
            fBlkCounts += 1;
          }
        } else {
          if (optVerbose) {
            fprintf(stdout,
                    "(DataRod *)(%p) fBlked at %s(%d,%d,%d) by edge\n",
                    this,
                    toConstCharPointer(v),
                    vc.L(),
                    vc.R(),
                    vc.C()
                    );
          }
          fBlkCounts += 1;
        }
      }
    }
    if (blockStateProperties[vProperties.blockableStates].isRBlkable) {
      VoxelCoordinant bc = vc.To(rodTypeProperties[rodType].bwd);
      if (volume->isVoxelCoordinantInBounds(bc)) {
        Voxel b = volume->voxelAt(bc);
        VoxelProperties const &bvProperties = voxelProperties[b];
        if (!bvProperties.IsOneOf(rodType) && b != Slot) {
          if (optVerbose) {
            fprintf(stdout,
                    "(DataRod *)(%p) rBlked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
                    this,
                    toConstCharPointer(v),
                    vc.L(),
                    vc.R(),
                    vc.C(),
                    toConstCharPointer(b),
                    bc.L(),
                    bc.R(),
                    bc.C()
                    );
          }
          rBlkCounts += 1;
        }
      } else {
        if (optVerbose) {
          fprintf(stdout,
                  "(DataRod *)(%p) rBlked at %s(%d,%d,%d) by edge\n",
                  this,
                  toConstCharPointer(v),
                  vc.L(),
                  vc.R(),
                  vc.C()
                  );
        }
        rBlkCounts += 1;
      }
    }
    if (blockStateProperties[vProperties.blockableStates].isLockable) {
      if (vProperties.dataType == dtLock) {
        if (optVerbose) {
          fprintf(stdout,
                  "(DataRod *)(%p) locked at %s(%d,%d,%d)\n",
                  this,
                  toConstCharPointer(v),
                  vc.L(),
                  vc.R(),
                  vc.C()
                  );
        }
        lockCounts += 1;
      }
    }
  }

  fBlkState = fBlkCounts != 0 ? fbBlkd : fbUnbk;
  rBlkState = rBlkCounts != 0 ? rbBlkd : rbUnbk;
  lockState = lockCounts != 0 ? lsLckd : lsUnlk;
  return !IsBlocked();
}

// bool DataRod::AttemptToMove
//     (Volume *volume,
//      FwdOrBwd fwdOrBwd,
//      Changes &changes
//     )
// {
//   bool isMovable = CheckForFreedomOfMovement(volume, fwdOrBwd);
//
//   if (optVerbose) {
//     fprintf(stdout,
//             "(DataRod *)(%p)->AttemptToMove(): fwdOrBwd=%s\n",
//             this,
//             toConstCharPointer(fwdOrBwd)
//             );
//     Dump(volume);
//     fprintf(stdout, "\n");
//   }
//
//   if (isMovable) {
//     TickPerCycle tick = volume->CurrentTickPerCycle();
//     TickPerCycleProperties const &tProperties = tickPerCycleProperties[tick];
//     assert(rodType == tProperties.rodType);
//     RodTypeProperties const &rProperties = rodTypeProperties[rodType];
//     VoxelType rodVoxelType = rProperties.voxelType;
//     Direction directionOfMotion =
//         fwdOrBwd == Fwd ? rProperties.fwd : rProperties.bwd;
//     Direction antiDirectionOfMotion =
//         fwdOrBwd == Fwd ? rProperties.bwd : rProperties.fwd;
//     bool madeChanges = false;
//
//     for (auto const &c : *this) {
//       Scenario scenario(volume, tick, c);
//       Voxel thisVoxel = scenario.Self();
//       Voxel prevVoxel = Unkn;
//       Voxel nextVoxel = Unkn;
//       VoxelProperties const &tvProperties = voxelProperties[thisVoxel];
//
//       if (thisVoxel == Slot || rodType == tvProperties.rodType[0]) {
//         prevVoxel = scenario.From(antiDirectionOfMotion);
//         VoxelProperties const &pvProperties = voxelProperties[prevVoxel];
//         if (pvProperties.IsOneOf(rodType)) {
//           if (pvProperties.voxelType == vtData &&
//               (pvProperties.dataType == dtInpt ||
//                pvProperties.dataType == dtOutp
//                )
//               )
//           {
//             nextVoxel = tvProperties.motion[fwdOrBwd].nextVoxel;
//           } else {
//             nextVoxel = pvProperties.motion[fwdOrBwd].nextVoxel;
//           }
//         } else {
//           nextVoxel = Slot;
//         }
//         if (nextVoxel != Unkn) {
//           changes[c] = nextVoxel;
//           madeChanges = true;
//         } else {
//           scenario.Dump();
//           fprintf(stdout, "thisVoxel=%s\n", toConstCharPointer(thisVoxel));
//           fprintf(stdout, "prevVoxel=%s\n", toConstCharPointer(prevVoxel));
//           fprintf(stdout,
//                   "nextVoxel(%s) %s Unkn\n",
//                   toConstCharPointer(nextVoxel),
//                   nextVoxel == Unkn ? "==" : "!="
//                  );
//           assert(nextVoxel != Unkn);
//         }
//       }
//     }
//     if (madeChanges) {
//       lastMovedAt = volume->CurrentClock();
//     }
//     return true;
//   }
//   return false;
// }

bool DataRod::IsValid(Volume const *volume) {
  if (!Item::IsValid(volume)) {
    return false;
  }

  array<size_t, eoRodType> lockStateCounts;
  array<size_t, eoRodType> dataStateCounts;

  lockStateCounts.fill(0);
  dataStateCounts.fill(0);

  bool foundMoreThan1LockState = false;
  lockState = eoLockState;
  bool foundMoreThan1DataState = false;
  dataState = eoDataState;
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];
    LockState lockState = vProperties.lockState;
    DataState dataState = vProperties.dataState;
    if (vProperties.voxelType != vtSlot) {
      lockStateCounts[vProperties.lockState] += 1;
      foundMoreThan1LockState |=
          lockState != eoLockState &&
          lockState != vProperties.lockState;
      lockState = vProperties.lockState;
      dataStateCounts[vProperties.dataState] += 1;
      foundMoreThan1DataState |=
          dataState != eoDataState &&
          dataState != vProperties.dataState;
      dataState = vProperties.dataState;
    }
  }
  if (foundMoreThan1LockState) {
    fprintf(stdout,
            "(DataRod *)(%p)->IsValid(volume=%p): foundMoreThan1LockState!",
            this,
            volume
           );
    fprintf(stdout,
            "  lockStateCounts={ [%s]%lu",
            toConstCharPointer(LockState(0)),
            lockStateCounts[0]
           );
    for (size_t t = 1; t < eoLockState; t += 1) {
      fprintf(stdout,
              ", [%s]%lu",
              toConstCharPointer(LockState(t)),
              lockStateCounts[t]
              );
    }
    fprintf(stdout, " }\n");
  }
  if (foundMoreThan1DataState) {
    fprintf(stdout,
            "(DataRod *)(%p)->IsValid(volume=%p): foundMoreThan1DataState!",
            this,
            volume
           );
    fprintf(stdout,
            "  dataStateCounts={ [%s]%lu",
            toConstCharPointer(DataState(0)),
            dataStateCounts[0]
           );
    for (size_t t = 1; t < eoDataState; t += 1) {
      fprintf(stdout,
              ", [%s]%lu",
              toConstCharPointer(DataState(t)),
              dataStateCounts[t]
              );
    }
    fprintf(stdout, " }\n");
  }
  return !foundMoreThan1LockState && !foundMoreThan1DataState;
}

void DataRod::Dump(Volume const *volume) const {
  fprintf(stdout, "(DataRod *)(%p)->{", this);
  fprintf(stdout, " lockState=%s, ", toConstCharPointer(lockState));
  fprintf(stdout, " dataState=%s, ", toConstCharPointer(dataState));
  Item::Dump(volume);
  fprintf(stdout, " }");
}

  //   abcdefghijklmnopqrstuvwxyzABCDEF
  // a .......v............... ........ a
  // b .......|.......>...>...^........ b
  // c .......|.......|...|...|........ c
  // d .......|.......>...>...|........ d
  // e  <------------>>-->>-----------< e
  // f .......|.......|...|...|........ f
  // g .......|.......v...v...|........ g
  // h .......|.......|...|...|........ h
  // i I---^-^^----0--|--1|>..|.......C i
  // j .......^.......|...|...|........ j
  // k .......|.......|...v...|........ k
  // l .......|.......|...|...|........ l
  // m I---^-^^-------|--1|>..|.......D m
  // n .......^.......|...|...|........ n
  // o .......|.......v...|...|........ o
  // p .......|...^...|...|...|........ p
  // q ....^-^^->-|--1|---|-----------O q
  // r .......^...1...0...0...|........ r
  // s .......|...|...|...|...|........ s
  // t .......|...|...|...|...v........ t
  // u .......|..<|--0|-<-|-<-vv-v..... u
  // v .......|...|...v...v...|........ v
  // w .......|...^...........|........ w
  // x .......|...|...........|........ x
  // y >----------<<----------------->  y
  // z .......|...<...........|........ z
  // A .......|...|...........|........ A
  // B .......v...<...........|........ B
  // C ....... ...............^........ C
  //   abcdefghijklmnopqrstuvwxyzABCDEF

VolArray initialVolume = {
  //   abcdefghijklmnopqrstuvwxyzABCDEF
  // a .......v............... ........ a
  // b .......|...............^........ b
  // c .......|...............|........ c
  // d .......|...............|........ d
  // e .......|...............|........ e
  // f .......|...............|........ f
  // g .......|...............|........ g
  // h .......|...............|........ h
  // i .......|...............|........ i
  // j .......+...............|........ j
  // k .......|...............|........ k
  // l .......|...............|........ l
  // m .......|...............|........ m
  // n .......+...............|........ n
  // o .......|...............|........ o
  // p .......|...............|........ p
  // q .......|...............|........ q
  // r .......+...............|........ r
  // s .......|...............|........ s
  // t .......|...............+........ t
  // u .......|...............|........ u
  // v .......|...............|........ v
  // w .......|...............|........ w
  // x .......|...............|........ x
  // y .......|...............|........ y
  // z .......|...............|........ z
  // A .......|...............|........ A
  // B .......v...............|........ B
  // C ....... ...............^........ C
  //   abcdefghijklmnopqrstuvwxyzABCDEF

  /*        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y    z    A    B    C    D    E    F        */
  /* a */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LTSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* a */
  /* b */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LHNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* b */
  /* c */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* c */
  /* d */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* d */
  /* e */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* e */
  /* f */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* f */
  /* g */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* g */
  /* h */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* h */
  /* i */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* i */
  /* j */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LPSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* j */
  /* k */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* k */
  /* l */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* l */
  /* m */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* m */
  /* n */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LPSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* n */
  /* o */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* o */
  /* p */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* p */
  /* q */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* q */
  /* r */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LPSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* r */
  /* s */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* s */
  /* t */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LPNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* t */
  /* u */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* u */
  /* v */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* v */
  /* w */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* w */
  /* x */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* x */
  /* y */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* y */
  /* z */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* z */
  /* A */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* A */
  /* B */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LHSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* B */
  /* C */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LTNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* C */
  /*        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y    z    A    B    C    D    E    F        */

  //   abcdefghijklmnopqrstuvwxyzABCDEF
  // a ................................ a
  // b ................................ b
  // c ................................ c
  // d ................................ d
  // e ................................ e
  // f ................................ f
  // g ................................ g
  // h ................................ h
  // i ....^ ^^   ..................... i
  // j .......^........................ j
  // k ....... ........................ k
  // l ................................ l
  // m ....^ ^^   ..................... m
  // n .......^........................ n
  // o ....... ........................ o
  // p ................................ p
  // q ....^ ^^   ..................... q
  // r .......^........................ r
  // s ....... ............... ........ s
  // t .......................v........ t
  // u ....................   vv v..... u
  // v ................................ v
  // w ................................ w
  // x ................................ x
  // y ................................ y
  // z ................................ z
  // A ................................ A
  // B ................................ B
  // C ................................ C
  //   abcdefghijklmnopqrstuvwxyzABCDEF

  /*        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y    z    A    B    C    D    E    F        */
  /* a */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* a */
  /* b */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* b */
  /* c */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* c */
  /* d */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* d */
  /* e */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* e */
  /* f */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* f */
  /* g */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* g */
  /* h */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* h */
  /* i */ Wall,Wall,Wall,Wall,DSER,Slot,DSER,DSER,Slot,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* i */
  /* j */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LKSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* j */
  /* k */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* k */
  /* l */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* l */
  /* m */ Wall,Wall,Wall,Wall,DSER,Slot,DSER,DSER,Slot,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* m */
  /* n */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LKSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* n */
  /* o */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* o */
  /* p */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* p */
  /* q */ Wall,Wall,Wall,Wall,DSER,Slot,DSER,DSER,Slot,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* q */
  /* r */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LKSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* r */
  /* s */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* s */
  /* t */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LKNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* t */
  /* u */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,Slot,DSWR,DSWR,Slot,DSWR,Wall,Wall,Wall,Wall,Wall, /* u */
  /* v */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* v */
  /* w */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* w */
  /* x */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* x */
  /* y */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* y */
  /* z */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* z */
  /* A */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* A */
  /* B */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* B */
  /* C */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* C */
  /*        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y    z    A    B    C    D    E    F        */

  //   abcdefghijklmnopqrstuvwxyzABCDEF
  // a ................................ a
  // b ................................ b
  // c ................................ c
  // d ................................ d
  // e ................................ e
  // f ................................ f
  // g ................................ g
  // h ................................ h
  // i I---+-++----+-----+->   ........ i
  // j ................................ j
  // k ................................ k
  // l ................................ l
  // m I---+-++----------+->   ........ m
  // n ................................ n
  // o ................................ o
  // p ................................ p
  // q ....+-++-+----+----------------O q
  // r ................................ r
  // s ................................ s
  // t ................................ t
  // u .......   <---+--+---+-++-+..... u
  // v ................................ v
  // w ................................ w
  // x ................................ x
  // y ................................ y
  // z ................................ z
  // A ................................ A
  // B ................................ B
  // C ................................ C
  //   abcdefghijklmnopqrstuvwxyzABCDEF

  /*        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y    z    A    B    C    D    E    F        */
  /* a */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* a */
  /* b */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* b */
  /* c */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* c */
  /* d */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* d */
  /* e */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* e */
  /* f */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* f */
  /* g */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* g */
  /* h */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* h */
  /* i */ DIER,DBER,DBER,DBER,DPER,DBER,DPER,DPER,DBER,DBER,DBER,DBER,DPER,DBER,DBER,DBER,DBER,DBER,DPER,DBER,DHER,Slot,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* i */
  /* j */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* j */
  /* k */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* k */
  /* l */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* l */
  /* m */ DIER,DBER,DBER,DBER,DPER,DBER,DPER,DPER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DPER,DBER,DHER,Slot,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* m */
  /* n */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* n */
  /* o */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* o */
  /* p */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* p */
  /* q */ Wall,Wall,Wall,Wall,DPER,DBER,DPER,DPER,DBER,DPER,DBER,DBER,DBER,DBER,DPER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DOER, /* q */
  /* r */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* r */
  /* s */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* s */
  /* t */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* t */
  /* u */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,Slot,DHWR,DBWR,DBWR,DBWR,DPWR,DBWR,DBWR,DPWR,DBWR,DBWR,DBWR,DPWR,DBWR,DPWR,DPWR,DBWR,DPWR,Wall,Wall,Wall,Wall,Wall, /* u */
  /* v */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* v */
  /* w */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* w */
  /* x */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* x */
  /* y */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* y */
  /* z */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* z */
  /* A */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* A */
  /* B */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* B */
  /* C */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* C */
  /*        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y    z    A    B    C    D    E    F        */

  //   abcdefghijklmnopqrstuvwxyzABCDEF
  // a ................................ a
  // b ................................ b
  // c ................................ c
  // d ................................ d
  // e ................................ e
  // f ................................ f
  // g ...............v...v............ g
  // h ............... ... ............ h
  // i ............0   ..1   .......... i
  // j ............... ... ............ j
  // k ...................v............ k
  // l ................... ............ l
  // m ..................1   .......... m
  // n ................................ n
  // o ........... ...v................ o
  // p ........... ... ................ p
  // q .........>   .1   .............. q
  // r ...........1...0...0............ r
  // s ............... ... ............ s
  // t ........... ... ... ............ t
  // u ...........   0  <   <.......... u
  // v ........... .................... v
  // w ...........^.................... w
  // x ................................ x
  // y ................................ y
  // z ................................ z
  // A ................................ A
  // B ................................ B
  // C ................................ C
  //   abcdefghijklmnopqrstuvwxyzABCDEF

  /*        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y    z    A    B    C    D    E    F        */
  /* a */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* a */
  /* b */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* b */
  /* c */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* c */
  /* d */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* d */
  /* e */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* e */
  /* f */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* f */
  /* g */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQSR,Wall,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* g */
  /* h */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* h */
  /* i */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DGER,Slot,Slot,Slot,Wall,Wall,DGER,Slot,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* i */
  /* j */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* j */
  /* k */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* k */
  /* l */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* l */
  /* m */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DGER,Slot,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* m */
  /* n */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* n */
  /* o */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* o */
  /* p */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* p */
  /* q */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQER,Slot,Slot,Slot,Wall,DGER,Slot,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* q */
  /* r */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DGNR,Wall,Wall,Wall,DGSR,Wall,Wall,Wall,DGSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* r */
  /* s */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* s */
  /* t */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* t */
  /* u */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,Slot,DGWR,Slot,Slot,DQWR,Slot,Slot,Slot,DQWR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* u */
  /* v */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* v */
  /* w */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* w */
  /* x */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* x */
  /* y */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* y */
  /* z */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* z */
  /* A */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* A */
  /* B */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* B */
  /* C */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* C */
  /*        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y    z    A    B    C    D    E    F        */

  //   abcdefghijklmnopqrstuvwxyzABCDEF
  // a ................................ a
  // b ...............+...+............ b
  // c ...............|...|............ c
  // d ...............+...+............ d
  // e ...............+...+............ e
  // f ...............|...|............ f
  // g ...............+...+............ g
  // h ...............|...|............ h
  // i ...............|...|............ i
  // j ...............|...|............ j
  // k ...............|...+............ k
  // l ...............|...|............ l
  // m ........... ...|...|............ m
  // n ........... ...|...|............ n
  // o ........... ...+...|............ o
  // p ...........^...|...|............ p
  // q ...........|...|...|............ q
  // r ...........+...+...+............ r
  // s ...........|...|...|............ s
  // t ...........|...|...|............ t
  // u ...........|...|...|............ u
  // v ...........|...v...v............ v
  // w ...........+... ... ............ w
  // x ...........|... ... ............ x
  // y ...........+... ... ............ y
  // z ...........+.................... z
  // A ...........|.................... A
  // B ...........+.................... B
  // C ................................ C
  //   abcdefghijklmnopqrstuvwxyzABCDEF

  /*        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y    z    A    B    C    D    E    F        */
  /* a */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* a */
  /* b */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* b */
  /* c */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* c */
  /* d */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* d */
  /* e */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* e */
  /* f */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* f */
  /* g */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* g */
  /* h */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* h */
  /* i */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* i */
  /* j */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* j */
  /* k */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* k */
  /* l */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* l */
  /* m */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* m */
  /* n */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* n */
  /* o */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* o */
  /* p */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DHNR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* p */
  /* q */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* q */
  /* r */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* r */
  /* s */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* s */
  /* t */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* t */
  /* u */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* u */
  /* v */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Wall,DHSR,Wall,Wall,Wall,DHSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* v */
  /* w */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* w */
  /* x */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* x */
  /* y */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* y */
  /* z */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* z */
  /* A */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* A */
  /* B */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* B */
  /* C */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* C */
  /*        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y    z    A    B    C    D    E    F        */

  //   abcdefghijklmnopqrstuvwxyzABCDEF
  // a ................................ a
  // b ...............>...>............ b
  // c ............... ... ............ c
  // d ...............>...>............ d
  // e ............. >>. >>............ e
  // f ............... ... ............ f
  // g ............... ... ............ g
  // h ............... ... ............ h
  // i ................................ i
  // j ................................ j
  // k ................................ k
  // l ................................ l
  // m ................................ m
  // n ................................ n
  // o ................................ o
  // p ................................ p
  // q ................................ q
  // r ................................ r
  // s ................................ s
  // t ................................ t
  // u ................................ u
  // v ........... .................... v
  // w ........... .................... w
  // x ........... .................... x
  // y ...........<< .................. y
  // z ...........<.................... z
  // A ........... .................... A
  // B ...........<.................... B
  // C ................................ C
  //   abcdefghijklmnopqrstuvwxyzABCDEF

  /*        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y    z    A    B    C    D    E    F        */
  /* a */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* a */
  /* b */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSSR,Wall,Wall,Wall,DSSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* b */
  /* c */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* c */
  /* d */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSSR,Wall,Wall,Wall,DSSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* d */
  /* e */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,LKWL,DSSR,Wall,Slot,LKWL,DSSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* e */
  /* f */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* f */
  /* g */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* g */
  /* h */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* h */
  /* i */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* i */
  /* j */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* j */
  /* k */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* k */
  /* l */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* l */
  /* m */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* m */
  /* n */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* n */
  /* o */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* o */
  /* p */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* p */
  /* q */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* q */
  /* r */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* r */
  /* s */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* s */
  /* t */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* t */
  /* u */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* u */
  /* v */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* v */
  /* w */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* w */
  /* x */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* x */
  /* y */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSNR,LKEL,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* y */
  /* z */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* z */
  /* A */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* A */
  /* B */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* B */
  /* C */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* C */
  /*        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y    z    A    B    C    D    E    F        */

  //   abcdefghijklmnopqrstuvwxyzABCDEF
  // a ................................ a
  // b ................................ b
  // c ................................ c
  // d ................................ d
  // e  <------------+---+------------< e
  // f ................................ f
  // g ................................ g
  // h ................................ h
  // i ................................ i
  // j ................................ j
  // k ................................ k
  // l ................................ l
  // m ................................ m
  // n ................................ n
  // o ................................ o
  // p ................................ p
  // q ................................ q
  // r ................................ r
  // s ................................ s
  // t ................................ t
  // u ................................ u
  // v ................................ v
  // w ................................ w
  // x ................................ x
  // y >-----------+----------------->  y
  // z ................................ z
  // A ................................ A
  // B ................................ B
  // C ................................ C
  //   abcdefghijklmnopqrstuvwxyzABCDEF

  /*        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y    z    A    B    C    D    E    F        */
  /* a */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* a */
  /* b */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* b */
  /* c */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* c */
  /* d */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* d */
  /* e */ Slot,LHWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LPWL,LBWL,LBWL,LBWL,LPWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LTWL, /* e */
  /* f */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* f */
  /* g */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* g */
  /* h */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* h */
  /* i */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* i */
  /* j */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* j */
  /* k */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* k */
  /* l */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* l */
  /* m */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* m */
  /* n */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* n */
  /* o */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* o */
  /* p */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* p */
  /* q */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* q */
  /* r */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* r */
  /* s */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* s */
  /* t */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* t */
  /* u */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* u */
  /* v */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* v */
  /* w */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* w */
  /* x */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* x */
  /* y */ LTEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LPEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LHEL,Slot, /* y */
  /* z */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* z */
  /* A */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* A */
  /* B */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* B */
  /* C */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* C */
  /*        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y    z    A    B    C    D    E    F        */
};

int main(int argc, char const *argv[]) {
  for (size_t i = 0; i < eoVoxel; i += 1) {
    Voxel v = Voxel(i);

    stringToVoxel[toConstCharPointer(v)] = v;
  }

  Volume volume(initialVolume);

  // #include "cam.8.rules.h"

  volume.AddInput
      (VoxelCoordinant(2,  8, 0),
       { dsSet0, dsSet0, dsSet0, dsSet0, dsSet1, dsSet0, dsSet1, dsSet0 }
      );
  volume.AddInput
      (VoxelCoordinant(2, 12, 0),
       { dsSet0, dsSet0, dsSet1, dsSet0, dsSet0, dsSet0, dsSet1, dsSet0 }
      );

  volume.PrintViewFlat();
  for (size_t t = 0; t < (8 * NTicksPerCycle); t += 1) {
    volume.ProceedOneTick();
    volume.PrintViewFlat();
    if (volume.FoundNewRules()) {
      break;
    }
  }
  volume.DumpUnusedRules();
  return 0;
}
