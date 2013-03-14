#ifndef VOXEL_H
#define VOXEL_H

#include <array>
using std::array;
#include <map>
using std::map;

#include "blockstate.h"
#include "datastate.h"
#include "datatype.h"
#include "direction.h"
#include "lockstate.h"
#include "locktype.h"
#include "rodtype.h"
#include "voxelcoordinant.h"
#include "voxeltype.h"

// U[nknown]
// W[all]
// S[lot]
// L[ock]{B[ody},H[ead],T[ail],P[ost],K[ey]}
//       {E,S,D,N,W,U}
//       {L[ocked],U[unlocked]}
// D[ata]{B[ody},H[ead],T[ail],P[ost],S[lot],[Loc]K,Q[uery],G[ate],J[oin],I[nput],O[utput}}
//       {E,S,D,W,N,U}
//       {R[eset],[Set]0,X,[Set]1}

enum Voxel {
  Unkn,

  Wall,

  Slot,

  LBEL, LBEU,  LBSL, LBSU,  LBDL, LBDU,  LBWL, LBWU,  LBNL, LBNU,  LBUL, LBUU, // L[ock]B[ody]
  LHEL, LHEU,  LHSL, LHSU,  LHDL, LHDU,  LHWL, LHWU,  LHNL, LHNU,  LHUL, LHUU, // L[ock]H[ead]
  LTEL, LTEU,  LTSL, LTSU,  LTDL, LTDU,  LTWL, LTWU,  LTNL, LTNU,  LTUL, LTUU, // L[ock]T[ail]
  LPEL, LPEU,  LPSL, LPSU,  LPDL, LPDU,  LPWL, LPWU,  LPNL, LPNU,  LPUL, LPUU, // L[ock]P[ost]
  LKEL, LKEU,  LKSL, LKSU,  LKDL, LKDU,  LKWL, LKWU,  LKNL, LKNU,  LKUL, LKUU, // L[ock]K[ey]

  DBER, DBE0, DBEX, DBE1,  DBSR, DBS0, DBSX, DBS1,  DBDR, DBD0, DBDX, DBD1, // D[ata]B[ody]
  DBWR, DBW0, DBWX, DBW1,  DBNR, DBN0, DBNX, DBN1,  DBUR, DBU0, DBUX, DBU1,
  DHER, DHE0, DHEX, DHE1,  DHSR, DHS0, DHSX, DHS1,  DHDR, DHD0, DHDX, DHD1, // D[ata]H[ead]
  DHWR, DHW0, DHWX, DHW1,  DHNR, DHN0, DHNX, DHN1,  DHUR, DHU0, DHUX, DHU1,
  DTER, DTE0, DTEX, DTE1,  DTSR, DTS0, DTSX, DTS1,  DTDR, DTD0, DTDX, DTD1, // D[ata]T[ail]
  DTWR, DTW0, DTWX, DTW1,  DTNR, DTN0, DTNX, DTN1,  DTUR, DTU0, DTUX, DTU1,
  DPER, DPE0, DPEX, DPE1,  DPSR, DPS0, DPSX, DPS1,  DPDR, DPD0, DPDX, DPD1, // D[ata]P[ost]
  DPWR, DPW0, DPWX, DPW1,  DPNR, DPN0, DPNX, DPN1,  DPUR, DPU0, DPUX, DPU1,
  DSER, DSE0, DSEX, DSE1,  DSSR, DSS0, DSSX, DSS1,  DSDR, DSD0, DSDX, DSD1, // D[ata]S[lot]
  DSWR, DSW0, DSWX, DSW1,  DSNR, DSN0, DSNX, DSN1,  DSUR, DSU0, DSUX, DSU1,
  DQER, DQE0, DQEX, DQE1,  DQSR, DQS0, DQSX, DQS1,  DQDR, DQD0, DQDX, DQD1, // D[ata]Q[uery]
  DQWR, DQW0, DQWX, DQW1,  DQNR, DQN0, DQNX, DQN1,  DQUR, DQU0, DQUX, DQU1,
  DGER, DGE0, DGEX, DGE1,  DGSR, DGS0, DGSX, DGS1,  DGDR, DGD0, DGDX, DGD1, // D[ata]G[ate]
  DGWR, DGW0, DGWX, DGW1,  DGNR, DGN0, DGNX, DGN1,  DGUR, DGU0, DGUX, DGU1,
  DIER, DIE0, DIEX, DIE1,  DISR, DIS0, DISX, DIS1,  DIDR, DID0, DIDX, DID1, // D[ata]I[nput]
  DIWR, DIW0, DIWX, DIW1,  DINR, DIN0, DINX, DIN1,  DIUR, DIU0, DIUX, DIU1,
  DOER, DOE0, DOEX, DOE1,  DOSR, DOS0, DOSX, DOS1,  DODR, DOD0, DODX, DOD1, // D[ata]O[utput]
  DOWR, DOW0, DOWX, DOW1,  DONR, DON0, DONX, DON1,  DOUR, DOU0, DOUX, DOU1,

  eoVoxel
};

char const *toConstCharPointer(Voxel v);

typedef map<VoxelCoordinant, Voxel> Changes;

struct VoxelProperties {
  bool IsOneOf(RodType r) const {
    for (auto const &rt : rodType) {
      if (r == rt) {
        return true;
      }
    }
    return false;
  }

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
};

extern VoxelProperties voxelProperties[eoVoxel];

#endif // VOXEL_H
