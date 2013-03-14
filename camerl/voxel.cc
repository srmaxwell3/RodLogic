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
