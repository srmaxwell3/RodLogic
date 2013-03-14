#include "voxel.h"

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

VoxelProperties voxelProperties[eoVoxel] = {
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
  { rt##rodType0 /* , rt##rodType1, rt##rodType2 */ },  \
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
