#ifndef VOXEL_H
#define VOXEL_H

#include <array>
#include "voxeltype.h"
#include "rodtype.h"
#include "locktype.h"
#include "lockstate.h"
#include "datatype.h"
#include "datastate.h"
#include "blockstate.h"
#include "direction.h"

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

char const *toConstCharPointer(Voxel v);

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
} voxelProperties[eoVoxel];

#endif // VOXEL_H
