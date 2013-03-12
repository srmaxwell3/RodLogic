#include <cstddef>
#include <cstdio>
#include <array>
using std::array;
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

  eoDirection
};

char const *toConstCharPointer(Direction d) {
  switch (d) {
    case DirE: return "DirE";
    case DirS: return "DirS";
    case DirD: return "DirD";
    case DirW: return "DirW";
    case DirN: return "DirN";
    case DirU: return "DirU";
  }
  return "Direction(?)";
}

enum MajorTickPerCycle {
  E,
  S,
  W,
  N,

  eoMajorTickPerCycle,
  Unknown = eoMajorTickPerCycle
};

char const *toConstCharPointer(MajorTickPerCycle t) {
  switch (t) {
    case E: return "E";
    case S: return "S";
    case W: return "W";
    case N: return "N";
    case Unknown: return "Unknown";
  }
  return "MajorTickPerCycle(?)";
}

struct MajorTickProperties {
  MajorTickPerCycle fwd;  // this major cycle direction forward (identity)
  MajorTickPerCycle bwd;  // this major cycle direction backward
  MajorTickPerCycle nxt;  // the next major cycle's forward direction
  MajorTickPerCycle lst;  // the last major cycle's forward direction
} majorTickProperties[eoMajorTickPerCycle] = {
  { E, W, S, N },  // E
  { S, N, W, E },  // S
  { W, E, N, S },  // W
  { N, S, E, W }   // N
};

enum MinorTickPerMajorTick {
  UnlockNextLogicRods,
  ResetNextLogicRodsFrom1,
  ResetNextLogicRodsFromX,
  ResetNextLogicRodsFrom0,
  RelockNextLogicRods,

  UnlockLogicRods,
  SetLogicRodsTo0,
  SetLogicRodsToX,
  SetLogicRodsTo1,
  RelockLogicRods,

  eoMinorTickPerMajorTick
};

char const *toConstCharPointer(MinorTickPerMajorTick t) {
  switch (t) {
    case UnlockNextLogicRods: return "UnlockNextLogicRods";
    case ResetNextLogicRodsFrom1: return "ResetNextLogicRodsFrom1";
    case ResetNextLogicRodsFromX: return "ResetNextLogicRodsFromX";
    case ResetNextLogicRodsFrom0: return "ResetNextLogicRodsFrom0";
    case RelockNextLogicRods: return "RelockNextLogicRods";
    case UnlockLogicRods: return "UnlockLogicRods";
    case SetLogicRodsTo0: return "SetLogicRodsTo0";
    case SetLogicRodsToX: return "SetLogicRodsToX";
    case SetLogicRodsTo1: return "SetLogicRodsTo1";
    case RelockLogicRods: return "RelockLogicRods";
  }
  return "MinorTickPerMajorTick(?)";
}

// {E,S,W,N}{UNL,RN1,RNX,RN0,LNL,UCL,SC0,SCX,SC1,LCL}

enum MinorTickPerCycle {
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

  eoMinorTickPerCycle
};

char const *toConstCharPointer(MinorTickPerCycle t) {
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
  }
  return "MinorTickPerCycle(?)";
}

size_t const NMajorTicksPerCycle = size_t(eoMajorTickPerCycle);
size_t const NMinorTicksPerMajorTick = size_t(eoMinorTickPerMajorTick);
size_t const NMinorTicksPerCycle = size_t(eoMinorTickPerCycle);

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
  VoxelCoordinant &To(MajorTickPerCycle c) {
    switch (c) {
      case E: return ToE();
      case S: return ToS();
      case W: return ToW();
      case N: return ToN();
    }
    return *this;
  }
  VoxelCoordinant &To(Direction d) {
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
  VoxelCoordinant &ToE() { c += 1; return *this; }
  VoxelCoordinant &ToS() { r += 1; return *this; }
  VoxelCoordinant &ToD() { l -= 1; return *this; }
  VoxelCoordinant &ToW() { c -= 1; return *this; }
  VoxelCoordinant &ToN() { r -= 1; return *this; }
  VoxelCoordinant &ToU() { l += 1; return *this; }

  int L() const { return l; }
  int R() const { return r; }
  int C() const { return c; }

  void Dump() const {
    fprintf(stderr,
            "(VoxelCoordinant *)(%p)->{ l=%d, r=%d, c=%d }",
            this,
            l,
            r,
            c
           );
  }

 private:
  int l;
  int r;
  int c;
};

