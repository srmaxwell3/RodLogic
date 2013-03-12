// ┃┇━┅▲△▶▷▼▽◀◁⬛
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

bool optVerbose = true;

// Directions of motion, in order (clockwise).

enum Direction {
  DirE, DirS, DirD, DirW, DirN, DirU,

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

enum PhasePerCycle {
  E, S, W, N,

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
  ResetNextDataRodsFrom0,
  RelockNextDataRods,

  UnlockDataRods,
  SetDataRodsTo0,
  SetDataRodsTo1,
  RelockDataRods,

  eoTickPerPhase
};

char const *toConstCharPointer(TickPerPhase t) {
  switch (t) {
    case UnlockNextDataRods: return "UnlockNextDataRods";
    case ResetNextDataRodsFrom1: return "ResetNextDataRodsFrom1";
    case ResetNextDataRodsFrom0: return "ResetNextDataRodsFrom0";
    case RelockNextDataRods: return "RelockNextDataRods";
    case UnlockDataRods: return "UnlockDataRods";
    case SetDataRodsTo0: return "SetDataRodsTo0";
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
  EUSL, ERS1, ERS0, ELSL, EUEL, ESE0, ESE1, ELEL,
  SUWL, SRW1, SRW0, SLWL, SUSL, SSS0, SSS1, SLSL,
  WUNL, WRN1, WRN0, WLNL, WUWL, WSW0, WSW1, WLWL,
  NUEL, NRE1, NRE0, NLEL, NUNL, NSN0, NSN1, NLNL,

  eoTickPerCycle
};

char const *toConstCharPointer(TickPerCycle t) {
  switch (t) {
    case EUSL: return "EUSL";
    case ERS1: return "ERS1";
    case ERS0: return "ERS0";
    case ELSL: return "ELSL";
    case EUEL: return "EUEL";
    case ESE0: return "ESE0";
    case ESE1: return "ESE1";
    case ELEL: return "ELEL";
    case SUWL: return "SUWL";
    case SRW1: return "SRW1";
    case SRW0: return "SRW0";
    case SLWL: return "SLWL";
    case SUSL: return "SUSL";
    case SSS0: return "SSS0";
    case SSS1: return "SSS1";
    case SLSL: return "SLSL";
    case WUNL: return "WUNL";
    case WRN1: return "WRN1";
    case WRN0: return "WRN0";
    case WLNL: return "WLNL";
    case WUWL: return "WUWL";
    case WSW0: return "WSW0";
    case WSW1: return "WSW1";
    case WLWL: return "WLWL";
    case NUEL: return "NUEL";
    case NRE1: return "NRE1";
    case NRE0: return "NRE0";
    case NLEL: return "NLEL";
    case NUNL: return "NUNL";
    case NSN0: return "NSN0";
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
  DTSlot,
  DTLock,
  DTTest,
  DTGate,
  DTJoin,
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
    case DTSlot: return "DTSlot";
    case DTLock: return "DTLock";
    case DTTest: return "DTTest";
    case DTGate: return "DTGate";
    case DTJoin: return "DTJoin";
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
  DSSet1,

  eoDataState
};

char const *toConstCharPointer(DataState s) {
  switch (s) {
    case DSUnkn: return "DSUnkn";
    case DSRset: return "DSRset";
    case DSSet0: return "DSSet0";
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

// L[ock]{E,S,W,N}
// D[ata]{E,S,W,N}

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
  VoxelType voxelType;
  TickPerCycle activeDuring[4];
  Direction fwd;
  Direction bwd;
  Direction lwd;
  Direction rwd;
} rodTypeProperties[eoRodType] = {
#undef ____
#define ____ eoTickPerCycle
  { VTUnkn, { ____, ____, ____, ____ }, DirX, DirX, DirX, DirX }, // RTUn
  { VTLock, { WUNL, WLNL, NUEL, NLEL }, DirE, DirW, DirN, DirS }, // RTLE
  { VTLock, { NUEL, NLEL, EUEL, ELEL }, DirS, DirN, DirW, DirE }, // RTLS
  { VTLock, { EUSL, ELSL, SUSL, SLSL }, DirW, DirE, DirS, DirN }, // RTLW
  { VTLock, { SUWL, SLWL, WUWL, WLWL }, DirN, DirS, DirE, DirW }, // RTLN
  { VTData, { NRE1, NRE0, ESE0, ESE1 }, DirE, DirW, DirN, DirS }, // RTDE
  { VTData, { ERS1, ERS0, SSS0, SSS1 }, DirS, DirN, DirW, DirE }, // RTDS
  { VTData, { SRW1, SRW0, WSW0, WSW1 }, DirW, DirE, DirS, DirN }, // RTDW
  { VTData, { WRN1, WRN0, NSN0, NSN1 }, DirN, DirS, DirE, DirW }, // RTDN
#undef ____
};

struct TickPerCycleProperties {
  RodType rodType;
  FwdOrBwd fwdOrBwd;
} tickPerCycleProperties[eoTickPerCycle] = {
  { RTLW, Fwd }, // EUSL
  { RTDS, Bwd }, // ERS1
  { RTDS, Bwd }, // ERS0
  { RTLW, Bwd }, // ELSL
  { RTLS, Fwd }, // EUEL
  { RTDE, Fwd }, // ESE0
  { RTDE, Fwd }, // ESE1
  { RTLS, Bwd }, // ELEL
  { RTLN, Fwd }, // SUWL
  { RTDW, Bwd }, // SRW1
  { RTDW, Bwd }, // SRW0
  { RTLN, Bwd }, // SLWL
  { RTLW, Fwd }, // SUSL
  { RTDS, Fwd }, // SSS0
  { RTDS, Fwd }, // SSS1
  { RTLW, Bwd }, // SLSL
  { RTLE, Fwd }, // WUNL
  { RTDN, Bwd }, // WRN1
  { RTDN, Bwd }, // WRN0
  { RTLE, Bwd }, // WLNL
  { RTLN, Fwd }, // WUWL
  { RTDW, Fwd }, // WSW0
  { RTDW, Fwd }, // WSW1
  { RTLN, Bwd }, // WLWL
  { RTLS, Fwd }, // NUEL
  { RTDE, Bwd }, // NRE1
  { RTDE, Bwd }, // NRE0
  { RTLS, Bwd }, // NLEL
  { RTLE, Fwd }, // NUEL
  { RTDN, Fwd }, // NSE0
  { RTDN, Fwd }, // NSE1
  { RTLE, Bwd }  // NLEL
};

// U[nknown]
// W[all]
// S[lot]({ES,SW,WN,NE}{L[ocked],U[unlocked],R[eset],[Set]0,I[llegal],[Set]1})?
// L[ock]{B[ody},H[ead],T[ail],P[ost],K[ey]}
//       {E,S,N,W}
//       {L[ocked],U[unlocked]}
// D[ata]{B[ody},H[ead],T[ail],P[ost],S[lot],[Loc]K,T[est],G[ate],J[oin],I[nput],O[utput}}
//       {E,S,W,N}
//       {R[eset],[Set]0,[Set]1}

enum Voxel {
  Unkn,

  Wall,

  Slot,

  LBEL, LBEU,  LBSL, LBSU,  LBWL, LBWU,  LBNL, LBNU,
  LHEL, LHEU,  LHSL, LHSU,  LHWL, LHWU,  LHNL, LHNU,
  LTEL, LTEU,  LTSL, LTSU,  LTWL, LTWU,  LTNL, LTNU,
  LPEL, LPEU,  LPSL, LPSU,  LPWL, LPWU,  LPNL, LPNU,
  LKEL, LKEU,  LKSL, LKSU,  LKWL, LKWU,  LKNL, LKNU,

  DBER, DBE0, DBE1,  DBSR, DBS0, DBS1,
  DBWR, DBW0, DBW1,  DBNR, DBN0, DBN1,
  DHER, DHE0, DHE1,  DHSR, DHS0, DHS1,
  DHWR, DHW0, DHW1,  DHNR, DHN0, DHN1,
  DTER, DTE0, DTE1,  DTSR, DTS0, DTS1,
  DTWR, DTW0, DTW1,  DTNR, DTN0, DTN1,
  DPER, DPE0, DPE1,  DPSR, DPS0, DPS1,
  DPWR, DPW0, DPW1,  DPNR, DPN0, DPN1,
  DSER, DSE0, DSE1,  DSSR, DSS0, DSS1,
  DSWR, DSW0, DSW1,  DSNR, DSN0, DSN1,
  DKER, DKE0, DKE1,  DKSR, DKS0, DKS1,
  DKWR, DKW0, DKW1,  DKNR, DKN0, DKN1,
  DQER, DQE0, DQE1,  DQSR, DQS0, DQS1,
  DQWR, DQW0, DQW1,  DQNR, DQN0, DQN1,
  DGER, DGE0, DGE1,  DGSR, DGS0, DGS1,
  DGWR, DGW0, DGW1,  DGNR, DGN0, DGN1,
  DJER, DJE0, DJE1,  DJSR, DJS0, DJS1,
  DJWR, DJW0, DJW1,  DJNR, DJN0, DJN1,
  DIER, DIE0, DIE1,  DISR, DIS0, DIS1,
  DIWR, DIW0, DIW1,  DINR, DIN0, DIN1,
  DOER, DOE0, DOE1,  DOSR, DOS0, DOS1,
  DOWR, DOW0, DOW1,  DONR, DON0, DON1,

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
  case DBE1: return "DBE1";
  case DBSR: return "DBSR";
  case DBS0: return "DBS0";
  case DBS1: return "DBS1";
  case DBWR: return "DBWR";
  case DBW0: return "DBW0";
  case DBW1: return "DBW1";
  case DBNR: return "DBNR";
  case DBN0: return "DBN0";
  case DBN1: return "DBN1";
  case DHER: return "DHER";
  case DHE0: return "DHE0";
  case DHE1: return "DHE1";
  case DHSR: return "DHSR";
  case DHS0: return "DHS0";
  case DHS1: return "DHS1";
  case DHWR: return "DHWR";
  case DHW0: return "DHW0";
  case DHW1: return "DHW1";
  case DHNR: return "DHNR";
  case DHN0: return "DHN0";
  case DHN1: return "DHN1";
  case DTER: return "DTER";
  case DTE0: return "DTE0";
  case DTE1: return "DTE1";
  case DTSR: return "DTSR";
  case DTS0: return "DTS0";
  case DTS1: return "DTS1";
  case DTWR: return "DTWR";
  case DTW0: return "DTW0";
  case DTW1: return "DTW1";
  case DTNR: return "DTNR";
  case DTN0: return "DTN0";
  case DTN1: return "DTN1";
  case DPER: return "DPER";
  case DPE0: return "DPE0";
  case DPE1: return "DPE1";
  case DPSR: return "DPSR";
  case DPS0: return "DPS0";
  case DPS1: return "DPS1";
  case DPWR: return "DPWR";
  case DPW0: return "DPW0";
  case DPW1: return "DPW1";
  case DPNR: return "DPNR";
  case DPN0: return "DPN0";
  case DPN1: return "DPN1";
  case DSER: return "DSER";
  case DSE0: return "DSE0";
  case DSE1: return "DSE1";
  case DSSR: return "DSSR";
  case DSS0: return "DSS0";
  case DSS1: return "DSS1";
  case DSWR: return "DSWR";
  case DSW0: return "DSW0";
  case DSW1: return "DSW1";
  case DSNR: return "DSNR";
  case DSN0: return "DSN0";
  case DSN1: return "DSN1";
  case DKER: return "DKER";
  case DKE0: return "DKE0";
  case DKE1: return "DKE1";
  case DKSR: return "DKSR";
  case DKS0: return "DKS0";
  case DKS1: return "DKS1";
  case DKWR: return "DKWR";
  case DKW0: return "DKW0";
  case DKW1: return "DKW1";
  case DKNR: return "DKNR";
  case DKN0: return "DKN0";
  case DKN1: return "DKN1";
  case DQER: return "DQER";
  case DQE0: return "DQE0";
  case DQE1: return "DQE1";
  case DQSR: return "DQSR";
  case DQS0: return "DQS0";
  case DQS1: return "DQS1";
  case DQWR: return "DQWR";
  case DQW0: return "DQW0";
  case DQW1: return "DQW1";
  case DQNR: return "DQNR";
  case DQN0: return "DQN0";
  case DQN1: return "DQN1";
  case DGER: return "DGER";
  case DGE0: return "DGE0";
  case DGE1: return "DGE1";
  case DGSR: return "DGSR";
  case DGS0: return "DGS0";
  case DGS1: return "DGS1";
  case DGWR: return "DGWR";
  case DGW0: return "DGW0";
  case DGW1: return "DGW1";
  case DGNR: return "DGNR";
  case DGN0: return "DGN0";
  case DGN1: return "DGN1";
  case DJER: return "DJER";
  case DJE0: return "DJE0";
  case DJE1: return "DJE1";
  case DJSR: return "DJSR";
  case DJS0: return "DJS0";
  case DJS1: return "DJS1";
  case DJWR: return "DJWR";
  case DJW0: return "DJW0";
  case DJW1: return "DJW1";
  case DJNR: return "DJNR";
  case DJN0: return "DJN0";
  case DJN1: return "DJN1";
  case DIER: return "DIER";
  case DIE0: return "DIE0";
  case DIE1: return "DIE1";
  case DISR: return "DISR";
  case DIS0: return "DIS0";
  case DIS1: return "DIS1";
  case DIWR: return "DIWR";
  case DIW0: return "DIW0";
  case DIW1: return "DIW1";
  case DINR: return "DINR";
  case DIN0: return "DIN0";
  case DIN1: return "DIN1";
  case DOER: return "DOER";
  case DOE0: return "DOE0";
  case DOE1: return "DOE1";
  case DOSR: return "DOSR";
  case DOS0: return "DOS0";
  case DOS1: return "DOS1";
  case DOWR: return "DOWR";
  case DOW0: return "DOW0";
  case DOW1: return "DOW1";
  case DONR: return "DONR";
  case DON0: return "DON0";
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
  array<RodType, 3> rodType;
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
  VT##voxelType, \
  RT##rodType0, \
  RT##rodType1, \
  RT##rodType2, \
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
  Props(2,'-',Lock,LW,Un,Un,Body,Lckd,Unkn,Unkn,T,___,W,LBWU,E,____), // LBWL
  Props(2,'-',Lock,LW,Un,Un,Body,Unlk,Unkn,Unkn,T,___,W,____,E,LBWL), // LBWU
  Props(2,'|',Lock,LN,Un,Un,Body,Lckd,Unkn,Unkn,T,___,N,LBNU,S,____), // LBNL
  Props(2,'|',Lock,LN,Un,Un,Body,Unlk,Unkn,Unkn,T,___,N,____,S,LBNL), // LBNU
  Props(3,'>',Lock,LE,Un,Un,Head,Lckd,Unkn,Unkn,T,___,E,LHEU,W,____), // LHEL
  Props(3,'>',Lock,LE,Un,Un,Head,Unlk,Unkn,Unkn,T,___,E,____,W,LHEL), // LHEU
  Props(3,'v',Lock,LS,Un,Un,Head,Lckd,Unkn,Unkn,T,___,S,LHSU,N,____), // LHSL
  Props(3,'v',Lock,LS,Un,Un,Head,Unlk,Unkn,Unkn,T,___,S,____,N,LHSL), // LHSU
  Props(3,'<',Lock,LW,Un,Un,Head,Lckd,Unkn,Unkn,T,___,W,LHWU,E,____), // LHWL
  Props(3,'<',Lock,LW,Un,Un,Head,Unlk,Unkn,Unkn,T,___,W,____,E,LHWL), // LHWU
  Props(3,'^',Lock,LN,Un,Un,Head,Lckd,Unkn,Unkn,T,___,N,LHNU,S,____), // LHNL
  Props(3,'^',Lock,LN,Un,Un,Head,Unlk,Unkn,Unkn,T,___,N,____,S,LHNL), // LHNU
  Props(3,'>',Lock,LE,Un,Un,Tail,Lckd,Unkn,Unkn,T,___,E,LTEU,W,____), // LTEL
  Props(3,'>',Lock,LE,Un,Un,Tail,Unlk,Unkn,Unkn,T,___,E,____,W,LTEL), // LTEU
  Props(3,'v',Lock,LS,Un,Un,Tail,Lckd,Unkn,Unkn,T,___,S,LTSU,N,____), // LTSL
  Props(3,'v',Lock,LS,Un,Un,Tail,Unlk,Unkn,Unkn,T,___,S,____,N,LTSL), // LTSU
  Props(3,'<',Lock,LW,Un,Un,Tail,Lckd,Unkn,Unkn,T,___,W,LTWU,E,____), // LTWL
  Props(3,'<',Lock,LW,Un,Un,Tail,Unlk,Unkn,Unkn,T,___,W,____,E,LTWL), // LTWU
  Props(3,'^',Lock,LN,Un,Un,Tail,Lckd,Unkn,Unkn,T,___,N,LTNU,S,____), // LTNL
  Props(3,'^',Lock,LN,Un,Un,Tail,Unlk,Unkn,Unkn,T,___,N,____,S,LTNL), // LTNU
  Props(2,'+',Lock,LE,Un,Un,Post,Lckd,Unkn,Unkn,T,___,E,LPEU,W,____), // LPEL
  Props(2,'+',Lock,LE,Un,Un,Post,Unlk,Unkn,Unkn,T,___,E,____,W,LPEL), // LPEU
  Props(2,'+',Lock,LS,Un,Un,Post,Lckd,Unkn,Unkn,T,___,S,LPSU,N,____), // LPSL
  Props(2,'+',Lock,LS,Un,Un,Post,Unlk,Unkn,Unkn,T,___,S,____,N,LPSL), // LPSU
  Props(2,'+',Lock,LW,Un,Un,Post,Lckd,Unkn,Unkn,T,___,W,LPWU,E,____), // LPWL
  Props(2,'+',Lock,LW,Un,Un,Post,Unlk,Unkn,Unkn,T,___,W,____,E,LPWL), // LPWU
  Props(2,'+',Lock,LN,Un,Un,Post,Lckd,Unkn,Unkn,T,___,N,LPNU,S,____), // LPNL
  Props(2,'+',Lock,LN,Un,Un,Post,Unlk,Unkn,Unkn,T,___,N,____,S,LPNL), // LPNU
  Props(3,'<',Lock,LE,Un,Un,Lock,Lckd,Unkn,Unkn,F,FRL,E,LKEU,W,____), // LKEL
  Props(3,'<',Lock,LE,Un,Un,Lock,Unlk,Unkn,Unkn,F,FRL,E,____,W,LKEL), // LKEU
  Props(3,'^',Lock,LS,Un,Un,Lock,Lckd,Unkn,Unkn,F,FRL,S,LKSU,N,____), // LKSL
  Props(3,'^',Lock,LS,Un,Un,Lock,Unlk,Unkn,Unkn,F,FRL,S,____,N,LKSL), // LKSU
  Props(3,'>',Lock,LW,Un,Un,Lock,Lckd,Unkn,Unkn,F,FRL,W,LKWU,E,____), // LKWL
  Props(3,'>',Lock,LW,Un,Un,Lock,Unlk,Unkn,Unkn,F,FRL,W,____,E,LKWL), // LKWU
  Props(3,'v',Lock,LN,Un,Un,Lock,Lckd,Unkn,Unkn,F,FRL,N,LKNU,S,____), // LKNL
  Props(3,'v',Lock,LN,Un,Un,Lock,Unlk,Unkn,Unkn,F,FRL,N,____,S,LKNL), // LKNU
  Props(4,'-',Data,DE,Un,Un,Unkn,Unkn,Body,Rset,T,___,E,DBE0,W,____), // DBER
  Props(4,'-',Data,DE,Un,Un,Unkn,Unkn,Body,Set0,T,___,E,DBE1,W,DBER), // DBE0
  Props(4,'-',Data,DE,Un,Un,Unkn,Unkn,Body,Set1,T,___,E,____,W,DBE0), // DBE1
  Props(4,'|',Data,DS,Un,Un,Unkn,Unkn,Body,Rset,T,___,S,DBS0,N,____), // DBSR
  Props(4,'|',Data,DS,Un,Un,Unkn,Unkn,Body,Set0,T,___,S,DBS1,N,DBSR), // DBS0
  Props(4,'|',Data,DS,Un,Un,Unkn,Unkn,Body,Set1,T,___,S,____,N,DBS0), // DBS1
  Props(4,'-',Data,DW,Un,Un,Unkn,Unkn,Body,Rset,T,___,W,DBW0,E,____), // DBWR
  Props(4,'-',Data,DW,Un,Un,Unkn,Unkn,Body,Set0,T,___,W,DBW1,E,DBWR), // DBW0
  Props(4,'-',Data,DW,Un,Un,Unkn,Unkn,Body,Set1,T,___,W,____,E,DBW0), // DBW1
  Props(4,'|',Data,DN,Un,Un,Unkn,Unkn,Body,Rset,T,___,N,DBN0,S,____), // DBNR
  Props(4,'|',Data,DN,Un,Un,Unkn,Unkn,Body,Set0,T,___,N,DBN1,S,DBNR), // DBN0
  Props(4,'|',Data,DN,Un,Un,Unkn,Unkn,Body,Set1,T,___,N,____,S,DBN0), // DBN1
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Head,Rset,T,F__,E,DHE0,W,____), // DHER
  Props(5,'0',Data,DE,Un,Un,Unkn,Unkn,Head,Set0,T,F__,E,DHE1,W,DHER), // DHE0
  Props(5,'1',Data,DE,Un,Un,Unkn,Unkn,Head,Set1,T,F__,E,____,W,DHE0), // DHE1
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Head,Rset,T,F__,S,DHS0,N,____), // DHSR
  Props(5,'0',Data,DS,Un,Un,Unkn,Unkn,Head,Set0,T,F__,S,DHS1,N,DHSR), // DHS0
  Props(5,'1',Data,DS,Un,Un,Unkn,Unkn,Head,Set1,T,F__,S,____,N,DHS0), // DHS1
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Head,Rset,T,F__,W,DHW0,E,____), // DHWR
  Props(5,'0',Data,DW,Un,Un,Unkn,Unkn,Head,Set0,T,F__,W,DHW1,E,DHWR), // DHW0
  Props(5,'1',Data,DW,Un,Un,Unkn,Unkn,Head,Set1,T,F__,W,____,E,DHW0), // DHW1
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Head,Rset,T,F__,N,DHN0,S,____), // DHNR
  Props(5,'0',Data,DN,Un,Un,Unkn,Unkn,Head,Set0,T,F__,N,DHN1,S,DHNR), // DHN0
  Props(5,'1',Data,DN,Un,Un,Unkn,Unkn,Head,Set1,T,F__,N,____,S,DHN0), // DHN1
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Tail,Rset,T,_R_,E,DTE0,W,____), // DTER
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Tail,Set0,T,_R_,E,DTE1,W,DTER), // DTE0
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Tail,Set1,T,_R_,E,____,W,DTE0), // DTE1
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Tail,Rset,T,_R_,S,DTS0,N,____), // DTSR
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Tail,Set0,T,_R_,S,DTS1,N,DTSR), // DTS0
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Tail,Set1,T,_R_,S,____,N,DTS0), // DTS1
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Tail,Rset,T,_R_,W,DTW0,E,____), // DTWR
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Tail,Set0,T,_R_,W,DTW1,E,DTWR), // DTW0
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Tail,Set1,T,_R_,W,____,E,DTW0), // DTW1
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Tail,Rset,T,_R_,N,DTN0,S,____), // DTNR
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Tail,Set0,T,_R_,N,DTN1,S,DTNR), // DTN0
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Tail,Set1,T,_R_,N,____,S,DTN0), // DTN1
  Props(4,'+',Data,DE,Un,Un,Unkn,Unkn,Post,Rset,T,FR_,E,DPE0,W,____), // DPER
  Props(4,'+',Data,DE,Un,Un,Unkn,Unkn,Post,Set0,T,FR_,E,DPE1,W,DPER), // DPE0
  Props(4,'+',Data,DE,Un,Un,Unkn,Unkn,Post,Set1,T,FR_,E,____,W,DPE0), // DPE1
  Props(4,'+',Data,DS,Un,Un,Unkn,Unkn,Post,Rset,T,FR_,S,DPS0,N,____), // DPSR
  Props(4,'+',Data,DS,Un,Un,Unkn,Unkn,Post,Set0,T,FR_,S,DPS1,N,DPSR), // DPS0
  Props(4,'+',Data,DS,Un,Un,Unkn,Unkn,Post,Set1,T,FR_,S,____,N,DPS0), // DPS1
  Props(4,'+',Data,DW,Un,Un,Unkn,Unkn,Post,Rset,T,FR_,W,DPW0,E,____), // DPWR
  Props(4,'+',Data,DW,Un,Un,Unkn,Unkn,Post,Set0,T,FR_,W,DPW1,E,DPWR), // DPW0
  Props(4,'+',Data,DW,Un,Un,Unkn,Unkn,Post,Set1,T,FR_,W,____,E,DPW0), // DPW1
  Props(4,'+',Data,DN,Un,Un,Unkn,Unkn,Post,Rset,T,FR_,N,DPN0,S,____), // DPNR
  Props(4,'+',Data,DN,Un,Un,Unkn,Unkn,Post,Set0,T,FR_,N,DPN1,S,DPNR), // DPN0
  Props(4,'+',Data,DN,Un,Un,Unkn,Unkn,Post,Set1,T,FR_,N,____,S,DPN0), // DPN1
  Props(5,'^',Data,DE,Un,Un,Unkn,Unkn,Slot,Rset,F,FRL,E,DSE0,W,____), // DSER
  Props(5,'^',Data,DE,Un,Un,Unkn,Unkn,Slot,Set0,F,FRL,E,DSE1,W,DSER), // DSE0
  Props(5,'^',Data,DE,Un,Un,Unkn,Unkn,Slot,Set1,F,FRL,E,____,W,DSE0), // DSE1
  Props(5,'>',Data,DS,Un,Un,Unkn,Unkn,Slot,Rset,F,FRL,S,DSS0,N,____), // DSSR
  Props(5,'>',Data,DS,Un,Un,Unkn,Unkn,Slot,Set0,F,FRL,S,DSS1,N,DSSR), // DSS0
  Props(5,'>',Data,DS,Un,Un,Unkn,Unkn,Slot,Set1,F,FRL,S,____,N,DSS0), // DSS1
  Props(5,'v',Data,DW,Un,Un,Unkn,Unkn,Slot,Rset,F,FRL,W,DSW0,E,____), // DSWR
  Props(5,'v',Data,DW,Un,Un,Unkn,Unkn,Slot,Set0,F,FRL,W,DSW1,E,DSWR), // DSW0
  Props(5,'v',Data,DW,Un,Un,Unkn,Unkn,Slot,Set1,F,FRL,W,____,E,DSW0), // DSW1
  Props(5,'<',Data,DN,Un,Un,Unkn,Unkn,Slot,Rset,F,FRL,N,DSN0,S,____), // DSNR
  Props(5,'<',Data,DN,Un,Un,Unkn,Unkn,Slot,Set0,F,FRL,N,DSN1,S,DSNR), // DSN0
  Props(5,'<',Data,DN,Un,Un,Unkn,Unkn,Slot,Set1,F,FRL,N,____,S,DSN0), // DSN1
  Props(5,'X',Data,DE,LS,Un,Lock,Lckd,Lock,Rset,F,FRL,E,____,W,____), // DKER
  Props(5,'X',Data,DE,LS,Un,Lock,Lckd,Lock,Set0,F,FRL,E,____,W,____), // DKE0
  Props(5,'X',Data,DE,LS,Un,Lock,Lckd,Lock,Set1,F,FRL,E,____,W,____), // DKE1
  Props(5,'X',Data,DS,LW,Un,Lock,Lckd,Lock,Rset,F,FRL,S,____,N,____), // DKSR
  Props(5,'X',Data,DS,LW,Un,Lock,Lckd,Lock,Set0,F,FRL,S,____,N,____), // DKS0
  Props(5,'X',Data,DS,LW,Un,Lock,Lckd,Lock,Set1,F,FRL,S,____,N,____), // DKS1
  Props(5,'X',Data,DW,LN,Un,Lock,Lckd,Lock,Rset,F,FRL,W,____,E,____), // DKWR
  Props(5,'X',Data,DW,LN,Un,Lock,Lckd,Lock,Set0,F,FRL,W,____,E,____), // DKW0
  Props(5,'X',Data,DW,LN,Un,Lock,Lckd,Lock,Set1,F,FRL,W,____,E,____), // DKW1
  Props(5,'X',Data,DN,LE,Un,Lock,Lckd,Lock,Rset,F,FRL,N,____,S,____), // DKNR
  Props(5,'X',Data,DN,LE,Un,Lock,Lckd,Lock,Set0,F,FRL,N,____,S,____), // DKN0
  Props(5,'X',Data,DN,LE,Un,Lock,Lckd,Lock,Set1,F,FRL,N,____,S,____), // DKN1
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Test,Rset,F,FR_,E,DQE0,W,____), // DQER
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Test,Set0,F,FR_,E,DQE1,W,DQER), // DQE0
  Props(5,'>',Data,DE,Un,Un,Unkn,Unkn,Test,Set1,F,FR_,E,____,W,DQE0), // DQE1
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Test,Rset,F,FR_,S,DQS0,N,____), // DQSR
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Test,Set0,F,FR_,S,DQS1,N,DQSR), // DQS0
  Props(5,'v',Data,DS,Un,Un,Unkn,Unkn,Test,Set1,F,FR_,S,____,N,DQS0), // DQS1
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Test,Rset,F,FR_,W,DQW0,E,____), // DQWR
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Test,Set0,F,FR_,W,DQW1,E,DQWR), // DQW0
  Props(5,'<',Data,DW,Un,Un,Unkn,Unkn,Test,Set1,F,FR_,W,____,E,DQW0), // DQW1
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Test,Rset,F,FR_,N,DQN0,S,____), // DQNR
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Test,Set0,F,FR_,N,DQN1,S,DQNR), // DQN0
  Props(5,'^',Data,DN,Un,Un,Unkn,Unkn,Test,Set1,F,FR_,N,____,S,DQN0), // DQN1
  Props(5,'v',Data,DE,Un,Un,Unkn,Unkn,Gate,Rset,F,FR_,E,DGE0,W,____), // DGER
  Props(5,'v',Data,DE,Un,Un,Unkn,Unkn,Gate,Set0,F,FR_,E,DGE1,W,DGER), // DGE0
  Props(5,'v',Data,DE,Un,Un,Unkn,Unkn,Gate,Set1,F,FR_,E,____,W,DGE0), // DGE1
  Props(5,'<',Data,DS,Un,Un,Unkn,Unkn,Gate,Rset,F,FR_,S,DGS0,N,____), // DGSR
  Props(5,'<',Data,DS,Un,Un,Unkn,Unkn,Gate,Set0,F,FR_,S,DGS1,N,DGSR), // DGS0
  Props(5,'<',Data,DS,Un,Un,Unkn,Unkn,Gate,Set1,F,FR_,S,____,N,DGS0), // DGS1
  Props(5,'^',Data,DW,Un,Un,Unkn,Unkn,Gate,Rset,F,FR_,W,DGW0,E,____), // DGWR
  Props(5,'^',Data,DW,Un,Un,Unkn,Unkn,Gate,Set0,F,FR_,W,DGW1,E,DGWR), // DGW0
  Props(5,'^',Data,DW,Un,Un,Unkn,Unkn,Gate,Set1,F,FR_,W,____,E,DGW0), // DGW1
  Props(5,'>',Data,DN,Un,Un,Unkn,Unkn,Gate,Rset,F,FR_,N,DGN0,S,____), // DGNR
  Props(5,'>',Data,DN,Un,Un,Unkn,Unkn,Gate,Set0,F,FR_,N,DGN1,S,DGNR), // DGN0
  Props(5,'>',Data,DN,Un,Un,Unkn,Unkn,Gate,Set1,F,FR_,N,____,S,DGN0), // DGN1
  Props(5,'X',Data,DE,DN,DS,Lock,Lckd,Join,Rset,F,FR_,E,____,W,____), // DJER
  Props(5,'X',Data,DE,DN,DS,Lock,Lckd,Join,Set0,F,FR_,E,____,W,____), // DJE0
  Props(5,'X',Data,DE,DN,DS,Lock,Lckd,Join,Set1,F,FR_,E,____,W,____), // DJE1
  Props(5,'X',Data,DS,DW,DE,Lock,Lckd,Join,Rset,F,FR_,S,____,N,____), // DJSR
  Props(5,'X',Data,DS,DW,DE,Lock,Lckd,Join,Set0,F,FR_,S,____,N,____), // DJS0
  Props(5,'X',Data,DS,DW,DE,Lock,Lckd,Join,Set1,F,FR_,S,____,N,____), // DJS1
  Props(5,'X',Data,DW,DS,DN,Lock,Lckd,Join,Rset,F,FR_,W,____,E,____), // DJWR
  Props(5,'X',Data,DW,DS,DN,Lock,Lckd,Join,Set0,F,FR_,W,____,E,____), // DJW0
  Props(5,'X',Data,DW,DS,DN,Lock,Lckd,Join,Set1,F,FR_,W,____,E,____), // DJW1
  Props(5,'X',Data,DN,DE,DW,Lock,Lckd,Join,Rset,F,FR_,N,____,S,____), // DJNR
  Props(5,'X',Data,DN,DE,DW,Lock,Lckd,Join,Set0,F,FR_,N,____,S,____), // DJN0
  Props(5,'X',Data,DN,DE,DW,Lock,Lckd,Join,Set1,F,FR_,N,____,S,____), // DJN1
  Props(6,'I',Data,DE,Un,Un,Unkn,Unkn,Inpt,Rset,T,F__,E,DIE0,W,____), // DIER
  Props(6,'0',Data,DE,Un,Un,Unkn,Unkn,Inpt,Set0,T,F__,E,DIE1,W,DIER), // DIE0
  Props(6,'1',Data,DE,Un,Un,Unkn,Unkn,Inpt,Set1,T,F__,E,____,W,DIE0), // DIE1
  Props(6,'I',Data,DS,Un,Un,Unkn,Unkn,Inpt,Rset,T,F__,S,DIS0,N,____), // DISR
  Props(6,'0',Data,DS,Un,Un,Unkn,Unkn,Inpt,Set0,T,F__,S,DIS1,N,DISR), // DIS0
  Props(6,'1',Data,DS,Un,Un,Unkn,Unkn,Inpt,Set1,T,F__,S,____,N,DIS0), // DIS1
  Props(6,'I',Data,DW,Un,Un,Unkn,Unkn,Inpt,Rset,T,F__,W,DIW0,E,____), // DIWR
  Props(6,'0',Data,DW,Un,Un,Unkn,Unkn,Inpt,Set0,T,F__,W,DIW1,E,DIWR), // DIW0
  Props(6,'1',Data,DW,Un,Un,Unkn,Unkn,Inpt,Set1,T,F__,W,____,E,DIW0), // DIW1
  Props(6,'I',Data,DN,Un,Un,Unkn,Unkn,Inpt,Rset,T,F__,N,DIN0,S,____), // DINR
  Props(6,'0',Data,DN,Un,Un,Unkn,Unkn,Inpt,Set0,T,F__,N,DIN1,S,DINR), // DIN0
  Props(6,'1',Data,DN,Un,Un,Unkn,Unkn,Inpt,Set1,T,F__,N,____,S,DIN0), // DIN1
  Props(6,'O',Data,DE,Un,Un,Unkn,Unkn,Outp,Rset,T,___,E,DOE0,W,____), // DOER
  Props(6,'0',Data,DE,Un,Un,Unkn,Unkn,Outp,Set0,T,___,E,DOE1,W,DOER), // DOE0
  Props(6,'1',Data,DE,Un,Un,Unkn,Unkn,Outp,Set1,T,___,E,____,W,DOE0), // DOE1
  Props(6,'O',Data,DS,Un,Un,Unkn,Unkn,Outp,Rset,T,___,S,DOS0,N,____), // DOSR
  Props(6,'0',Data,DS,Un,Un,Unkn,Unkn,Outp,Set0,T,___,S,DOS1,N,DOSR), // DOS0
  Props(6,'1',Data,DS,Un,Un,Unkn,Unkn,Outp,Set1,T,___,S,____,N,DOS0), // DOS1
  Props(6,'O',Data,DW,Un,Un,Unkn,Unkn,Outp,Rset,T,___,W,DOW0,E,____), // DOWR
  Props(6,'0',Data,DW,Un,Un,Unkn,Unkn,Outp,Set0,T,___,W,DOW1,E,DOWR), // DOW0
  Props(6,'1',Data,DW,Un,Un,Unkn,Unkn,Outp,Set1,T,___,W,____,E,DOW0), // DOW1
  Props(6,'O',Data,DN,Un,Un,Unkn,Unkn,Outp,Rset,T,___,N,DON0,S,____), // DONR
  Props(6,'0',Data,DN,Un,Un,Unkn,Unkn,Outp,Set0,T,___,N,DON1,S,DONR), // DON0
  Props(6,'1',Data,DN,Un,Un,Unkn,Unkn,Outp,Set1,T,___,N,____,S,DON0)  // DON1
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
size_t const NRows = 28;
size_t const NCols = 25;

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
  void AddRule(Scenario const &scenario, Voxel newVoxel) {
    rules[scenario] = newVoxel;
  }
  Rules &GetRules() { return rules; }
  void AddInput(VoxelCoordinant const &vc, deque<DataState> const &values) {
    inputs[vc] = values;
  }
  deque<DataState> &GetInputsFor(VoxelCoordinant const &vc) { return inputs[vc]; }
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
  virtual bool CheckForFreedomOfMovement(Volume *volume, FwdOrBwd fwdOrBwd) = 0;
  virtual bool CheckForFreedomOfMovement(Volume *volume) = 0;
  virtual bool AttemptToMove
      (Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes) = 0;
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
  bool AttemptToMove(Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes);
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
  bool AttemptToMove(Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes);
  bool IsValid(Volume const *volume);
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
  area[DirE] = volume->voxelAt(vc.To(DirE));
  area[DirS] = volume->voxelAt(vc.To(DirS));
  area[DirD] = volume->voxelAt(vc.To(DirD));
  area[DirW] = volume->voxelAt(vc.To(DirW));
  area[DirN] = volume->voxelAt(vc.To(DirN));
  area[DirU] = volume->voxelAt(vc.To(DirU));
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
    case RTLE: case RTLS: case RTLW: case RTLN:
      item = new LockRod(rodType);
      break;
    case RTDE: case RTDS: case RTDW: case RTDN:
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

