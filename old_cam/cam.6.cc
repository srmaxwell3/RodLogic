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

bool optVerbose = false;

// Directions of motion, in order (clockwise).

enum Direction {
  DirE,
  DirS,
  DirD,
  DirW,
  DirN,
  DirU,

  eoDirection,
  DirX = eoDirection
};

char const *toConstCharPointer(Direction d) {
  switch (d) {
    case DirE: return "DirE";
    case DirS: return "DirS";
    case DirD: return "DirD";
    case DirW: return "DirW";
    case DirN: return "DirN";
    case DirU: return "DirU";
    case eoDirection:
      return "eoDirection";
  }
  return "Direction(?)";
}

enum FwdOrBwd {
  Fwd,
  Bwd,

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

enum PhasePerCycle {
  E,
  S,
  W,
  N,

  eoPhasePerCycle,
  Unknown = eoPhasePerCycle
};

char const *toConstCharPointer(PhasePerCycle t) {
  switch (t) {
    case E: return "E";
    case S: return "S";
    case W: return "W";
    case N: return "N";
    case eoPhasePerCycle:
      return "eoPhasePerCycle";
  }
  return "PhasePerCycle(?)";
}

struct PhaseProperties {
  PhasePerCycle fwd;  // this major cycle direction forward (identity)
  PhasePerCycle bwd;  // this major cycle direction backward
  PhasePerCycle nxt;  // the next major cycle's forward direction
  PhasePerCycle lst;  // the last major cycle's forward direction
} phaseProperties[eoPhasePerCycle] = {
  { E, W, S, N },  // E
  { S, N, W, E },  // S
  { W, E, N, S },  // W
  { N, S, E, W }   // N
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
  EUSL,
  ERS1,
  ERSX,
  ERS0,
  ELSL,
  EUEL,
  ESE0,
  ESEX,
  ESE1,
  ELEL,

  SUWL,
  SRW1,
  SRWX,
  SRW0,
  SLWL,
  SUSL,
  SSS0,
  SSSX,
  SSS1,
  SLSL,

  WUNL,
  WRN1,
  WRNX,
  WRN0,
  WLNL,
  WUWL,
  WSW0,
  WSWX,
  WSW1,
  WLWL,

  NUEL,
  NRE1,
  NREX,
  NRE0,
  NLEL,
  NUNL,
  NSN0,
  NSNX,
  NSN1,
  NLNL,

  eoTickPerCycle
};

char const *toConstCharPointer(TickPerCycle t) {
  switch (t) {
    case EUSL: return "EUSL";
    case ERS1: return "ERS1";
    case ERSX: return "ERSX";
    case ERS0: return "ERS0";
    case ELSL: return "ELSL";
    case EUEL: return "EUEL";
    case ESE0: return "ESE0";
    case ESEX: return "ESEX";
    case ESE1: return "ESE1";
    case ELEL: return "ELEL";
    case SUWL: return "SUWL";
    case SRW1: return "SRW1";
    case SRWX: return "SRWX";
    case SRW0: return "SRW0";
    case SLWL: return "SLWL";
    case SUSL: return "SUSL";
    case SSS0: return "SSS0";
    case SSSX: return "SSSX";
    case SSS1: return "SSS1";
    case SLSL: return "SLSL";
    case WUNL: return "WUNL";
    case WRN1: return "WRN1";
    case WRNX: return "WRNX";
    case WRN0: return "WRN0";
    case WLNL: return "WLNL";
    case WUWL: return "WUWL";
    case WSW0: return "WSW0";
    case WSWX: return "WSWX";
    case WSW1: return "WSW1";
    case WLWL: return "WLWL";
    case NUEL: return "NUEL";
    case NRE1: return "NRE1";
    case NREX: return "NREX";
    case NRE0: return "NRE0";
    case NLEL: return "NLEL";
    case NUNL: return "NUNL";
    case NSN0: return "NSN0";
    case NSNX: return "NSNX";
    case NSN1: return "NSN1";
    case NLNL: return "NLNL";
    case eoTickPerCycle:
      return "eoTickPerCycle";
  }
  return "TickPerCycle(?)";
}

size_t const NPhasesPerCycle = size_t(eoPhasePerCycle);
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
  VoxelCoordinant &Move(PhasePerCycle c) {
    switch (c) {
      case E: return MoveE();
      case S: return MoveS();
      case W: return MoveW();
      case N: return MoveN();
    default:
      return *this;
    }
    return *this;
  }
  VoxelCoordinant &Move(Direction d) {
    switch (d) {
      case DirE: return MoveE();
      case DirS: return MoveS();
      case DirD: return MoveD();
      case DirW: return MoveW();
      case DirN: return MoveN();
      case DirU: return MoveU();
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

  VoxelCoordinant To(PhasePerCycle c) const {
    switch (c) {
      case E: return ToE();
      case S: return ToS();
      case W: return ToW();
      case N: return ToN();
    default:
      return *this;
    }
    return *this;
  }
  VoxelCoordinant To(Direction d) const {
    switch (d) {
      case DirE: return ToE();
      case DirS: return ToS();
      case DirD: return ToD();
      case DirW: return ToW();
      case DirN: return ToN();
      case DirU: return ToU();
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

  VoxelCoordinant &EFrom(VoxelCoordinant const &v) {
    l = v.l;
    r = v.r;
    c = v.c + 1;
    return *this;
  }
  VoxelCoordinant &SFrom(VoxelCoordinant const &v) {
    l = v.l;
    r = v.r + 1;
    c = v.c;
    return *this;
  }
  VoxelCoordinant &DFrom(VoxelCoordinant const &v) {
    l = v.l - 1;
    r = v.r;
    c = v.c;
    return *this;
  }
  VoxelCoordinant &WFrom(VoxelCoordinant const &v) {
    l = v.l;
    r = v.r;
    c = v.c - 1;
    return *this;
  }
  VoxelCoordinant &NFrom(VoxelCoordinant const &v) {
    l = v.l;
    r = v.r - 1;
    c = v.c;
    return *this;
  }
  VoxelCoordinant &UFrom(VoxelCoordinant const &v) {
    l = v.l + 1;
    r = v.r;
    c = v.c;
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
  FBUnkn,

  FBBlkd,
  FBUnbk,

  eoFBlkState
};

char const *toConstCharPointer(FBlkState s) {
  switch (s) {
    case FBUnkn: return "FBUnkn";
    case FBBlkd: return "FBBlkd";
    case FBUnbk: return "FBUnbk";
    case eoFBlkState:
      return "eoFBlkState";
  }
  return "FBlkState(?)";
}

enum RBlkState {
  RBUnkn,

  RBBlkd,
  RBUnbk,

  eoRBlkState
};

char const *toConstCharPointer(RBlkState s) {
  switch (s) {
    case RBUnkn: return "RBUnkn";
    case RBBlkd: return "RBBlkd";
    case RBUnbk: return "RBUnbk";
    case eoRBlkState:
      return "eoRBlkState";
  }
  return "FBlkState(?)";
}

enum VoxelType {
  VTUnkn,

  VTWall,
  VTSlot,
  VTLock,
  VTData,

  eoVoxelType
};

char const *toConstCharPointer(VoxelType t) {
  switch (t) {
    case VTUnkn: return "VTUnkn";
    case VTWall: return "VTWall";
    case VTSlot: return "VTSlot";
    case VTLock: return "VTLock";
    case VTData: return "VTData";
    case eoVoxelType:
      return "eoVoxelType";
  }
  return "VoxelType(?)";
}

enum LockType {
  LTUnkn,

  LTBody,
  LTHead,
  LTTail,
  LTPost,
  LTLock,

  eoLockType
};

char const *toConstCharPointer(LockType t) {
  switch (t) {
    case LTUnkn: return "LTUnkn";
    case LTBody: return "LTBody";
    case LTHead: return "LTHead";
    case LTTail: return "LTTail";
    case LTPost: return "LTPost";
    case LTLock: return "LTLock";
    case eoLockType:
      return "eoLockType";
  }
  return "LockType(?)";
}

enum LockState {
  LSUnkn,

  LSLckd,
  LSUnlk,

  eoLockState
};

char const *toConstCharPointer(LockState t) {
  switch (t) {
    case LSUnkn: return "LSUnkn";
    case LSLckd: return "LSLckd";
    case LSUnlk: return "LSUnlk";
    case eoLockState:
      return "eoLockState";
  }
  return "LockState(?)";
}

enum DataType {
  DTUnkn,

  DTBody,
  DTHead,
  DTTail,
  DTPost,
  DTLock,
  DTTest,
  DTGate,
  DTInpt,
  DTOutp,

  eoDataType
};

char const *toConstCharPointer(DataType t) {
  switch (t) {
    case DTUnkn: return "DTUnkn";
    case DTBody: return "DTBody";
    case DTHead: return "DTHead";
    case DTTail: return "DTTail";
    case DTPost: return "DTPost";
    case DTLock: return "DTLock";
    case DTTest: return "DTTest";
    case DTGate: return "DTGate";
    case DTInpt: return "DTInpt";
    case DTOutp: return "DTOutp";
    case eoDataType:
      return "eoDataType";
  }
  return "DataType(?)";
}

enum DataState {
  DSUnkn,

  DSRset,
  DSSet0,
  DSSetX,
  DSSet1,

  eoDataState
};

char const *toConstCharPointer(DataState s) {
  switch (s) {
    case DSUnkn: return "DSUnkn";
    case DSRset: return "DSRset";
    case DSSet0: return "DSSet0";
    case DSSetX: return "DSSetX";
    case DSSet1: return "DSSet1";
    case eoDataState:
      return "eoDataState";
  }
  return "DataState(?)";
}

enum BlockState {
  ___,  // FBUnbk, RBUnbk, LSUnlk
  F__,  // FBBlkd, RBUnbk, LSUnlk
  _R_,  // FBUnbk, RBBlkd, LSUnlk
  FR_,  // FBBlkd, RBBlkd, LSUnlk
  __L,  // FBUnbk, RBUnbk, LSLckd
  F_L,  // FBBlkd, RBUnbk, LSLckd
  _RL,  // FBUnbk, RBBlkd, LSLckd
  FRL,  // FBBlkd, RBBlkd, LSLckd

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

// [latc]K{E,S,W,N}
// L[ogic]{E,S,W,N}

enum RodType {
  RTUn,

  RTLE, RTLS, RTLW, RTLN,
  RTDE, RTDS, RTDW, RTDN,

  eoRodType
};

char const *toConstCharPointer(RodType t) {
  switch (t) {
    case RTUn: return "RTUn";
    case RTLE: return "RTLE";
    case RTLS: return "RTLS";
    case RTLW: return "RTLW";
    case RTLN: return "RTLN";
    case RTDE: return "RTDE";
    case RTDS: return "RTDS";
    case RTDW: return "RTDW";
    case RTDN: return "RTDN";
  case eoRodType:
      return "eoRodType";
  }
  return "RodType(?)";
}

struct RodTypeProperties {
  TickPerCycle activeDuring[6];
  Direction fwd;
  Direction bwd;
  Direction lwd;
  Direction rwd;
} rodTypeProperties[eoRodType] = {
#undef ____
#define ____ eoTickPerCycle
  { { ____, ____, ____, ____, ____, ____ }, DirX, DirX, DirX, DirX }, // RTUn
  { { WUNL, WLNL, NUEL, NLEL, ____, ____ }, DirE, DirW, DirN, DirS }, // RTLE
  { { NUEL, NLEL, EUEL, ELEL, ____, ____ }, DirS, DirN, DirW, DirE }, // RTLS
  { { EUSL, ELSL, SUSL, SLSL, ____, ____ }, DirW, DirE, DirS, DirN }, // RTLW
  { { SUWL, SLWL, WUWL, WLWL, ____, ____ }, DirN, DirS, DirE, DirW }, // RTLN
  { { NRE1, NREX, NRE0, ESE0, ESEX, ESE1 }, DirE, DirW, DirN, DirS }, // RTDE
  { { ERS1, ERSX, ERS0, SSS0, SSSX, SSS1 }, DirS, DirN, DirW, DirE }, // RTDS
  { { SRW1, SRWX, SRW0, WSW0, WSWX, WSW1 }, DirW, DirE, DirS, DirN }, // RTDW
  { { WRN1, WRNX, WRN0, NSN0, NSNX, NSN1 }, DirN, DirS, DirE, DirW }, // RTDN
#undef ____
};

struct TickPerCycleProperties {
  RodType rodType;
  FwdOrBwd fwdOrBwd;
} tickPerCycleProperties[eoTickPerCycle] = {
  { RTLW, Fwd }, // EUSL
  { RTDS, Bwd }, // ERS1
  { RTDS, Bwd }, // ERSX
  { RTDS, Bwd }, // ERS0
  { RTLW, Bwd }, // ELSL
  { RTLS, Fwd }, // EUEL
  { RTDE, Fwd }, // ESE0
  { RTDE, Fwd }, // ESEX
  { RTDE, Fwd }, // ESE1
  { RTLS, Bwd }, // ELEL
  { RTLN, Fwd }, // SUWL
  { RTDW, Bwd }, // SRW1
  { RTDW, Bwd }, // SRWX
  { RTDW, Bwd }, // SRW0
  { RTLN, Bwd }, // SLWL
  { RTLW, Fwd }, // SUSL
  { RTDS, Fwd }, // SSS0
  { RTDS, Fwd }, // SSSX
  { RTDS, Fwd }, // SSS1
  { RTLW, Bwd }, // SLSL
  { RTLE, Fwd }, // WUNL
  { RTDN, Bwd }, // WRN1
  { RTDN, Bwd }, // WRNX
  { RTDN, Bwd }, // WRN0
  { RTLE, Bwd }, // WLNL
  { RTLN, Fwd }, // WUWL
  { RTDW, Fwd }, // WSW0
  { RTDW, Fwd }, // WSWX
  { RTDW, Fwd }, // WSW1
  { RTLN, Bwd }, // WLWL
  { RTLS, Fwd }, // NUEL
  { RTDE, Bwd }, // NRE1
  { RTDE, Bwd }, // NREX
  { RTDE, Bwd }, // NRE0
  { RTLS, Bwd }, // NLEL
  { RTLE, Fwd }, // NUEL
  { RTDN, Fwd }, // NSE0
  { RTDN, Fwd }, // NSEX
  { RTDN, Fwd }, // NSE1
  { RTLE, Bwd }  // NLEL
};

// U[nknown]
// W[all]
// S[lot]({ES,SW,WN,NE}{L[ocked],U[unlocked],R[eset],[Set]0,I[llegal],[Set]1})?
// [latc]K{B[ody},H[ead],T[ail],P[ost],K[ey]}
//        {E,S,N,W}
//        {L[ocked],U[unlocked]}
// L[ogic]{B[ody},H[ead],T[ail],P[ost],[Loc]K,Q[uery],G[ate],I[nput],O[utput}}
//        {E,S,W,N}
//        {R[eset],[Set]0,I[llegal],[Set]1}

enum Voxel {
  Unkn,

  Wall,

  Slot,

  LBEL, LBEU,  LBSL, LBSU,  LBWL, LBWU,  LBNL, LBNU,
  LHEL, LHEU,  LHSL, LHSU,  LHWL, LHWU,  LHNL, LHNU,
  LTEL, LTEU,  LTSL, LTSU,  LTWL, LTWU,  LTNL, LTNU,
  LPEL, LPEU,  LPSL, LPSU,  LPWL, LPWU,  LPNL, LPNU,
  LKEL, LKEU,  LKSL, LKSU,  LKWL, LKWU,  LKNL, LKNU,

  DBER, DBE0, DBEX, DBE1,  DBSR, DBS0, DBSX, DBS1,
  DBWR, DBW0, DBWX, DBW1,  DBNR, DBN0, DBNX, DBN1,
  DHER, DHE0, DHEX, DHE1,  DHSR, DHS0, DHSX, DHS1,
  DHWR, DHW0, DHWX, DHW1,  DHNR, DHN0, DHNX, DHN1,
  DTER, DTE0, DTEX, DTE1,  DTSR, DTS0, DTSX, DTS1,
  DTWR, DTW0, DTWX, DTW1,  DTNR, DTN0, DTNX, DTN1,
  DPER, DPE0, DPEX, DPE1,  DPSR, DPS0, DPSX, DPS1,
  DPWR, DPW0, DPWX, DPW1,  DPNR, DPN0, DPNX, DPN1,
  DKER, DKE0, DKEX, DKE1,  DKSR, DKS0, DKSX, DKS1,
  DKWR, DKW0, DKWX, DKW1,  DKNR, DKN0, DKNX, DKN1,
  DQER, DQE0, DQEX, DQE1,  DQSR, DQS0, DQSX, DQS1,
  DQWR, DQW0, DQWX, DQW1,  DQNR, DQN0, DQNX, DQN1,
  DGER, DGE0, DGEX, DGE1,  DGSR, DGS0, DGSX, DGS1,
  DGWR, DGW0, DGWX, DGW1,  DGNR, DGN0, DGNX, DGN1,
  DIER, DIE0, DIEX, DIE1,  DISR, DIS0, DISX, DIS1,
  DIWR, DIW0, DIWX, DIW1,  DINR, DIN0, DINX, DIN1,
  DOER, DOE0, DOEX, DOE1,  DOSR, DOS0, DOSX, DOS1,
  DOWR, DOW0, DOWX, DOW1,  DONR, DON0, DONX, DON1,

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
  case LBWL: return "LBWL";
  case LBWU: return "LBWU";
  case LBNL: return "LBNL";
  case LBNU: return "LBNU";
  case LHEL: return "LHEL";
  case LHEU: return "LHEU";
  case LHSL: return "LHSL";
  case LHSU: return "LHSU";
  case LHWL: return "LHWL";
  case LHWU: return "LHWU";
  case LHNL: return "LHNL";
  case LHNU: return "LHNU";
  case LTEL: return "LTEL";
  case LTEU: return "LTEU";
  case LTSL: return "LTSL";
  case LTSU: return "LTSU";
  case LTWL: return "LTWL";
  case LTWU: return "LTWU";
  case LTNL: return "LTNL";
  case LTNU: return "LTNU";
  case LPEL: return "LPEL";
  case LPEU: return "LPEU";
  case LPSL: return "LPSL";
  case LPSU: return "LPSU";
  case LPWL: return "LPWL";
  case LPWU: return "LPWU";
  case LPNL: return "LPNL";
  case LPNU: return "LPNU";
  case LKEL: return "LKEL";
  case LKEU: return "LKEU";
  case LKSL: return "LKSL";
  case LKSU: return "LKSU";
  case LKWL: return "LKWL";
  case LKWU: return "LKWU";
  case LKNL: return "LKNL";
  case LKNU: return "LKNU";
  case DBER: return "DBER";
  case DBE0: return "DBE0";
  case DBEX: return "DBEX";
  case DBE1: return "DBE1";
  case DBSR: return "DBSR";
  case DBS0: return "DBS0";
  case DBSX: return "DBSX";
  case DBS1: return "DBS1";
  case DBWR: return "DBWR";
  case DBW0: return "DBW0";
  case DBWX: return "DBWX";
  case DBW1: return "DBW1";
  case DBNR: return "DBNR";
  case DBN0: return "DBN0";
  case DBNX: return "DBNX";
  case DBN1: return "DBN1";
  case DHER: return "DHER";
  case DHE0: return "DHE0";
  case DHEX: return "DHEX";
  case DHE1: return "DHE1";
  case DHSR: return "DHSR";
  case DHS0: return "DHS0";
  case DHSX: return "DHSX";
  case DHS1: return "DHS1";
  case DHWR: return "DHWR";
  case DHW0: return "DHW0";
  case DHWX: return "DHWX";
  case DHW1: return "DHW1";
  case DHNR: return "DHNR";
  case DHN0: return "DHN0";
  case DHNX: return "DHNX";
  case DHN1: return "DHN1";
  case DTER: return "DTER";
  case DTE0: return "DTE0";
  case DTEX: return "DTEX";
  case DTE1: return "DTE1";
  case DTSR: return "DTSR";
  case DTS0: return "DTS0";
  case DTSX: return "DTSX";
  case DTS1: return "DTS1";
  case DTWR: return "DTWR";
  case DTW0: return "DTW0";
  case DTWX: return "DTWX";
  case DTW1: return "DTW1";
  case DTNR: return "DTNR";
  case DTN0: return "DTN0";
  case DTNX: return "DTNX";
  case DTN1: return "DTN1";
  case DPER: return "DPER";
  case DPE0: return "DPE0";
  case DPEX: return "DPEX";
  case DPE1: return "DPE1";
  case DPSR: return "DPSR";
  case DPS0: return "DPS0";
  case DPSX: return "DPSX";
  case DPS1: return "DPS1";
  case DPWR: return "DPWR";
  case DPW0: return "DPW0";
  case DPWX: return "DPWX";
  case DPW1: return "DPW1";
  case DPNR: return "DPNR";
  case DPN0: return "DPN0";
  case DPNX: return "DPNX";
  case DPN1: return "DPN1";
  case DKER: return "DKER";
  case DKE0: return "DKE0";
  case DKEX: return "DKEX";
  case DKE1: return "DKE1";
  case DKSR: return "DKSR";
  case DKS0: return "DKS0";
  case DKSX: return "DKSX";
  case DKS1: return "DKS1";
  case DKWR: return "DKWR";
  case DKW0: return "DKW0";
  case DKWX: return "DKWX";
  case DKW1: return "DKW1";
  case DKNR: return "DKNR";
  case DKN0: return "DKN0";
  case DKNX: return "DKNX";
  case DKN1: return "DKN1";
  case DQER: return "DQER";
  case DQE0: return "DQE0";
  case DQEX: return "DQEX";
  case DQE1: return "DQE1";
  case DQSR: return "DQSR";
  case DQS0: return "DQS0";
  case DQSX: return "DQSX";
  case DQS1: return "DQS1";
  case DQWR: return "DQWR";
  case DQW0: return "DQW0";
  case DQWX: return "DQWX";
  case DQW1: return "DQW1";
  case DQNR: return "DQNR";
  case DQN0: return "DQN0";
  case DQNX: return "DQNX";
  case DQN1: return "DQN1";
  case DGER: return "DGER";
  case DGE0: return "DGE0";
  case DGEX: return "DGEX";
  case DGE1: return "DGE1";
  case DGSR: return "DGSR";
  case DGS0: return "DGS0";
  case DGSX: return "DGSX";
  case DGS1: return "DGS1";
  case DGWR: return "DGWR";
  case DGW0: return "DGW0";
  case DGWX: return "DGWX";
  case DGW1: return "DGW1";
  case DGNR: return "DGNR";
  case DGN0: return "DGN0";
  case DGNX: return "DGNX";
  case DGN1: return "DGN1";
  case DIER: return "DIER";
  case DIE0: return "DIE0";
  case DIEX: return "DIEX";
  case DIE1: return "DIE1";
  case DISR: return "DISR";
  case DIS0: return "DIS0";
  case DISX: return "DISX";
  case DIS1: return "DIS1";
  case DIWR: return "DIWR";
  case DIW0: return "DIW0";
  case DIWX: return "DIWX";
  case DIW1: return "DIW1";
  case DINR: return "DINR";
  case DIN0: return "DIN0";
  case DINX: return "DINX";
  case DIN1: return "DIN1";
  case DOER: return "DOER";
  case DOE0: return "DOE0";
  case DOEX: return "DOEX";
  case DOE1: return "DOE1";
  case DOSR: return "DOSR";
  case DOS0: return "DOS0";
  case DOSX: return "DOSX";
  case DOS1: return "DOS1";
  case DOWR: return "DOWR";
  case DOW0: return "DOW0";
  case DOWX: return "DOWX";
  case DOW1: return "DOW1";
  case DONR: return "DONR";
  case DON0: return "DON0";
  case DONX: return "DONX";
  case DON1: return "DON1";
  case eoVoxel:
    return "eoVoxel";
  }
  return "Voxel(?)";
}

struct VoxelProperties {
  int displayPriority;
  char const text;
  VoxelType voxelType;
  RodType rodType;
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
} voxelProperties[eoVoxel] = {
#undef Props
#define Props(displayPriority, \
              text, \
              voxelType, \
              rodType, \
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
  VT##voxelType, \
  RT##rodType, \
  LT##lockType, \
  LS##lockState, \
  DT##dataType, \
  DS##dataState, \
  isRodBody, \
  blockableStates, \
  { { Dir##fwd, fwdVoxel }, \
    { Dir##bwd, bwdVoxel } \
  } \
}

#undef T
#define T true
#undef F
#define F false
#undef ____
#define ____ Unkn

  //                   displayPriority
  //                  /  text
  //                 /  /    voxelType
  //                /  /    /   rodType
  //               /  /    /   /    lockType
  //              /  /    /   /    /    lockState
  //             /  /    /   /    /    /    dataType
  //            /  /    /   /    /    /    /    dataState
  //           /  /    /   /    /    /    /    / isRodBody
  //          /  /    /   /    /    /    /    / /   blockableStates
  //         /  /    /   /    /    /    /    / /   / fwd
  //        /  /    /   /    /    /    /    / /   / /    fwdVoxel
  //       /  /    /   /    /    /    /    / /   / /    / bwd
  //      /  /    /   /    /    /    /    / /   / /    / /    bwdVoxel
  //     /  /    /   /    /    /    /    / /   / /    / /    /
  Props(9,'?',Unkn,Un,Unkn,Unkn,Unkn,Unkn,F,___,X,____,X,____), // Unkn
  Props(0,'.',Wall,Un,Unkn,Unkn,Unkn,Unkn,F,___,X,____,X,____), // Wall
  Props(1,' ',Slot,Un,Unkn,Unkn,Unkn,Unkn,F,___,X,____,X,____), // Slot
  Props(2,'-',Lock,LE,Body,Lckd,Unkn,Unkn,T,___,E,LBEU,W,____), // LBEL
  Props(2,'-',Lock,LE,Body,Unlk,Unkn,Unkn,T,___,E,____,W,LBEL), // LBEU
  Props(2,'|',Lock,LS,Body,Lckd,Unkn,Unkn,T,___,S,LBSU,N,____), // LBSL
  Props(2,'|',Lock,LS,Body,Unlk,Unkn,Unkn,T,___,S,____,N,LBSL), // LBSU
  Props(2,'-',Lock,LW,Body,Lckd,Unkn,Unkn,T,___,W,LBWU,E,____), // LBWL
  Props(2,'-',Lock,LW,Body,Unlk,Unkn,Unkn,T,___,W,____,E,LBWL), // LBWU
  Props(2,'|',Lock,LN,Body,Lckd,Unkn,Unkn,T,___,N,LBNU,S,____), // LBNL
  Props(2,'|',Lock,LN,Body,Unlk,Unkn,Unkn,T,___,N,____,S,LBNL), // LBNU
  Props(3,'>',Lock,LE,Head,Lckd,Unkn,Unkn,T,___,E,LHEU,W,____), // LHEL
  Props(3,'>',Lock,LE,Head,Unlk,Unkn,Unkn,T,___,E,____,W,LHEL), // LHEU
  Props(3,'v',Lock,LS,Head,Lckd,Unkn,Unkn,T,___,S,LHSU,N,____), // LHSL
  Props(3,'v',Lock,LS,Head,Unlk,Unkn,Unkn,T,___,S,____,N,LHSL), // LHSU
  Props(3,'<',Lock,LW,Head,Lckd,Unkn,Unkn,T,___,W,LHWU,E,____), // LHWL
  Props(3,'<',Lock,LW,Head,Unlk,Unkn,Unkn,T,___,W,____,E,LHWL), // LHWU
  Props(3,'^',Lock,LN,Head,Lckd,Unkn,Unkn,T,___,N,LHNU,S,____), // LHNL
  Props(3,'^',Lock,LN,Head,Unlk,Unkn,Unkn,T,___,N,____,S,LHNL), // LHNU
  Props(3,'>',Lock,LE,Tail,Lckd,Unkn,Unkn,T,___,E,LTEU,W,____), // LTEL
  Props(3,'>',Lock,LE,Tail,Unlk,Unkn,Unkn,T,___,E,____,W,LTEL), // LTEU
  Props(3,'v',Lock,LS,Tail,Lckd,Unkn,Unkn,T,___,S,LTSU,N,____), // LTSL
  Props(3,'v',Lock,LS,Tail,Unlk,Unkn,Unkn,T,___,S,____,N,LTSL), // LTSU
  Props(3,'<',Lock,LW,Tail,Lckd,Unkn,Unkn,T,___,W,LTWU,E,____), // LTWL
  Props(3,'<',Lock,LW,Tail,Unlk,Unkn,Unkn,T,___,W,____,E,LTWL), // LTWU
  Props(3,'^',Lock,LN,Tail,Lckd,Unkn,Unkn,T,___,N,LTNU,S,____), // LTNL
  Props(3,'^',Lock,LN,Tail,Unlk,Unkn,Unkn,T,___,N,____,S,LTNL), // LTNU
  Props(2,'+',Lock,LE,Post,Lckd,Unkn,Unkn,T,___,E,LPEU,W,____), // LPEL
  Props(2,'+',Lock,LE,Post,Unlk,Unkn,Unkn,T,___,E,____,W,LPEL), // LPEU
  Props(2,'+',Lock,LS,Post,Lckd,Unkn,Unkn,T,___,S,LPSU,N,____), // LPSL
  Props(2,'+',Lock,LS,Post,Unlk,Unkn,Unkn,T,___,S,____,N,LPSL), // LPSU
  Props(2,'+',Lock,LW,Post,Lckd,Unkn,Unkn,T,___,W,LPWU,E,____), // LPWL
  Props(2,'+',Lock,LW,Post,Unlk,Unkn,Unkn,T,___,W,____,E,LPWL), // LPWU
  Props(2,'+',Lock,LN,Post,Lckd,Unkn,Unkn,T,___,N,LPNU,S,____), // LPNL
  Props(2,'+',Lock,LN,Post,Unlk,Unkn,Unkn,T,___,N,____,S,LPNL), // LPNU
  Props(3,'<',Lock,LE,Lock,Lckd,Unkn,Unkn,F,FRL,E,LKEU,W,____), // LKEL
  Props(3,'<',Lock,LE,Lock,Unlk,Unkn,Unkn,F,FRL,E,____,W,LKEL), // LKEU
  Props(3,'^',Lock,LS,Lock,Lckd,Unkn,Unkn,F,FRL,S,LKSU,N,____), // LKSL
  Props(3,'^',Lock,LS,Lock,Unlk,Unkn,Unkn,F,FRL,S,____,N,LKSL), // LKSU
  Props(3,'>',Lock,LW,Lock,Lckd,Unkn,Unkn,F,FRL,W,LKWU,E,____), // LKWL
  Props(3,'>',Lock,LW,Lock,Unlk,Unkn,Unkn,F,FRL,W,____,E,LKWL), // LKWU
  Props(3,'v',Lock,LN,Lock,Lckd,Unkn,Unkn,F,FRL,N,LKNU,S,____), // LKNL
  Props(3,'v',Lock,LN,Lock,Unlk,Unkn,Unkn,F,FRL,N,____,S,LKNL), // LKNU
  Props(4,'-',Data,DE,Unkn,Unkn,Body,Rset,T,___,E,DBE0,W,____), // DBER
  Props(4,'-',Data,DE,Unkn,Unkn,Body,Set0,T,___,E,DBEX,W,DBER), // DBE0
  Props(4,'-',Data,DE,Unkn,Unkn,Body,SetX,T,___,E,DBE1,W,DBE0), // DBEX
  Props(4,'-',Data,DE,Unkn,Unkn,Body,Set1,T,___,E,____,W,DBEX), // DBE1
  Props(4,'|',Data,DS,Unkn,Unkn,Body,Rset,T,___,S,DBS0,N,____), // DBSR
  Props(4,'|',Data,DS,Unkn,Unkn,Body,Set0,T,___,S,DBSX,N,DBSR), // DBS0
  Props(4,'|',Data,DS,Unkn,Unkn,Body,SetX,T,___,S,DBS1,N,DBS0), // DBSX
  Props(4,'|',Data,DS,Unkn,Unkn,Body,Set1,T,___,S,____,N,DBSX), // DBS1
  Props(4,'-',Data,DW,Unkn,Unkn,Body,Rset,T,___,W,DBW0,E,____), // DBWR
  Props(4,'-',Data,DW,Unkn,Unkn,Body,Set0,T,___,W,DBWX,E,DBWR), // DBW0
  Props(4,'-',Data,DW,Unkn,Unkn,Body,SetX,T,___,W,DBW1,E,DBW0), // DBWX
  Props(4,'-',Data,DW,Unkn,Unkn,Body,Set1,T,___,W,____,E,DBWX), // DBW1
  Props(4,'|',Data,DN,Unkn,Unkn,Body,Rset,T,___,N,DBN0,S,____), // DBNR
  Props(4,'|',Data,DN,Unkn,Unkn,Body,Set0,T,___,N,DBNX,S,DBNR), // DBN0
  Props(4,'|',Data,DN,Unkn,Unkn,Body,SetX,T,___,N,DBN1,S,DBN0), // DBNX
  Props(4,'|',Data,DN,Unkn,Unkn,Body,Set1,T,___,N,____,S,DBNX), // DBN1
  Props(5,'>',Data,DE,Unkn,Unkn,Head,Rset,T,F__,E,DHE0,W,____), // DHER
  Props(5,'0',Data,DE,Unkn,Unkn,Head,Set0,T,F__,E,DHEX,W,DHER), // DHE0
  Props(5,'X',Data,DE,Unkn,Unkn,Head,SetX,T,F__,E,DHE1,W,DHE0), // DHEX
  Props(5,'1',Data,DE,Unkn,Unkn,Head,Set1,T,F__,E,____,W,DHEX), // DHE1
  Props(5,'v',Data,DS,Unkn,Unkn,Head,Rset,T,F__,S,DHS0,N,____), // DHSR
  Props(5,'0',Data,DS,Unkn,Unkn,Head,Set0,T,F__,S,DHSX,N,DHSR), // DHS0
  Props(5,'X',Data,DS,Unkn,Unkn,Head,SetX,T,F__,S,DHS1,N,DHS0), // DHSX
  Props(5,'1',Data,DS,Unkn,Unkn,Head,Set1,T,F__,S,____,N,DHSX), // DHS1
  Props(5,'<',Data,DW,Unkn,Unkn,Head,Rset,T,F__,W,DHW0,E,____), // DHWR
  Props(5,'0',Data,DW,Unkn,Unkn,Head,Set0,T,F__,W,DHWX,E,DHWR), // DHW0
  Props(5,'X',Data,DW,Unkn,Unkn,Head,SetX,T,F__,W,DHW1,E,DHW0), // DHWX
  Props(5,'1',Data,DW,Unkn,Unkn,Head,Set1,T,F__,W,____,E,DHWX), // DHW1
  Props(5,'^',Data,DN,Unkn,Unkn,Head,Rset,T,F__,N,DHN0,S,____), // DHNR
  Props(5,'0',Data,DN,Unkn,Unkn,Head,Set0,T,F__,N,DHNX,S,DHNR), // DHN0
  Props(5,'X',Data,DN,Unkn,Unkn,Head,SetX,T,F__,N,DHN1,S,DHN0), // DHNX
  Props(5,'1',Data,DN,Unkn,Unkn,Head,Set1,T,F__,N,____,S,DHNX), // DHN1
  Props(5,'>',Data,DE,Unkn,Unkn,Tail,Rset,T,_R_,E,DTE0,W,____), // DTER
  Props(5,'>',Data,DE,Unkn,Unkn,Tail,Set0,T,_R_,E,DTEX,W,DTER), // DTE0
  Props(5,'>',Data,DE,Unkn,Unkn,Tail,SetX,T,_R_,E,DTE1,W,DTE0), // DTEX
  Props(5,'>',Data,DE,Unkn,Unkn,Tail,Set1,T,_R_,E,____,W,DTEX), // DTE1
  Props(5,'v',Data,DS,Unkn,Unkn,Tail,Rset,T,_R_,S,DTS0,N,____), // DTSR
  Props(5,'v',Data,DS,Unkn,Unkn,Tail,Set0,T,_R_,S,DTSX,N,DTSR), // DTS0
  Props(5,'v',Data,DS,Unkn,Unkn,Tail,SetX,T,_R_,S,DTS1,N,DTS0), // DTSX
  Props(5,'v',Data,DS,Unkn,Unkn,Tail,Set1,T,_R_,S,____,N,DTSX), // DTS1
  Props(5,'<',Data,DW,Unkn,Unkn,Tail,Rset,T,_R_,W,DTW0,E,____), // DTWR
  Props(5,'<',Data,DW,Unkn,Unkn,Tail,Set0,T,_R_,W,DTWX,E,DTWR), // DTW0
  Props(5,'<',Data,DW,Unkn,Unkn,Tail,SetX,T,_R_,W,DTW1,E,DTW0), // DTWX
  Props(5,'<',Data,DW,Unkn,Unkn,Tail,Set1,T,_R_,W,____,E,DTWX), // DTW1
  Props(5,'^',Data,DN,Unkn,Unkn,Tail,Rset,T,_R_,N,DTN0,S,____), // DTNR
  Props(5,'^',Data,DN,Unkn,Unkn,Tail,Set0,T,_R_,N,DTNX,S,DTNR), // DTN0
  Props(5,'^',Data,DN,Unkn,Unkn,Tail,SetX,T,_R_,N,DTN1,S,DTN0), // DTNX
  Props(5,'^',Data,DN,Unkn,Unkn,Tail,Set1,T,_R_,N,____,S,DTNX), // DTN1
  Props(4,'+',Data,DE,Unkn,Unkn,Post,Rset,T,FR_,E,DPE0,W,____), // DPER
  Props(4,'+',Data,DE,Unkn,Unkn,Post,Set0,T,FR_,E,DPEX,W,DPER), // DPE0
  Props(4,'+',Data,DE,Unkn,Unkn,Post,SetX,T,FR_,E,DPE1,W,DPE0), // DPEX
  Props(4,'+',Data,DE,Unkn,Unkn,Post,Set1,T,FR_,E,____,W,DPEX), // DPE1
  Props(4,'+',Data,DS,Unkn,Unkn,Post,Rset,T,FR_,S,DPS0,N,____), // DPSR
  Props(4,'+',Data,DS,Unkn,Unkn,Post,Set0,T,FR_,S,DPSX,N,DPSR), // DPS0
  Props(4,'+',Data,DS,Unkn,Unkn,Post,SetX,T,FR_,S,DPS1,N,DPS0), // DPSX
  Props(4,'+',Data,DS,Unkn,Unkn,Post,Set1,T,FR_,S,____,N,DPSX), // DPS1
  Props(4,'+',Data,DW,Unkn,Unkn,Post,Rset,T,FR_,W,DPW0,E,____), // DPWR
  Props(4,'+',Data,DW,Unkn,Unkn,Post,Set0,T,FR_,W,DPWX,E,DPWR), // DPW0
  Props(4,'+',Data,DW,Unkn,Unkn,Post,SetX,T,FR_,W,DPW1,E,DPW0), // DPWX
  Props(4,'+',Data,DW,Unkn,Unkn,Post,Set1,T,FR_,W,____,E,DPWX), // DPW1
  Props(4,'+',Data,DN,Unkn,Unkn,Post,Rset,T,FR_,N,DPN0,S,____), // DPNR
  Props(4,'+',Data,DN,Unkn,Unkn,Post,Set0,T,FR_,N,DPNX,S,DPNR), // DPN0
  Props(4,'+',Data,DN,Unkn,Unkn,Post,SetX,T,FR_,N,DPN1,S,DPN0), // DPNX
  Props(4,'+',Data,DN,Unkn,Unkn,Post,Set1,T,FR_,N,____,S,DPNX), // DPN1
  Props(5,'^',Data,DE,Unkn,Unkn,Lock,Rset,F,FRL,E,DKE0,W,____), // DKER
  Props(5,'^',Data,DE,Unkn,Unkn,Lock,Set0,F,FRL,E,DKEX,W,DKER), // DKE0
  Props(5,'^',Data,DE,Unkn,Unkn,Lock,SetX,F,FRL,E,DKE1,W,DKE0), // DKEX
  Props(5,'^',Data,DE,Unkn,Unkn,Lock,Set1,F,FRL,E,____,W,DKEX), // DKE1
  Props(5,'>',Data,DS,Unkn,Unkn,Lock,Rset,F,FRL,S,DKS0,N,____), // DKSR
  Props(5,'>',Data,DS,Unkn,Unkn,Lock,Set0,F,FRL,S,DKSX,N,DKSR), // DKS0
  Props(5,'>',Data,DS,Unkn,Unkn,Lock,SetX,F,FRL,S,DKS1,N,DKS0), // DKSX
  Props(5,'>',Data,DS,Unkn,Unkn,Lock,Set1,F,FRL,S,____,N,DKSX), // DKS1
  Props(5,'v',Data,DW,Unkn,Unkn,Lock,Rset,F,FRL,W,DKW0,E,____), // DKWR
  Props(5,'v',Data,DW,Unkn,Unkn,Lock,Set0,F,FRL,W,DKWX,E,DKWR), // DKW0
  Props(5,'v',Data,DW,Unkn,Unkn,Lock,SetX,F,FRL,W,DKW1,E,DKW0), // DKWX
  Props(5,'v',Data,DW,Unkn,Unkn,Lock,Set1,F,FRL,W,____,E,DKWX), // DKW1
  Props(5,'<',Data,DN,Unkn,Unkn,Lock,Rset,F,FRL,N,DKN0,S,____), // DKNR
  Props(5,'<',Data,DN,Unkn,Unkn,Lock,Set0,F,FRL,N,DKNX,S,DKNR), // DKN0
  Props(5,'<',Data,DN,Unkn,Unkn,Lock,SetX,F,FRL,N,DKN1,S,DKN0), // DKNX
  Props(5,'<',Data,DN,Unkn,Unkn,Lock,Set1,F,FRL,N,____,S,DKNX), // DKN1
  Props(5,'>',Data,DE,Unkn,Unkn,Test,Rset,F,FR_,E,DQE0,W,____), // DQER
  Props(5,'>',Data,DE,Unkn,Unkn,Test,Set0,F,FR_,E,DQEX,W,DQER), // DQE0
  Props(5,'>',Data,DE,Unkn,Unkn,Test,SetX,F,FR_,E,DQE1,W,DQE0), // DQEX
  Props(5,'>',Data,DE,Unkn,Unkn,Test,Set1,F,FR_,E,____,W,DQEX), // DQE1
  Props(5,'v',Data,DS,Unkn,Unkn,Test,Rset,F,FR_,S,DQS0,N,____), // DQSR
  Props(5,'v',Data,DS,Unkn,Unkn,Test,Set0,F,FR_,S,DQSX,N,DQSR), // DQS0
  Props(5,'v',Data,DS,Unkn,Unkn,Test,SetX,F,FR_,S,DQS1,N,DQS0), // DQSX
  Props(5,'v',Data,DS,Unkn,Unkn,Test,Set1,F,FR_,S,____,N,DQSX), // DQS1
  Props(5,'<',Data,DW,Unkn,Unkn,Test,Rset,F,FR_,W,DQW0,E,____), // DQWR
  Props(5,'<',Data,DW,Unkn,Unkn,Test,Set0,F,FR_,W,DQWX,E,DQWR), // DQW0
  Props(5,'<',Data,DW,Unkn,Unkn,Test,SetX,F,FR_,W,DQW1,E,DQW0), // DQWX
  Props(5,'<',Data,DW,Unkn,Unkn,Test,Set1,F,FR_,W,____,E,DQWX), // DQW1
  Props(5,'^',Data,DN,Unkn,Unkn,Test,Rset,F,FR_,N,DQN0,S,____), // DQNR
  Props(5,'^',Data,DN,Unkn,Unkn,Test,Set0,F,FR_,N,DQNX,S,DQNR), // DQN0
  Props(5,'^',Data,DN,Unkn,Unkn,Test,SetX,F,FR_,N,DQN1,S,DQN0), // DQNX
  Props(5,'^',Data,DN,Unkn,Unkn,Test,Set1,F,FR_,N,____,S,DQNX), // DQN1
  Props(5,'@',Data,DE,Unkn,Unkn,Gate,Rset,F,FR_,E,DGE0,W,____), // DGER
  Props(5,'@',Data,DE,Unkn,Unkn,Gate,Set0,F,FR_,E,DGEX,W,DGER), // DGE0
  Props(5,'@',Data,DE,Unkn,Unkn,Gate,SetX,F,FR_,E,DGE1,W,DGE0), // DGEX
  Props(5,'@',Data,DE,Unkn,Unkn,Gate,Set1,F,FR_,E,____,W,DGEX), // DGE1
  Props(5,'@',Data,DS,Unkn,Unkn,Gate,Rset,F,FR_,S,DGS0,N,____), // DGSR
  Props(5,'@',Data,DS,Unkn,Unkn,Gate,Set0,F,FR_,S,DGSX,N,DGSR), // DGS0
  Props(5,'@',Data,DS,Unkn,Unkn,Gate,SetX,F,FR_,S,DGS1,N,DGS0), // DGSX
  Props(5,'@',Data,DS,Unkn,Unkn,Gate,Set1,F,FR_,S,____,N,DGSX), // DGS1
  Props(5,'@',Data,DW,Unkn,Unkn,Gate,Rset,F,FR_,W,DGW0,E,____), // DGWR
  Props(5,'@',Data,DW,Unkn,Unkn,Gate,Set0,F,FR_,W,DGWX,E,DGWR), // DGW0
  Props(5,'@',Data,DW,Unkn,Unkn,Gate,SetX,F,FR_,W,DGW1,E,DGW0), // DGWX
  Props(5,'@',Data,DW,Unkn,Unkn,Gate,Set1,F,FR_,W,____,E,DGWX), // DGW1
  Props(5,'@',Data,DN,Unkn,Unkn,Gate,Rset,F,FR_,N,DGN0,S,____), // DGNR
  Props(5,'@',Data,DN,Unkn,Unkn,Gate,Set0,F,FR_,N,DGNX,S,DGNR), // DGN0
  Props(5,'@',Data,DN,Unkn,Unkn,Gate,SetX,F,FR_,N,DGN1,S,DGN0), // DGNX
  Props(5,'@',Data,DN,Unkn,Unkn,Gate,Set1,F,FR_,N,____,S,DGNX), // DGN1
  Props(6,'I',Data,DE,Unkn,Unkn,Inpt,Rset,T,F__,E,DIE0,W,____), // DIER
  Props(6,'0',Data,DE,Unkn,Unkn,Inpt,Set0,T,F__,E,DIEX,W,DIER), // DIE0
  Props(6,'X',Data,DE,Unkn,Unkn,Inpt,SetX,T,F__,E,DIE1,W,DIE0), // DIEX
  Props(6,'1',Data,DE,Unkn,Unkn,Inpt,Set1,T,F__,E,____,W,DIEX), // DIE1
  Props(6,'I',Data,DS,Unkn,Unkn,Inpt,Rset,T,F__,S,DIS0,N,____), // DISR
  Props(6,'0',Data,DS,Unkn,Unkn,Inpt,Set0,T,F__,S,DISX,N,DISR), // DIS0
  Props(6,'X',Data,DS,Unkn,Unkn,Inpt,SetX,T,F__,S,DIS1,N,DIS0), // DISX
  Props(6,'1',Data,DS,Unkn,Unkn,Inpt,Set1,T,F__,S,____,N,DISX), // DIS1
  Props(6,'I',Data,DW,Unkn,Unkn,Inpt,Rset,T,F__,W,DIW0,E,____), // DIWR
  Props(6,'0',Data,DW,Unkn,Unkn,Inpt,Set0,T,F__,W,DIWX,E,DIWR), // DIW0
  Props(6,'X',Data,DW,Unkn,Unkn,Inpt,SetX,T,F__,W,DIW1,E,DIW0), // DIWX
  Props(6,'1',Data,DW,Unkn,Unkn,Inpt,Set1,T,F__,W,____,E,DIWX), // DIW1
  Props(6,'I',Data,DN,Unkn,Unkn,Inpt,Rset,T,F__,N,DIN0,S,____), // DINR
  Props(6,'0',Data,DN,Unkn,Unkn,Inpt,Set0,T,F__,N,DINX,S,DINR), // DIN0
  Props(6,'X',Data,DN,Unkn,Unkn,Inpt,SetX,T,F__,N,DIN1,S,DIN0), // DINX
  Props(6,'1',Data,DN,Unkn,Unkn,Inpt,Set1,T,F__,N,____,S,DINX), // DIN1
  Props(6,'O',Data,DE,Unkn,Unkn,Outp,Rset,T,___,E,DOE0,W,____), // DOER
  Props(6,'0',Data,DE,Unkn,Unkn,Outp,Set0,T,___,E,DOEX,W,DOER), // DOE0
  Props(6,'X',Data,DE,Unkn,Unkn,Outp,SetX,T,___,E,DOE1,W,DOE0), // DOEX
  Props(6,'1',Data,DE,Unkn,Unkn,Outp,Set1,T,___,E,____,W,DOEX), // DOE1
  Props(6,'O',Data,DS,Unkn,Unkn,Outp,Rset,T,___,S,DOS0,N,____), // DOSR
  Props(6,'0',Data,DS,Unkn,Unkn,Outp,Set0,T,___,S,DOSX,N,DOSR), // DOS0
  Props(6,'X',Data,DS,Unkn,Unkn,Outp,SetX,T,___,S,DOS1,N,DOS0), // DOSX
  Props(6,'1',Data,DS,Unkn,Unkn,Outp,Set1,T,___,S,____,N,DOSX), // DOS1
  Props(6,'O',Data,DW,Unkn,Unkn,Outp,Rset,T,___,W,DOW0,E,____), // DOWR
  Props(6,'0',Data,DW,Unkn,Unkn,Outp,Set0,T,___,W,DOWX,E,DOWR), // DOW0
  Props(6,'X',Data,DW,Unkn,Unkn,Outp,SetX,T,___,W,DOW1,E,DOW0), // DOWX
  Props(6,'1',Data,DW,Unkn,Unkn,Outp,Set1,T,___,W,____,E,DOWX), // DOW1
  Props(6,'O',Data,DN,Unkn,Unkn,Outp,Rset,T,___,N,DON0,S,____), // DONR
  Props(6,'0',Data,DN,Unkn,Unkn,Outp,Set0,T,___,N,DONX,S,DONR), // DON0
  Props(6,'X',Data,DN,Unkn,Unkn,Outp,SetX,T,___,N,DON1,S,DON0), // DONX
  Props(6,'1',Data,DN,Unkn,Unkn,Outp,Set1,T,___,N,____,S,DONX)  // DON1
#undef _
#undef T
#undef F
};

typedef map<string, Voxel> StringToVoxel;
StringToVoxel stringToVoxel;

class Scenario {
  friend class Rule;
 public:
  Scenario(TickPerCycle _tick, Voxel _n, Voxel _w, Voxel _self, Voxel _e, Voxel _s) :
      tick(_tick),
      self(_self)
  {
    area[DirE] = _e;
    area[DirS] = _s;
    area[DirD] = Unkn;
    area[DirW] = _w;
    area[DirN] = _n;
    area[DirU] = Unkn;
  }
  Scenario(TickPerCycle _tick, Voxel _self, array<Voxel, eoDirection> _area) :
      tick(_tick),
      self(_self),
      area(_area)
  {
  }
  Scenario(Volume const *volume, TickPerCycle t, VoxelCoordinant const &v);
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
  Voxel FromE() const { return area[DirE]; }
  Voxel FromS() const { return area[DirS]; }
  Voxel FromD() const { return area[DirD]; }
  Voxel FromW() const { return area[DirW]; }
  Voxel FromN() const { return area[DirN]; }
  Voxel FromU() const { return area[DirU]; }
  Voxel Self() const { return self; }
  void Dump() const {
    fprintf(stdout,
            "(Scenario *)(%p)->{ %s, %s, { %s, %s, %s, %s } }\n",
            this,
            toConstCharPointer(tick),
            toConstCharPointer(self),
            toConstCharPointer(area[DirE]),
            toConstCharPointer(area[DirS]),
            toConstCharPointer(area[DirW]),
            toConstCharPointer(area[DirN])
           );
  }

 private:
  TickPerCycle tick;
  Voxel self;
  array<Voxel, eoDirection> area;
};

struct Rule {
  Rule(TickPerCycle tick, Voxel self, Voxel n, Voxel w, Voxel e, Voxel s, Voxel v) :
      scenario(tick, self, n, w, e, s),
      newVoxel(v)
  {
  }
  Rule(Scenario const &s, Voxel v) : scenario(s), newVoxel(v) { }
  void Dump() const {
    fprintf(stdout,
            "Rule(Scenario(%s, /* { */ %s, %s, %s, %s, %s /* } */), %s)",
            toConstCharPointer(scenario.tick),
            toConstCharPointer(scenario.area[DirN]),
            toConstCharPointer(scenario.area[DirW]),
            toConstCharPointer(scenario.self),
            toConstCharPointer(scenario.area[DirE]),
            toConstCharPointer(scenario.area[DirS]),
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
typedef array<array<char, NCols>, NRows> ViewLvlArray;

class Volume: public VolArray
{
 public:
  Volume(VolArray const &initial);
  Volume(Voxel initialVoxel = Unkn);
  void AddRule(Rule const &rule) { rules[rule.scenario] = rule.newVoxel; }
  void AddRule(Scenario const &scenario, Voxel newVoxel) { rules[scenario] = newVoxel; }
  Rules &GetRules() { return rules; }
  void AddInput(VoxelCoordinant v, deque<DataState> const &values) {
    inputs[v] = values;
  }
  deque<DataState> &GetInputsFor(VoxelCoordinant v) { return inputs[v]; }
  void ProceedOneCycle();
  void ProceedOnePhase();
  void ProceedOneTick();
  void PrintViewFlat() const;
  bool isVoxelCoordinantInBounds(VoxelCoordinant v) const {
    return 0 <= v.L() && v.L() < NLvls &&
        0 <= v.R() && v.R() < NRows &&
        0 <= v.C() && v.C() < NCols;
  }
  bool isPartOf(RodType r, VoxelCoordinant v, bool &reachedEnd) const;
  bool isPartOf(RodType r, VoxelCoordinant v) const;
  Voxel voxelAt(VoxelCoordinant v) const {
    if (isVoxelCoordinantInBounds(v)) {
      return (*this)[v.L()][v.R()][v.C()];
    }
    return Unkn;
  }
  int CurrentClock() const { return clock; }
  int CurrentCycle() const { return clock / NTicksPerCycle; }
  PhasePerCycle CurrentPhasePerCycle() const {
    return PhasePerCycle((CurrentClock() % NTicksPerCycle) / NTicksPerPhase);
  }
  TickPerCycle CurrentTickPerCycle() const {
    return TickPerCycle(CurrentClock() % NTicksPerCycle);
  }
  TickPerPhase CurrentTickPerPhase() const {
    return TickPerPhase(CurrentClock() % NTicksPerPhase);
  }

 private:
  // Voxel &voxelAt(VoxelCoordinant v) {
  //   assert(isVoxelCoordinantInBounds(v));
  //   return (*this)[v.L()][v.R()][v.C()];
  // }
  void ViewFlat(ViewLvlArray &view) const;
  void AddToRod(RodType rodType, Direction fwd, Direction bwd, Item *item, VoxelCoordinant v);
  Item *FormRodContaining(set<VoxelCoordinant> &seenSofar, VoxelCoordinant v);
  void FindItems();

  Rules rules;
  int clock;
  array<SetOfItems, eoRodType> itemsByRodType;
  map<VoxelCoordinant, deque<DataState>> inputs;
};

class Item: public set<VoxelCoordinant> {
 public:
  Item(RodType t):
      set<VoxelCoordinant>(),
      rodType(t),
      lastMovedAt(-1),
      fBlkState(FBUnkn),
      rBlkState(RBUnkn)
  {}
  virtual ~Item() {}

  virtual bool IsFBlked() const { return fBlkState == FBBlkd; }
  virtual bool IsRBlked() const { return rBlkState == RBBlkd; }
  virtual bool IsBlocked() const { return IsFBlked() || IsRBlked(); };
  virtual bool CheckForFreedomOfMovement(
      Volume *volume, FwdOrBwd fwdOrBwd) = 0;
  virtual bool CheckForFreedomOfMovement(Volume *volume) = 0;
  virtual bool AttemptToMove(
      Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes);
  virtual bool IsValid(Volume const *volume);
  virtual RodType GetRodType() const { return rodType; }
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
  LockRod(RodType t): Item(t), lockState(LSUnkn) {}

  bool IsLocked() const { return lockState == LSLckd; }

  bool CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd);
  bool CheckForFreedomOfMovement(Volume *volume);
  bool IsValid(Volume const *volume);
  void Dump(Volume const *volume = 0) const;
  char const *TypeName() const { return "LockRod"; }

 private:
  LockState lockState;
};

class DataRod: public Item {
 public:
  DataRod(RodType t): Item(t), lockState(LSUnkn), dataState(DSUnkn) {}

  bool IsLocked() const { return lockState == LSLckd; }

  bool IsFBlked() const { return Item::IsFBlked() || IsLocked(); }
  bool IsRBlked() const { return Item::IsRBlked() || IsLocked(); }
  bool IsBlocked() const { return Item::IsBlocked() || IsLocked(); };

  bool CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd);
  bool CheckForFreedomOfMovement(Volume *volume);
  bool IsValid(Volume const *volume);
  void Dump(Volume const *volume = 0) const;
  char const *TypeName() const { return "DataRod"; }

 private:
  LockState lockState;
  DataState dataState;
};

void VoxelCoordinant::Dump(Volume const *volume) const {
  if (volume) {
    fprintf(stdout,
            "%s(%d,%d,%d)",
            toConstCharPointer(volume->voxelAt(*this)),
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
                   VoxelCoordinant const &v
                  ) :
    tick(t),
    self(volume->voxelAt(v))
{
  area[DirE] = volume->voxelAt(v.To(DirE));
  area[DirS] = volume->voxelAt(v.To(DirS));
  area[DirD] = volume->voxelAt(v.To(DirD));
  area[DirW] = volume->voxelAt(v.To(DirW));
  area[DirN] = volume->voxelAt(v.To(DirN));
  area[DirU] = volume->voxelAt(v.To(DirU));
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
    VoxelCoordinant v = c.first;
    assert(isVoxelCoordinantInBounds(v));
    (*this)[v.L()][v.R()][v.C()] = c.second;
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

bool Volume::isPartOf(RodType r, VoxelCoordinant v, bool &reachedEnd) const {
  if (isVoxelCoordinantInBounds(v)) {
    Voxel voxel = voxelAt(v);
    if (!reachedEnd) {
      if (voxelProperties[voxelAt(v)].rodType == r) {
        return true;
      }
      reachedEnd = true;
    }
    return voxel == Slot;
  }
  reachedEnd = true;
  return false;
}

bool Volume::isPartOf(RodType r, VoxelCoordinant v) const {
  return isVoxelCoordinantInBounds(v) &&
      voxelProperties[voxelAt(v)].rodType == r;
}

void Volume::AddToRod(RodType rodType, Direction fwd, Direction bwd, Item *item, VoxelCoordinant v) {
  item->insert(v);
  bool reachedFwdEnd = false;
  for (VoxelCoordinant f = v.To(fwd);
       isPartOf(rodType, f, reachedFwdEnd);
       f.Move(fwd)
      )
  {
    item->insert(f);
    if (!reachedFwdEnd) {
      VoxelCoordinant u = f.ToU();
      if (isPartOf(rodType, u) && item->find(u) == item->end()) {
	AddToRod(rodType, fwd, bwd, item, u);
      }
      VoxelCoordinant d = f.ToD();
      if (isPartOf(rodType, d) && item->find(d) == item->end()) {
	AddToRod(rodType, fwd, bwd, item, d);
      }
    }
  }
  bool reachedBwdEnd = false;
  for (VoxelCoordinant b = v.To(bwd);
       isPartOf(rodType, b, reachedBwdEnd);
       b.Move(bwd)
      )
  {
    item->insert(b);
    if (!reachedBwdEnd) {
      VoxelCoordinant u = b.ToU();
      if (isPartOf(rodType, u) && item->find(u) == item->end()) {
	AddToRod(rodType, fwd, bwd, item, u);
      }
      VoxelCoordinant d = b.ToD();
      if (isPartOf(rodType, d) && item->find(d) == item->end()) {
	AddToRod(rodType, fwd, bwd, item, d);
      }
    }
  }
}

Item *Volume::FormRodContaining
  (set<VoxelCoordinant> &seenSofar, VoxelCoordinant v)
{
  VoxelProperties const &vProperties = voxelProperties[voxelAt(v)];
  RodTypeProperties const &rProperties = rodTypeProperties[vProperties.rodType];
  Item *item = 0;
  switch (vProperties.rodType) {
    case RTLE: case RTLS: case RTLW: case RTLN:
      item = new LockRod(vProperties.rodType);
      break;
    case RTDE: case RTDS: case RTDW: case RTDN:
      item = new DataRod(vProperties.rodType);
      break;
    default:
      return 0;
  }
  AddToRod(vProperties.rodType, rProperties.fwd,rProperties.bwd, item, v);
  seenSofar.insert(item->begin(), item->end());
  if (item->IsValid(this)) {
    itemsByRodType[vProperties.rodType].insert(item);
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
        VoxelCoordinant v(l, r, c);

	if (voxelProperties[voxelAt(v)].isRodBody) {
	  if (seenSofar.find(v) == seenSofar.end()) {
            Item *item = FormRodContaining(seenSofar, v);
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
  bool isMovable = CheckForFreedomOfMovement(volume, fwdOrBwd);

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

  if (isMovable) {
    TickPerCycle tick = volume->CurrentTickPerCycle();
    TickPerCycleProperties const &tProperties =
        tickPerCycleProperties[tick];
    assert(rodType == tProperties.rodType);
    RodTypeProperties const &rProperties = rodTypeProperties[rodType];
    Direction directionOfMotion =
        fwdOrBwd == Fwd ? rProperties.fwd : rProperties.bwd;
    Direction antiDirectionOfMotion =
        fwdOrBwd == Fwd ? rProperties.bwd : rProperties.fwd;
    bool madeChanges = false;

    for (auto const &c : *this) {
      Scenario scenario(volume, tick, c);
      Voxel thisVoxel = scenario.Self();
      Voxel prevVoxel = Unkn;
      Voxel nextVoxel = Unkn;
      VoxelProperties const &tvProperties = voxelProperties[thisVoxel];

      if (thisVoxel == Slot || rodType == tvProperties.rodType) {
        if (tvProperties.voxelType == VTData &&
            (tvProperties.dataType == DTInpt ||
             tvProperties.dataType == DTOutp
            )
           )
        {
          nextVoxel = tvProperties.motion[fwdOrBwd].nextVoxel;
        } else {
          prevVoxel = scenario.From(antiDirectionOfMotion);
          VoxelProperties const &pvProperties = voxelProperties[prevVoxel];
          if (pvProperties.rodType == rodType) {
            if (pvProperties.voxelType == VTData &&
                (pvProperties.dataType == DTInpt ||
                 pvProperties.dataType == DTOutp
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
    // Rules &rules = volume->GetRules();
    // for (auto const &v : *this) {
    //   Scenario scenario(volume, tick, v);
    //   if (rules.find(scenario) == rules.end()) {
    //     TickPerCycleProperties const &tProperties =
    //         tickPerCycleProperties[tick];
    //     RodTypeProperties const &rProperties =
    //         rodTypeProperties[tProperties.rodType];
    //     Direction fwdOrBwd =
    //         tProperties.fwdOrBwd == Fwd ? rProperties.fwd : rProperties.bwd;
    //     Direction fwd = rProperties.fwd;
    //     char const *directionOfMotion[eoDirection][eoFwdOrBwd] = {
    //       { ">", "<" }, // DirE, DirW
    //       { "v", "^" }, // DirS, DirN
    //       { "d", "u" }, // DirD, DirU
    //       { "<", ">" }, // DirW, DirE
    //       { "^", "v" }, // DirN, DirS
    //       { "u", "d" }  // DirU, DirD
    //     };
    // 
    //     switch (fwd) {
    //     case DirE:
    //     case DirW:
    //       fprintf(stdout,
    //               "     +------+------+------|\n"
    //               "%s | ???? | %s | ???? |\n"
    //               "     +------+------+------|\n"
    //               "     %s %s %s %s %s %s %s\n"
    //               "     +------+------+------|\n"
    //               "     | ???? | %s | ???? |\n"
    //               "     +------+------+------|\n",
    //     	  toConstCharPointer(tick),
    //     	  toConstCharPointer(scenario.N()),
    //     	  directionOfMotion[fwd][Fwd],
    //     	  toConstCharPointer(scenario.W()),
    //     	  directionOfMotion[fwd][tProperties.fwdOrBwd],
    //     	  toConstCharPointer(scenario.O()),
    //     	  directionOfMotion[fwd][tProperties.fwdOrBwd],
    //     	  toConstCharPointer(scenario.E()),
    //     	  directionOfMotion[fwd][Fwd],
    //     	  toConstCharPointer(scenario.S())
    //     	 );
    //       break;
    //     case DirN:
    //     case DirS:
    //       fprintf(stdout,
    //               "     +------+--%s---+------|\n"
    //               "%s | ???? | %s | ???? |\n"
    //               "     +------+--%s---+------|\n"
    //               "     | %s | %s | %s |\n"
    //               "     +------+--%s---+------|\n"
    //               "     | ???? | %s | ???? |\n"
    //               "     +------+--%s---+------|\n",
    //     	  directionOfMotion[fwd][Fwd],
    //     	  toConstCharPointer(tick),
    //     	  toConstCharPointer(scenario.N()),
    //     	  directionOfMotion[fwd][tProperties.fwdOrBwd],
    //     	  toConstCharPointer(scenario.W()),
    //     	  toConstCharPointer(scenario.O()),
    //     	  toConstCharPointer(scenario.E()),
    //     	  directionOfMotion[fwd][tProperties.fwdOrBwd],
    //     	  toConstCharPointer(scenario.S()),
    //     	  directionOfMotion[fwd][Fwd]
    //     	 );
    //       break;
    //     default:
    //       break;
    //     }
    // 
    //     char newVoxelChars[128];
    //     do {
    //       fprintf(stdout, "New voxel value? ");
    //       fscanf(stdin, " %s", newVoxelChars);
    //     } while (stringToVoxel.find(newVoxelChars) == stringToVoxel.end());
    // 
    //     Voxel newVoxel = stringToVoxel[newVoxelChars];
    //     rules[scenario] = newVoxel;
    // 
    //     Rule newRule(scenario, newVoxel);
    //     newRule.Dump();
    //     fprintf(stdout, "\n");
    //   }
    //   Voxel newVoxel = rules[scenario];
    // 
    //   fprintf(stdout, "(%s *)(%p)->AttemptToMove(): At ", TypeName(), this);
    //   v.Dump(volume);
    //   fprintf(stdout, ", applying rule ");
    //   Rule rule(scenario, newVoxel);
    //   rule.Dump();
    //   fprintf(stdout, "\n");
    // 
    //   changes[v] = newVoxel;
    //   madeChanges = true;
    // }
    if (madeChanges) {
      lastMovedAt = volume->CurrentClock();
    }
    return true;
  }
  return false;
}

bool Item::IsValid(Volume const *volume) {
  array<size_t, eoRodType> rodTypeCounts;

  rodTypeCounts.fill(0);
  rodType = eoRodType;
  bool foundMoreThan1RodType = false;
  for (auto const &v : *this) {
    VoxelProperties const &vProperties = voxelProperties[volume->voxelAt(v)];
    if (vProperties.voxelType != VTSlot) {
      rodTypeCounts[vProperties.rodType] += 1;
      foundMoreThan1RodType |=
          rodType != eoRodType &&
          rodType != vProperties.rodType;
      rodType = vProperties.rodType;
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
  lockState = LSUnkn;

  fBlkState = FBUnkn;
  rBlkState = RBUnkn;

  size_t fBlkCounts = 0;
  size_t rBlkCounts = 0;
  for (auto const &v : *this) {
    VoxelProperties const &vProperties = voxelProperties[volume->voxelAt(v)];
    if (vProperties.lockState != LSUnkn) {
      lockStateCounts[vProperties.lockState] += 1;
      foundMoreThan1LockState |=
	lockState != LSUnkn &&
	lockState != vProperties.lockState;
      lockState = vProperties.lockState;
    }

    if (blockStateProperties[vProperties.blockableStates].isFBlkable) {
      VoxelCoordinant f = v.To(rodTypeProperties[rodType].fwd);
      if (volume->isVoxelCoordinantInBounds(f)) {
        if (find(f) == end() && volume->voxelAt(f) != Slot) {
          if (optVerbose) {
            fprintf(stdout,
                    "(LockRod *)(%p) fBlked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
                    this,
                    toConstCharPointer(volume->voxelAt(v)),
                    v.L(),
                    v.R(),
                    v.C(),
                    toConstCharPointer(volume->voxelAt(f)),
                    f.L(),
                    f.R(),
                    f.C()
                    );
          }
          fBlkCounts += 1;
        }
      } else {
        if (optVerbose) {
          fprintf(stdout,
                  "(LockRod *)(%p) fBlked at %s(%d,%d,%d) by edge\n",
                  this,
                  toConstCharPointer(volume->voxelAt(v)),
                  v.L(),
                  v.R(),
                  v.C()
                  );
        }
        fBlkCounts += 1;
      }
    }
    if (blockStateProperties[vProperties.blockableStates].isRBlkable) {
      VoxelCoordinant b = v.To(rodTypeProperties[rodType].bwd);
      if (volume->isVoxelCoordinantInBounds(b)) {
        if (find(b) == end() && volume->voxelAt(b) != Slot) {
          if (optVerbose) {
            fprintf(stdout,
                    "(LockRod *)(%p) rBlked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
                    this,
                    toConstCharPointer(volume->voxelAt(v)),
                    v.L(),
                    v.R(),
                    v.C(),
                    toConstCharPointer(volume->voxelAt(b)),
                    b.L(),
                    b.R(),
                    b.C()
                    );
          }
          rBlkCounts += 1;
        }
      } else {
        if (optVerbose) {
          fprintf(stdout,
                  "(LockRod *)(%p) rBlked at %s(%d,%d,%d) by edge\n",
                  this,
                  toConstCharPointer(volume->voxelAt(v)),
                  v.L(),
                  v.R(),
                  v.C()
                  );
        }
        rBlkCounts += 1;
      }
    }
  }

  fBlkState = fBlkCounts != 0 ? FBBlkd : FBUnbk;
  rBlkState = rBlkCounts != 0 ? RBBlkd : RBUnbk;

  return !IsBlocked();
}

bool LockRod::IsValid(Volume const *volume) {
  if (!Item::IsValid(volume)) {
    return false;
  }

  array<size_t, eoRodType> lockStateCounts;

  lockStateCounts.fill(0);

  bool foundMoreThan1LockState = false;
  lockState = eoLockState;
  for (auto const &v : *this) {
    VoxelProperties const &vProperties = voxelProperties[volume->voxelAt(v)];
    if (vProperties.voxelType != VTSlot) {
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
  dataState = DSUnkn;

  fBlkState = FBUnkn;
  rBlkState = RBUnkn;
  lockState = LSUnkn;

  size_t fBlkCounts = 0;
  size_t rBlkCounts = 0;
  size_t lockCounts = 0;
  for (auto const &v : *this) {
    VoxelProperties const &vProperties = voxelProperties[volume->voxelAt(v)];
    if (rodType != vProperties.rodType) {
      continue;
    }

    if (vProperties.dataState != DSUnkn) {
      dataStateCounts[vProperties.dataState] += 1;
      foundMoreThan1DataState |=
	dataState != DSUnkn &&
	dataState != vProperties.dataState;
      dataState = vProperties.dataState;
    }

    if (blockStateProperties[vProperties.blockableStates].isFBlkable) {
      if (vProperties.dataType == DTInpt) {
        TickPerPhase tickPerPhase = volume->CurrentTickPerPhase();
        deque<DataState> &values = volume->GetInputsFor(v);
        DataState inputState = values.empty() ? DSSet0 : values.front();
        bool fBlockedByInput = false;
        switch (tickPerPhase) {
          case SetDataRodsTo0:
            fBlockedByInput = inputState < DSSet0;
            break;
          case SetDataRodsToX:
            fBlockedByInput = inputState < DSSetX;
            break;
          case SetDataRodsTo1:
            fBlockedByInput = inputState < DSSet1;
            if (!values.empty()) {
              values.pop_front();
            }
          default:
            break;
        }
        // fprintf(stdout,
        //         "(DataRod *)(%p)->CheckForFreedomOfMovement(...)"
        //         ": tickPerPhase=%s"
        //         ", values.size()=%ld"
        //         ", inputState=%s"
        //         ", fBlockedByInput=%s"
        //         "\n",
        //         this,
        //         toConstCharPointer(tickPerPhase),
        //         values.size(),
        //         toConstCharPointer(inputState),
        //         fBlockedByInput ? "true" : "false"
        //         );
        if (fBlockedByInput) {
          fBlkCounts += 1;
          if (optVerbose) {
            fprintf(stdout,
                    "(DataRod *)(%p) fBlked at %s(%d,%d,%d) by input (%s)\n",
                    this,
                    toConstCharPointer(volume->voxelAt(v)),
                    v.L(),
                    v.R(),
                    v.C(),
                    toConstCharPointer(inputState)
                    );
          }
        }
      } else {
        VoxelCoordinant f = v.To(rodTypeProperties[rodType].fwd);
        if (volume->isVoxelCoordinantInBounds(f)) {
          VoxelProperties const &fvProperties =
              voxelProperties[volume->voxelAt(f)];
          if (rodType != fvProperties.rodType && volume->voxelAt(f) != Slot) {
            if (optVerbose) {
              fprintf(stdout,
                      "(DataRod *)(%p) fBlked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
                      this,
                      toConstCharPointer(volume->voxelAt(v)),
                      v.L(),
                      v.R(),
                      v.C(),
                      toConstCharPointer(volume->voxelAt(f)),
                      f.L(),
                      f.R(),
                      f.C()
                      );
            }
            fBlkCounts += 1;
          }
        } else {
          if (optVerbose) {
            fprintf(stdout,
                    "(DataRod *)(%p) fBlked at %s(%d,%d,%d) by edge\n",
                    this,
                    toConstCharPointer(volume->voxelAt(v)),
                    v.L(),
                    v.R(),
                    v.C()
                    );
          }
          fBlkCounts += 1;
        }
      }
    }
    if (blockStateProperties[vProperties.blockableStates].isRBlkable) {
      VoxelCoordinant b = v.To(rodTypeProperties[rodType].bwd);
      if (volume->isVoxelCoordinantInBounds(b)) {
        VoxelProperties const &bvProperties =
            voxelProperties[volume->voxelAt(b)];
        if (rodType != bvProperties.rodType && volume->voxelAt(b) != Slot) {
          if (optVerbose) {
            fprintf(stdout,
                    "(DataRod *)(%p) rBlked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
                    this,
                    toConstCharPointer(volume->voxelAt(v)),
                    v.L(),
                    v.R(),
                    v.C(),
                    toConstCharPointer(volume->voxelAt(b)),
                    b.L(),
                    b.R(),
                    b.C()
                    );
          }
          rBlkCounts += 1;
        }
      } else {
        if (optVerbose) {
          fprintf(stdout,
                  "(DataRod *)(%p) rBlked at %s(%d,%d,%d) by edge\n",
                  this,
                  toConstCharPointer(volume->voxelAt(v)),
                  v.L(),
                  v.R(),
                  v.C()
                  );
        }
        rBlkCounts += 1;
      }
    }
    if (blockStateProperties[vProperties.blockableStates].isLockable) {
      VoxelCoordinant r = v.To(rodTypeProperties[rodType].rwd);
      if (volume->isVoxelCoordinantInBounds(r)) {
	Voxel rv = volume->voxelAt(r);
	VoxelProperties const rvProperties = voxelProperties[rv];
        if (rvProperties.voxelType == VTLock &&
            rvProperties.lockType == LTLock
           )
        {
          if (optVerbose) {
            fprintf(stdout,
                    "(DataRod *)(%p) locked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
                    this,
                    toConstCharPointer(volume->voxelAt(v)),
                    v.L(),
                    v.R(),
                    v.C(),
                    toConstCharPointer(volume->voxelAt(r)),
                    r.L(),
                    r.R(),
                    r.C()
                    );
          }
          lockCounts += 1;
        }
      }
    }
  }

  fBlkState = fBlkCounts != 0 ? FBBlkd : FBUnbk;
  rBlkState = rBlkCounts != 0 ? RBBlkd : RBUnbk;
  lockState = lockCounts != 0 ? LSLckd : LSUnlk;
  return !IsBlocked();
}

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
  for (auto const &v : *this) {
    VoxelProperties const &vProperties = voxelProperties[volume->voxelAt(v)];
    LockState lockState = vProperties.lockState;
    DataState dataState = vProperties.dataState;
    if (vProperties.voxelType != VTSlot) {
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
  /* i */ Wall,Wall,Wall,Wall,DKER,Slot,DKER,DKER,Slot,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* i */
  /* j */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LKSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* j */
  /* k */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* k */
  /* l */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* l */
  /* m */ Wall,Wall,Wall,Wall,DKER,Slot,DKER,DKER,Slot,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* m */
  /* n */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LKSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* n */
  /* o */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* o */
  /* p */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* p */
  /* q */ Wall,Wall,Wall,Wall,DKER,Slot,DKER,DKER,Slot,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* q */
  /* r */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,LKSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* r */
  /* s */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* s */
  /* t */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LKNL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* t */
  /* u */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,Slot,DKWR,DKWR,Slot,DKWR,Wall,Wall,Wall,Wall,Wall, /* u */
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
  /* b */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DKSR,Wall,Wall,Wall,DKSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* b */
  /* c */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* c */
  /* d */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DKSR,Wall,Wall,Wall,DKSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* d */
  /* e */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,LKWL,DKSR,Wall,Slot,LKWL,DKSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* e */
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
  /* y */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DKNR,LKEL,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* y */
  /* z */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DKNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* z */
  /* A */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* A */
  /* B */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DKNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* B */
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
  volume.AddInput
      (VoxelCoordinant(2,  8, 0),
       { DSSet1, DSSet0, DSSet0, DSSet1, DSSet0, DSSet0 }
      );
  volume.AddInput
      (VoxelCoordinant(2, 12, 0),
       { DSSet0, DSSet0, DSSet1, DSSet1, DSSet0, DSSet1 }
      );

  volume.PrintViewFlat();
  for (size_t t = 0; t < (6 * NTicksPerCycle); t += 1) {
    volume.ProceedOneTick();
    volume.PrintViewFlat();
    // for (auto const &r : volume.GetRules()) {
    //   if (r.second == Unkn) {
    //     fprintf(stdout, "  volume.Add");
    //     Rule rule(r.first, r.second);
    //     rule.Dump();
    //     fprintf(stdout, ";\n");
    //   }
    // }
  }
  return 0;
}
