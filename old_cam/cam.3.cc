#include <cstddef>
#include <cstdio>
#include <array>
using std::array;
#include <map>
using std::map;
#include <set>
using std::set;
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
  EUNL,
  ERN1,
  ERNX,
  ERN0,
  ELNL,
  EUCL,
  ESC0,
  ESCX,
  ESC1,
  ELCL,
  SUNL,
  SRN1,
  SRNX,
  SRN0,
  SLNL,
  SUCL,
  SSC0,
  SSCX,
  SSC1,
  SLCL,
  WUNL,
  WRN1,
  WRNX,
  WRN0,
  WLNL,
  WUCL,
  WSC0,
  WSCX,
  WSC1,
  WLCL,
  NUNL,
  NRN1,
  NRNX,
  NRN0,
  NLNL,
  NUCL,
  NSC0,
  NSCX,
  NSC1,
  NLCL,

  eoTickPerCycle
};

char const *toConstCharPointer(TickPerCycle t) {
  switch (t) {
    case EUNL: return "EUNL";
    case ERN1: return "ERN1";
    case ERNX: return "ERNX";
    case ERN0: return "ERN0";
    case ELNL: return "ELNL";
    case EUCL: return "EUCL";
    case ESC0: return "ESC0";
    case ESCX: return "ESCX";
    case ESC1: return "ESC1";
    case ELCL: return "ELCL";
    case SUNL: return "SUNL";
    case SRN1: return "SRN1";
    case SRNX: return "SRNX";
    case SRN0: return "SRN0";
    case SLNL: return "SLNL";
    case SUCL: return "SUCL";
    case SSC0: return "SSC0";
    case SSCX: return "SSCX";
    case SSC1: return "SSC1";
    case SLCL: return "SLCL";
    case WUNL: return "WUNL";
    case WRN1: return "WRN1";
    case WRNX: return "WRNX";
    case WRN0: return "WRN0";
    case WLNL: return "WLNL";
    case WUCL: return "WUCL";
    case WSC0: return "WSC0";
    case WSCX: return "WSCX";
    case WSC1: return "WSC1";
    case WLCL: return "WLCL";
    case NUNL: return "NUNL";
    case NRN1: return "NRN1";
    case NRNX: return "NRNX";
    case NRN0: return "NRN0";
    case NLNL: return "NLNL";
    case NUCL: return "NUCL";
    case NSC0: return "NSC0";
    case NSCX: return "NSCX";
    case NSC1: return "NSC1";
    case NLCL: return "NLCL";
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
  KTUnkn,

  KTBody,
  KTHead,
  KTTail,
  KTPost,
  KTLock,

  eoLockType
};

char const *toConstCharPointer(LockType t) {
  switch (t) {
    case KTUnkn: return "KTUnkn";
    case KTBody: return "KTBody";
    case KTHead: return "KTHead";
    case KTTail: return "KTTail";
    case KTPost: return "KTPost";
    case KTLock: return "KTLock";
    case eoLockType:
      return "eoLockType";
  }
  return "LockType(?)";
}

enum LockState {
  KSUnkn,

  KSLckd,
  KSUnlk,

  eoLockState
};

char const *toConstCharPointer(LockState t) {
  switch (t) {
    case KSUnkn: return "KSUnkn";
    case KSLckd: return "KSLckd";
    case KSUnlk: return "KSUnlk";
    case eoLockState:
      return "eoLockState";
  }
  return "LockState(?)";
}

enum DataType {
  LTUnkn,

  LTBody,
  LTHead,
  LTTail,
  LTPost,
  LTLock,
  LTTest,
  LTGate,
  LTInpt,
  LTOutp,

  eoDataType
};

char const *toConstCharPointer(DataType t) {
  switch (t) {
    case LTUnkn: return "LTUnkn";
    case LTBody: return "LTBody";
    case LTHead: return "LTHead";
    case LTTail: return "LTTail";
    case LTPost: return "LTPost";
    case LTLock: return "LTLock";
    case LTTest: return "LTTest";
    case LTGate: return "LTGate";
    case LTInpt: return "LTInpt";
    case LTOutp: return "LTOutp";
    case eoDataType:
      return "eoDataType";
  }
  return "DataType(?)";
}

enum DataState {
  LSUnkn,

  LSRset,
  LSSet0,
  LSXXXX,
  LSSet1,

  eoDataState
};

char const *toConstCharPointer(DataState s) {
  switch (s) {
    case LSUnkn: return "LSUnkn";
    case LSRset: return "LSRset";
    case LSSet0: return "LSSet0";
    case LSXXXX: return "LSXXXX";
    case LSSet1: return "LSSet1";
    case eoDataState:
      return "eoDataState";
  }
  return "DataState(?)";
}

enum BlockState {
  ___,  // FBUnbk, RBUnbk, KSUnlk
  F__,  // FBBlkd, RBUnbk, KSUnlk
  _R_,  // FBUnbk, RBBlkd, KSUnlk
  FR_,  // FBBlkd, RBBlkd, KSUnlk
  __L,  // FBUnbk, RBUnbk, KSLckd
  F_L,  // FBBlkd, RBUnbk, KSLckd
  _RL,  // FBUnbk, RBBlkd, KSLckd
  FRL,  // FBBlkd, RBBlkd, KSLckd

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
  { { WUNL, WLNL, NUCL, NLCL, ____, ____ }, DirE, DirW, DirN, DirS }, // RTKE
  { { SUNL, SLNL, EUCL, ELCL, ____, ____ }, DirS, DirN, DirW, DirE }, // RTKS
  { { EUNL, ELNL, SUCL, SLCL, ____, ____ }, DirW, DirE, DirS, DirN }, // RTKW
  { { NUNL, NLNL, WUCL, WLCL, ____, ____ }, DirN, DirS, DirE, DirW }, // RTKN
  { { NRN1, NRNX, NRN0, ESC0, ESCX, ESC1 }, DirE, DirW, DirN, DirS }, // RTLE
  { { ERN1, ERNX, ERN0, SSC0, SSCX, SSC1 }, DirS, DirN, DirW, DirE }, // RTLS
  { { SRN1, SRNX, SRN0, WSC0, WSCX, WSC1 }, DirW, DirE, DirS, DirN }, // RTLW
  { { WRN1, WRNX, WRN0, NSC0, NSCX, NSC1 }, DirN, DirS, DirE, DirW }, // RTLN
#undef ____
};

struct TickPerCycleProperties {
  RodType rodType;
  FwdOrBwd fwdOrBwd;
} tickPerCycleProperties[eoTickPerCycle] = {
  { RTKW, Fwd }, // EUNL
  { RTLS, Bwd }, // ERN1
  { RTLS, Bwd }, // ERNX
  { RTLS, Bwd }, // ERN0
  { RTKW, Bwd }, // ELNL
  { RTKS, Fwd }, // EUCL
  { RTLE, Fwd }, // ESC0
  { RTLE, Fwd }, // ESCX
  { RTLE, Fwd }, // ESC1
  { RTKS, Bwd }, // ELCL
  { RTKS, Fwd }, // SUNL
  { RTLW, Bwd }, // SRN1
  { RTLW, Bwd }, // SRNX
  { RTLW, Bwd }, // SRN0
  { RTKS, Bwd }, // SLNL
  { RTKW, Fwd }, // SUCL
  { RTLS, Fwd }, // SSC0
  { RTLS, Fwd }, // SSCX
  { RTLS, Fwd }, // SSC1
  { RTKW, Bwd }, // SLCL
  { RTKE, Fwd }, // WUNL
  { RTLN, Bwd }, // WRN1
  { RTLN, Bwd }, // WRNX
  { RTLN, Bwd }, // WRN0
  { RTKE, Bwd }, // WLNL
  { RTKN, Fwd }, // WUCL
  { RTLW, Fwd }, // WSC0
  { RTLW, Fwd }, // WSCX
  { RTLW, Fwd }, // WSC1
  { RTKN, Bwd }, // WLCL
  { RTKN, Fwd }, // NUNL
  { RTLE, Bwd }, // NRN1
  { RTLE, Bwd }, // NRNX
  { RTLE, Bwd }, // NRN0
  { RTKN, Bwd }, // NLNL
  { RTKE, Fwd }, // NUCL
  { RTLN, Fwd }, // NSC0
  { RTLN, Fwd }, // NSCX
  { RTLN, Fwd }, // NSC1
  { RTKE, Bwd }  // NLCL
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

  KBEL, KBEU,  KBSL, KBSU,  KBWL, KBWU,  KBNL, KBNU,
  KHEL, KHEU,  KHSL, KHSU,  KHWL, KHWU,  KHNL, KHNU,
  KTEL, KTEU,  KTSL, KTSU,  KTWL, KTWU,  KTNL, KTNU,
  KPEL, KPEU,  KPSL, KPSU,  KPWL, KPWU,  KPNL, KPNU,
  KKEL, KKEU,  KKSL, KKSU,  KKWL, KKWU,  KKNL, KKNU,

  LBER, LBE0, LBEI, LBE1,  LBSR, LBS0, LBSI, LBS1,
  LBWR, LBW0, LBWI, LBW1,  LBNR, LBN0, LBNI, LBN1,
  LHER, LHE0, LHEI, LHE1,  LHSR, LHS0, LHSI, LHS1,
  LHWR, LHW0, LHWI, LHW1,  LHNR, LHN0, LHNI, LHN1,
  LTER, LTE0, LTEI, LTE1,  LTSR, LTS0, LTSI, LTS1,
  LTWR, LTW0, LTWI, LTW1,  LTNR, LTN0, LTNI, LTN1,
  LPER, LPE0, LPEI, LPE1,  LPSR, LPS0, LPSI, LPS1,
  LPWR, LPW0, LPWI, LPW1,  LPNR, LPN0, LPNI, LPN1,
  LKER, LKE0, LKEI, LKE1,  LKSR, LKS0, LKSI, LKS1,
  LKWR, LKW0, LKWI, LKW1,  LKNR, LKN0, LKNI, LKN1,
  LQER, LQE0, LQEI, LQE1,  LQSR, LQS0, LQSI, LQS1,
  LQWR, LQW0, LQWI, LQW1,  LQNR, LQN0, LQNI, LQN1,
  LGER, LGE0, LGEI, LGE1,  LGSR, LGS0, LGSI, LGS1,
  LGWR, LGW0, LGWI, LGW1,  LGNR, LGN0, LGNI, LGN1,
  LIER, LIE0, LIEI, LIE1,  LISR, LIS0, LISI, LIS1,
  LIWR, LIW0, LIWI, LIW1,  LINR, LIN0, LINI, LIN1,
  LOER, LOE0, LOEI, LOE1,  LOSR, LOS0, LOSI, LOS1,
  LOWR, LOW0, LOWI, LOW1,  LONR, LON0, LONI, LON1,

  eoVoxel
};

char const *toConstCharPointer(Voxel v) {
  switch (v) {
  case Unkn: return "Unkn";
  case Wall: return "Wall";
  case Slot: return "Slot";
  case KBEL: return "KBEL";
  case KBEU: return "KBEU";
  case KBSL: return "KBSL";
  case KBSU: return "KBSU";
  case KBWL: return "KBWL";
  case KBWU: return "KBWU";
  case KBNL: return "KBNL";
  case KBNU: return "KBNU";
  case KHEL: return "KHEL";
  case KHEU: return "KHEU";
  case KHSL: return "KHSL";
  case KHSU: return "KHSU";
  case KHWL: return "KHWL";
  case KHWU: return "KHWU";
  case KHNL: return "KHNL";
  case KHNU: return "KHNU";
  case KTEL: return "KTEL";
  case KTEU: return "KTEU";
  case KTSL: return "KTSL";
  case KTSU: return "KTSU";
  case KTWL: return "KTWL";
  case KTWU: return "KTWU";
  case KTNL: return "KTNL";
  case KTNU: return "KTNU";
  case KPEL: return "KPEL";
  case KPEU: return "KPEU";
  case KPSL: return "KPSL";
  case KPSU: return "KPSU";
  case KPWL: return "KPWL";
  case KPWU: return "KPWU";
  case KPNL: return "KPNL";
  case KPNU: return "KPNU";
  case KKEL: return "KKEL";
  case KKEU: return "KKEU";
  case KKSL: return "KKSL";
  case KKSU: return "KKSU";
  case KKWL: return "KKWL";
  case KKWU: return "KKWU";
  case KKNL: return "KKNL";
  case KKNU: return "KKNU";
  case LBER: return "LBER";
  case LBE0: return "LBE0";
  case LBEI: return "LBEI";
  case LBE1: return "LBE1";
  case LBSR: return "LBSR";
  case LBS0: return "LBS0";
  case LBSI: return "LBSI";
  case LBS1: return "LBS1";
  case LBWR: return "LBWR";
  case LBW0: return "LBW0";
  case LBWI: return "LBWI";
  case LBW1: return "LBW1";
  case LBNR: return "LBNR";
  case LBN0: return "LBN0";
  case LBNI: return "LBNI";
  case LBN1: return "LBN1";
  case LHER: return "LHER";
  case LHE0: return "LHE0";
  case LHEI: return "LHEI";
  case LHE1: return "LHE1";
  case LHSR: return "LHSR";
  case LHS0: return "LHS0";
  case LHSI: return "LHSI";
  case LHS1: return "LHS1";
  case LHWR: return "LHWR";
  case LHW0: return "LHW0";
  case LHWI: return "LHWI";
  case LHW1: return "LHW1";
  case LHNR: return "LHNR";
  case LHN0: return "LHN0";
  case LHNI: return "LHNI";
  case LHN1: return "LHN1";
  case LTER: return "LTER";
  case LTE0: return "LTE0";
  case LTEI: return "LTEI";
  case LTE1: return "LTE1";
  case LTSR: return "LTSR";
  case LTS0: return "LTS0";
  case LTSI: return "LTSI";
  case LTS1: return "LTS1";
  case LTWR: return "LTWR";
  case LTW0: return "LTW0";
  case LTWI: return "LTWI";
  case LTW1: return "LTW1";
  case LTNR: return "LTNR";
  case LTN0: return "LTN0";
  case LTNI: return "LTNI";
  case LTN1: return "LTN1";
  case LPER: return "LPER";
  case LPE0: return "LPE0";
  case LPEI: return "LPEI";
  case LPE1: return "LPE1";
  case LPSR: return "LPSR";
  case LPS0: return "LPS0";
  case LPSI: return "LPSI";
  case LPS1: return "LPS1";
  case LPWR: return "LPWR";
  case LPW0: return "LPW0";
  case LPWI: return "LPWI";
  case LPW1: return "LPW1";
  case LPNR: return "LPNR";
  case LPN0: return "LPN0";
  case LPNI: return "LPNI";
  case LPN1: return "LPN1";
  case LKER: return "LKER";
  case LKE0: return "LKE0";
  case LKEI: return "LKEI";
  case LKE1: return "LKE1";
  case LKSR: return "LKSR";
  case LKS0: return "LKS0";
  case LKSI: return "LKSI";
  case LKS1: return "LKS1";
  case LKWR: return "LKWR";
  case LKW0: return "LKW0";
  case LKWI: return "LKWI";
  case LKW1: return "LKW1";
  case LKNR: return "LKNR";
  case LKN0: return "LKN0";
  case LKNI: return "LKNI";
  case LKN1: return "LKN1";
  case LQER: return "LQER";
  case LQE0: return "LQE0";
  case LQEI: return "LQEI";
  case LQE1: return "LQE1";
  case LQSR: return "LQSR";
  case LQS0: return "LQS0";
  case LQSI: return "LQSI";
  case LQS1: return "LQS1";
  case LQWR: return "LQWR";
  case LQW0: return "LQW0";
  case LQWI: return "LQWI";
  case LQW1: return "LQW1";
  case LQNR: return "LQNR";
  case LQN0: return "LQN0";
  case LQNI: return "LQNI";
  case LQN1: return "LQN1";
  case LGER: return "LGER";
  case LGE0: return "LGE0";
  case LGEI: return "LGEI";
  case LGE1: return "LGE1";
  case LGSR: return "LGSR";
  case LGS0: return "LGS0";
  case LGSI: return "LGSI";
  case LGS1: return "LGS1";
  case LGWR: return "LGWR";
  case LGW0: return "LGW0";
  case LGWI: return "LGWI";
  case LGW1: return "LGW1";
  case LGNR: return "LGNR";
  case LGN0: return "LGN0";
  case LGNI: return "LGNI";
  case LGN1: return "LGN1";
  case LIER: return "LIER";
  case LIE0: return "LIE0";
  case LIEI: return "LIEI";
  case LIE1: return "LIE1";
  case LISR: return "LISR";
  case LIS0: return "LIS0";
  case LISI: return "LISI";
  case LIS1: return "LIS1";
  case LIWR: return "LIWR";
  case LIW0: return "LIW0";
  case LIWI: return "LIWI";
  case LIW1: return "LIW1";
  case LINR: return "LINR";
  case LIN0: return "LIN0";
  case LINI: return "LINI";
  case LIN1: return "LIN1";
  case LOER: return "LOER";
  case LOE0: return "LOE0";
  case LOEI: return "LOEI";
  case LOE1: return "LOE1";
  case LOSR: return "LOSR";
  case LOS0: return "LOS0";
  case LOSI: return "LOSI";
  case LOS1: return "LOS1";
  case LOWR: return "LOWR";
  case LOW0: return "LOW0";
  case LOWI: return "LOWI";
  case LOW1: return "LOW1";
  case LONR: return "LONR";
  case LON0: return "LON0";
  case LONI: return "LONI";
  case LON1: return "LON1";
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
  { 9,'?',VTUnkn,_,RTUn,KTUnkn,KSUnkn,LTUnkn,LSUnkn,F,___ }, // Unkn
  { 0,'#',VTWall,_,RTUn,KTUnkn,KSUnkn,LTUnkn,LSUnkn,F,___ }, // Wall
  { 1,' ',VTSlot,_,RTUn,KTUnkn,KSUnkn,LTUnkn,LSUnkn,F,___ }, // Slot
  { 2,'-',VTLock,E,RTKE,KTBody,KSLckd,LTUnkn,LSUnkn,T,___ }, // KBEL
  { 2,'-',VTLock,E,RTKE,KTBody,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KBEU
  { 2,'|',VTLock,S,RTKS,KTBody,KSLckd,LTUnkn,LSUnkn,T,___ }, // KBSL
  { 2,'|',VTLock,S,RTKS,KTBody,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KBSU
  { 2,'-',VTLock,W,RTKW,KTBody,KSLckd,LTUnkn,LSUnkn,T,___ }, // KBWL
  { 2,'-',VTLock,W,RTKW,KTBody,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KBWU
  { 2,'|',VTLock,N,RTKN,KTBody,KSLckd,LTUnkn,LSUnkn,T,___ }, // KBNL
  { 2,'|',VTLock,N,RTKN,KTBody,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KBNU
  { 3,'>',VTLock,E,RTKE,KTHead,KSLckd,LTUnkn,LSUnkn,T,___ }, // KHEL
  { 3,'>',VTLock,E,RTKE,KTHead,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KHEU
  { 3,'v',VTLock,S,RTKS,KTHead,KSLckd,LTUnkn,LSUnkn,T,___ }, // KHSL
  { 3,'v',VTLock,S,RTKS,KTHead,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KHSU
  { 3,'<',VTLock,W,RTKW,KTHead,KSLckd,LTUnkn,LSUnkn,T,___ }, // KHWL
  { 3,'<',VTLock,W,RTKW,KTHead,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KHWU
  { 3,'^',VTLock,N,RTKN,KTHead,KSLckd,LTUnkn,LSUnkn,T,___ }, // KHNL
  { 3,'^',VTLock,N,RTKN,KTHead,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KHNU
  { 3,'>',VTLock,E,RTKE,KTTail,KSLckd,LTUnkn,LSUnkn,T,___ }, // KTEL
  { 3,'>',VTLock,E,RTKE,KTTail,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KTEU
  { 3,'v',VTLock,S,RTKS,KTTail,KSLckd,LTUnkn,LSUnkn,T,___ }, // KTSL
  { 3,'v',VTLock,S,RTKS,KTTail,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KTSU
  { 3,'<',VTLock,W,RTKW,KTTail,KSLckd,LTUnkn,LSUnkn,T,___ }, // KTWL
  { 3,'<',VTLock,W,RTKW,KTTail,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KTWU
  { 3,'^',VTLock,N,RTKN,KTTail,KSLckd,LTUnkn,LSUnkn,T,___ }, // KTNL
  { 3,'^',VTLock,N,RTKN,KTTail,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KTNU
  { 2,'+',VTLock,E,RTKE,KTPost,KSLckd,LTUnkn,LSUnkn,T,___ }, // KPEL
  { 2,'+',VTLock,E,RTKE,KTPost,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KPEU
  { 2,'+',VTLock,S,RTKS,KTPost,KSLckd,LTUnkn,LSUnkn,T,___ }, // KPSL
  { 2,'+',VTLock,S,RTKS,KTPost,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KPSU
  { 2,'+',VTLock,W,RTKW,KTPost,KSLckd,LTUnkn,LSUnkn,T,___ }, // KPWL
  { 2,'+',VTLock,W,RTKW,KTPost,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KPWU
  { 2,'+',VTLock,N,RTKN,KTPost,KSLckd,LTUnkn,LSUnkn,T,___ }, // KPNL
  { 2,'+',VTLock,N,RTKN,KTPost,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KPNU
  { 3,'<',VTLock,E,RTKE,KTLock,KSLckd,LTUnkn,LSUnkn,F,FRL }, // KKEL
  { 3,'<',VTLock,E,RTKE,KTLock,KSUnlk,LTUnkn,LSUnkn,F,FRL }, // KKEU
  { 3,'^',VTLock,S,RTKS,KTLock,KSLckd,LTUnkn,LSUnkn,F,FRL }, // KKSL
  { 3,'^',VTLock,S,RTKS,KTLock,KSUnlk,LTUnkn,LSUnkn,F,FRL }, // KKSU
  { 3,'>',VTLock,W,RTKW,KTLock,KSLckd,LTUnkn,LSUnkn,F,FRL }, // KKWL
  { 3,'>',VTLock,W,RTKW,KTLock,KSUnlk,LTUnkn,LSUnkn,F,FRL }, // KKWU
  { 3,'v',VTLock,N,RTKN,KTLock,KSLckd,LTUnkn,LSUnkn,F,FRL }, // KKNL
  { 3,'v',VTLock,N,RTKN,KTLock,KSUnlk,LTUnkn,LSUnkn,F,FRL }, // KKNU
  { 4,'-',VTData,E,RTLE,KTUnkn,KSUnkn,LTBody,LSRset,T,___ }, // LBER
  { 4,'-',VTData,E,RTLE,KTUnkn,KSUnkn,LTBody,LSSet0,T,___ }, // LBE0
  { 4,'-',VTData,E,RTLE,KTUnkn,KSUnkn,LTBody,LSXXXX,T,___ }, // LBEI
  { 4,'-',VTData,E,RTLE,KTUnkn,KSUnkn,LTBody,LSSet1,T,___ }, // LBE1
  { 4,'|',VTData,S,RTLS,KTUnkn,KSUnkn,LTBody,LSRset,T,___ }, // LBSR
  { 4,'|',VTData,S,RTLS,KTUnkn,KSUnkn,LTBody,LSSet0,T,___ }, // LBS0
  { 4,'|',VTData,S,RTLS,KTUnkn,KSUnkn,LTBody,LSXXXX,T,___ }, // LBSI
  { 4,'|',VTData,S,RTLS,KTUnkn,KSUnkn,LTBody,LSSet1,T,___ }, // LBS1
  { 4,'-',VTData,W,RTLW,KTUnkn,KSUnkn,LTBody,LSRset,T,___ }, // LBWR
  { 4,'-',VTData,W,RTLW,KTUnkn,KSUnkn,LTBody,LSSet0,T,___ }, // LBW0
  { 4,'-',VTData,W,RTLW,KTUnkn,KSUnkn,LTBody,LSXXXX,T,___ }, // LBWI
  { 4,'-',VTData,W,RTLW,KTUnkn,KSUnkn,LTBody,LSSet1,T,___ }, // LBW1
  { 4,'|',VTData,N,RTLN,KTUnkn,KSUnkn,LTBody,LSRset,T,___ }, // LBNR
  { 4,'|',VTData,N,RTLN,KTUnkn,KSUnkn,LTBody,LSSet0,T,___ }, // LBN0
  { 4,'|',VTData,N,RTLN,KTUnkn,KSUnkn,LTBody,LSXXXX,T,___ }, // LBNI
  { 4,'|',VTData,N,RTLN,KTUnkn,KSUnkn,LTBody,LSSet1,T,___ }, // LBN1
  { 5,'>',VTData,E,RTLE,KTUnkn,KSUnkn,LTHead,LSRset,T,F__ }, // LHER
  { 5,'>',VTData,E,RTLE,KTUnkn,KSUnkn,LTHead,LSSet0,T,F__ }, // LHE0
  { 5,'>',VTData,E,RTLE,KTUnkn,KSUnkn,LTHead,LSXXXX,T,F__ }, // LHEI
  { 5,'>',VTData,E,RTLE,KTUnkn,KSUnkn,LTHead,LSSet1,T,F__ }, // LHE1
  { 5,'v',VTData,S,RTLS,KTUnkn,KSUnkn,LTHead,LSRset,T,F__ }, // LHSR
  { 5,'v',VTData,S,RTLS,KTUnkn,KSUnkn,LTHead,LSSet0,T,F__ }, // LHS0
  { 5,'v',VTData,S,RTLS,KTUnkn,KSUnkn,LTHead,LSXXXX,T,F__ }, // LHSI
  { 5,'v',VTData,S,RTLS,KTUnkn,KSUnkn,LTHead,LSSet1,T,F__ }, // LHS1
  { 5,'<',VTData,W,RTLW,KTUnkn,KSUnkn,LTHead,LSRset,T,F__ }, // LHWR
  { 5,'<',VTData,W,RTLW,KTUnkn,KSUnkn,LTHead,LSSet0,T,F__ }, // LHW0
  { 5,'<',VTData,W,RTLW,KTUnkn,KSUnkn,LTHead,LSXXXX,T,F__ }, // LHWI
  { 5,'<',VTData,W,RTLW,KTUnkn,KSUnkn,LTHead,LSSet1,T,F__ }, // LHW1
  { 5,'^',VTData,N,RTLN,KTUnkn,KSUnkn,LTHead,LSRset,T,F__ }, // LHNR
  { 5,'^',VTData,N,RTLN,KTUnkn,KSUnkn,LTHead,LSSet0,T,F__ }, // LHN0
  { 5,'^',VTData,N,RTLN,KTUnkn,KSUnkn,LTHead,LSXXXX,T,F__ }, // LHNI
  { 5,'^',VTData,N,RTLN,KTUnkn,KSUnkn,LTHead,LSSet1,T,F__ }, // LHN1
  { 5,'>',VTData,E,RTLE,KTUnkn,KSUnkn,LTTail,LSRset,T,_R_ }, // LTER
  { 5,'>',VTData,E,RTLE,KTUnkn,KSUnkn,LTTail,LSSet0,T,_R_ }, // LTE0
  { 5,'>',VTData,E,RTLE,KTUnkn,KSUnkn,LTTail,LSXXXX,T,_R_ }, // LTEI
  { 5,'>',VTData,E,RTLE,KTUnkn,KSUnkn,LTTail,LSSet1,T,_R_ }, // LTE1
  { 5,'v',VTData,S,RTLS,KTUnkn,KSUnkn,LTTail,LSRset,T,_R_ }, // LTSR
  { 5,'v',VTData,S,RTLS,KTUnkn,KSUnkn,LTTail,LSSet0,T,_R_ }, // LTS0
  { 5,'v',VTData,S,RTLS,KTUnkn,KSUnkn,LTTail,LSXXXX,T,_R_ }, // LTSI
  { 5,'v',VTData,S,RTLS,KTUnkn,KSUnkn,LTTail,LSSet1,T,_R_ }, // LTS1
  { 5,'<',VTData,W,RTLW,KTUnkn,KSUnkn,LTTail,LSRset,T,_R_ }, // LTWR
  { 5,'<',VTData,W,RTLW,KTUnkn,KSUnkn,LTTail,LSSet0,T,_R_ }, // LTW0
  { 5,'<',VTData,W,RTLW,KTUnkn,KSUnkn,LTTail,LSXXXX,T,_R_ }, // LTWI
  { 5,'<',VTData,W,RTLW,KTUnkn,KSUnkn,LTTail,LSSet1,T,_R_ }, // LTW1
  { 5,'^',VTData,N,RTLN,KTUnkn,KSUnkn,LTTail,LSRset,T,_R_ }, // LTNR
  { 5,'^',VTData,N,RTLN,KTUnkn,KSUnkn,LTTail,LSSet0,T,_R_ }, // LTN0
  { 5,'^',VTData,N,RTLN,KTUnkn,KSUnkn,LTTail,LSXXXX,T,_R_ }, // LTNI
  { 5,'^',VTData,N,RTLN,KTUnkn,KSUnkn,LTTail,LSSet1,T,_R_ }, // LTN1
  { 4,'+',VTData,E,RTLE,KTUnkn,KSUnkn,LTPost,LSRset,T,FR_ }, // LPER
  { 4,'+',VTData,E,RTLE,KTUnkn,KSUnkn,LTPost,LSSet0,T,FR_ }, // LPE0
  { 4,'+',VTData,E,RTLE,KTUnkn,KSUnkn,LTPost,LSXXXX,T,FR_ }, // LPEI
  { 4,'+',VTData,E,RTLE,KTUnkn,KSUnkn,LTPost,LSSet1,T,FR_ }, // LPE1
  { 4,'+',VTData,S,RTLS,KTUnkn,KSUnkn,LTPost,LSRset,T,FR_ }, // LPSR
  { 4,'+',VTData,S,RTLS,KTUnkn,KSUnkn,LTPost,LSSet0,T,FR_ }, // LPS0
  { 4,'+',VTData,S,RTLS,KTUnkn,KSUnkn,LTPost,LSXXXX,T,FR_ }, // LPSI
  { 4,'+',VTData,S,RTLS,KTUnkn,KSUnkn,LTPost,LSSet1,T,FR_ }, // LPS1
  { 4,'+',VTData,W,RTLW,KTUnkn,KSUnkn,LTPost,LSRset,T,FR_ }, // LPWR
  { 4,'+',VTData,W,RTLW,KTUnkn,KSUnkn,LTPost,LSSet0,T,FR_ }, // LPW0
  { 4,'+',VTData,W,RTLW,KTUnkn,KSUnkn,LTPost,LSXXXX,T,FR_ }, // LPWI
  { 4,'+',VTData,W,RTLW,KTUnkn,KSUnkn,LTPost,LSSet1,T,FR_ }, // LPW1
  { 4,'+',VTData,N,RTLN,KTUnkn,KSUnkn,LTPost,LSRset,T,FR_ }, // LPNR
  { 4,'+',VTData,N,RTLN,KTUnkn,KSUnkn,LTPost,LSSet0,T,FR_ }, // LPN0
  { 4,'+',VTData,N,RTLN,KTUnkn,KSUnkn,LTPost,LSXXXX,T,FR_ }, // LPNI
  { 4,'+',VTData,N,RTLN,KTUnkn,KSUnkn,LTPost,LSSet1,T,FR_ }, // LPN1
  { 5,'^',VTData,E,RTLE,KTUnkn,KSUnkn,LTLock,LSRset,F,FRL }, // LKER
  { 5,'^',VTData,E,RTLE,KTUnkn,KSUnkn,LTLock,LSSet0,F,FRL }, // LKE0
  { 5,'^',VTData,E,RTLE,KTUnkn,KSUnkn,LTLock,LSXXXX,F,FRL }, // LKEI
  { 5,'^',VTData,E,RTLE,KTUnkn,KSUnkn,LTLock,LSSet1,F,FRL }, // LKE1
  { 5,'>',VTData,S,RTLS,KTUnkn,KSUnkn,LTLock,LSRset,F,FRL }, // LKSR
  { 5,'>',VTData,S,RTLS,KTUnkn,KSUnkn,LTLock,LSSet0,F,FRL }, // LKS0
  { 5,'>',VTData,S,RTLS,KTUnkn,KSUnkn,LTLock,LSXXXX,F,FRL }, // LKSI
  { 5,'>',VTData,S,RTLS,KTUnkn,KSUnkn,LTLock,LSSet1,F,FRL }, // LKS1
  { 5,'v',VTData,W,RTLW,KTUnkn,KSUnkn,LTLock,LSRset,F,FRL }, // LKWR
  { 5,'v',VTData,W,RTLW,KTUnkn,KSUnkn,LTLock,LSSet0,F,FRL }, // LKW0
  { 5,'v',VTData,W,RTLW,KTUnkn,KSUnkn,LTLock,LSXXXX,F,FRL }, // LKWI
  { 5,'v',VTData,W,RTLW,KTUnkn,KSUnkn,LTLock,LSSet1,F,FRL }, // LKW1
  { 5,'<',VTData,N,RTLN,KTUnkn,KSUnkn,LTLock,LSRset,F,FRL }, // LKNR
  { 5,'<',VTData,N,RTLN,KTUnkn,KSUnkn,LTLock,LSSet0,F,FRL }, // LKN0
  { 5,'<',VTData,N,RTLN,KTUnkn,KSUnkn,LTLock,LSXXXX,F,FRL }, // LKNI
  { 5,'<',VTData,N,RTLN,KTUnkn,KSUnkn,LTLock,LSSet1,F,FRL }, // LKN1
  { 5,'>',VTData,E,RTLE,KTUnkn,KSUnkn,LTTest,LSRset,F,FR_ }, // LQER
  { 5,'>',VTData,E,RTLE,KTUnkn,KSUnkn,LTTest,LSSet0,F,FR_ }, // LQE0
  { 5,'>',VTData,E,RTLE,KTUnkn,KSUnkn,LTTest,LSXXXX,F,FR_ }, // LQEI
  { 5,'>',VTData,E,RTLE,KTUnkn,KSUnkn,LTTest,LSSet1,F,FR_ }, // LQE1
  { 5,'v',VTData,S,RTLS,KTUnkn,KSUnkn,LTTest,LSRset,F,FR_ }, // LQSR
  { 5,'v',VTData,S,RTLS,KTUnkn,KSUnkn,LTTest,LSSet0,F,FR_ }, // LQS0
  { 5,'v',VTData,S,RTLS,KTUnkn,KSUnkn,LTTest,LSXXXX,F,FR_ }, // LQSI
  { 5,'v',VTData,S,RTLS,KTUnkn,KSUnkn,LTTest,LSSet1,F,FR_ }, // LQS1
  { 5,'<',VTData,W,RTLW,KTUnkn,KSUnkn,LTTest,LSRset,F,FR_ }, // LQWR
  { 5,'<',VTData,W,RTLW,KTUnkn,KSUnkn,LTTest,LSSet0,F,FR_ }, // LQW0
  { 5,'<',VTData,W,RTLW,KTUnkn,KSUnkn,LTTest,LSXXXX,F,FR_ }, // LQWI
  { 5,'<',VTData,W,RTLW,KTUnkn,KSUnkn,LTTest,LSSet1,F,FR_ }, // LQW1
  { 5,'^',VTData,N,RTLN,KTUnkn,KSUnkn,LTTest,LSRset,F,FR_ }, // LQNR
  { 5,'^',VTData,N,RTLN,KTUnkn,KSUnkn,LTTest,LSSet0,F,FR_ }, // LQN0
  { 5,'^',VTData,N,RTLN,KTUnkn,KSUnkn,LTTest,LSXXXX,F,FR_ }, // LQNI
  { 5,'^',VTData,N,RTLN,KTUnkn,KSUnkn,LTTest,LSSet1,F,FR_ }, // LQN1
  { 5,'@',VTData,E,RTLE,KTUnkn,KSUnkn,LTGate,LSRset,F,FR_ }, // LGER
  { 5,'@',VTData,E,RTLE,KTUnkn,KSUnkn,LTGate,LSSet0,F,FR_ }, // LGE0
  { 5,'@',VTData,E,RTLE,KTUnkn,KSUnkn,LTGate,LSXXXX,F,FR_ }, // LGEI
  { 5,'@',VTData,E,RTLE,KTUnkn,KSUnkn,LTGate,LSSet1,F,FR_ }, // LGE1
  { 5,'@',VTData,S,RTLS,KTUnkn,KSUnkn,LTGate,LSRset,F,FR_ }, // LGSR
  { 5,'@',VTData,S,RTLS,KTUnkn,KSUnkn,LTGate,LSSet0,F,FR_ }, // LGS0
  { 5,'@',VTData,S,RTLS,KTUnkn,KSUnkn,LTGate,LSXXXX,F,FR_ }, // LGSI
  { 5,'@',VTData,S,RTLS,KTUnkn,KSUnkn,LTGate,LSSet1,F,FR_ }, // LGS1
  { 5,'@',VTData,W,RTLW,KTUnkn,KSUnkn,LTGate,LSRset,F,FR_ }, // LGWR
  { 5,'@',VTData,W,RTLW,KTUnkn,KSUnkn,LTGate,LSSet0,F,FR_ }, // LGW0
  { 5,'@',VTData,W,RTLW,KTUnkn,KSUnkn,LTGate,LSXXXX,F,FR_ }, // LGWI
  { 5,'@',VTData,W,RTLW,KTUnkn,KSUnkn,LTGate,LSSet1,F,FR_ }, // LGW1
  { 5,'@',VTData,N,RTLN,KTUnkn,KSUnkn,LTGate,LSRset,F,FR_ }, // LGNR
  { 5,'@',VTData,N,RTLN,KTUnkn,KSUnkn,LTGate,LSSet0,F,FR_ }, // LGN0
  { 5,'@',VTData,N,RTLN,KTUnkn,KSUnkn,LTGate,LSXXXX,F,FR_ }, // LGNI
  { 5,'@',VTData,N,RTLN,KTUnkn,KSUnkn,LTGate,LSSet1,F,FR_ }, // LGN1
  { 6,'X',VTData,E,RTLE,KTUnkn,KSUnkn,LTInpt,LSRset,T,___ }, // LIER
  { 6,'0',VTData,E,RTLE,KTUnkn,KSUnkn,LTInpt,LSSet0,T,___ }, // LIE0
  { 6,'X',VTData,E,RTLE,KTUnkn,KSUnkn,LTInpt,LSXXXX,T,___ }, // LIEI
  { 6,'1',VTData,E,RTLE,KTUnkn,KSUnkn,LTInpt,LSSet1,T,___ }, // LIE1
  { 6,'X',VTData,S,RTLS,KTUnkn,KSUnkn,LTInpt,LSRset,T,___ }, // LISR
  { 6,'0',VTData,S,RTLS,KTUnkn,KSUnkn,LTInpt,LSSet0,T,___ }, // LIS0
  { 6,'X',VTData,S,RTLS,KTUnkn,KSUnkn,LTInpt,LSXXXX,T,___ }, // LISI
  { 6,'1',VTData,S,RTLS,KTUnkn,KSUnkn,LTInpt,LSSet1,T,___ }, // LIS1
  { 6,'X',VTData,W,RTLW,KTUnkn,KSUnkn,LTInpt,LSRset,T,___ }, // LIWR
  { 6,'0',VTData,W,RTLW,KTUnkn,KSUnkn,LTInpt,LSSet0,T,___ }, // LIW0
  { 6,'X',VTData,W,RTLW,KTUnkn,KSUnkn,LTInpt,LSXXXX,T,___ }, // LIWI
  { 6,'1',VTData,W,RTLW,KTUnkn,KSUnkn,LTInpt,LSSet1,T,___ }, // LIW1
  { 6,'X',VTData,N,RTLN,KTUnkn,KSUnkn,LTInpt,LSRset,T,___ }, // LINR
  { 6,'0',VTData,N,RTLN,KTUnkn,KSUnkn,LTInpt,LSSet0,T,___ }, // LIN0
  { 6,'X',VTData,N,RTLN,KTUnkn,KSUnkn,LTInpt,LSXXXX,T,___ }, // LINI
  { 6,'1',VTData,N,RTLN,KTUnkn,KSUnkn,LTInpt,LSSet1,T,___ }, // LIN1
  { 6,'O',VTData,E,RTLE,KTUnkn,KSUnkn,LTOutp,LSRset,T,___ }, // LOER
  { 6,'0',VTData,E,RTLE,KTUnkn,KSUnkn,LTOutp,LSSet0,T,___ }, // LOE0
  { 6,'X',VTData,E,RTLE,KTUnkn,KSUnkn,LTOutp,LSXXXX,T,___ }, // LOEI
  { 6,'1',VTData,E,RTLE,KTUnkn,KSUnkn,LTOutp,LSSet1,T,___ }, // LOE1
  { 6,'O',VTData,S,RTLS,KTUnkn,KSUnkn,LTOutp,LSRset,T,___ }, // LOSR
  { 6,'0',VTData,S,RTLS,KTUnkn,KSUnkn,LTOutp,LSSet0,T,___ }, // LOS0
  { 6,'X',VTData,S,RTLS,KTUnkn,KSUnkn,LTOutp,LSXXXX,T,___ }, // LOSI
  { 6,'1',VTData,S,RTLS,KTUnkn,KSUnkn,LTOutp,LSSet1,T,___ }, // LOS1
  { 6,'O',VTData,W,RTLW,KTUnkn,KSUnkn,LTOutp,LSRset,T,___ }, // LOWR
  { 6,'0',VTData,W,RTLW,KTUnkn,KSUnkn,LTOutp,LSSet0,T,___ }, // LOW0
  { 6,'X',VTData,W,RTLW,KTUnkn,KSUnkn,LTOutp,LSXXXX,T,___ }, // LOWI
  { 6,'1',VTData,W,RTLW,KTUnkn,KSUnkn,LTOutp,LSSet1,T,___ }, // LOW1
  { 6,'O',VTData,N,RTLN,KTUnkn,KSUnkn,LTOutp,LSRset,T,___ }, // LONR
  { 6,'0',VTData,N,RTLN,KTUnkn,KSUnkn,LTOutp,LSSet0,T,___ }, // LON0
  { 6,'X',VTData,N,RTLN,KTUnkn,KSUnkn,LTOutp,LSXXXX,T,___ }, // LONI
  { 6,'1',VTData,N,RTLN,KTUnkn,KSUnkn,LTOutp,LSSet1,T,___ }  // LON1
#undef _
#undef T
#undef F
};

class Scenario {
  friend class Rule;
 public:
  Scenario(TickPerCycle _tick, Voxel _self, Voxel _n, Voxel _w, Voxel _e, Voxel _s) :
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
  Scenario(Volume const *volume,
           TickPerCycle t,
           VoxelCoordinant const &v
          );
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
            "{{ %s, %s, { %s, %s, %s, %s }}, %s }",
            toConstCharPointer(scenario.tick),
            toConstCharPointer(scenario.self),
            toConstCharPointer(scenario.area[0]),
            toConstCharPointer(scenario.area[1]),
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


  virtual bool IsFBlked() const { return fBlkState == FBBlkd; }
  virtual bool IsRBlked() const { return rBlkState == RBBlkd; }
  virtual bool IsBlocked() const { return IsFBlked() || IsRBlked(); };
  bool CheckForBlockages(Volume const *volume, FwdOrBwd fwdOrBwd);
  virtual bool CheckForBlockages(Volume const *volume);
  virtual bool AttemptToMove(Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes);
  virtual bool IsValid(Volume const *volume);
  virtual RodType GetRodType() const { return rodType; }
  virtual void Dump(Volume const *volume = 0) const;

 protected:
  RodType rodType;
  int lastMovedAt;
  FBlkState fBlkState;
  RBlkState rBlkState;
};

class LockRod: public Item {
 public:
  LockRod(RodType t): Item(t), lockState(KSUnkn) {}

  bool IsLocked() const { return lockState == KSLckd; }

  bool IsValid(Volume const *volume);
  void Dump(Volume const *volume = 0) const;

 private:
  LockState lockState;
};

class DataRod: public Item {
 public:
  DataRod(RodType t): Item(t), lockState(KSUnkn), dataState(LSUnkn) {}

  bool IsLocked() const { return lockState == KSLckd; }

  bool IsFBlked() const { return Item::IsFBlked() || IsLocked(); }
  bool IsRBlked() const { return Item::IsRBlked() || IsLocked(); }
  bool IsBlocked() const { return Item::IsBlocked() || IsLocked(); };
  bool CheckForBlockages(Volume const *volume);
  bool IsValid(Volume const *volume);
  void Dump(Volume const *volume = 0) const;

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
  area[0] = volume->voxelAt(v.To(N));
  area[1] = volume->voxelAt(v.To(W));
  area[2] = volume->voxelAt(v.To(E));
  area[3] = volume->voxelAt(v.To(S));
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

Item *Volume::FormRodContaining
  (set<VoxelCoordinant> &seenSofar, VoxelCoordinant v)
{
  VoxelProperties const &vProperties = voxelProperties[voxelAt(v)];
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
  item->insert(v);
  PhaseProperties const &pProperties =
      phaseProperties[vProperties.direction];
  bool reachedFwdEnd = false;
  for (VoxelCoordinant f = v.To(pProperties.fwd);
       isPartOf(vProperties.rodType, f, reachedFwdEnd);
       f.Move(pProperties.fwd)
      )
  {
    item->insert(f);
    VoxelCoordinant u = f.ToU();
    if (isPartOf(vProperties.rodType, u)) {
      item->insert(u);

      bool reachedUFwdEnd = false;
      for (VoxelCoordinant uf = u.To(pProperties.fwd);
           isPartOf(vProperties.rodType, uf, reachedUFwdEnd);
           uf.Move(pProperties.fwd)
          )
      {
        item->insert(uf);
      }
    }
    VoxelCoordinant d = f.ToD();
    if (isPartOf(vProperties.rodType, d)) {
      item->insert(d);

      bool reachedDFwdEnd = false;
      for (VoxelCoordinant df = d.To(pProperties.fwd);
           isPartOf(vProperties.rodType, df, reachedDFwdEnd);
           df.Move(pProperties.fwd)
          )
      {
        item->insert(df);
      }
    }
  }
  bool reachedBwdEnd = false;
  for (VoxelCoordinant b = v.To(pProperties.bwd);
       isPartOf(vProperties.rodType, b, reachedBwdEnd);
       b.Move(pProperties.bwd)
      )
  {
    item->insert(b);
    VoxelCoordinant u = b.ToU();
    if (isPartOf(vProperties.rodType, u)) {
      item->insert(u);

      bool reachedUBwdEnd = false;
      for (VoxelCoordinant ub = u.To(pProperties.bwd);
           isPartOf(vProperties.rodType, ub, reachedUBwdEnd);
           ub.Move(pProperties.bwd)
          )
      {
        item->insert(ub);
      }
    }
    VoxelCoordinant d = b.ToD();
    if (isPartOf(vProperties.rodType, d)) {
      item->insert(d);

      bool reachedDBwdEnd = false;
      for (VoxelCoordinant db = d.To(pProperties.bwd);
           isPartOf(vProperties.rodType, db, reachedDBwdEnd);
           db.Move(pProperties.bwd)
          )
      {
        item->insert(db);
      }
    }
  }
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

        if (seenSofar.find(v) == seenSofar.end()) {
          if (voxelProperties[voxelAt(v)].isRodBody) {
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

bool Item::CheckForBlockages(Volume const *volume, FwdOrBwd fwdOrBwd) {
  return CheckForBlockages(volume) &&
      fwdOrBwd == Fwd ? IsFBlked() : IsRBlked();
}

bool Item::CheckForBlockages(Volume const *volume) {
  fBlkState = FBUnkn;
  rBlkState = RBUnkn;

  size_t fBlkCounts = 0;
  size_t rBlkCounts = 0;
  for (auto const &v : *this) {
    VoxelProperties const &vProperties = voxelProperties[volume->voxelAt(v)];
    if (blockStateProperties[vProperties.blockableStates].isFBlkable) {
      VoxelCoordinant f = v.To(rodTypeProperties[rodType].fwd);
      if (volume->isVoxelCoordinantInBounds(f)) {
        if (find(f) == end() && volume->voxelAt(f) != Slot) {
          fprintf(stderr,
                  "(Item *)(%p) fBlked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
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
                "(Item *)(%p) fBlked at %s(%d,%d,%d) by edge\n",
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
                  "(Item *)(%p) rBlked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
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
                "(Item *)(%p) rBlked at %s(%d,%d,%d) by edge\n",
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

bool Item::AttemptToMove(Volume *volume, FwdOrBwd fwdOrBwd, Changes &changes) {
  if (CheckForBlockages(volume, fwdOrBwd)) {
    TickPerCycle tick = volume->CurrentTick();
    Rules &rules = volume->GetRules();
    bool madeChanges = false;
    for (auto const &v : *this) {
      Scenario scenario(volume, tick, v);
      if (rules.find(scenario) == rules.end()) {
        rules[scenario] = Unkn;

        Rule rule(scenario, Unkn);
        rule.Dump();
        fprintf(stderr, "\n");
      }
      Voxel newVoxel = rules[scenario];

      fprintf(stderr, "(Item *)(%p)->AttemptToMove(): At ", this);
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
            "(Item *)(%p)->IsValid(volume=%p): foundMoreThan1RodType!",
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
    fprintf(stderr, "%c ", *comma);
    c.Dump(volume);
    comma = ",";
  }
  fprintf(stderr, " }");
  fprintf(stderr, " }");
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

bool DataRod::CheckForBlockages(Volume const *volume) {
  fBlkState = FBUnkn;
  rBlkState = RBUnkn;
  lockState = KSUnkn;

  size_t fBlkCounts = 0;
  size_t rBlkCounts = 0;
  size_t lockCounts = 0;
  for (auto const &v : *this) {
    VoxelProperties const &vProperties = voxelProperties[volume->voxelAt(v)];
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
                  "(Item *)(%p) rBlked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
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
      VoxelCoordinant r = v.To(rodTypeProperties[rodType].fwd);
      if (volume->isVoxelCoordinantInBounds(r)) {
        if (volume->voxelAt(r) != Slot) {
          fprintf(stderr,
                  "(Item *)(%p) locked at %s(%d,%d,%d) by %s(%d,%d,%d)\n",
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
      } else {
        fprintf(stderr,
                "(DataRod *)(%p) locked at %s(%d,%d,%d) by edge\n",
                this,
                toConstCharPointer(volume->voxelAt(v)),
                v.L(),
                v.R(),
                v.C()
                );
        lockCounts += 1;
      }
    }
  }

  fBlkState = fBlkCounts != 0 ? FBBlkd : FBUnbk;
  rBlkState = rBlkCounts != 0 ? RBBlkd : RBUnbk;
  lockState = lockCounts != 0 ? KSLckd : KSUnlk;
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
  Wall,Wall,Wall,Wall,KTSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall, //  0
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KHNL,Wall,Wall,Wall,Wall, //  1
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, //  2
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, //  3
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, //  4
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, //  5
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, //  6
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, //  7
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, //  8
  Wall,Wall,Wall,Wall,KPSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, //  9
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, // 10
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KPNL,Wall,Wall,Wall,Wall, // 11
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, // 12
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, // 13
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, // 14
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, // 15
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, // 16
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, // 17
  Wall,Wall,Wall,Wall,KBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, // 18
  Wall,Wall,Wall,Wall,KHSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KBNL,Wall,Wall,Wall,Wall, // 19
  Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KTNL,Wall,Wall,Wall,Wall, // 20

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
  // 1 -A,C} Keys/Locks
  // 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  0
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  1
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  2
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  3
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  4
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  5
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  6
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  7
  Wall,LKER,Slot,LKER,LKER,Slot,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  8
  Wall,Wall,Wall,Wall,KKSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  9
  Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall, // 10
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,KKNL,Wall,Wall,Wall,Wall, // 11
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,Slot,LKWR,LKWR,Slot,LKWR,Wall, // 12
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
  // .^-^^-->-0----------O  8
  // .....................  9
  // ..................... 10
  // ..................... 11
  // ....   <-1---<--vv-v. 12
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
  Wall,LPER,LBER,LPER,LPER,LBER,LBER,LPER,LBER,LPER,LBER,LBER,LBER,LBER,LBER,LBER,LBER,LBER,LBER,LBER,LOER, //  8
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  9
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 10
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 11
  Wall,Wall,Wall,Wall,Slot,Slot,Slot,LHWR,LBWR,LPWR,LBWR,LBWR,LBWR,LPWR,LBWR,LBWR,LPWR,LPWR,LBWR,LPWR,Wall, // 12
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
  // ........ ............  6
  // ........ ...v........  7
  // .......> 0     ......  8
  // ........1... ........  9
  // ........ ... ........ 10
  // ........ ...1........ 11
  // ......   1   <....... 12
  // ........^... ........ 13
  // ............ ........ 14
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
  Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  6
  Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,LQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  7
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,LQER,Slot,LGER,Slot,Slot,Slot,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall, //  8
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LGNR,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  9
  Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 10
  Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,LGSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 11
  Wall,Wall,Wall,Wall,Slot,Slot,Slot,Slot,Slot,LGWR,Slot,LQWR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 12
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LQNR,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 13
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 14
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
  // ........ ...|........  6
  // ........^...v........  7
  // ........|...|........  8
  // ........1...|........  9
  // ........|...|........ 10
  // ........|...1........ 11
  // ........|...|........ 12
  // ........^...v........ 13
  // ........|... ........ 14
  // ........|... ........ 15
  // ........<... ........ 16
  // ........<............ 17
  // ........|............ 18
  // ........<............ 19
  // ..................... 20
  // 4 -B,D} Data Rods
  // 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  0
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  1
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  2
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  3
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,LPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  4
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,LBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  5
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,LBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  6
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LHNR,Wall,Wall,Wall,LPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  7
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNR,Wall,Wall,Wall,LBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  8
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LPNR,Wall,Wall,Wall,LBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  9
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNR,Wall,Wall,Wall,LBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 10
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNR,Wall,Wall,Wall,LPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 11
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNR,Wall,Wall,Wall,LBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 12
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LPNR,Wall,Wall,Wall,LHSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 13
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNR,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 14
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNR,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 15
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LPNR,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 16
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LPNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 17
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 18
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LPNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 19
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
  // 5 -B,D} Keys/Locks
  // 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  0
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LKSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  1
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  2
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LKSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  3
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,KKWL,LKSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  4
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
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LKNR,KKEL,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 16
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LKNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 17
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 18
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LKNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 19
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
  // 6 -B,D} Lock Rods
  // 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  0
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  1
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  2
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, //  3
  Slot,KHWL,KBWL,KBWL,KBWL,KBWL,KBWL,KBWL,KBWL,KBWL,KBWL,KPWL,KBWL,KBWL,KBWL,KBWL,KBWL,KBWL,KBWL,KBWL,KTWL, //  4
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
  KTEL,KBEL,KBEL,KBEL,KBEL,KBEL,KBEL,KBEL,KBEL,KPEL,KBEL,KBEL,KBEL,KBEL,KBEL,KBEL,KBEL,KBEL,KBEL,KHEL,Slot, // 16
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 17
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 18
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, // 19
  Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall  // 20
};

int main(int argc, char const *argv[]) {
  Rule rules[] = {
    { EUNL, Slot, Wall, Wall, KKWL, Wall, KKWU },
    { EUNL, KKWL, Wall, Slot, LKSR, Wall, Slot },
    { EUNL, Slot, Wall, Wall, KHWL, Wall, KHWU },
    { EUNL, KHWL, Wall, Slot, KBWL, Wall, KBWU },
    { EUNL, KBWL, Wall, KHWL, KBWL, Wall, KBWU },
    { EUNL, KBWL, Wall, KBWL, KBWL, Wall, KBWU },
    { EUNL, KBWL, Wall, KBWL, KPWL, Wall, KPWU },
    { EUNL, KPWL, Wall, KBWL, KBWL, Wall, KBWU },
    { EUNL, KBWL, Wall, KPWL, KBWL, Wall, KBWU },
    { EUNL, KBWL, Wall, KBWL, KTWL, Wall, KTWU },
    { EUNL, KTWL, Wall, KBWL, Wall, Wall, Slot },
  };

  Volume volume(initialVolume);
  for (size_t r = 0; r < (sizeof(rules) / sizeof(rules[0])); r += 1) {
    volume.AddRule(rules[r]);
  }

  volume.PrintViewFlat();
  volume.Tick();
  volume.PrintViewFlat();
  return 0;
}
