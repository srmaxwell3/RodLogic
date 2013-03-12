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
#include <vector>
using std::vector;
#include <initializer_list>
using std::initializer_list;

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

// {E,S,W,N}{UNL,RN1,RNX,RN0,LNL,UCL,SC0,SCX,SC1,LCL}

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
  DSXXXX,
  DSSet1,

  eoDataState
};

char const *toConstCharPointer(DataState s) {
  switch (s) {
    case DSUnkn: return "DSUnkn";
    case DSRset: return "DSRset";
    case DSSet0: return "DSSet0";
    case DSXXXX: return "DSXXXX";
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

  RTKE, RTKS, RTKW, RTKN,
  RTLE, RTLS, RTLW, RTLN,

  eoRodType
};

char const *toConstCharPointer(RodType t) {
  switch (t) {
    case RTUn: return "RTUn";
    case RTKE: return "RTKE";
    case RTKS: return "RTKS";
    case RTKW: return "RTKW";
    case RTKN: return "RTKN";
    case RTLE: return "RTLE";
    case RTLS: return "RTLS";
    case RTLW: return "RTLW";
    case RTLN: return "RTLN";
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
  { { WUNL, WLNL, NUEL, NLEL, ____, ____ }, DirE, DirW, DirN, DirS }, // RTKE
  { { NUEL, NLEL, EUEL, ELEL, ____, ____ }, DirS, DirN, DirW, DirE }, // RTKS
  { { EUSL, ELSL, SUSL, SLSL, ____, ____ }, DirW, DirE, DirS, DirN }, // RTKW
  { { SUWL, SLWL, WUWL, WLWL, ____, ____ }, DirN, DirS, DirE, DirW }, // RTKN
  { { NRE1, NREX, NRE0, ESE0, ESEX, ESE1 }, DirE, DirW, DirN, DirS }, // RTLE
  { { ERS1, ERSX, ERS0, SSS0, SSSX, SSS1 }, DirS, DirN, DirW, DirE }, // RTLS
  { { SRW1, SRWX, SRW0, WSW0, WSWX, WSW1 }, DirW, DirE, DirS, DirN }, // RTLW
  { { WRN1, WRNX, WRN0, NSN0, NSNX, NSN1 }, DirN, DirS, DirE, DirW }, // RTLN
#undef ____
};

struct TickPerCycleProperties {
  RodType rodType;
  FwdOrBwd fwdOrBwd;
} tickPerCycleProperties[eoTickPerCycle] = {
  { RTKW, Fwd }, // EUSL
  { RTLS, Bwd }, // ERS1
  { RTLS, Bwd }, // ERSX
  { RTLS, Bwd }, // ERS0
  { RTKW, Bwd }, // ELSL
  { RTKS, Fwd }, // EUEL
  { RTLE, Fwd }, // ESE0
  { RTLE, Fwd }, // ESEX
  { RTLE, Fwd }, // ESE1
  { RTKS, Bwd }, // ELEL
  { RTKN, Fwd }, // SUWL
  { RTLW, Bwd }, // SRW1
  { RTLW, Bwd }, // SRWX
  { RTLW, Bwd }, // SRW0
  { RTKN, Bwd }, // SLWL
  { RTKW, Fwd }, // SUSL
  { RTLS, Fwd }, // SSS0
  { RTLS, Fwd }, // SSSX
  { RTLS, Fwd }, // SSS1
  { RTKW, Bwd }, // SLSL
  { RTKE, Fwd }, // WUNL
  { RTLN, Bwd }, // WRN1
  { RTLN, Bwd }, // WRNX
  { RTLN, Bwd }, // WRN0
  { RTKE, Bwd }, // WLNL
  { RTKN, Fwd }, // WUWL
  { RTLW, Fwd }, // WSW0
  { RTLW, Fwd }, // WSWX
  { RTLW, Fwd }, // WSW1
  { RTKN, Bwd }, // WLWL
  { RTKS, Fwd }, // NUEL
  { RTLE, Bwd }, // NRE1
  { RTLE, Bwd }, // NREX
  { RTLE, Bwd }, // NRE0
  { RTKS, Bwd }, // NLEL
  { RTKE, Fwd }, // NUEL
  { RTLN, Fwd }, // NSE0
  { RTLN, Fwd }, // NSEX
  { RTLN, Fwd }, // NSE1
  { RTKE, Bwd }  // NLEL
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
  VoxelType type;
  PhasePerCycle direction;
  RodType rodType;
  LockType lockType;
  LockState lockState;
  DataType dataType;
  DataState dataState;
  bool isRodBody;
  BlockState blockableStates;
} voxelProperties[eoVoxel] = {
#undef _
#define _ Unknown
#undef T
#define T true
#undef F
#define F false
  { 9,'?',VTUnkn,_,RTUn,LTUnkn,LSUnkn,DTUnkn,DSUnkn,F,___ }, // Unkn
  { 0,'#',VTWall,_,RTUn,LTUnkn,LSUnkn,DTUnkn,DSUnkn,F,___ }, // Wall
  { 1,' ',VTSlot,_,RTUn,LTUnkn,LSUnkn,DTUnkn,DSUnkn,F,___ }, // Slot
  { 2,'-',VTLock,E,RTKE,LTBody,LSLckd,DTUnkn,DSUnkn,T,___ }, // LBEL
  { 2,'-',VTLock,E,RTKE,LTBody,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LBEU
  { 2,'|',VTLock,S,RTKS,LTBody,LSLckd,DTUnkn,DSUnkn,T,___ }, // LBSL
  { 2,'|',VTLock,S,RTKS,LTBody,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LBSU
  { 2,'-',VTLock,W,RTKW,LTBody,LSLckd,DTUnkn,DSUnkn,T,___ }, // LBWL
  { 2,'-',VTLock,W,RTKW,LTBody,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LBWU
  { 2,'|',VTLock,N,RTKN,LTBody,LSLckd,DTUnkn,DSUnkn,T,___ }, // LBNL
  { 2,'|',VTLock,N,RTKN,LTBody,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LBNU
  { 3,'>',VTLock,E,RTKE,LTHead,LSLckd,DTUnkn,DSUnkn,T,___ }, // LHEL
  { 3,'>',VTLock,E,RTKE,LTHead,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LHEU
  { 3,'v',VTLock,S,RTKS,LTHead,LSLckd,DTUnkn,DSUnkn,T,___ }, // LHSL
  { 3,'v',VTLock,S,RTKS,LTHead,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LHSU
  { 3,'<',VTLock,W,RTKW,LTHead,LSLckd,DTUnkn,DSUnkn,T,___ }, // LHWL
  { 3,'<',VTLock,W,RTKW,LTHead,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LHWU
  { 3,'^',VTLock,N,RTKN,LTHead,LSLckd,DTUnkn,DSUnkn,T,___ }, // LHNL
  { 3,'^',VTLock,N,RTKN,LTHead,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LHNU
  { 3,'>',VTLock,E,RTKE,LTTail,LSLckd,DTUnkn,DSUnkn,T,___ }, // LTEL
  { 3,'>',VTLock,E,RTKE,LTTail,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LTEU
  { 3,'v',VTLock,S,RTKS,LTTail,LSLckd,DTUnkn,DSUnkn,T,___ }, // LTSL
  { 3,'v',VTLock,S,RTKS,LTTail,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LTSU
  { 3,'<',VTLock,W,RTKW,LTTail,LSLckd,DTUnkn,DSUnkn,T,___ }, // LTWL
  { 3,'<',VTLock,W,RTKW,LTTail,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LTWU
  { 3,'^',VTLock,N,RTKN,LTTail,LSLckd,DTUnkn,DSUnkn,T,___ }, // LTNL
  { 3,'^',VTLock,N,RTKN,LTTail,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LTNU
  { 2,'+',VTLock,E,RTKE,LTPost,LSLckd,DTUnkn,DSUnkn,T,___ }, // LPEL
  { 2,'+',VTLock,E,RTKE,LTPost,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LPEU
  { 2,'+',VTLock,S,RTKS,LTPost,LSLckd,DTUnkn,DSUnkn,T,___ }, // LPSL
  { 2,'+',VTLock,S,RTKS,LTPost,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LPSU
  { 2,'+',VTLock,W,RTKW,LTPost,LSLckd,DTUnkn,DSUnkn,T,___ }, // LPWL
  { 2,'+',VTLock,W,RTKW,LTPost,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LPWU
  { 2,'+',VTLock,N,RTKN,LTPost,LSLckd,DTUnkn,DSUnkn,T,___ }, // LPNL
  { 2,'+',VTLock,N,RTKN,LTPost,LSUnlk,DTUnkn,DSUnkn,T,___ }, // LPNU
  { 3,'<',VTLock,E,RTKE,LTLock,LSLckd,DTUnkn,DSUnkn,F,FRL }, // LKEL
  { 3,'<',VTLock,E,RTKE,LTLock,LSUnlk,DTUnkn,DSUnkn,F,FRL }, // LKEU
  { 3,'^',VTLock,S,RTKS,LTLock,LSLckd,DTUnkn,DSUnkn,F,FRL }, // LKSL
  { 3,'^',VTLock,S,RTKS,LTLock,LSUnlk,DTUnkn,DSUnkn,F,FRL }, // LKSU
  { 3,'>',VTLock,W,RTKW,LTLock,LSLckd,DTUnkn,DSUnkn,F,FRL }, // LKWL
  { 3,'>',VTLock,W,RTKW,LTLock,LSUnlk,DTUnkn,DSUnkn,F,FRL }, // LKWU
  { 3,'v',VTLock,N,RTKN,LTLock,LSLckd,DTUnkn,DSUnkn,F,FRL }, // LKNL
  { 3,'v',VTLock,N,RTKN,LTLock,LSUnlk,DTUnkn,DSUnkn,F,FRL }, // LKNU
  { 4,'-',VTData,E,RTLE,LTUnkn,LSUnkn,DTBody,DSRset,T,___ }, // DBER
  { 4,'-',VTData,E,RTLE,LTUnkn,LSUnkn,DTBody,DSSet0,T,___ }, // DBE0
  { 4,'-',VTData,E,RTLE,LTUnkn,LSUnkn,DTBody,DSXXXX,T,___ }, // DBEX
  { 4,'-',VTData,E,RTLE,LTUnkn,LSUnkn,DTBody,DSSet1,T,___ }, // DBE1
  { 4,'|',VTData,S,RTLS,LTUnkn,LSUnkn,DTBody,DSRset,T,___ }, // DBSR
  { 4,'|',VTData,S,RTLS,LTUnkn,LSUnkn,DTBody,DSSet0,T,___ }, // DBS0
  { 4,'|',VTData,S,RTLS,LTUnkn,LSUnkn,DTBody,DSXXXX,T,___ }, // DBSX
  { 4,'|',VTData,S,RTLS,LTUnkn,LSUnkn,DTBody,DSSet1,T,___ }, // DBS1
  { 4,'-',VTData,W,RTLW,LTUnkn,LSUnkn,DTBody,DSRset,T,___ }, // DBWR
  { 4,'-',VTData,W,RTLW,LTUnkn,LSUnkn,DTBody,DSSet0,T,___ }, // DBW0
  { 4,'-',VTData,W,RTLW,LTUnkn,LSUnkn,DTBody,DSXXXX,T,___ }, // DBWX
  { 4,'-',VTData,W,RTLW,LTUnkn,LSUnkn,DTBody,DSSet1,T,___ }, // DBW1
  { 4,'|',VTData,N,RTLN,LTUnkn,LSUnkn,DTBody,DSRset,T,___ }, // DBNR
  { 4,'|',VTData,N,RTLN,LTUnkn,LSUnkn,DTBody,DSSet0,T,___ }, // DBN0
  { 4,'|',VTData,N,RTLN,LTUnkn,LSUnkn,DTBody,DSXXXX,T,___ }, // DBNX
  { 4,'|',VTData,N,RTLN,LTUnkn,LSUnkn,DTBody,DSSet1,T,___ }, // DBN1
  { 5,'>',VTData,E,RTLE,LTUnkn,LSUnkn,DTHead,DSRset,T,F__ }, // DHER
  { 5,'>',VTData,E,RTLE,LTUnkn,LSUnkn,DTHead,DSSet0,T,F__ }, // DHE0
  { 5,'>',VTData,E,RTLE,LTUnkn,LSUnkn,DTHead,DSXXXX,T,F__ }, // DHEX
  { 5,'>',VTData,E,RTLE,LTUnkn,LSUnkn,DTHead,DSSet1,T,F__ }, // DHE1
  { 5,'v',VTData,S,RTLS,LTUnkn,LSUnkn,DTHead,DSRset,T,F__ }, // DHSR
  { 5,'v',VTData,S,RTLS,LTUnkn,LSUnkn,DTHead,DSSet0,T,F__ }, // DHS0
  { 5,'v',VTData,S,RTLS,LTUnkn,LSUnkn,DTHead,DSXXXX,T,F__ }, // DHSX
  { 5,'v',VTData,S,RTLS,LTUnkn,LSUnkn,DTHead,DSSet1,T,F__ }, // DHS1
  { 5,'<',VTData,W,RTLW,LTUnkn,LSUnkn,DTHead,DSRset,T,F__ }, // DHWR
  { 5,'<',VTData,W,RTLW,LTUnkn,LSUnkn,DTHead,DSSet0,T,F__ }, // DHW0
  { 5,'<',VTData,W,RTLW,LTUnkn,LSUnkn,DTHead,DSXXXX,T,F__ }, // DHWX
  { 5,'<',VTData,W,RTLW,LTUnkn,LSUnkn,DTHead,DSSet1,T,F__ }, // DHW1
  { 5,'^',VTData,N,RTLN,LTUnkn,LSUnkn,DTHead,DSRset,T,F__ }, // DHNR
  { 5,'^',VTData,N,RTLN,LTUnkn,LSUnkn,DTHead,DSSet0,T,F__ }, // DHN0
  { 5,'^',VTData,N,RTLN,LTUnkn,LSUnkn,DTHead,DSXXXX,T,F__ }, // DHNX
  { 5,'^',VTData,N,RTLN,LTUnkn,LSUnkn,DTHead,DSSet1,T,F__ }, // DHN1
  { 5,'>',VTData,E,RTLE,LTUnkn,LSUnkn,DTTail,DSRset,T,_R_ }, // DTER
  { 5,'>',VTData,E,RTLE,LTUnkn,LSUnkn,DTTail,DSSet0,T,_R_ }, // DTE0
  { 5,'>',VTData,E,RTLE,LTUnkn,LSUnkn,DTTail,DSXXXX,T,_R_ }, // DTEX
  { 5,'>',VTData,E,RTLE,LTUnkn,LSUnkn,DTTail,DSSet1,T,_R_ }, // DTE1
  { 5,'v',VTData,S,RTLS,LTUnkn,LSUnkn,DTTail,DSRset,T,_R_ }, // DTSR
  { 5,'v',VTData,S,RTLS,LTUnkn,LSUnkn,DTTail,DSSet0,T,_R_ }, // DTS0
  { 5,'v',VTData,S,RTLS,LTUnkn,LSUnkn,DTTail,DSXXXX,T,_R_ }, // DTSX
  { 5,'v',VTData,S,RTLS,LTUnkn,LSUnkn,DTTail,DSSet1,T,_R_ }, // DTS1
  { 5,'<',VTData,W,RTLW,LTUnkn,LSUnkn,DTTail,DSRset,T,_R_ }, // DTWR
  { 5,'<',VTData,W,RTLW,LTUnkn,LSUnkn,DTTail,DSSet0,T,_R_ }, // DTW0
  { 5,'<',VTData,W,RTLW,LTUnkn,LSUnkn,DTTail,DSXXXX,T,_R_ }, // DTWX
  { 5,'<',VTData,W,RTLW,LTUnkn,LSUnkn,DTTail,DSSet1,T,_R_ }, // DTW1
  { 5,'^',VTData,N,RTLN,LTUnkn,LSUnkn,DTTail,DSRset,T,_R_ }, // DTNR
  { 5,'^',VTData,N,RTLN,LTUnkn,LSUnkn,DTTail,DSSet0,T,_R_ }, // DTN0
  { 5,'^',VTData,N,RTLN,LTUnkn,LSUnkn,DTTail,DSXXXX,T,_R_ }, // DTNX
  { 5,'^',VTData,N,RTLN,LTUnkn,LSUnkn,DTTail,DSSet1,T,_R_ }, // DTN1
  { 4,'+',VTData,E,RTLE,LTUnkn,LSUnkn,DTPost,DSRset,T,FR_ }, // DPER
  { 4,'+',VTData,E,RTLE,LTUnkn,LSUnkn,DTPost,DSSet0,T,FR_ }, // DPE0
  { 4,'+',VTData,E,RTLE,LTUnkn,LSUnkn,DTPost,DSXXXX,T,FR_ }, // DPEX
  { 4,'+',VTData,E,RTLE,LTUnkn,LSUnkn,DTPost,DSSet1,T,FR_ }, // DPE1
  { 4,'+',VTData,S,RTLS,LTUnkn,LSUnkn,DTPost,DSRset,T,FR_ }, // DPSR
  { 4,'+',VTData,S,RTLS,LTUnkn,LSUnkn,DTPost,DSSet0,T,FR_ }, // DPS0
  { 4,'+',VTData,S,RTLS,LTUnkn,LSUnkn,DTPost,DSXXXX,T,FR_ }, // DPSX
  { 4,'+',VTData,S,RTLS,LTUnkn,LSUnkn,DTPost,DSSet1,T,FR_ }, // DPS1
  { 4,'+',VTData,W,RTLW,LTUnkn,LSUnkn,DTPost,DSRset,T,FR_ }, // DPWR
  { 4,'+',VTData,W,RTLW,LTUnkn,LSUnkn,DTPost,DSSet0,T,FR_ }, // DPW0
  { 4,'+',VTData,W,RTLW,LTUnkn,LSUnkn,DTPost,DSXXXX,T,FR_ }, // DPWX
  { 4,'+',VTData,W,RTLW,LTUnkn,LSUnkn,DTPost,DSSet1,T,FR_ }, // DPW1
  { 4,'+',VTData,N,RTLN,LTUnkn,LSUnkn,DTPost,DSRset,T,FR_ }, // DPNR
  { 4,'+',VTData,N,RTLN,LTUnkn,LSUnkn,DTPost,DSSet0,T,FR_ }, // DPN0
  { 4,'+',VTData,N,RTLN,LTUnkn,LSUnkn,DTPost,DSXXXX,T,FR_ }, // DPNX
  { 4,'+',VTData,N,RTLN,LTUnkn,LSUnkn,DTPost,DSSet1,T,FR_ }, // DPN1
  { 5,'^',VTData,E,RTLE,LTUnkn,LSUnkn,DTLock,DSRset,F,FRL }, // DKER
  { 5,'^',VTData,E,RTLE,LTUnkn,LSUnkn,DTLock,DSSet0,F,FRL }, // DKE0
  { 5,'^',VTData,E,RTLE,LTUnkn,LSUnkn,DTLock,DSXXXX,F,FRL }, // DKEX
  { 5,'^',VTData,E,RTLE,LTUnkn,LSUnkn,DTLock,DSSet1,F,FRL }, // DKE1
  { 5,'>',VTData,S,RTLS,LTUnkn,LSUnkn,DTLock,DSRset,F,FRL }, // DKSR
  { 5,'>',VTData,S,RTLS,LTUnkn,LSUnkn,DTLock,DSSet0,F,FRL }, // DKS0
  { 5,'>',VTData,S,RTLS,LTUnkn,LSUnkn,DTLock,DSXXXX,F,FRL }, // DKSX
  { 5,'>',VTData,S,RTLS,LTUnkn,LSUnkn,DTLock,DSSet1,F,FRL }, // DKS1
  { 5,'v',VTData,W,RTLW,LTUnkn,LSUnkn,DTLock,DSRset,F,FRL }, // DKWR
  { 5,'v',VTData,W,RTLW,LTUnkn,LSUnkn,DTLock,DSSet0,F,FRL }, // DKW0
  { 5,'v',VTData,W,RTLW,LTUnkn,LSUnkn,DTLock,DSXXXX,F,FRL }, // DKWX
  { 5,'v',VTData,W,RTLW,LTUnkn,LSUnkn,DTLock,DSSet1,F,FRL }, // DKW1
  { 5,'<',VTData,N,RTLN,LTUnkn,LSUnkn,DTLock,DSRset,F,FRL }, // DKNR
  { 5,'<',VTData,N,RTLN,LTUnkn,LSUnkn,DTLock,DSSet0,F,FRL }, // DKN0
  { 5,'<',VTData,N,RTLN,LTUnkn,LSUnkn,DTLock,DSXXXX,F,FRL }, // DKNX
  { 5,'<',VTData,N,RTLN,LTUnkn,LSUnkn,DTLock,DSSet1,F,FRL }, // DKN1
  { 5,'>',VTData,E,RTLE,LTUnkn,LSUnkn,DTTest,DSRset,F,FR_ }, // DQER
  { 5,'>',VTData,E,RTLE,LTUnkn,LSUnkn,DTTest,DSSet0,F,FR_ }, // DQE0
  { 5,'>',VTData,E,RTLE,LTUnkn,LSUnkn,DTTest,DSXXXX,F,FR_ }, // DQEX
  { 5,'>',VTData,E,RTLE,LTUnkn,LSUnkn,DTTest,DSSet1,F,FR_ }, // DQE1
  { 5,'v',VTData,S,RTLS,LTUnkn,LSUnkn,DTTest,DSRset,F,FR_ }, // DQSR
  { 5,'v',VTData,S,RTLS,LTUnkn,LSUnkn,DTTest,DSSet0,F,FR_ }, // DQS0
  { 5,'v',VTData,S,RTLS,LTUnkn,LSUnkn,DTTest,DSXXXX,F,FR_ }, // DQSX
  { 5,'v',VTData,S,RTLS,LTUnkn,LSUnkn,DTTest,DSSet1,F,FR_ }, // DQS1
  { 5,'<',VTData,W,RTLW,LTUnkn,LSUnkn,DTTest,DSRset,F,FR_ }, // DQWR
  { 5,'<',VTData,W,RTLW,LTUnkn,LSUnkn,DTTest,DSSet0,F,FR_ }, // DQW0
  { 5,'<',VTData,W,RTLW,LTUnkn,LSUnkn,DTTest,DSXXXX,F,FR_ }, // DQWX
  { 5,'<',VTData,W,RTLW,LTUnkn,LSUnkn,DTTest,DSSet1,F,FR_ }, // DQW1
  { 5,'^',VTData,N,RTLN,LTUnkn,LSUnkn,DTTest,DSRset,F,FR_ }, // DQNR
  { 5,'^',VTData,N,RTLN,LTUnkn,LSUnkn,DTTest,DSSet0,F,FR_ }, // DQN0
  { 5,'^',VTData,N,RTLN,LTUnkn,LSUnkn,DTTest,DSXXXX,F,FR_ }, // DQNX
  { 5,'^',VTData,N,RTLN,LTUnkn,LSUnkn,DTTest,DSSet1,F,FR_ }, // DQN1
  { 5,'@',VTData,E,RTLE,LTUnkn,LSUnkn,DTGate,DSRset,F,FR_ }, // DGER
  { 5,'@',VTData,E,RTLE,LTUnkn,LSUnkn,DTGate,DSSet0,F,FR_ }, // DGE0
  { 5,'@',VTData,E,RTLE,LTUnkn,LSUnkn,DTGate,DSXXXX,F,FR_ }, // DGEX
  { 5,'@',VTData,E,RTLE,LTUnkn,LSUnkn,DTGate,DSSet1,F,FR_ }, // DGE1
  { 5,'@',VTData,S,RTLS,LTUnkn,LSUnkn,DTGate,DSRset,F,FR_ }, // DGSR
  { 5,'@',VTData,S,RTLS,LTUnkn,LSUnkn,DTGate,DSSet0,F,FR_ }, // DGS0
  { 5,'@',VTData,S,RTLS,LTUnkn,LSUnkn,DTGate,DSXXXX,F,FR_ }, // DGSX
  { 5,'@',VTData,S,RTLS,LTUnkn,LSUnkn,DTGate,DSSet1,F,FR_ }, // DGS1
  { 5,'@',VTData,W,RTLW,LTUnkn,LSUnkn,DTGate,DSRset,F,FR_ }, // DGWR
  { 5,'@',VTData,W,RTLW,LTUnkn,LSUnkn,DTGate,DSSet0,F,FR_ }, // DGW0
  { 5,'@',VTData,W,RTLW,LTUnkn,LSUnkn,DTGate,DSXXXX,F,FR_ }, // DGWX
  { 5,'@',VTData,W,RTLW,LTUnkn,LSUnkn,DTGate,DSSet1,F,FR_ }, // DGW1
  { 5,'@',VTData,N,RTLN,LTUnkn,LSUnkn,DTGate,DSRset,F,FR_ }, // DGNR
  { 5,'@',VTData,N,RTLN,LTUnkn,LSUnkn,DTGate,DSSet0,F,FR_ }, // DGN0
  { 5,'@',VTData,N,RTLN,LTUnkn,LSUnkn,DTGate,DSXXXX,F,FR_ }, // DGNX
  { 5,'@',VTData,N,RTLN,LTUnkn,LSUnkn,DTGate,DSSet1,F,FR_ }, // DGN1
  { 6,'X',VTData,E,RTLE,LTUnkn,LSUnkn,DTInpt,DSRset,T,___ }, // DIER
  { 6,'0',VTData,E,RTLE,LTUnkn,LSUnkn,DTInpt,DSSet0,T,___ }, // DIE0
  { 6,'X',VTData,E,RTLE,LTUnkn,LSUnkn,DTInpt,DSXXXX,T,___ }, // DIEX
  { 6,'1',VTData,E,RTLE,LTUnkn,LSUnkn,DTInpt,DSSet1,T,___ }, // DIE1
  { 6,'X',VTData,S,RTLS,LTUnkn,LSUnkn,DTInpt,DSRset,T,___ }, // DISR
  { 6,'0',VTData,S,RTLS,LTUnkn,LSUnkn,DTInpt,DSSet0,T,___ }, // DIS0
  { 6,'X',VTData,S,RTLS,LTUnkn,LSUnkn,DTInpt,DSXXXX,T,___ }, // DISX
  { 6,'1',VTData,S,RTLS,LTUnkn,LSUnkn,DTInpt,DSSet1,T,___ }, // DIS1
  { 6,'X',VTData,W,RTLW,LTUnkn,LSUnkn,DTInpt,DSRset,T,___ }, // DIWR
  { 6,'0',VTData,W,RTLW,LTUnkn,LSUnkn,DTInpt,DSSet0,T,___ }, // DIW0
  { 6,'X',VTData,W,RTLW,LTUnkn,LSUnkn,DTInpt,DSXXXX,T,___ }, // DIWX
  { 6,'1',VTData,W,RTLW,LTUnkn,LSUnkn,DTInpt,DSSet1,T,___ }, // DIW1
  { 6,'X',VTData,N,RTLN,LTUnkn,LSUnkn,DTInpt,DSRset,T,___ }, // DINR
  { 6,'0',VTData,N,RTLN,LTUnkn,LSUnkn,DTInpt,DSSet0,T,___ }, // DIN0
  { 6,'X',VTData,N,RTLN,LTUnkn,LSUnkn,DTInpt,DSXXXX,T,___ }, // DINX
  { 6,'1',VTData,N,RTLN,LTUnkn,LSUnkn,DTInpt,DSSet1,T,___ }, // DIN1
  { 6,'O',VTData,E,RTLE,LTUnkn,LSUnkn,DTOutp,DSRset,T,___ }, // DOER
  { 6,'0',VTData,E,RTLE,LTUnkn,LSUnkn,DTOutp,DSSet0,T,___ }, // DOE0
  { 6,'X',VTData,E,RTLE,LTUnkn,LSUnkn,DTOutp,DSXXXX,T,___ }, // DOEX
  { 6,'1',VTData,E,RTLE,LTUnkn,LSUnkn,DTOutp,DSSet1,T,___ }, // DOE1
  { 6,'O',VTData,S,RTLS,LTUnkn,LSUnkn,DTOutp,DSRset,T,___ }, // DOSR
  { 6,'0',VTData,S,RTLS,LTUnkn,LSUnkn,DTOutp,DSSet0,T,___ }, // DOS0
  { 6,'X',VTData,S,RTLS,LTUnkn,LSUnkn,DTOutp,DSXXXX,T,___ }, // DOSX
  { 6,'1',VTData,S,RTLS,LTUnkn,LSUnkn,DTOutp,DSSet1,T,___ }, // DOS1
  { 6,'O',VTData,W,RTLW,LTUnkn,LSUnkn,DTOutp,DSRset,T,___ }, // DOWR
  { 6,'0',VTData,W,RTLW,LTUnkn,LSUnkn,DTOutp,DSSet0,T,___ }, // DOW0
  { 6,'X',VTData,W,RTLW,LTUnkn,LSUnkn,DTOutp,DSXXXX,T,___ }, // DOWX
  { 6,'1',VTData,W,RTLW,LTUnkn,LSUnkn,DTOutp,DSSet1,T,___ }, // DOW1
  { 6,'O',VTData,N,RTLN,LTUnkn,LSUnkn,DTOutp,DSRset,T,___ }, // DONR
  { 6,'0',VTData,N,RTLN,LTUnkn,LSUnkn,DTOutp,DSSet0,T,___ }, // DON0
  { 6,'X',VTData,N,RTLN,LTUnkn,LSUnkn,DTOutp,DSXXXX,T,___ }, // DONX
  { 6,'1',VTData,N,RTLN,LTUnkn,LSUnkn,DTOutp,DSSet1,T,___ }  // DON1
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
    area[0] = _n;
    area[1] = _w;
    area[2] = _e;
    area[3] = _s;
  }
  Scenario(TickPerCycle _tick, Voxel _self, array<Voxel, 4> _area) :
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
  Voxel E() const { return area[2]; }
  Voxel S() const { return area[3]; }
  Voxel W() const { return area[1]; }
  Voxel N() const { return area[0]; }
  Voxel O() const { return self; }
  void Dump() const {
    fprintf(stderr,
            "(Scenario *)(%p)->{ %s, %s, { %s, %s, %s, %s } }\n",
            this,
            toConstCharPointer(tick),
            toConstCharPointer(self),
            toConstCharPointer(area[0]),
            toConstCharPointer(area[1]),
            toConstCharPointer(area[2]),
            toConstCharPointer(area[3])
           );
  }

 private:
  TickPerCycle tick;
  Voxel self;
  array<Voxel, 4> area;
};

struct Rule {
  Rule(TickPerCycle tick, Voxel self, Voxel n, Voxel w, Voxel e, Voxel s, Voxel v) :
      scenario(tick, self, n, w, e, s),
      newVoxel(v)
  {
  }
  Rule(Scenario const &s, Voxel v) : scenario(s), newVoxel(v) { }
  void Dump() const {
    fprintf(stderr,
            "Rule(Scenario(%s, /* { */ %s, %s, %s, %s, %s /* } */), %s)",
            toConstCharPointer(scenario.tick),
            toConstCharPointer(scenario.area[0]),
            toConstCharPointer(scenario.area[1]),
            toConstCharPointer(scenario.self),
            toConstCharPointer(scenario.area[2]),
            toConstCharPointer(scenario.area[3]),
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
size_t const NRows = 21;
size_t const NCols = 21;

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
  void Phase();
  void Tick();
  void PrintViewFlat() const;
  bool isVoxelCoordinantInBounds(VoxelCoordinant v) const {
    return 0 <= v.L() && v.L() < NLvls &&
        0 <= v.R() && v.R() < NRows &&
        0 <= v.C() && v.C() < NCols;
  }
  bool isPartOf(RodType r, VoxelCoordinant v, bool &reachedEnd) const;
  bool isPartOf(RodType r, VoxelCoordinant v) const;
  Voxel const &voxelAt(VoxelCoordinant v) const {
    return (*this)[v.L()][v.R()][v.C()];
  }
  Voxel &voxelAt(VoxelCoordinant v) {
    return (*this)[v.L()][v.R()][v.C()];
  }
  int CurrentClock() const { return clock; }
  TickPerCycle CurrentTick() const {
    return TickPerCycle(CurrentClock() % NTicksPerCycle);
  }

 private:
  void ViewFlat(ViewLvlArray &view) const;
  void AddToRod(RodType rodType, Direction fwd, Direction bwd, Item *item, VoxelCoordinant v);
  Item *FormRodContaining(set<VoxelCoordinant> &seenSofar, VoxelCoordinant v);
  void FindItems();

  Rules rules;
  int clock;
  array<SetOfItems, eoRodType> itemsByRodType;
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
  virtual bool CheckForBlockages(Volume const *volume, FwdOrBwd fwdOrBwd) = 0;
  virtual bool CheckForBlockages(Volume const *volume) = 0;
  virtual bool AttemptToMove(Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes);
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

  bool CheckForBlockages(Volume const *volume, FwdOrBwd fwdOrBwd);
  bool CheckForBlockages(Volume const *volume);
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

  bool CheckForBlockages(Volume const *volume, FwdOrBwd fwdOrBwd);
  bool CheckForBlockages(Volume const *volume);
  bool IsValid(Volume const *volume);
  void Dump(Volume const *volume = 0) const;
  char const *TypeName() const { return "DataRod"; }

 private:
  LockState lockState;
  DataState dataState;
};

void VoxelCoordinant::Dump(Volume const *volume) const {
  if (volume) {
    fprintf(stderr,
            "%s(%d,%d,%d)",
            toConstCharPointer(volume->voxelAt(*this)),
            l,
            r,
            c
           );
  } else {
    fprintf(stderr,
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
  area[0] = volume->voxelAt(v.To(DirN));
  area[1] = volume->voxelAt(v.To(DirW));
  area[2] = volume->voxelAt(v.To(DirE));
  area[3] = volume->voxelAt(v.To(DirS));
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

void Volume::Phase() {
  size_t eoTick = clock + NTicksPerPhase;
  while (clock < eoTick) {
    Tick();
  }
}

void Volume::Tick() {
  size_t tick = clock % NTicksPerCycle;
  TickPerCycleProperties const &tProperties = tickPerCycleProperties[tick];
  fprintf(stderr,
	  "(Volume *)(%p)->Tick(): tick=%s, tProperties.rodType=%s, tProperties.fwdOrBwd=%s\n",
	  this,
	  toConstCharPointer(TickPerCycle(tick)),
	  toConstCharPointer(tProperties.rodType),
	  toConstCharPointer(tProperties.fwdOrBwd)
	 );

  Changes changes;
  for (Item *i : itemsByRodType[tProperties.rodType]) {
    i->AttemptToMove(this, tProperties.fwdOrBwd, changes);
  }
  for (auto const &c : changes) {
    voxelAt(c.first) = c.second;
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
    fprintf(stdout, "\n");
  }
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
    case RTKE: case RTKS: case RTKW: case RTKN:
      item = new LockRod(vProperties.rodType);
      break;
    case RTLE: case RTLS: case RTLW: case RTLN:
      item = new DataRod(vProperties.rodType);
      break;
    default:
      return 0;
  }
  AddToRod(vProperties.rodType, rProperties.fwd,rProperties.bwd, item, v);
  seenSofar.insert(item->begin(), item->end());
  if (item->IsValid(this)) {
    itemsByRodType[vProperties.rodType].insert(item);
    item->CheckForBlockages(this);
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
            item->Dump(this);
            fprintf(stderr, "\n");
            break;
          }
        }
      }
    }
  }
}

bool Item::AttemptToMove(Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes) {
  fprintf(stderr, "(%s *)(%p)->AttemptToMove(): fwdOrBwd=%s\n", TypeName(), this, toConstCharPointer(fwdOrBwd));
  bool isMovable = CheckForBlockages(volume, fwdOrBwd);
  Dump(volume);
  fprintf(stderr, "\n");

  if (isMovable) {
    TickPerCycle tick = volume->CurrentTick();
    Rules &rules = volume->GetRules();
    bool madeChanges = false;
    for (auto const &v : *this) {
      Scenario scenario(volume, tick, v);
      if (rules.find(scenario) == rules.end()) {
	TickPerCycleProperties const &tProperties = tickPerCycleProperties[tick];
	RodTypeProperties const &rProperties = rodTypeProperties[tProperties.rodType];
	Direction fwdOrBwd = tProperties.fwdOrBwd == Fwd ? rProperties.fwd : rProperties.bwd;
	Direction fwd = rProperties.fwd;
	char const *directionOfMotion[eoDirection][eoFwdOrBwd] = {
	  { ">", "<" }, // DirE, DirW
	  { "v", "^" }, // DirS, DirN
	  { "d", "u" }, // DirD, DirU
	  { "<", ">" }, // DirW, DirE
	  { "^", "v" }, // DirN, DirS
	  { "u", "d" }  // DirU, DirD
	};
	
	switch (fwd) {
	case DirE:
	case DirW:
	  fprintf(stdout, "\
     +------+------+------|\n\
%s | ???? | %s | ???? |\n\
     +------+------+------|\n\
     %s %s %s %s %s %s %s\n\
     +------+------+------|\n\
     | ???? | %s | ???? |\n\
     +------+------+------|\n\
",
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
	case DirN:
	case DirS:
	  fprintf(stdout, "\
     +------+--%s---+------|\n\
%s | ???? | %s | ???? |\n\
     +------+--%s---+------|\n\
     | %s | %s | %s |\n\
     +------+--%s---+------|\n\
     | ???? | %s | ???? |\n\
     +------+--%s---+------|\n\
",
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

	Rule newRule(scenario, newVoxel);
	newRule.Dump();
	fprintf(stderr, "\n");
      }
      Voxel newVoxel = rules[scenario];

      fprintf(stderr, "(%s *)(%p)->AttemptToMove(): At ", TypeName(), this);
      v.Dump(volume);
      fprintf(stderr, ", applying rule ");
      Rule rule(scenario, newVoxel);
      rule.Dump();
      fprintf(stderr, "\n");

      changes[v] = newVoxel;
      madeChanges = true;
    }
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
    if (vProperties.type != VTSlot) {
      rodTypeCounts[vProperties.rodType] += 1;
      foundMoreThan1RodType |=
          rodType != eoRodType &&
          rodType != vProperties.rodType;
      rodType = vProperties.rodType;
    }
  }
  if (foundMoreThan1RodType) {
    fprintf(stderr,
            "(%s *)(%p)->IsValid(volume=%p): foundMoreThan1RodType!",
            TypeName(),
	    this,
            volume
           );
    fprintf(stderr,
            "  rodTypeCounts={ [%s]%lu",
            toConstCharPointer(RodType(0)),
            rodTypeCounts[0]
           );
    for (size_t t = 1; t < eoRodType; t += 1) {
      fprintf(stderr,
              ", [%s]%lu",
              toConstCharPointer(RodType(t)),
              rodTypeCounts[t]
              );
    }
    fprintf(stderr, " }\n");
  }
  return !foundMoreThan1RodType;
}

void Item::Dump(Volume const *volume) const {
  fprintf(stderr, "(Item *)(%p)->{", this);
  fprintf(stderr, " rodType=%s, ", toConstCharPointer(rodType));
  fprintf(stderr, " fBlkState=%s, ", toConstCharPointer(fBlkState));
  fprintf(stderr, " rBlkState=%s, {", toConstCharPointer(rBlkState));
  char const *comma = "";
  for (auto const &c : *this) {
    fprintf(stderr, "%s ", comma);
    c.Dump(volume);
    comma = ",";
  }
  fprintf(stderr, " }");
  fprintf(stderr, " }");
}

bool LockRod::CheckForBlockages(Volume const *volume, FwdOrBwd fwdOrBwd) {
  return CheckForBlockages(volume) ||
      fwdOrBwd == Fwd ? !IsFBlked() : !IsRBlked();
}

bool LockRod::CheckForBlockages(Volume const *volume) {
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
          fprintf(stderr,
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
          fBlkCounts += 1;
        }
      } else {
        fprintf(stderr,
                "(LockRod *)(%p) fBlked at %s(%d,%d,%d) by edge\n",
                this,
                toConstCharPointer(volume->voxelAt(v)),
                v.L(),
                v.R(),
                v.C()
                );
        fBlkCounts += 1;
      }
    }
    if (blockStateProperties[vProperties.blockableStates].isRBlkable) {
      VoxelCoordinant b = v.To(rodTypeProperties[rodType].bwd);
      if (volume->isVoxelCoordinantInBounds(b)) {
        if (find(b) == end() && volume->voxelAt(b) != Slot) {
          fprintf(stderr,
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
          rBlkCounts += 1;
        }
      } else {
        fprintf(stderr,
                "(LockRod *)(%p) rBlked at %s(%d,%d,%d) by edge\n",
                this,
                toConstCharPointer(volume->voxelAt(v)),
                v.L(),
                v.R(),
                v.C()
                );
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
    if (vProperties.type != VTSlot) {
      lockStateCounts[vProperties.lockState] += 1;
      foundMoreThan1LockState |=
          lockState != eoLockState &&
          lockState != vProperties.lockState;
      lockState = vProperties.lockState;
    }
  }
  if (foundMoreThan1LockState) {
    fprintf(stderr,
            "(LockRod *)(%p)->IsValid(volume=%p): foundMoreThan1LockState!",
            this,
            volume
           );
    fprintf(stderr,
            "  lockStateCounts={ [%s]%lu",
            toConstCharPointer(LockState(0)),
            lockStateCounts[0]
           );
    for (size_t t = 1; t < eoLockState; t += 1) {
      fprintf(stderr,
              ", [%s]%lu",
              toConstCharPointer(LockState(t)),
              lockStateCounts[t]
              );
    }
    fprintf(stderr, " }\n");
  }
  return !foundMoreThan1LockState;
}

void LockRod::Dump(Volume const *volume) const {
  fprintf(stderr, "(LockRod *)(%p)->{ ", this);
  fprintf(stderr, " lockState=%s, ", toConstCharPointer(lockState));
  Item::Dump(volume);
  fprintf(stderr, " }");
}

bool DataRod::CheckForBlockages(Volume const *volume, FwdOrBwd fwdOrBwd) {
  return CheckForBlockages(volume) ||
      fwdOrBwd == Fwd ? !IsFBlked() : !IsRBlked();
}

bool DataRod::CheckForBlockages(Volume const *volume) {
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
    if (vProperties.dataState != DSUnkn) {
      dataStateCounts[vProperties.dataState] += 1;
      foundMoreThan1DataState |=
	dataState != DSUnkn &&
	dataState != vProperties.dataState;
      dataState = vProperties.dataState;
    }

    if (blockStateProperties[vProperties.blockableStates].isFBlkable) {
      VoxelCoordinant f = v.To(rodTypeProperties[rodType].fwd);
      if (volume->isVoxelCoordinantInBounds(f)) {
        if (find(f) == end() && volume->voxelAt(f) != Slot) {
          fprintf(stderr,
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
          fBlkCounts += 1;
        }
      } else {
        fprintf(stderr,
                "(DataRod *)(%p) fBlked at %s(%d,%d,%d) by edge\n",
                this,
                toConstCharPointer(volume->voxelAt(v)),
                v.L(),
                v.R(),
                v.C()
                );
        fBlkCounts += 1;
      }
    }
    if (blockStateProperties[vProperties.blockableStates].isRBlkable) {
      VoxelCoordinant b = v.To(rodTypeProperties[rodType].bwd);
      if (volume->isVoxelCoordinantInBounds(b)) {
        if (find(b) == end() && volume->voxelAt(b) != Slot) {
          fprintf(stderr,
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
          rBlkCounts += 1;
        }
      } else {
        fprintf(stderr,
                "(DataRod *)(%p) rBlked at %s(%d,%d,%d) by edge\n",
                this,
                toConstCharPointer(volume->voxelAt(v)),
                v.L(),
                v.R(),
                v.C()
                );
        rBlkCounts += 1;
      }
    }
    if (blockStateProperties[vProperties.blockableStates].isLockable) {
      VoxelCoordinant r = v.To(rodTypeProperties[rodType].rwd);
      if (volume->isVoxelCoordinantInBounds(r)) {
	Voxel rv = volume->voxelAt(r);
	VoxelProperties const rvProperties = voxelProperties[rv];
        if (rvProperties.type == VTLock && rvProperties.lockType == LTLock) {
          fprintf(stderr,
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
    if (vProperties.type != VTSlot) {
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
    fprintf(stderr,
            "(DataRod *)(%p)->IsValid(volume=%p): foundMoreThan1LockState!",
            this,
            volume
           );
    fprintf(stderr,
            "  lockStateCounts={ [%s]%lu",
            toConstCharPointer(LockState(0)),
            lockStateCounts[0]
           );
    for (size_t t = 1; t < eoLockState; t += 1) {
      fprintf(stderr,
              ", [%s]%lu",
              toConstCharPointer(LockState(t)),
              lockStateCounts[t]
              );
    }
    fprintf(stderr, " }\n");
  }
  if (foundMoreThan1DataState) {
    fprintf(stderr,
            "(DataRod *)(%p)->IsValid(volume=%p): foundMoreThan1DataState!",
            this,
            volume
           );
    fprintf(stderr,
            "  dataStateCounts={ [%s]%lu",
            toConstCharPointer(DataState(0)),
            dataStateCounts[0]
           );
    for (size_t t = 1; t < eoDataState; t += 1) {
      fprintf(stderr,
              ", [%s]%lu",
              toConstCharPointer(DataState(t)),
              dataStateCounts[t]
              );
    }
    fprintf(stderr, " }\n");
  }
  return !foundMoreThan1LockState && !foundMoreThan1DataState;
}

void DataRod::Dump(Volume const *volume) const {
  fprintf(stderr, "(DataRod *)(%p)->{", this);
  fprintf(stderr, " lockState=%s, ", toConstCharPointer(lockState));
  fprintf(stderr, " dataState=%s, ", toConstCharPointer(dataState));
  Item::Dump(volume);
  fprintf(stderr, " }");
}

// {
//   Volume<7, 20, 20> block;
//
//   for (int l = 0; l < block.size(); l += 1) {
//     for (int r = 0; r < block[l].size(); r += 1) {
//       for (int c = 0; c < block[l][r].size(); c += 1) {
//         ???
//       }
//     }
//   }
// }

VolArray initialVolume = {
  //           11111111112
  // 012345678901234567890
  // ....v........... ....  0
  // ....|...........^....  1
  // ....|...........|....  2
  // ....|...........|....  3
  // ....|...........|....  4
  // ....|...........|....  5
  // ....|...........|....  6
  // ....|...........|....  7
  // ....|...........|....  8
  // ....^...........|....  9
  // ....|...........|.... 10
  // ....|...........v.... 11
  // ....|...........|.... 12
  // ....|...........|.... 13
  // ....|...........|.... 14
  // ....|...........|.... 15
  // ....|...........|.... 16
  // ....|...........|.... 17
  // ....|...........|.... 18
  // ....v...........|.... 19
  // .... ...........^.... 20
  // 0 - {A,C} Lock Rods
  // 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
  Wall,Wall,Wall,Wall,LTSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall, //  0
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LHNL,Wall,Wall,Wall,Wall, //  1
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, //  2
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, //  3
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, //  4
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, //  5
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, //  6
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, //  7
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, //  8
  Wall,Wall,Wall,Wall,LPSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, //  9
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, // 10
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LPNL,Wall,Wall,Wall,Wall, // 11
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, // 12
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, // 13
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, // 14
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, // 15
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, // 16
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, // 17
  Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, // 18
  Wall,Wall,Wall,Wall,LHSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall, // 19
  Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LTNL,Wall,Wall,Wall,Wall, // 20

  //           11111111112
  // 012345678901234567890
  // .....................  0
  // .....................  1
  // .....................  2
  // .....................  3
  // .....................  4
  // .....................  5
  // .....................  6
  // .....................  7
  // .^ ^^   .............  8
  // ....^................  9
  // .... ........... .... 10
  // ................v.... 11
  // .............   vv v. 12
  // ..................... 13
  // ..................... 14
  // ..................... 15
  // ..................... 16
  // ..................... 17
  // ..................... 18
  // ..................... 19
  // ..................... 20
  // 1 -A,C} Leys/Locks
  // 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  0
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  1
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  2
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  3
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  4
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  5
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  6
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  7
  Wall,DKER,Slot,DKER,DKER,Slot,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  8
  Wall,Wall,Wall,Wall,LKSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  9
  Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall, // 10
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LKNL,Wall,Wall,Wall,Wall, // 11
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,Slot,DKWR,DKWR,Slot,DKWR,Wall, // 12
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 13
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 14
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 15
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 16
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 17
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 18
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 19
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 20

  //           11111111112
  // 012345678901234567890
  // .....................  0
  // .....................  1
  // .....................  2
  // .....................  3
  // .....................  4
  // .....................  5
  // .....................  6
  // .....................  7
  // .^-^^->--0----------O  8
  // .....................  9
  // ..................... 10
  // ..................... 11
  // ....   <-1----<-vv-v. 12
  // ..................... 13
  // ..................... 14
  // ..................... 15
  // ..................... 16
  // ..................... 17
  // ..................... 18
  // ..................... 19
  // ..................... 20
  // 2 -A,C} Data Rods
  // 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  0
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  1
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  2
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  3
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  4
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  5
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  6
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  7
  Wall,DPER,DBER,DPER,DPER,DBER,DPER,DBER,DBER,DPER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DOER, //  8
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  9
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 10
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 11
  Wall,Wall,Wall,Wall,Slot,Slot,Slot,DHWR,DBWR,DPWR,DBWR,DBWR,DBWR,DBWR,DPWR,DBWR,DPWR,DPWR,DBWR,DPWR,Wall, // 12
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 13
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 14
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 15
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 16
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 17
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 18
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 19
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 20

  //           11111111112
  // 012345678901234567890
  // .....................  0
  // .....................  1
  // .....................  2
  // .....................  3
  // .....................  4
  // .....................  5
  // ........ ...v........  6
  // ........ ... ........  7
  // ......>  0     ......  8
  // ........1... ........  9
  // ........ ... ........ 10
  // ........ ...1........ 11
  // ......   1    <...... 12
  // ........ ... ........ 13
  // ........^... ........ 14
  // ..................... 15
  // ..................... 16
  // ..................... 17
  // ..................... 18
  // ..................... 19
  // ..................... 20
  // 3 -A,B,C,D} Probes/Blocks
  // 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  0
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  1
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  2
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  3
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  4
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  5
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  6
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  7
  Wall,Wall,Wall,Wall,Wall,Wall,DQER,Slot,Slot,DGER,Slot,Slot,Slot,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall, //  8
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DGNR,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  9
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 10
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,DGSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 11
  Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,Slot,DGWR,Slot,Slot,Slot,Slot,DQWR,Wall,Wall,Wall,Wall,Wall,Wall, // 12
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 13
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQNR,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 14
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 15
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 16
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 17
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 18
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 19
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 20

  //           11111111112
  // 012345678901234567890
  // .....................  0
  // ............>........  1
  // ............|........  2
  // ............>........  3
  // ........ ...>........  4
  // ........ ...|........  5
  // ........ ...v........  6
  // ........^...|........  7
  // ........|...|........  8
  // ........1...|........  9
  // ........|...|........ 10
  // ........|...1........ 11
  // ........|...|........ 12
  // ........|...v........ 13
  // ........^... ........ 14
  // ........|... ........ 15
  // ........<... ........ 16
  // ........<............ 17
  // ........|............ 18
  // ........<............ 19
  // ..................... 20
  // 4 -B,D} Data Rods
  // 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  0
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  1
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  2
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  3
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  4
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  5
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  6
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DHNR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  7
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  8
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  9
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 10
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 11
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 12
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Wall,DHSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 13
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 14
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 15
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 16
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 17
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 18
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 19
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 20

  //           11111111112
  // 012345678901234567890
  // .....................  0
  // ............>........  1
  // ............ ........  2
  // ............>........  3
  // .......... >>........  4
  // ............ ........  5
  // ............ ........  6
  // ............ ........  7
  // .....................  8
  // .....................  9
  // ..................... 10
  // ..................... 11
  // ..................... 12
  // ........ ............ 13
  // ........ ............ 14
  // ........ ............ 15
  // ........<< .......... 16
  // ........<............ 17
  // ........ ............ 18
  // ........<............ 19
  // ..................... 20
  // 5 -B,D} Leys/Locks
  // 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  0
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DKSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  1
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  2
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DKSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  3
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,LKWL,DKSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  4
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  5
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  6
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  7
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  8
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  9
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 10
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 11
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 12
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 13
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 14
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 15
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DKNR,LKEL,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 16
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DKNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 17
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 18
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DKNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 19
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 20

  //           11111111112
  // 012345678901234567890
  // .....................  0
  // .....................  1
  // .....................  2
  // .....................  3
  //  <--------->--------<  4
  // .....................  5
  // .....................  6
  // .....................  7
  // .....................  8
  // .....................  9
  // ..................... 10
  // ..................... 11
  // ..................... 12
  // ..................... 13
  // ..................... 14
  // ..................... 15
  // >--------<--------->  16
  // ..................... 17
  // ..................... 18
  // ..................... 19
  // ..................... 20
  // 6 -B,D} Dock Rods
  // 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  0
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  1
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  2
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  3
  Slot,LHWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LPWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LTWL, //  4
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  5
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  6
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  7
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  8
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  9
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 10
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 11
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 12
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 13
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 14
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 15
  LTEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LPEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LBEL,LHEL,Slot, // 16
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 17
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 18
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 19
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall  // 20
};

int main(int argc, char const *argv[]) {
  // Rule rules[] = {
  //   { { EUSL, Slot, Wall, Wall, LKWL, Wall }, LKWU },
  //   { { EUSL, LKWL, Wall, Slot, DKSR, Wall }, Slot },
  //   { { EUSL, Slot, Wall, Wall, LHWL, Wall }, LHWU },
  //   { { EUSL, LHWL, Wall, Slot, LBWL, Wall }, LBWU },
  //   { { EUSL, LBWL, Wall, LHWL, LBWL, Wall }, LBWU },
  //   { { EUSL, LBWL, Wall, LBWL, LBWL, Wall }, LBWU },
  //   { { EUSL, LBWL, Wall, LBWL, LPWL, Wall }, LPWU },
  //   { { EUSL, LPWL, Wall, LBWL, LBWL, Wall }, LBWU },
  //   { { EUSL, LBWL, Wall, LPWL, LBWL, Wall }, LBWU },
  //   { { EUSL, LBWL, Wall, LBWL, LTWL, Wall }, LTWU },
  //   { { EUSL, LTWL, Wall, LBWL, Wall, Wall }, Slot },
  // };

  for (size_t i = 0; i < eoVoxel; i += 1) {
    Voxel v = Voxel(i);

    stringToVoxel[toConstCharPointer(v)] = v;
  }

  Volume volume(initialVolume);
  // for (size_t r = 0; r < (sizeof(rules) / sizeof(rules[0])); r += 1) {
  //   volume.AddRule(rules[r]);
  // }

  // volume.AddRule(Scenario(EUSL, Wall, Wall, Slot, LKWL, Wall), LKWU);
  // volume.AddRule(Scenario(EUSL, Wall, Slot, LKWL, DKSR, Wall), Slot);
  // volume.AddRule(Scenario(EUSL, Wall, Wall, Slot, LHWL, Wall), LHWU);
  // volume.AddRule(Scenario(EUSL, Wall, Slot, LHWL, LBWL, Wall), LBWU);
  // volume.AddRule(Scenario(EUSL, Wall, LHWL, LBWL, LBWL, Wall), LBWU);
  // volume.AddRule(Scenario(EUSL, Wall, LBWL, LBWL, LBWL, Wall), LBWU);
  // volume.AddRule(Scenario(EUSL, Wall, LBWL, LBWL, LPWL, Wall), LPWU);
  // volume.AddRule(Scenario(EUSL, Wall, LBWL, LPWL, LBWL, Wall), LBWU);
  // volume.AddRule(Scenario(EUSL, Wall, LPWL, LBWL, LBWL, Wall), LBWU);
  // volume.AddRule(Scenario(EUSL, Wall, LBWL, LBWL, LTWL, Wall), LTWU);
  // volume.AddRule(Scenario(EUSL, Wall, LBWL, LTWL, Wall, Wall), Slot);
  // 					          
  // volume.AddRule(Scenario(ERS1, Wall, Wall, DPSR, Wall, DBSR), DPSR);
  // volume.AddRule(Scenario(ERS1, DPSR, Wall, DBSR, Wall, DPSR), DBSR);
  // volume.AddRule(Scenario(ERS1, DBSR, Wall, DPSR, Wall, DPSR), DPSR);
  // volume.AddRule(Scenario(ERS1, DPSR, Wall, DPSR, Wall, DBSR), DPSR);
  // volume.AddRule(Scenario(ERS1, DPSR, Wall, DBSR, Wall, DBSR), DBSR);
  // volume.AddRule(Scenario(ERS1, DBSR, Wall, DBSR, Wall, DPSR), DBSR);
  // volume.AddRule(Scenario(ERS1, DBSR, Wall, DPSR, Wall, DBSR), DPSR);
  // volume.AddRule(Scenario(ERS1, DBSR, Wall, DBSR, Wall, DBSR), DBSR);
  // volume.AddRule(Scenario(ERS1, DPSR, Wall, DBSR, Wall, DHSR), DBSR);
  // volume.AddRule(Scenario(ERS1, DBSR, Wall, DHSR, Wall, Slot), DHSR);
  // volume.AddRule(Scenario(ERS1, DHSR, Wall, Slot, Wall, Slot), Slot);
  // volume.AddRule(Scenario(ERS1, Slot, Wall, Slot, Wall, Slot), Slot);
  // volume.AddRule(Scenario(ERS1, Slot, Wall, Slot, Wall, Wall), Slot);
  // volume.AddRule(Scenario(ERS1, Wall, Wall, DKSR, Wall, Slot), DKSR);
  // volume.AddRule(Scenario(ERS1, DKSR, Wall, Slot, Wall, DKSR), Slot);
  // volume.AddRule(Scenario(ERS1, Slot, Wall, DKSR, Wall, DKSR), DKSR);
  // volume.AddRule(Scenario(ERS1, DKSR, Slot, DKSR, Wall, Slot), DKSR);
  // volume.AddRule(Scenario(ERS1, DKSR, Wall, Slot, Wall, Slot), Slot);
  // 					          
  // volume.AddRule(Scenario(ERSX, Wall, Wall, DPSR, Wall, DBSR), DPSR);
  // volume.AddRule(Scenario(ERSX, DPSR, Wall, DBSR, Wall, DPSR), DBSR);
  // volume.AddRule(Scenario(ERSX, DBSR, Wall, DPSR, Wall, DPSR), DPSR);
  // volume.AddRule(Scenario(ERSX, DPSR, Wall, DPSR, Wall, DBSR), DPSR);
  // volume.AddRule(Scenario(ERSX, DPSR, Wall, DBSR, Wall, DBSR), DBSR);
  // volume.AddRule(Scenario(ERSX, DBSR, Wall, DBSR, Wall, DPSR), DBSR);
  // volume.AddRule(Scenario(ERSX, DBSR, Wall, DPSR, Wall, DBSR), DPSR);
  // volume.AddRule(Scenario(ERSX, DBSR, Wall, DBSR, Wall, DBSR), DBSR);
  // volume.AddRule(Scenario(ERSX, DPSR, Wall, DBSR, Wall, DHSR), DBSR);
  // volume.AddRule(Scenario(ERSX, DBSR, Wall, DHSR, Wall, Slot), DHSR);
  // volume.AddRule(Scenario(ERSX, DHSR, Wall, Slot, Wall, Slot), Slot);
  // volume.AddRule(Scenario(ERSX, Slot, Wall, Slot, Wall, Slot), Slot);
  // volume.AddRule(Scenario(ERSX, Slot, Wall, Slot, Wall, Wall), Slot);
  // volume.AddRule(Scenario(ERSX, Wall, Wall, DKSR, Wall, Slot), DKSR);
  // volume.AddRule(Scenario(ERSX, DKSR, Wall, Slot, Wall, DKSR), Slot);
  // volume.AddRule(Scenario(ERSX, Slot, Wall, DKSR, Wall, DKSR), DKSR);
  // volume.AddRule(Scenario(ERSX, DKSR, Slot, DKSR, Wall, Slot), DKSR);
  // volume.AddRule(Scenario(ERSX, DKSR, Wall, Slot, Wall, Slot), Slot);
  // 					          
  // volume.AddRule(Scenario(ERS0, Wall, Wall, DPSR, Wall, DBSR), DPSR);
  // volume.AddRule(Scenario(ERS0, DPSR, Wall, DBSR, Wall, DPSR), DBSR);
  // volume.AddRule(Scenario(ERS0, DBSR, Wall, DPSR, Wall, DPSR), DPSR);
  // volume.AddRule(Scenario(ERS0, DPSR, Wall, DPSR, Wall, DBSR), DPSR);
  // volume.AddRule(Scenario(ERS0, DPSR, Wall, DBSR, Wall, DBSR), DBSR);
  // volume.AddRule(Scenario(ERS0, DBSR, Wall, DBSR, Wall, DPSR), DBSR);
  // volume.AddRule(Scenario(ERS0, DBSR, Wall, DPSR, Wall, DBSR), DPSR);
  // volume.AddRule(Scenario(ERS0, DBSR, Wall, DBSR, Wall, DBSR), DBSR);
  // volume.AddRule(Scenario(ERS0, DPSR, Wall, DBSR, Wall, DHSR), DBSR);
  // volume.AddRule(Scenario(ERS0, DBSR, Wall, DHSR, Wall, Slot), DHSR);
  // volume.AddRule(Scenario(ERS0, DHSR, Wall, Slot, Wall, Slot), Slot);
  // volume.AddRule(Scenario(ERS0, Slot, Wall, Slot, Wall, Slot), Slot);
  // volume.AddRule(Scenario(ERS0, Slot, Wall, Slot, Wall, Wall), Slot);
  // volume.AddRule(Scenario(ERS0, Wall, Wall, DKSR, Wall, Slot), DKSR);
  // volume.AddRule(Scenario(ERS0, DKSR, Wall, Slot, Wall, DKSR), Slot);
  // volume.AddRule(Scenario(ERS0, Slot, Wall, DKSR, Wall, DKSR), DKSR);
  // volume.AddRule(Scenario(ERS0, DKSR, Slot, DKSR, Wall, Slot), DKSR);
  // volume.AddRule(Scenario(ERS0, DKSR, Wall, Slot, Wall, Slot), Slot);
  // 					          
  // volume.AddRule(Scenario(ELSL, Wall, Wall, LKWU, Slot, Wall), Slot);
  // volume.AddRule(Scenario(ELSL, Wall, LKWU, Slot, DKSR, Wall), LKWL);
  // volume.AddRule(Scenario(ELSL, Wall, Wall, LHWU, LBWU, Wall), Slot);
  // volume.AddRule(Scenario(ELSL, Wall, LHWU, LBWU, LBWU, Wall), LHWL);
  // volume.AddRule(Scenario(ELSL, Wall, LBWU, LBWU, LBWU, Wall), LBWL);
  // volume.AddRule(Scenario(ELSL, Wall, LBWU, LBWU, LPWU, Wall), LBWL);
  // volume.AddRule(Scenario(ELSL, Wall, LBWU, LPWU, LBWU, Wall), LBWL);
  // volume.AddRule(Scenario(ELSL, Wall, LPWU, LBWU, LBWU, Wall), LPWL);
  // volume.AddRule(Scenario(ELSL, Wall, LBWU, LBWU, LTWU, Wall), LBWL);
  // volume.AddRule(Scenario(ELSL, Wall, LBWU, LTWU, Slot, Wall), LBWL);
  // volume.AddRule(Scenario(ELSL, Wall, LTWU, Slot, Wall, Wall), LTWL);
  // 					          
  // volume.AddRule(Scenario(EUEL, Wall, Wall, LTSL, Wall, LBSL), Slot);
  // volume.AddRule(Scenario(EUEL, LTSL, Wall, LBSL, Wall, LBSL), LTSU);
  // volume.AddRule(Scenario(EUEL, LBSL, Wall, LBSL, Wall, LBSL), LBSU);
  // volume.AddRule(Scenario(EUEL, LBSL, Wall, LBSL, Wall, LPSL), LBSU);
  // volume.AddRule(Scenario(EUEL, LBSL, Wall, LPSL, Wall, LBSL), LBSU);
  // volume.AddRule(Scenario(EUEL, LPSL, Wall, LBSL, Wall, LBSL), LPSU);
  // volume.AddRule(Scenario(EUEL, LBSL, Wall, LBSL, Wall, LHSL), LBSU);
  // volume.AddRule(Scenario(EUEL, LBSL, Wall, LHSL, Wall, Slot), LBSU);
  // volume.AddRule(Scenario(EUEL, LHSL, Wall, Slot, Wall, Wall), LHSU);
  // volume.AddRule(Scenario(EUEL, DKER, Wall, LKSL, Wall, Slot), Slot);
  // volume.AddRule(Scenario(EUEL, LKSL, Wall, Slot, Wall, Wall), LKSU);
  // 					          
  // volume.AddRule(Scenario(ESE0, Wall, Wall, DKER, Slot, Wall), Slot);
  // volume.AddRule(Scenario(ESE0, Wall, DKER, Slot, DKER, Wall), DKE0);
  // volume.AddRule(Scenario(ESE0, Wall, Slot, DKER, DKER, Wall), Slot);
  // volume.AddRule(Scenario(ESE0, Wall, DKER, DKER, Slot, Slot), DKE0);
  // volume.AddRule(Scenario(ESE0, Wall, DKER, Slot, Slot, Wall), DKE0);
  // volume.AddRule(Scenario(ESE0, Wall, Slot, Slot, Slot, Wall), Slot);
  // volume.AddRule(Scenario(ESE0, Wall, Slot, Slot, Wall, Wall), Slot);
  // volume.AddRule(Scenario(ESE0, Wall, Wall, DPER, DBER, Wall), Slot);
  // volume.AddRule(Scenario(ESE0, Wall, DPER, DBER, DPER, Wall), DPE0);
  // volume.AddRule(Scenario(ESE0, Wall, DBER, DPER, DPER, Wall), DBE0);
  // volume.AddRule(Scenario(ESE0, Wall, DPER, DPER, DBER, Wall), DPE0);
  // volume.AddRule(Scenario(ESE0, Wall, DPER, DBER, DBER, Wall), DPE0);
  // volume.AddRule(Scenario(ESE0, Wall, DBER, DBER, DPER, Wall), DBE0);
  // volume.AddRule(Scenario(ESE0, Wall, DBER, DPER, DBER, Wall), DBE0);
  // volume.AddRule(Scenario(ESE0, Wall, DBER, DBER, DBER, Wall), DBE0);
  // volume.AddRule(Scenario(ESE0, Wall, DBER, DBER, DOER, Wall), DBE0);
  // volume.AddRule(Scenario(ESE0, Wall, DBER, DOER, Wall, Wall), DOE0);
  // volume.AddRule(Scenario(ESE0, Wall, Wall, DQER, Slot, Wall), Slot);
  // volume.AddRule(Scenario(ESE0, Wall, DQER, Slot, DGER, DGNR), DQE0);
  // volume.AddRule(Scenario(ESE0, Wall, Slot, DGER, Slot, Wall), Slot);
  // volume.AddRule(Scenario(ESE0, DQSR, DGER, Slot, Slot, Slot), DGE0);
  // 					          
  // volume.AddRule(Scenario(ESE0, Slot, DQER, Slot, DGER, DGNR), DQE0);
  // volume.AddRule(Scenario(ESE0, Wall, DGER, Slot, Slot, Wall), DGE0);
  // volume.AddRule(Scenario(ESE0, DQSR, Slot, Slot, Slot, Slot), Slot);
  // 					          
  // volume.AddRule(Scenario(ESEX, Wall, Wall, Slot, DKE0, Wall), Slot);
  // volume.AddRule(Scenario(ESEX, Wall, Slot, DKE0, Slot, Wall), Slot);
  // volume.AddRule(Scenario(ESEX, Wall, DKE0, Slot, DKE0, Wall), DKEX);
  // volume.AddRule(Scenario(ESEX, Wall, Slot, DKE0, DKE0, Slot), Slot);
  // volume.AddRule(Scenario(ESEX, Wall, DKE0, DKE0, Slot, Wall), DKEX);
  // volume.AddRule(Scenario(ESEX, Wall, DKE0, Slot, Slot, Wall), DKEX);
  // volume.AddRule(Scenario(ESEX, Wall, Slot, Slot, Wall, Wall), Slot);
  // volume.AddRule(Scenario(ESEX, Wall, Wall, Slot, DPE0, Wall), Slot);
  // volume.AddRule(Scenario(ESEX, Wall, Slot, DPE0, DBE0, Wall), Slot);
  // volume.AddRule(Scenario(ESEX, Wall, DPE0, DBE0, DPE0, Wall), DPEX);
  // volume.AddRule(Scenario(ESEX, Wall, DBE0, DPE0, DPE0, Wall), DBEX);
  // volume.AddRule(Scenario(ESEX, Wall, DPE0, DPE0, DBE0, Wall), DPEX);
  // volume.AddRule(Scenario(ESEX, Wall, DPE0, DBE0, DBE0, Wall), DPEX);
  // volume.AddRule(Scenario(ESEX, Wall, DBE0, DBE0, DPE0, Wall), DBEX);
  // volume.AddRule(Scenario(ESEX, Wall, DBE0, DPE0, DBE0, Wall), DBEX);
  // volume.AddRule(Scenario(ESEX, Wall, DBE0, DBE0, DBE0, Wall), DBEX);
  // volume.AddRule(Scenario(ESEX, Wall, DBE0, DBE0, DOE0, Wall), DBEX);
  // volume.AddRule(Scenario(ESEX, Wall, DBE0, DOE0, Wall, Wall), DOEX);
  // volume.AddRule(Scenario(ESEX, Wall, Wall, Slot, DQE0, Wall), Slot);
  // volume.AddRule(Scenario(ESEX, Wall, Slot, DQE0, Slot, DGNR), Slot);
  // volume.AddRule(Scenario(ESEX, Wall, DQE0, Slot, DGE0, Wall), DQEX);
  // volume.AddRule(Scenario(ESEX, DQSR, Slot, DGE0, Slot, Slot), Slot);
  // volume.AddRule(Scenario(ESEX, Wall, DGE0, Slot, Slot, Wall), DGEX);
  // volume.AddRule(Scenario(ESEX, Wall, Slot, Slot, Slot, Wall), Slot);
  // 					          
  // volume.AddRule(Scenario(ESEX, Slot, Slot, DQE0, Slot, DGNR), Slot);
  // volume.AddRule(Scenario(ESEX, Wall, Slot, DGE0, Slot, Wall), Slot);
  // volume.AddRule(Scenario(ESEX, DQSR, Slot, Slot, Slot, Slot), Slot);
  // 					          
  // volume.AddRule(Scenario(ESE1, Wall, Wall, Slot, Slot, Wall), Slot);
  // volume.AddRule(Scenario(ESE1, Wall, Slot, Slot, DKEX, Wall), Slot);
  // volume.AddRule(Scenario(ESE1, Wall, Slot, DKEX, Slot, Wall), Slot);
  // volume.AddRule(Scenario(ESE1, Wall, DKEX, Slot, DKEX, Slot), DKE1);
  // volume.AddRule(Scenario(ESE1, Wall, Slot, DKEX, DKEX, Wall), Slot);
  // volume.AddRule(Scenario(ESE1, Wall, DKEX, DKEX, Slot, Wall), DKE1);
  // volume.AddRule(Scenario(ESE1, Wall, DKEX, Slot, Wall, Wall), DKE1);
  // volume.AddRule(Scenario(ESE1, Wall, Slot, Slot, DPEX, Wall), Slot);
  // volume.AddRule(Scenario(ESE1, Wall, Slot, DPEX, DBEX, Wall), Slot);
  // volume.AddRule(Scenario(ESE1, Wall, DPEX, DBEX, DPEX, Wall), DPE1);
  // volume.AddRule(Scenario(ESE1, Wall, DBEX, DPEX, DPEX, Wall), DBE1);
  // volume.AddRule(Scenario(ESE1, Wall, DPEX, DPEX, DBEX, Wall), DPE1);
  // volume.AddRule(Scenario(ESE1, Wall, DPEX, DBEX, DBEX, Wall), DPE1);
  // volume.AddRule(Scenario(ESE1, Wall, DBEX, DBEX, DPEX, Wall), DBE1);
  // volume.AddRule(Scenario(ESE1, Wall, DBEX, DPEX, DBEX, Wall), DBE1);
  // volume.AddRule(Scenario(ESE1, Wall, DBEX, DBEX, DBEX, Wall), DBE1);
  // volume.AddRule(Scenario(ESE1, Wall, DBEX, DBEX, DOEX, Wall), DBE1);
  // volume.AddRule(Scenario(ESE1, Wall, DBEX, DOEX, Wall, Wall), DOE1);
  // volume.AddRule(Scenario(ESE1, Wall, Slot, Slot, DQEX, DGNR), Slot);
  // volume.AddRule(Scenario(ESE1, Wall, Slot, DQEX, Slot, Wall), Slot);
  // volume.AddRule(Scenario(ESE1, DQSR, DQEX, Slot, DGEX, Slot), DQE1);
  // volume.AddRule(Scenario(ESE1, Wall, Slot, DGEX, Slot, Wall), Slot);
  // volume.AddRule(Scenario(ESE1, Wall, DGEX, Slot, Slot, Wall), DGE1);
  // volume.AddRule(Scenario(ESE1, Wall, Slot, Slot, Slot, Wall), Slot);
  // volume.AddRule(Scenario(ESE1, Wall, Slot, Slot, Wall, Wall), Slot);
  // 					          
  // volume.AddRule(Scenario(ESE1, Slot, Slot, Slot, DQEX, DGNR), Slot);
  // volume.AddRule(Scenario(ESE1, Wall, DQEX, Slot, DGEX, Wall), DQE1);
  // volume.AddRule(Scenario(ESE1, DQSR, DGEX, Slot, Slot, Slot), DGE1);
  // 					          
  // volume.AddRule(Scenario(ELEL, Wall, Wall, Slot, Wall, LTSU), LTSL);
  // volume.AddRule(Scenario(ELEL, Slot, Wall, LTSU, Wall, LBSU), LBSL);
  // volume.AddRule(Scenario(ELEL, LTSU, Wall, LBSU, Wall, LBSU), LBSL);
  // volume.AddRule(Scenario(ELEL, LBSU, Wall, LBSU, Wall, LBSU), LBSL);
  // volume.AddRule(Scenario(ELEL, LBSU, Wall, LBSU, Wall, LPSU), LPSL);
  // volume.AddRule(Scenario(ELEL, LBSU, Wall, LPSU, Wall, LBSU), LBSL);
  // volume.AddRule(Scenario(ELEL, LPSU, Wall, LBSU, Wall, LBSU), LBSL);
  // volume.AddRule(Scenario(ELEL, LBSU, Wall, LBSU, Wall, LHSU), LHSL);
  // volume.AddRule(Scenario(ELEL, LBSU, Wall, LHSU, Wall, Wall), Slot);
  // volume.AddRule(Scenario(ELEL, DKE1, Wall, Slot, Wall, LKSU), LKSL);
  // volume.AddRule(Scenario(ELEL, Slot, Wall, LKSU, Wall, Wall), Slot);
  // 					          
  // volume.AddRule(Scenario(SUWL, Slot, Wall, Slot, Wall, LHNL), LHNU);
  // volume.AddRule(Scenario(SUWL, Slot, Wall, LHNL, Wall, LBNL), LBNU);
  // volume.AddRule(Scenario(SUWL, LHNL, Wall, LBNL, Wall, LBNL), LBNU);
  // volume.AddRule(Scenario(SUWL, LBNL, Wall, LBNL, Wall, LBNL), LBNU);
  // volume.AddRule(Scenario(SUWL, LBNL, Wall, LBNL, Wall, LPNL), LPNU);
  // volume.AddRule(Scenario(SUWL, LBNL, Wall, LPNL, Wall, LBNL), LBNU);
  // volume.AddRule(Scenario(SUWL, LPNL, Wall, LBNL, Wall, LBNL), LBNU);
  // volume.AddRule(Scenario(SUWL, LBNL, Wall, LBNL, Wall, LTNL), LTNU);
  // volume.AddRule(Scenario(SUWL, LBNL, Wall, LTNL, Wall, Wall), Slot);
  // volume.AddRule(Scenario(SUWL, Wall, Wall, Slot, Wall, LKNL), LKNU);
  // volume.AddRule(Scenario(SUWL, Slot, Wall, LKNL, Wall, DKWR), Slot);
  // 					          
  // volume.AddRule(Scenario(SLWL, Slot, Wall, LHNU, Wall, LBNU), Slot);
  // volume.AddRule(Scenario(SLWL, LHNU, Wall, LBNU, Wall, LBNU), LHNL);
  // volume.AddRule(Scenario(SLWL, LBNU, Wall, LBNU, Wall, LBNU), LBNL);
  // volume.AddRule(Scenario(SLWL, LBNU, Wall, LBNU, Wall, LPNU), LBNL);
  // volume.AddRule(Scenario(SLWL, LBNU, Wall, LPNU, Wall, LBNU), LBNL);
  // volume.AddRule(Scenario(SLWL, LPNU, Wall, LBNU, Wall, LBNU), LPNL);
  // volume.AddRule(Scenario(SLWL, LBNU, Wall, LBNU, Wall, LTNU), LBNU);
  // volume.AddRule(Scenario(SLWL, LBNU, Wall, LTNU, Wall, Slot), LBNL);
  // volume.AddRule(Scenario(SLWL, LTNU, Wall, Slot, Wall, Wall), LTNL);
  // volume.AddRule(Scenario(SLWL, Wall, Wall, LKNU, Wall, Slot), Slot);
  // volume.AddRule(Scenario(SLWL, LKNU, Wall, Slot, Wall, DKWR), LKNL);
  // 					          
  // volume.AddRule(Scenario(SUSL, Wall, Wall, Slot, LKWL, Wall), LKWU);
  // volume.AddRule(Scenario(SUSL, Wall, Slot, LKWL, DKSR, Wall), Slot);
  // volume.AddRule(Scenario(SUSL, Wall, Wall, Slot, LHWL, Wall), LHWU);
  // volume.AddRule(Scenario(SUSL, Wall, Slot, LHWL, LBWL, Wall), LBWU);
  // volume.AddRule(Scenario(SUSL, Wall, LHWL, LBWL, LBWL, Wall), LBWU);
  // volume.AddRule(Scenario(SUSL, Wall, LBWL, LBWL, LBWL, Wall), LBWU);
  // volume.AddRule(Scenario(SUSL, Wall, LBWL, LBWL, LPWL, Wall), LPWU);
  // volume.AddRule(Scenario(SUSL, Wall, LBWL, LPWL, LBWL, Wall), LBWU);
  // volume.AddRule(Scenario(SUSL, Wall, LPWL, LBWL, LBWL, Wall), LBWU);
  // volume.AddRule(Scenario(SUSL, Wall, LBWL, LBWL, LTWL, Wall), LTWU);
  // volume.AddRule(Scenario(SUSL, Wall, LBWL, LTWL, Wall, Wall), Slot);
  // 
  // volume.AddRule(Scenario(ESE0, /* { */ Wall, DQER, Slot, Slot, Wall /* } */), DQE0);
  // volume.AddRule(Scenario(ESE0, /* { */ Slot, Slot, Slot, DGER, DGNR /* } */), Slot);
  // volume.AddRule(Scenario(ESE0, /* { */ Slot, Slot, Slot, Slot, Slot /* } */), Slot);
  // 
  // volume.AddRule(Scenario(ESEX, /* { */ Wall, Slot, DQE0, Slot, Wall /* } */), Slot);
  // volume.AddRule(Scenario(ESEX, /* { */ Slot, DQE0, Slot, Slot, DGNR /* } */), DQEX);
  // volume.AddRule(Scenario(ESEX, /* { */ Wall, Slot, Slot, DGE0, Wall /* } */), Slot);
  // volume.AddRule(Scenario(ESEX, /* { */ Slot, Slot, Slot, Slot, Slot /* } */), Slot);
  // 
  // volume.AddRule(Scenario(ESE1, /* { */ Wall, Slot, Slot, DQEX, Wall /* } */), Slot);
  // volume.AddRule(Scenario(ESE1, /* { */ Slot, Slot, DQEX, Slot, DGNR /* } */), Slot);
  // volume.AddRule(Scenario(ESE1, /* { */ Wall, DQEX, Slot, Slot, Wall /* } */), DQE1);
  // volume.AddRule(Scenario(ESE1, /* { */ Wall, Slot, Slot, DGEX, Wall /* } */), Slot);
  // volume.AddRule(Scenario(ESE1, /* { */ Slot, DGEX, Slot, Slot, Slot /* } */), DGE1);
  // 
  // volume.AddRule(Scenario(SSS0, /* { */ Wall, Wall, DQSR, Wall, Slot /* } */), Slot);
  // volume.AddRule(Scenario(SSS0, /* { */ DQSR, Wall, Slot, Wall, DGE1 /* } */), DQS0);
  // volume.AddRule(Scenario(SSS0, /* { */ Slot, Slot, DGE1, Slot, Slot /* } */), DGE1);
  // volume.AddRule(Scenario(SSS0, /* { */ DGE1, Wall, Slot, Wall, Slot /* } */), Slot);
  // volume.AddRule(Scenario(SSS0, /* { */ Slot, Wall, Slot, Wall, DGSR /* } */), Slot);
  // volume.AddRule(Scenario(SSS0, /* { */ Slot, Wall, DGSR, Wall, Slot /* } */), Slot);
  // volume.AddRule(Scenario(SSS0, /* { */ DGSR, Slot, Slot, Slot, Slot /* } */), DGS0);
  // volume.AddRule(Scenario(SSS0, /* { */ Slot, Wall, Slot, Wall, Slot /* } */), Slot);
  // volume.AddRule(Scenario(SSS0, /* { */ Slot, Wall, Slot, Wall, Wall /* } */), Slot);
  // volume.AddRule(Scenario(SSS0, /* { */ Wall, Wall, DPSR, Wall, DBSR /* } */), Slot);
  // volume.AddRule(Scenario(SSS0, /* { */ DPSR, Wall, DBSR, Wall, DPSR /* } */), DPS0);
  // volume.AddRule(Scenario(SSS0, /* { */ DBSR, Wall, DPSR, Wall, DPSR /* } */), DBS0);
  // volume.AddRule(Scenario(SSS0, /* { */ DPSR, Wall, DPSR, Wall, DBSR /* } */), DPS0);
  // volume.AddRule(Scenario(SSS0, /* { */ DBSR, Wall, DPSR, Wall, DBSR /* } */), DBS0);
  // volume.AddRule(Scenario(SSS0, /* { */ DPSR, Wall, DBSR, Wall, DBSR /* } */), DPS0);
  // volume.AddRule(Scenario(SSS0, /* { */ DBSR, Wall, DBSR, Wall, DBSR /* } */), DBS0);
  // volume.AddRule(Scenario(SSS0, /* { */ DBSR, Wall, DBSR, Wall, DPSR /* } */), DBS0);
  // volume.AddRule(Scenario(SSS0, /* { */ DPSR, Wall, DBSR, Wall, DHSR /* } */), DPS0);
  // volume.AddRule(Scenario(SSS0, /* { */ DBSR, Wall, DHSR, Wall, Slot /* } */), DBS0);
  // volume.AddRule(Scenario(SSS0, /* { */ DHSR, Wall, Slot, Wall, Slot /* } */), DHS0);
  // volume.AddRule(Scenario(SSS0, /* { */ Wall, Wall, DKSR, Wall, Slot /* } */), DKS0);
  // volume.AddRule(Scenario(SSS0, /* { */ DKSR, Wall, Slot, Wall, DKSR /* } */), DKS0);
  // volume.AddRule(Scenario(SSS0, /* { */ Slot, Wall, DKSR, Wall, DKSR /* } */), Slot);
  // volume.AddRule(Scenario(SSS0, /* { */ DKSR, Slot, DKSR, Wall, Slot /* } */), DKS0);
  // volume.AddRule(Scenario(SSS0, /* { */ DKSR, Wall, Slot, Wall, Slot /* } */), DKS0);
  // 
  // volume.AddRule(Scenario(SSSX, /* { */ Wall, Wall, Slot, Wall, DQS0 /* } */), Slot);
  // volume.AddRule(Scenario(SSSX, /* { */ Slot, Wall, DQS0, Wall, DGE1 /* } */), Slot);
  // volume.AddRule(Scenario(SSSX, /* { */ DQS0, Slot, DGE1, Slot, Slot /* } */), DQSX);
  // volume.AddRule(Scenario(SSSX, /* { */ DGE1, Wall, Slot, Wall, Slot /* } */), Slot);
  // volume.AddRule(Scenario(SSSX, /* { */ Slot, Wall, Slot, Wall, Slot /* } */), Slot);
  // volume.AddRule(Scenario(SSSX, /* { */ Slot, Wall, Slot, Wall, DGS0 /* } */), Slot);
  // volume.AddRule(Scenario(SSSX, /* { */ Slot, Slot, DGS0, Slot, Slot /* } */), Slot);
  // volume.AddRule(Scenario(SSSX, /* { */ DGS0, Wall, Slot, Wall, Slot /* } */), DGSX);
  // volume.AddRule(Scenario(SSSX, /* { */ Slot, Wall, Slot, Wall, Wall /* } */), Slot);
  // volume.AddRule(Scenario(SSSX, /* { */ Wall, Wall, Slot, Wall, DPS0 /* } */), Slot);
  // volume.AddRule(Scenario(SSSX, /* { */ Slot, Wall, DPS0, Wall, DBS0 /* } */), Slot);
  // volume.AddRule(Scenario(SSSX, /* { */ DPS0, Wall, DBS0, Wall, DPS0 /* } */), DPSX);
  // volume.AddRule(Scenario(SSSX, /* { */ DBS0, Wall, DPS0, Wall, DPS0 /* } */), DBSX);
  // volume.AddRule(Scenario(SSSX, /* { */ DPS0, Wall, DPS0, Wall, DBS0 /* } */), DPSX);
  // volume.AddRule(Scenario(SSSX, /* { */ DBS0, Wall, DPS0, Wall, DBS0 /* } */), DBSX);
  // volume.AddRule(Scenario(SSSX, /* { */ DPS0, Wall, DBS0, Wall, DBS0 /* } */), DPSX);
  // volume.AddRule(Scenario(SSSX, /* { */ DBS0, Wall, DBS0, Wall, DBS0 /* } */), DBSX);
  // volume.AddRule(Scenario(SSSX, /* { */ DBS0, Wall, DBS0, Wall, DPS0 /* } */), DBSX);
  // volume.AddRule(Scenario(SSSX, /* { */ DPS0, Wall, DBS0, Wall, DHS0 /* } */), DPSX);
  // volume.AddRule(Scenario(SSSX, /* { */ DBS0, Wall, DHS0, Wall, Slot /* } */), DBSX);
  // volume.AddRule(Scenario(SSSX, /* { */ DHS0, Wall, Slot, Wall, Slot /* } */), DHSX);
  // volume.AddRule(Scenario(SSSX, /* { */ Wall, Wall, DKS0, Wall, DKS0 /* } */), DKSX);
  // volume.AddRule(Scenario(SSSX, /* { */ DKS0, Wall, DKS0, Wall, Slot /* } */), DKSX);
  // volume.AddRule(Scenario(SSSX, /* { */ DKS0, Wall, Slot, Wall, DKS0 /* } */), DKSX);
  // volume.AddRule(Scenario(SSSX, /* { */ Slot, Slot, DKS0, Wall, DKS0 /* } */), Slot);
  // volume.AddRule(Scenario(SSSX, /* { */ DKS0, Wall, Slot, Wall, Slot /* } */), DKSX);

  volume.PrintViewFlat();
  for (size_t t = 0; t < NTicksPerCycle; t += 1) {
    volume.Tick();
    volume.PrintViewFlat();
    for (auto const &r : volume.GetRules()) {
      if (r.second == Unkn) {
	fprintf(stderr, "  volume.Add");
	Rule rule(r.first, r.second);
	rule.Dump();
	fprintf(stderr, ";\n");
      }
    }
  }
  return 0;
}