enum FBlockState {
  FBUnkn,

  FBBlkd,
  FBUnbk,

  eoFBlockState
};

char const *toConstCharPointer(FBlockState s) {
  switch (s) {
    case FBUnkn: return "FBUnkn";
    case FBBlkd: return "FBBlkd";
    case FBUnbk: return "FBUnbk";
  }
  return "FBlockState(?)";
}

enum RBlockState {
  RBUnkn,

  RBBlkd,
  RBUnbk,

  eoRBlockState
};

char const *toConstCharPointer(RBlockState s) {
  switch (s) {
    case RBUnkn: return "RBUnkn";
    case RBBlkd: return "RBBlkd";
    case RBUnbk: return "RBUnbk";
  }
  return "FBlockState(?)";
}

enum VoxelType {
  VTUnkn,

  VTWall,
  VTSlot,
  VTLatch,
  VTLogic,

  eoVoxelType
};

char const *toConstCharPointer(VoxelType t) {
  switch (t) {
    case VTUnkn: return "VTUnkn";
    case VTWall: return "VTWall";
    case VTSlot: return "VTSlot";
    case VTLatch: return "VTLatch";
    case VTLogic: return "VTLogic";
  }
  return "VoxelType(?)";
}

enum LatchType {
  KTUnkn,

  KTBody,
  KTHead,
  KTTail,
  KTPost,
  KTLock,

  eoLatchType
};

char const *toConstCharPointer(LatchType t) {
  switch (t) {
    case KTUnkn: return "KTUnkn";
    case KTBody: return "KTBody";
    case KTHead: return "KTHead";
    case KTTail: return "KTTail";
    case KTPost: return "KTPost";
    case KTLock: return "KTLock";
  }
  return "LatchType(?)";
}

enum LatchState {
  KSUnkn,

  KSLckd,
  KSUnlk,

  eoLatchState
};

char const *toConstCharPointer(LatchState t) {
  switch (t) {
    case KSUnkn: return "KSUnkn";
    case KSLckd: return "KSLckd";
    case KSUnlk: return "KSUnlk";
  }
  return "LatchState(?)";
}

enum LogicType {
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

  eoLogicType
};

char const *toConstCharPointer(LogicType t) {
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
  }
  return "LogicType(?)";
}

enum LogicState {
  LSUnkn,

  LSRset,
  LSSet0,
  LSXXXX,
  LSSet1,

  eoLogicState
};