bool Item::IsValid(Volume const *volume) {
  array<size_t, eoRodType> rodTypeCounts;

  rodTypeCounts.fill(0);
  bool foundMoreThan1RodType = false;
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];
    if (vProperties.voxelType != VTSlot) {
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
  lockState = LSUnkn;

  fBlkState = FBUnkn;
  rBlkState = RBUnkn;

  size_t fBlkCounts = 0;
  size_t rBlkCounts = 0;
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];
    if (vProperties.lockState != LSUnkn) {
      lockStateCounts[vProperties.lockState] += 1;
      foundMoreThan1LockState |=
	lockState != LSUnkn &&
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

  fBlkState = fBlkCounts != 0 ? FBBlkd : FBUnbk;
  rBlkState = rBlkCounts != 0 ? RBBlkd : RBUnbk;

  return !IsBlocked();
}

bool LockRod::AttemptToMove
    (Volume *volume,
     FwdOrBwd fwdOrBwd,
     Changes &changes
    )
{
  bool isMovable = CheckForFreedomOfMovement(volume, fwdOrBwd);

  if (optVerbose) {
    fprintf(stdout,
            "(LockRod *)(%p)->AttemptToMove(): fwdOrBwd=%s\n",
            this,
            toConstCharPointer(fwdOrBwd)
            );
    Dump(volume);
    fprintf(stdout, "\n");
  }

  if (isMovable) {
    TickPerCycle tick = volume->CurrentTickPerCycle();
    TickPerCycleProperties const &tProperties = tickPerCycleProperties[tick];
    assert(rodType == tProperties.rodType);
    RodTypeProperties const &rProperties = rodTypeProperties[rodType];
    VoxelType rodVoxelType = VTLock;
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

      if (thisVoxel == Slot || rodType == tvProperties.rodType[0]) {
        prevVoxel = scenario.From(antiDirectionOfMotion);
        VoxelProperties const &pvProperties = voxelProperties[prevVoxel];
        if (pvProperties.IsOneOf(rodType)) {
          nextVoxel = pvProperties.motion[fwdOrBwd].nextVoxel;
        } else {
          nextVoxel = Slot;
        }
      } else {
        assert(tvProperties.voxelType == VTData);
        assert(tvProperties.dataType == DTSlot ||
               tvProperties.dataType == DTJoin
              );
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
    }
    if (madeChanges) {
      lastMovedAt = volume->CurrentClock();
    }
    return true;
  }
  return false;
}

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
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];
    if (rodType != vProperties.rodType[0]) {
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
        deque<DataState> &values = volume->GetInputsFor(vc);
        DataState inputState = values.empty() ? DSSet0 : values.front();
        bool fBlockedByInput = false;
        switch (tickPerPhase) {
          case SetDataRodsTo0:
            fBlockedByInput = inputState < DSSet0;
            break;
          case SetDataRodsTo1:
            fBlockedByInput = inputState < DSSet1;
            if (!values.empty()) {
              values.pop_front();
            }
          default:
            break;
        }
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
      if (vProperties.dataType == DTLock) {
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

  fBlkState = fBlkCounts != 0 ? FBBlkd : FBUnbk;
  rBlkState = rBlkCounts != 0 ? RBBlkd : RBUnbk;
  lockState = lockCounts != 0 ? LSLckd : LSUnlk;
  return !IsBlocked();
}

bool DataRod::AttemptToMove
    (Volume *volume,
     FwdOrBwd fwdOrBwd,
     Changes &changes
    )
{
  bool isMovable = CheckForFreedomOfMovement(volume, fwdOrBwd);

  if (optVerbose) {
    fprintf(stdout,
            "(DataRod *)(%p)->AttemptToMove(): fwdOrBwd=%s\n",
            this,
            toConstCharPointer(fwdOrBwd)
            );
    Dump(volume);
    fprintf(stdout, "\n");
  }

  if (isMovable) {
    TickPerCycle tick = volume->CurrentTickPerCycle();
    TickPerCycleProperties const &tProperties = tickPerCycleProperties[tick];
    assert(rodType == tProperties.rodType);
    RodTypeProperties const &rProperties = rodTypeProperties[rodType];
    VoxelType rodVoxelType = rProperties.voxelType;
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

      if (thisVoxel == Slot || rodType == tvProperties.rodType[0]) {
        prevVoxel = scenario.From(antiDirectionOfMotion);
        VoxelProperties const &pvProperties = voxelProperties[prevVoxel];
        if (pvProperties.IsOneOf(rodType)) {
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
      }
    }
    if (madeChanges) {
      lastMovedAt = volume->CurrentClock();
    }
    return true;
  }
  return false;
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
  for (auto const &vc : *this) {
    Voxel v = volume->voxelAt(vc);
    VoxelProperties const &vProperties = voxelProperties[v];
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

  //   abcdefghijklmnopqrstuvwxy
  // a ......v........... ...... a
  // b ......|...........^...... b
  // c ......|...........|...... c
  // d ......|...........|...... d
  // e ......|.....>..>..|...... e
  // f ......|.....>..>..|...... f
  // g  <----------X- X--------< g
  // h ......|.....|..|..|...... h
  // i ......|.....v..v..|...... i
  // j I---^^X---0-|-1|>.|.....C j
  // k ......|.....|..|..|...... k
  // l ......|.....|..v..|...... l
  // m I---^^X-----|-1|>.|.....D m
  // n ......|.....|..|..|...... n
  // o ......|..^..v..|..|...... o
  // p ....^^X->|-1|--|--------O p
  // q ......|..1..0..0..|...... q
  // r ......|..|..|..|..|...... r
  // s ......|.<|-0|<-|<-Xvv.... s
  // t ......|..^..v..v..|...... t
  // u ......|..|........|...... u
  // v >--------X------------->  v
  // w ......|..<........|...... w
  // x ......|..<........|...... x
  // y ......|...........|...... y
  // z ......|...........|...... z
  // A ......v...........|...... A
  // B ...... ...........^...... B
  //   abcdefghijklmnopqrstuvwxy

VolArray initialVolume = {
  //   abcdefghijklmnopqrstuvwxy
  // a ......v........... ...... a
  // b ......|...........^...... b
  // c ......|...........|...... c
  // d ......|...........|...... d
  // e ......|...........|...... e
  // f ......|...........|...... f
  // g ......|...........|...... g
  // h ......|...........|...... h
  // i ......|...........|...... i
  // j ......+...........|...... j
  // k ......|...........|...... k
  // l ......|...........|...... l
  // m ......+...........|...... m
  // n ......|...........|...... n
  // o ......|...........|...... o
  // p ......+...........|...... p
  // q ......|...........|...... q
  // r ......|...........|...... r
  // s ......|...........+...... s
  // t ......|...........|...... t
  // u ......|...........|...... u
  // v ......|...........|...... v
  // w ......|...........|...... w
  // x ......|...........|...... x
  // y ......|...........|...... y
  // z ......|...........|...... z
  // A ......v...........|...... A
  // B ...... ...........^...... B
  //   abcdefghijklmnopqrstuvwxy

  //        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y
  /* a */ Wall,Wall,Wall,Wall,Wall,Wall,LTSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall, /* a */
  /* b */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LHNL,Wall,Wall,Wall,Wall,Wall,Wall, /* b */
  /* c */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* c */
  /* d */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* d */
  /* e */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* e */
  /* f */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* f */
  /* g */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* g */
  /* h */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* h */
  /* i */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* i */
  /* j */ Wall,Wall,Wall,Wall,Wall,Wall,LPSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* j */
  /* k */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* k */
  /* l */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* l */
  /* m */ Wall,Wall,Wall,Wall,Wall,Wall,LPSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* m */
  /* n */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* n */
  /* o */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* o */
  /* p */ Wall,Wall,Wall,Wall,Wall,Wall,LPSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* p */
  /* q */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* q */
  /* r */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* r */
  /* s */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LPNL,Wall,Wall,Wall,Wall,Wall,Wall, /* s */
  /* t */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* t */
  /* u */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* u */
  /* v */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* v */
  /* w */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* w */
  /* x */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* x */
  /* y */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* y */
  /* z */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* z */
  /* A */ Wall,Wall,Wall,Wall,Wall,Wall,LHSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* A */
  /* B */ Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LTNL,Wall,Wall,Wall,Wall,Wall,Wall, /* B */
  //        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ......................... e
  // f ......................... f
  // g ......................... g
  // h ......................... h
  // i ......................... i
  // j ....^^X  ................ j
  // k ...... .................. k
  // l ......................... l
  // m ....^^X  ................ m
  // n ...... .................. n
  // o ......................... o
  // p ....^^X  ................ p
  // q ...... .................. q
  // r .................. ...... r
  // s ................  Xvv.... s
  // t ......................... t
  // u ......................... u
  // v ......................... v
  // w ......................... w
  // x ......................... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

  //        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y
  /* a */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* a */
  /* b */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* b */
  /* c */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* c */
  /* d */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* d */
  /* e */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* e */
  /* f */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* f */
  /* g */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* g */
  /* h */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* h */
  /* i */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* i */
  /* j */ Wall,Wall,Wall,Wall,DSER,DSER,DKER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* j */
  /* k */ Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* k */
  /* l */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* l */
  /* m */ Wall,Wall,Wall,Wall,DSER,DSER,DKER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* m */
  /* n */ Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* n */
  /* o */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* o */
  /* p */ Wall,Wall,Wall,Wall,DSER,DSER,DKER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* p */
  /* q */ Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* q */
  /* r */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall, /* r */
  /* s */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,DKWR,DSWR,DSWR,Wall,Wall,Wall,Wall, /* s */
  /* t */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* t */
  /* u */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* u */
  /* v */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* v */
  /* w */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* w */
  /* x */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* x */
  /* y */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* y */
  /* z */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* z */
  /* A */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* A */
  /* B */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* B */
  //        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ......................... e
  // f ......................... f
  // g ......................... g
  // h ......................... h
  // i ......................... i
  // j I---^^X---0---1->  ...... j
  // k ......................... k
  // l ......................... l
  // m I---^^X-------1->  ...... m
  // n ......................... n
  // o ......................... o
  // p ....^^X->--1------------O p
  // q ......................... q
  // r ......................... r
  // s ......  <--0-<--<-Xvv.... s
  // t ......................... t
  // u ......................... u
  // v ......................... v
  // w ......................... w
  // x ......................... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
DIER,DBER,DBER,DBER,DPER,DPER,DPER,DBER,DBER,DBER,DPER,DBER,DBER,DBER,DPER,DBER,DHER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
DIER,DBER,DBER,DBER,DPER,DPER,DPER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DPER,DBER,DHER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,DPER,DPER,DPER,DBER,DTER,DBER,DBER,DPER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DOER,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,DHWR,DBWR,DBWR,DPWR,DBWR,DTWR,DBWR,DBWR,DTWR,DBWR,DPWR,DPWR,DPWR,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ......................... e
  // f ......................... f
  // g ......................... g
  // h ......................... h
  // i ............v..v......... i
  // j ..........0  .1  ........ j
  // k ............ .. ......... k
  // l ...............v......... l
  // m ..............1  ........ m
  // n ............... ......... n
  // o ......... ..v............ o
  // p ........>  1  ........... p
  // q .........1..0..0......... q
  // r ......... .. .. ......... r
  // s ........   0 <  <........ s
  // t .........^.. .. ......... t
  // u ......................... u
  // v ......................... v
  // w ......................... w
  // x ......................... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQSR,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DGER,Slot,Slot,Wall,DGER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DGER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQER,Slot,Slot,DQER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQNR,Wall,Wall,DQSR,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,Slot,DGWR,Slot,DQWR,Slot,Slot,DQWR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQNR,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ............>..>......... e
  // f ............>..>......... f
  // g ............X..X......... g
  // h ............|..|......... h
  // i ............v..v......... i
  // j ............|..|......... j
  // k ............|..|......... k
  // l ............|..v......... l
  // m ......... ..|..|......... m
  // n ......... ..|..|......... n
  // o .........^..v..|......... o
  // p .........|..|..|......... p
  // q .........1..0..0......... q
  // r .........|..|..|......... r
  // s .........|..|..|......... s
  // t .........^..v..v......... t
  // u .........|.. .. ......... u
  // v .........X.. .. ......... v
  // w .........<............... w
  // x .........<............... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DHNR,Wall,Wall,DPSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,DPSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,DHSR,Wall,Wall,DHSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ............>..>......... e
  // f ............>..>......... f
  // g ........... X. X......... g
  // h ............ .. ......... h
  // i ............ .. ......... i
  // j ......................... j
  // k ......................... k
  // l ......................... l
  // m ......................... m
  // n ......................... n
  // o ......................... o
  // p ......................... p
  // q ......................... q
  // r ......................... r
  // s ......................... s
  // t ......... ............... t
  // u ......... ............... u
  // v .........X .............. v
  // w .........<............... w
  // x .........<............... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSSR,Wall,Wall,DSSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSSR,Wall,Wall,DSSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,DKSR,Wall,Slot,DKSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DKNR,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ......................... e
  // f ......................... f
  // g  <----------X--X--------< g
  // h ......................... h
  // i ......................... i
  // j ......................... j
  // k ......................... k
  // l ......................... l
  // m ......................... m
  // n ......................... n
  // o ......................... o
  // p ......................... p
  // q ......................... q
  // r ......................... r
  // s ......................... s
  // t ......................... t
  // u ......................... u
  // v >--------X------------->  v
  // w ......................... w
  // x ......................... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Slot,LHWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LPWL,LBWL,LBWL,LPWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LTWL,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
LTWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LPWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LHWL,Slot,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
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