char const *toConstCharPointer(LogicState s) {
  switch (s) {
    case LSUnkn: return "LSUnkn";
    case LSRset: return "LSRset";
    case LSSet0: return "LSSet0";
    case LSXXXX: return "LSXXXX";
    case LSSet1: return "LSSet1";
  }
  return "LogicState(?)";
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
  }
  return "BlockState(?)";
}

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
  MinorTickPerCycle activeDuring[6];
} rodTypeProperties[eoRodType] = {
#undef ____
#define ____ eoMinorTickPerCycle
  { { ____, ____, ____, ____, ____, ____ } }, // RTUn
  { { WUNL, WLNL, NUCL, NLCL, ____, ____ } }, // RTKE
  { { SUNL, SLNL, EUCL, ELCL, ____, ____ } }, // RTKS
  { { EUNL, ELNL, SUCL, SLCL, ____, ____ } }, // RTKW
  { { NUNL, NLNL, WUCL, WLCL, ____, ____ } }, // RTKN
  { { NRN1, NRNX, NRN0, ESC0, ESCX, ESC1 } }, // RTLE
  { { ERN1, ERNX, ERN0, SSC0, SSCX, SSC1 } }, // RTLS
  { { SRN1, SRNX, SRN0, WSC0, WSCX, WSC1 } }, // RTLW
  { { WRN1, WRNX, WRN0, NSC0, NSCX, NSC1 } }, // RTLN
#undef ____
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
  MajorTickPerCycle direction;
  RodType rodType;
  LatchType latchType;
  LatchState latchState;
  LogicType logicType;
  LogicState logicState;
  bool isRodBody;
  BlockState blockableStates;
} voxelProperties[eoVoxel] = {
#undef _
#define _ Unknown
#undef T
#define T true
#undef F
#define F false
  { 9,'?',VTUnkn, _,RTUn,KTUnkn,KSUnkn,LTUnkn,LSUnkn,F,___ }, // Unkn
  { 0,'#',VTWall, _,RTUn,KTUnkn,KSUnkn,LTUnkn,LSUnkn,F,___ }, // Wall
  { 1,' ',VTSlot, _,RTUn,KTUnkn,KSUnkn,LTUnkn,LSUnkn,F,___ }, // Slot
  { 2,'-',VTLatch,E,RTKE,KTBody,KSLckd,LTUnkn,LSUnkn,T,___ }, // KBEL
  { 2,'-',VTLatch,E,RTKE,KTBody,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KBEU
  { 2,'|',VTLatch,S,RTKS,KTBody,KSLckd,LTUnkn,LSUnkn,T,___ }, // KBSL
  { 2,'|',VTLatch,S,RTKS,KTBody,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KBSU
  { 2,'-',VTLatch,W,RTKW,KTBody,KSLckd,LTUnkn,LSUnkn,T,___ }, // KBWL
  { 2,'-',VTLatch,W,RTKW,KTBody,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KBWU
  { 2,'|',VTLatch,N,RTKN,KTBody,KSLckd,LTUnkn,LSUnkn,T,___ }, // KBNL
  { 2,'|',VTLatch,N,RTKN,KTBody,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KBNU
  { 3,'>',VTLatch,E,RTKE,KTHead,KSLckd,LTUnkn,LSUnkn,T,___ }, // KHEL
  { 3,'>',VTLatch,E,RTKE,KTHead,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KHEU
  { 3,'v',VTLatch,S,RTKS,KTHead,KSLckd,LTUnkn,LSUnkn,T,___ }, // KHSL
  { 3,'v',VTLatch,S,RTKS,KTHead,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KHSU
  { 3,'<',VTLatch,W,RTKW,KTHead,KSLckd,LTUnkn,LSUnkn,T,___ }, // KHWL
  { 3,'<',VTLatch,W,RTKW,KTHead,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KHWU
  { 3,'^',VTLatch,N,RTKN,KTHead,KSLckd,LTUnkn,LSUnkn,T,___ }, // KHNL
  { 3,'^',VTLatch,N,RTKN,KTHead,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KHNU
  { 3,'>',VTLatch,E,RTKE,KTTail,KSLckd,LTUnkn,LSUnkn,T,___ }, // KTEL
  { 3,'>',VTLatch,E,RTKE,KTTail,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KTEU
  { 3,'v',VTLatch,S,RTKS,KTTail,KSLckd,LTUnkn,LSUnkn,T,___ }, // KTSL
  { 3,'v',VTLatch,S,RTKS,KTTail,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KTSU
  { 3,'<',VTLatch,W,RTKW,KTTail,KSLckd,LTUnkn,LSUnkn,T,___ }, // KTWL
  { 3,'<',VTLatch,W,RTKW,KTTail,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KTWU
  { 3,'^',VTLatch,N,RTKN,KTTail,KSLckd,LTUnkn,LSUnkn,T,___ }, // KTNL
  { 3,'^',VTLatch,N,RTKN,KTTail,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KTNU
  { 2,'+',VTLatch,E,RTKE,KTPost,KSLckd,LTUnkn,LSUnkn,T,___ }, // KPEL
  { 2,'+',VTLatch,E,RTKE,KTPost,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KPEU
  { 2,'+',VTLatch,S,RTKS,KTPost,KSLckd,LTUnkn,LSUnkn,T,___ }, // KPSL
  { 2,'+',VTLatch,S,RTKS,KTPost,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KPSU
  { 2,'+',VTLatch,W,RTKW,KTPost,KSLckd,LTUnkn,LSUnkn,T,___ }, // KPWL
  { 2,'+',VTLatch,W,RTKW,KTPost,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KPWU
  { 2,'+',VTLatch,N,RTKN,KTPost,KSLckd,LTUnkn,LSUnkn,T,___ }, // KPNL
  { 2,'+',VTLatch,N,RTKN,KTPost,KSUnlk,LTUnkn,LSUnkn,T,___ }, // KPNU
  { 3,'<',VTLatch,E,RTKE,KTLock,KSLckd,LTUnkn,LSUnkn,F,___ }, // KKEL
  { 3,'<',VTLatch,E,RTKE,KTLock,KSUnlk,LTUnkn,LSUnkn,F,___ }, // KKEU
  { 3,'^',VTLatch,S,RTKS,KTLock,KSLckd,LTUnkn,LSUnkn,F,___ }, // KKSL
  { 3,'^',VTLatch,S,RTKS,KTLock,KSUnlk,LTUnkn,LSUnkn,F,___ }, // KKSU
  { 3,'>',VTLatch,W,RTKW,KTLock,KSLckd,LTUnkn,LSUnkn,F,___ }, // KKWL
  { 3,'>',VTLatch,W,RTKW,KTLock,KSUnlk,LTUnkn,LSUnkn,F,___ }, // KKWU
  { 3,'v',VTLatch,N,RTKN,KTLock,KSLckd,LTUnkn,LSUnkn,F,___ }, // KKNL
  { 3,'v',VTLatch,N,RTKN,KTLock,KSUnlk,LTUnkn,LSUnkn,F,___ }, // KKNU
  { 4,'-',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTBody,LSRset,T,___ }, // LBER
  { 4,'-',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTBody,LSSet0,T,___ }, // LBE0
  { 4,'-',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTBody,LSXXXX,T,___ }, // LBEI
  { 4,'-',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTBody,LSSet1,T,___ }, // LBE1
  { 4,'|',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTBody,LSRset,T,___ }, // LBSR
  { 4,'|',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTBody,LSSet0,T,___ }, // LBS0
  { 4,'|',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTBody,LSXXXX,T,___ }, // LBSI
  { 4,'|',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTBody,LSSet1,T,___ }, // LBS1
  { 4,'-',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTBody,LSRset,T,___ }, // LBWR
  { 4,'-',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTBody,LSSet0,T,___ }, // LBW0
  { 4,'-',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTBody,LSXXXX,T,___ }, // LBWI
  { 4,'-',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTBody,LSSet1,T,___ }, // LBW1
  { 4,'|',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTBody,LSRset,T,___ }, // LBNR
  { 4,'|',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTBody,LSSet0,T,___ }, // LBN0
  { 4,'|',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTBody,LSXXXX,T,___ }, // LBNI
  { 4,'|',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTBody,LSSet1,T,___ }, // LBN1
  { 5,'>',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTHead,LSRset,T,F__ }, // LHER
  { 5,'>',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTHead,LSSet0,T,F__ }, // LHE0
  { 5,'>',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTHead,LSXXXX,T,F__ }, // LHEI
  { 5,'>',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTHead,LSSet1,T,F__ }, // LHE1
  { 5,'v',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTHead,LSRset,T,F__ }, // LHSR
  { 5,'v',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTHead,LSSet0,T,F__ }, // LHS0
  { 5,'v',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTHead,LSXXXX,T,F__ }, // LHSI
  { 5,'v',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTHead,LSSet1,T,F__ }, // LHS1
  { 5,'<',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTHead,LSRset,T,F__ }, // LHWR
  { 5,'<',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTHead,LSSet0,T,F__ }, // LHW0
  { 5,'<',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTHead,LSXXXX,T,F__ }, // LHWI
  { 5,'<',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTHead,LSSet1,T,F__ }, // LHW1
  { 5,'^',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTHead,LSRset,T,F__ }, // LHNR
  { 5,'^',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTHead,LSSet0,T,F__ }, // LHN0
  { 5,'^',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTHead,LSXXXX,T,F__ }, // LHNI
  { 5,'^',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTHead,LSSet1,T,F__ }, // LHN1
  { 5,'>',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTTail,LSRset,T,_R_ }, // LTER
  { 5,'>',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTTail,LSSet0,T,_R_ }, // LTE0
  { 5,'>',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTTail,LSXXXX,T,_R_ }, // LTEI
  { 5,'>',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTTail,LSSet1,T,_R_ }, // LTE1
  { 5,'v',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTTail,LSRset,T,_R_ }, // LTSR
  { 5,'v',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTTail,LSSet0,T,_R_ }, // LTS0
  { 5,'v',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTTail,LSXXXX,T,_R_ }, // LTSI
  { 5,'v',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTTail,LSSet1,T,_R_ }, // LTS1
  { 5,'<',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTTail,LSRset,T,_R_ }, // LTWR
  { 5,'<',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTTail,LSSet0,T,_R_ }, // LTW0
  { 5,'<',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTTail,LSXXXX,T,_R_ }, // LTWI
  { 5,'<',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTTail,LSSet1,T,_R_ }, // LTW1
  { 5,'^',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTTail,LSRset,T,_R_ }, // LTNR
  { 5,'^',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTTail,LSSet0,T,_R_ }, // LTN0
  { 5,'^',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTTail,LSXXXX,T,_R_ }, // LTNI
  { 5,'^',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTTail,LSSet1,T,_R_ }, // LTN1
  { 4,'+',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTPost,LSRset,T,FR_ }, // LPER
  { 4,'+',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTPost,LSSet0,T,FR_ }, // LPE0
  { 4,'+',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTPost,LSXXXX,T,FR_ }, // LPEI
  { 4,'+',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTPost,LSSet1,T,FR_ }, // LPE1
  { 4,'+',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTPost,LSRset,T,FR_ }, // LPSR
  { 4,'+',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTPost,LSSet0,T,FR_ }, // LPS0
  { 4,'+',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTPost,LSXXXX,T,FR_ }, // LPSI
  { 4,'+',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTPost,LSSet1,T,FR_ }, // LPS1
  { 4,'+',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTPost,LSRset,T,FR_ }, // LPWR
  { 4,'+',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTPost,LSSet0,T,FR_ }, // LPW0
  { 4,'+',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTPost,LSXXXX,T,FR_ }, // LPWI
  { 4,'+',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTPost,LSSet1,T,FR_ }, // LPW1
  { 4,'+',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTPost,LSRset,T,FR_ }, // LPNR
  { 4,'+',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTPost,LSSet0,T,FR_ }, // LPN0
  { 4,'+',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTPost,LSXXXX,T,FR_ }, // LPNI
  { 4,'+',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTPost,LSSet1,T,FR_ }, // LPN1
  { 5,'^',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTLock,LSRset,F,FRL }, // LKER
  { 5,'^',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTLock,LSSet0,F,FRL }, // LKE0
  { 5,'^',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTLock,LSXXXX,F,FRL }, // LKEI
  { 5,'^',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTLock,LSSet1,F,FRL }, // LKE1
  { 5,'>',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTLock,LSRset,F,FRL }, // LKSR
  { 5,'>',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTLock,LSSet0,F,FRL }, // LKS0
  { 5,'>',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTLock,LSXXXX,F,FRL }, // LKSI
  { 5,'>',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTLock,LSSet1,F,FRL }, // LKS1
  { 5,'v',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTLock,LSRset,F,FRL }, // LKWR
  { 5,'v',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTLock,LSSet0,F,FRL }, // LKW0
  { 5,'v',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTLock,LSXXXX,F,FRL }, // LKWI
  { 5,'v',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTLock,LSSet1,F,FRL }, // LKW1
  { 5,'<',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTLock,LSRset,F,FRL }, // LKNR
  { 5,'<',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTLock,LSSet0,F,FRL }, // LKN0
  { 5,'<',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTLock,LSXXXX,F,FRL }, // LKNI
  { 5,'<',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTLock,LSSet1,F,FRL }, // LKN1
  { 5,'>',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTTest,LSRset,F,FR_ }, // LQER
  { 5,'>',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTTest,LSSet0,F,FR_ }, // LQE0
  { 5,'>',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTTest,LSXXXX,F,FR_ }, // LQEI
  { 5,'>',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTTest,LSSet1,F,FR_ }, // LQE1
  { 5,'v',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTTest,LSRset,F,FR_ }, // LQSR
  { 5,'v',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTTest,LSSet0,F,FR_ }, // LQS0
  { 5,'v',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTTest,LSXXXX,F,FR_ }, // LQSI
  { 5,'v',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTTest,LSSet1,F,FR_ }, // LQS1
  { 5,'<',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTTest,LSRset,F,FR_ }, // LQWR
  { 5,'<',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTTest,LSSet0,F,FR_ }, // LQW0
  { 5,'<',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTTest,LSXXXX,F,FR_ }, // LQWI
  { 5,'<',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTTest,LSSet1,F,FR_ }, // LQW1
  { 5,'^',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTTest,LSRset,F,FR_ }, // LQNR
  { 5,'^',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTTest,LSSet0,F,FR_ }, // LQN0
  { 5,'^',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTTest,LSXXXX,F,FR_ }, // LQNI
  { 5,'^',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTTest,LSSet1,F,FR_ }, // LQN1
  { 5,'@',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTGate,LSRset,F,FR_ }, // LGER
  { 5,'@',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTGate,LSSet0,F,FR_ }, // LGE0
  { 5,'@',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTGate,LSXXXX,F,FR_ }, // LGEI
  { 5,'@',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTGate,LSSet1,F,FR_ }, // LGE1
  { 5,'@',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTGate,LSRset,F,FR_ }, // LGSR
  { 5,'@',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTGate,LSSet0,F,FR_ }, // LGS0
  { 5,'@',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTGate,LSXXXX,F,FR_ }, // LGSI
  { 5,'@',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTGate,LSSet1,F,FR_ }, // LGS1
  { 5,'@',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTGate,LSRset,F,FR_ }, // LGWR
  { 5,'@',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTGate,LSSet0,F,FR_ }, // LGW0
  { 5,'@',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTGate,LSXXXX,F,FR_ }, // LGWI
  { 5,'@',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTGate,LSSet1,F,FR_ }, // LGW1
  { 5,'@',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTGate,LSRset,F,FR_ }, // LGNR
  { 5,'@',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTGate,LSSet0,F,FR_ }, // LGN0
  { 5,'@',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTGate,LSXXXX,F,FR_ }, // LGNI
  { 5,'@',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTGate,LSSet1,F,FR_ }, // LGN1
  { 6,'X',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTInpt,LSRset,T,___ }, // LIER
  { 6,'0',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTInpt,LSSet0,T,___ }, // LIE0
  { 6,'X',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTInpt,LSXXXX,T,___ }, // LIEI
  { 6,'1',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTInpt,LSSet1,T,___ }, // LIE1
  { 6,'X',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTInpt,LSRset,T,___ }, // LISR
  { 6,'0',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTInpt,LSSet0,T,___ }, // LIS0
  { 6,'X',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTInpt,LSXXXX,T,___ }, // LISI
  { 6,'1',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTInpt,LSSet1,T,___ }, // LIS1
  { 6,'X',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTInpt,LSRset,T,___ }, // LIWR
  { 6,'0',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTInpt,LSSet0,T,___ }, // LIW0
  { 6,'X',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTInpt,LSXXXX,T,___ }, // LIWI
  { 6,'1',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTInpt,LSSet1,T,___ }, // LIW1
  { 6,'X',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTInpt,LSRset,T,___ }, // LINR
  { 6,'0',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTInpt,LSSet0,T,___ }, // LIN0
  { 6,'X',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTInpt,LSXXXX,T,___ }, // LINI
  { 6,'1',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTInpt,LSSet1,T,___ }, // LIN1
  { 6,'O',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTOutp,LSRset,T,___ }, // LOER
  { 6,'0',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTOutp,LSSet0,T,___ }, // LOE0
  { 6,'X',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTOutp,LSXXXX,T,___ }, // LOEI
  { 6,'1',VTLogic,E,RTLE,KTUnkn,KSUnkn,LTOutp,LSSet1,T,___ }, // LOE1
  { 6,'O',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTOutp,LSRset,T,___ }, // LOSR
  { 6,'0',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTOutp,LSSet0,T,___ }, // LOS0
  { 6,'X',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTOutp,LSXXXX,T,___ }, // LOSI
  { 6,'1',VTLogic,S,RTLS,KTUnkn,KSUnkn,LTOutp,LSSet1,T,___ }, // LOS1
  { 6,'O',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTOutp,LSRset,T,___ }, // LOWR
  { 6,'0',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTOutp,LSSet0,T,___ }, // LOW0
  { 6,'X',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTOutp,LSXXXX,T,___ }, // LOWI
  { 6,'1',VTLogic,W,RTLW,KTUnkn,KSUnkn,LTOutp,LSSet1,T,___ }, // LOW1
  { 6,'O',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTOutp,LSRset,T,___ }, // LONR
  { 6,'0',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTOutp,LSSet0,T,___ }, // LON0
  { 6,'X',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTOutp,LSXXXX,T,___ }, // LONI
  { 6,'1',VTLogic,N,RTLN,KTUnkn,KSUnkn,LTOutp,LSSet1,T,___ }  // LON1
#undef _
#undef T
#undef F
};

class Item;
typedef set<Item *> ItemsActiveForMinorTick;
class LatchRod;
class LogicRod;

size_t const NLvls = 7;
size_t const NRows = 21;
size_t const NCols = 21;

typedef array<array<array<Voxel, NCols>, NRows>, NLvls> VolArray;
typedef array<array<char, NCols>, NRows> ViewLvlArray;

class Volume: public VolArray
{
 public:
  Volume(VolArray const &initial);
  Volume(Voxel initialVoxel = Unkn);
  void MajorTick();
  void MinorTick();
  void ViewFlat(ViewLvlArray &view) const;
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

 private:
  Item *FormRodContaining(set<VoxelCoordinant> &seenSofar, VoxelCoordinant v);
  void FindItems();

  size_t clock;
  array<ItemsActiveForMinorTick, NMinorTicksPerCycle> itemsActiveByTick;
};

class Item: public set<VoxelCoordinant> {
 public:
  Item(RodType t):
      set<VoxelCoordinant>(),
      rodType(t),
      fBlockState(FBUnkn),
      rBlockState(RBUnkn),
      lockState(KSUnkn)
  {}

  bool IsFBlocked() const { return fBlockState == FBBlkd; }
  bool IsRBlocked() const { return rBlockState == RBBlkd; }
  bool IsLocked() const { return lockState == KSLckd; }

  virtual bool IsFreeToMoveAt(Volume *volume, MinorTickPerCycle tick) const = 0;
  virtual bool AttemptToMoveAt(Volume *volume, MinorTickPerCycle tick) = 0;
  virtual void Dump() const;

 private:
  RodType rodType;
  FBlockState fBlockState;
  RBlockState rBlockState;
  LatchState lockState;
};

class LatchRod: public Item {
 public:
  LatchRod(RodType t): Item(t) {}
  bool IsFreeToMoveAt(Volume *volume, MinorTickPerCycle tick) const;
  bool AttemptToMoveAt(Volume *volume, MinorTickPerCycle tick);
  void Dump() const;
};

class LogicRod: public Item {
 public:
  LogicRod(RodType t): Item(t), logicState(LSUnkn) {}
  bool IsFreeToMoveAt(Volume *volume, MinorTickPerCycle tick) const;
  bool AttemptToMoveAt(Volume *volume, MinorTickPerCycle tick);
  void Dump() const;

 private:
  LogicState logicState;
};

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

void Volume::MajorTick() {
  size_t eoTick = clock + NMinorTicksPerMajorTick;
  while (clock < eoTick) {
    MinorTick();
  }
}

void Volume::MinorTick() {
  size_t tick = clock % NMinorTicksPerCycle;
  ItemsActiveForMinorTick &itemsActiveThisMinorTick = itemsActiveByTick[tick];

  for (Item *i : itemsActiveThisMinorTick) {
    i->AttemptToMoveAt(this, MinorTickPerCycle(tick));
  }
  clock += 1;
}

void Volume::ViewFlat(ViewLvlArray &view) const {
  for (int r = 0; r < NRows; r += 1) {
    for (int c = 0; c < NCols; c += 1) {
      int lOfMaxDisplayPriority = NLvls;
      int maxDisplayPriority = -1;
      for (int l = NLvls - 1; 0 <= l; l -= 1) {
        Voxel const &voxel = (*this)[l][r][c];
        VoxelProperties const &properties = voxelProperties[voxel];
        if (maxDisplayPriority < properties.displayPriority) {
          maxDisplayPriority = properties.displayPriority;
          lOfMaxDisplayPriority = l;
        }
      }
      view[r][c] = voxelProperties[(*this)[lOfMaxDisplayPriority][r][c]].text;
    }
  }
}

// Given a voxel's coordinant, representing the first voxel of a
// {latch,logic} rod, create an appropriate {Latch,Logic}Rod item, and
// populate it with all attached voxels (and slots).

bool Volume::isPartOf(RodType r, VoxelCoordinant v, bool &reachedEnd) const {
  return isVoxelCoordinantInBounds(v) ?
      (!reachedEnd ?
       voxelProperties[voxelAt(v)].rodType == r :
       (reachedEnd = true, voxelProperties[voxelAt(v)].type == VTSlot)
      ) :
      (reachedEnd = true, false);
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
      item = new LatchRod(vProperties.rodType);
      break;
    case RTLE: case RTLS: case RTLW: case RTLN:
      item = new LogicRod(vProperties.rodType);
      break;
    default:
      return 0;
  }
  item->insert(v);
  MajorTickProperties const &cProperties =
      majorTickProperties[vProperties.direction];
  bool reachedFwdEnd = false;
  for (VoxelCoordinant f = v.To(cProperties.fwd);
       isPartOf(vProperties.rodType, f, reachedFwdEnd);
       f.To(cProperties.fwd)
      )
  {
    item->insert(f);
    VoxelCoordinant u = f.ToU();
    if (isPartOf(vProperties.rodType, u)) {
      item->insert(u);
    }
    VoxelCoordinant d = f.ToD();
    if (isPartOf(vProperties.rodType, d)) {
      item->insert(d);
    }
  }
  bool reachedBwdEnd = false;
  for (VoxelCoordinant b = v.To(cProperties.bwd);
       isPartOf(vProperties.rodType, b, reachedBwdEnd);
       b.To(cProperties.bwd)
      )
  {
    item->insert(b);
    VoxelCoordinant u = b.ToU();
    if (isPartOf(vProperties.rodType, u)) {
      item->insert(u);
    }
    VoxelCoordinant d = b.ToD();
    if (isPartOf(vProperties.rodType, d)) {
      item->insert(d);
    }
  }
  seenSofar.insert(item->begin(), item->end());
  itemsActiveByTick[0].insert(item);
  return item;
}

void Volume::FindItems() {
  for (size_t i = 0; i < NMinorTicksPerMajorTick; i += 1) {
    ItemsActiveForMinorTick &items = itemsActiveByTick[i];
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
            item->Dump();
            fprintf(stderr, "\n");
            break;
          }
        }
      }
    }
  }
}

void Item::Dump() const {
  fprintf(stderr, "(Item *)(%p)->{", this);
  fprintf(stderr, " rodType=%s, ", toConstCharPointer(rodType));
  fprintf(stderr, " fBlockState=%s, ", toConstCharPointer(fBlockState));
  fprintf(stderr, " rBlockState=%s, ", toConstCharPointer(rBlockState));
  fprintf(stderr, " lockState=%s, { ", toConstCharPointer(lockState));
  char const *comma = "";
  for (auto const &c : *this) {
    fprintf(stderr, "%c (%2d,%2d,%2d)", *comma, c.L(), c.R(), c.C());
    comma = ",";
  }
  fprintf(stderr, " }");
  fprintf(stderr, " }");
}

bool LatchRod::IsFreeToMoveAt(Volume *volume, MinorTickPerCycle tick) const {
}

bool LatchRod::AttemptToMoveAt(Volume *volume, MinorTickPerCycle tick) {
  if (!IsFreeToMoveAt(volume, tick)) {
    return false;
  }
  for (auto const &vc : *this) {
    // MoveAt(volume, minorTick, vc);
  }
}

void LatchRod::Dump() const {
  fprintf(stderr, "(LatchRod *)(%p)->{ ", this);
  Item::Dump();
  fprintf(stderr, " }");
}

bool LogicRod::IsFreeToMoveAt(Volume *volume, MinorTickPerCycle tick) const {
}

bool LogicRod::AttemptToMoveAt(Volume *volume, MinorTickPerCycle tick) {
  if (!IsFreeToMoveAt(volume, tick)) {
    return false;
  }
  for (auto const &vc : *this) {
    // MoveAt(volume, minorTick, vc);
  }
}

void LogicRod::Dump() const {
  fprintf(stderr, "(LogicRod *)(%p)->{", this);
  fprintf(stderr, " logicState=%s, ", toConstCharPointer(logicState));
  Item::Dump();
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
  // 0 - {A,C} Latch Rods
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
  // 2 -A,C} Logic Rods
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
  // 4 -B,D} Logic Rods
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
  // 6 -B,D} Latch Rods
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
  ViewLvlArray view;
  Volume volume(initialVolume);

  volume.ViewFlat(view);

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
  return 0;
}
