#ifndef CAM_8_RULES_H
#define CAM_8_RULES_H

  volume.AddRule(tcEUSL, Wall, Wall, Wall, Slot, LHWL, Wall, Wall, LHWU);
  volume.AddRule(tcEUSL, Wall, LBWL, Wall, Slot, DKSR, Wall, Wall, LKWU);
  volume.AddRule(tcEUSL, Wall, Wall, LBWL, LBWL, LBWL, Wall, Wall, LBWU);
  volume.AddRule(tcEUSL, Wall, Wall, LHWL, LBWL, LBWL, Wall, Wall, LBWU);
  volume.AddRule(tcEUSL, Wall, Wall, LPWL, LBWL, LBWL, Wall, Wall, LBWU);
  volume.AddRule(tcEUSL, Wall, Wall, LBWL, LBWL, LTWL, Wall, Wall, LTWU);
  volume.AddRule(tcEUSL, Wall, Wall, LBWL, LBWL, LPWL, Slot, Wall, LPWU);
  volume.AddRule(tcEUSL, Wall, Wall, Slot, LHWL, LBWL, Wall, Wall, LBWU);
  volume.AddRule(tcEUSL, Wall, Wall, LBWL, LTWL, Wall, Wall, Wall, Slot);
  volume.AddRule(tcEUSL, Wall, Wall, LBWL, LPWL, LBWL, DKSR, Wall, LBWU);
  volume.AddRule(tcEUSL, DSSR, LPWL, Slot, DKSR, Wall, DPSR, Slot, DSSR);

  volume.AddRule(tcELSL, Wall, Wall, LTWU, Slot, Wall, Wall, Wall, LTWL);
  volume.AddRule(tcELSL, Wall, Wall, LBWU, LBWU, LBWU, Wall, Wall, LBWL);
  volume.AddRule(tcELSL, Wall, Wall, LHWU, LBWU, LBWU, Wall, Wall, LHWL);
  volume.AddRule(tcELSL, Wall, Wall, LPWU, LBWU, LBWU, DSSR, Wall, LPWL);
  volume.AddRule(tcELSL, Wall, Wall, LBWU, LBWU, LTWU, Wall, Wall, LBWL);
  volume.AddRule(tcELSL, Wall, Wall, LBWU, LBWU, LPWU, Wall, Wall, LBWL);
  volume.AddRule(tcELSL, Wall, Wall, Wall, LHWU, LBWU, Wall, Wall, Slot);
  volume.AddRule(tcELSL, Wall, Wall, LBWU, LTWU, Slot, Wall, Wall, LBWL);
  volume.AddRule(tcELSL, Wall, Wall, LBWU, LPWU, LBWU, LKWU, Wall, LBWL);
  volume.AddRule(tcELSL, Wall, LPWU, Wall, LKWU, DSSR, Wall, Wall, Slot);
  volume.AddRule(tcELSL, DSSR, LBWU, LKWU, DSSR, Wall, DPSR, Slot, DKSR);

  volume.AddRule(tcEUEL, LHSL, Wall, Wall, Slot, Wall, Wall, Wall, LHSU);
  volume.AddRule(tcEUEL, DKER, Wall, Wall, Slot, Wall, LBSL, Wall, LKSU);
  volume.AddRule(tcEUEL, LBSL, Wall, Wall, LBSL, Wall, Wall, LBSL, LBSU);
  volume.AddRule(tcEUEL, LTSL, Wall, Wall, LBSL, Wall, Wall, LBSL, LTSU);
  volume.AddRule(tcEUEL, LPSL, Slot, Wall, LBSL, Wall, Wall, LBSL, LPSU);
  volume.AddRule(tcEUEL, LBSL, Wall, Wall, LBSL, Wall, Wall, LHSL, LBSU);
  volume.AddRule(tcEUEL, LBSL, Wall, Wall, LBSL, Wall, Wall, LPSL, LBSU);
  volume.AddRule(tcEUEL, LBSL, Wall, Wall, LHSL, Wall, Wall, Slot, LBSU);
  volume.AddRule(tcEUEL, Wall, Wall, Wall, LTSL, Wall, Wall, LBSL, Slot);
  volume.AddRule(tcEUEL, LBSL, DKER, Wall, LPSL, Wall, Wall, LBSL, LBSU);
  volume.AddRule(tcEUEL, Wall, DPER, DSER, DKER, Slot, LPSL, Slot, DSER);

  volume.AddRule(tcESE0, Wall, Wall, Slot, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcESE0, Wall, DBER, Slot, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcESE0, Wall, DTER, Slot, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcESE0, Wall, Wall, Slot, Slot, Wall, DBER, Wall, Slot);
  volume.AddRule(tcESE0, Wall, Wall, Slot, Slot, Wall, DHER, Wall, Slot);
  volume.AddRule(tcESE0, DQSR, DBSR, Slot, Slot, Wall, DBER, Slot, Slot);
  volume.AddRule(tcESE0, Wall, Wall, DHER, Slot, Slot, Wall, Wall, DHE0);
  volume.AddRule(tcESE0, Wall, DBER, DSER, Slot, Slot, Wall, Wall, DSE0);
  volume.AddRule(tcESE0, Wall, Wall, DGER, Slot, Slot, DBER, Wall, DGE0);
  volume.AddRule(tcESE0, DQSR, DBSR, DGER, Slot, Slot, DBER, Slot, DGE0);
  volume.AddRule(tcESE0, DQSR, DBSR, DQER, Slot, Slot, DBER, DQSR, DQE0);
  volume.AddRule(tcESE0, Slot, DBNR, DQER, Slot, Slot, DBER, DQNR, DQE0);
  volume.AddRule(tcESE0, Wall, Wall, Slot, Slot, DQER, DBER, Wall, Slot);
  volume.AddRule(tcESE0, Wall, Wall, DBER, DBER, DBER, Wall, Wall, DBE0);
  volume.AddRule(tcESE0, Wall, Slot, DBER, DBER, DBER, Wall, Wall, DBE0);
  volume.AddRule(tcESE0, Wall, Slot, DTER, DBER, DBER, Wall, Wall, DTE0);
  volume.AddRule(tcESE0, Wall, Slot, DPER, DBER, DBER, Wall, Wall, DPE0);
  volume.AddRule(tcESE0, Wall, Wall, DIER, DBER, DBER, Wall, Wall, DBE0);
  volume.AddRule(tcESE0, Wall, Wall, DBER, DBER, DBER, Slot, Wall, DBE0);
  volume.AddRule(tcESE0, Wall, Wall, DPER, DBER, DBER, Slot, Wall, DPE0);
  volume.AddRule(tcESE0, Wall, Slot, DPER, DBER, DHER, Wall, Wall, DPE0);
  volume.AddRule(tcESE0, Wall, Wall, DPER, DBER, DTER, Slot, Wall, DPE0);
  volume.AddRule(tcESE0, Wall, Wall, DBER, DBER, DPER, Wall, Wall, DBE0);
  volume.AddRule(tcESE0, Wall, Slot, DBER, DBER, DPER, Wall, Wall, DBE0);
  volume.AddRule(tcESE0, Wall, Wall, DBER, DBER, DOER, Wall, Wall, DBE0);
  volume.AddRule(tcESE0, Wall, Slot, DBER, DHER, Slot, Wall, Wall, DBE0);
  volume.AddRule(tcESE0, Wall, DQER, DBER, DPER, DBER, Slot, Wall, DBE0);
  volume.AddRule(tcESE0, Wall, DQER, DBER, DPER, DBER, Wall, Wall, DBE0);
  volume.AddRule(tcESE0, Wall, DGER, DBER, DPER, DBER, Wall, Wall, DBE0);
  volume.AddRule(tcESE0, Wall, Wall, DPER, DPER, DBER, DSER, Wall, DPE0);
  volume.AddRule(tcESE0, Wall, Wall, Wall, DPER, DPER, DSER, Wall, Slot);
  volume.AddRule(tcESE0, Wall, Wall, DBER, DPER, DPER, DSER, Wall, DBE0);
  volume.AddRule(tcESE0, Wall, Wall, DPER, DPER, DPER, DSER, Wall, DPE0);
  volume.AddRule(tcESE0, Wall, DPER, DSER, DSER, Slot, LBSU, LKSU, DSE0);
  volume.AddRule(tcESE0, Wall, DPER, Wall, DSER, DSER, Wall, Wall, Slot);
  volume.AddRule(tcESE0, Wall, DPER, DSER, DSER, DSER, Wall, Wall, DSE0);
  volume.AddRule(tcESE0, Wall, Wall, Wall, DQER, Slot, DTER, Wall, Slot);
  volume.AddRule(tcESE0, Wall, Wall, Slot, DQER, Slot, DPER, Wall, Slot);
  volume.AddRule(tcESE0, Wall, Wall, Wall, DGER, Slot, DPER, Wall, Slot);
  volume.AddRule(tcESE0, Wall, Wall, Wall, DIER, DBER, Wall, Wall, DIE0);
  volume.AddRule(tcESE0, Wall, Wall, DBER, DOER, Wall, Wall, Wall, DOE0);

  volume.AddRule(tcESE0, Wall, DPER, Slot, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcESE0, Wall, Wall, DPER, DBER, DPER, Slot, Wall, DPE0);
  volume.AddRule(tcESE0, Wall, Wall, Wall, DQER, Slot, DPER, Wall, Slot);

  volume.AddRule(tcESE1, Wall, DBE0, DSE0, Slot, Wall, Wall, Wall, DSE1);
  volume.AddRule(tcESE1, Wall, Wall, DQE0, Slot, Wall, DBE0, Wall, DQE1);
  volume.AddRule(tcESE1, Wall, Wall, DQE0, Slot, Slot, DBE0, Wall, DQE1);
  volume.AddRule(tcESE1, Wall, Wall, Wall, Slot, DPE0, Slot, Wall, Slot);
  volume.AddRule(tcESE1, Wall, Slot, Wall, Slot, DSE0, Wall, Wall, Slot);
  volume.AddRule(tcESE1, Wall, Wall, Wall, Slot, DQE0, DBE0, Wall, Slot);
  volume.AddRule(tcESE1, Wall, Wall, Slot, Slot, DQE0, DBE0, Wall, Slot);
  volume.AddRule(tcESE1, Wall, Wall, DBE0, DBE0, DBE0, Wall, Wall, DBE1);
  volume.AddRule(tcESE1, Wall, Slot, DPE0, DBE0, DBE0, Wall, Wall, DPE1);
  volume.AddRule(tcESE1, Wall, Slot, DBE0, DBE0, DPE0, Wall, Wall, DBE1);
  volume.AddRule(tcESE1, Wall, Slot, DPE0, DBE0, DPE0, Slot, Wall, DPE1);
  volume.AddRule(tcESE1, Wall, Wall, DBE0, DBE0, DOE0, Wall, Wall, DBE1);
  volume.AddRule(tcESE1, Wall, DQE0, DBE0, DPE0, DBE0, Wall, Wall, DBE1);
  volume.AddRule(tcESE1, Wall, Wall, DPE0, DPE0, DBE0, DSE0, Wall, DPE1);
  volume.AddRule(tcESE1, Wall, Wall, Slot, DPE0, DPE0, DSE0, Wall, Slot);
  volume.AddRule(tcESE1, Wall, Wall, DPE0, DPE0, DPE0, DSE0, Wall, DPE1);
  volume.AddRule(tcESE1, Wall, DPE0, DSE0, DSE0, Slot, Wall, Wall, DSE1);
  volume.AddRule(tcESE1, Wall, DPE0, Slot, DSE0, DSE0, Wall, Wall, Slot);
  volume.AddRule(tcESE1, Wall, DPE0, DSE0, DSE0, DSE0, LBSU, LKSU, DSE1);
  volume.AddRule(tcESE1, DQSR, DBSR, Slot, DQE0, Slot, DPE0, DQSR, Slot);
  volume.AddRule(tcESE1, Slot, DBNR, Slot, DQE0, Slot, DPE0, DQNR, Slot);
  volume.AddRule(tcESE1, Wall, Wall, DBE0, DOE0, Wall, Wall, Wall, DOE1);

  volume.AddRule(tcELEL, Wall, Wall, Wall, Slot, Wall, Wall, LTSU, LTSL);
  volume.AddRule(tcELEL, LBSU, Wall, Wall, LBSU, Wall, Wall, LBSU, LBSL);
  volume.AddRule(tcELEL, LTSU, Wall, Wall, LBSU, Wall, Wall, LBSU, LBSL);
  volume.AddRule(tcELEL, LPSU, Wall, Wall, LBSU, Wall, Wall, LBSU, LBSL);
  volume.AddRule(tcELEL, LBSU, Wall, Wall, LBSU, Wall, Wall, LHSU, LHSL);
  volume.AddRule(tcELEL, LBSU, DSE0, Wall, LBSU, Wall, Wall, LPSU, LPSL);
  volume.AddRule(tcELEL, LBSU, DSE1, Wall, LBSU, Wall, Wall, LPSU, LPSL);
  volume.AddRule(tcELEL, LBSU, Wall, Wall, LHSU, Wall, Wall, Wall, Slot);
  volume.AddRule(tcELEL, Slot, Wall, Wall, LTSU, Wall, Wall, LBSU, LBSL);
  volume.AddRule(tcELEL, LBSU, LKSU, Wall, LPSU, Wall, Wall, LBSU, LBSL);
  volume.AddRule(tcELEL, DSE0, Wall, Wall, LKSU, Wall, LPSU, Wall, Slot);
  volume.AddRule(tcELEL, DSE1, Wall, Wall, LKSU, Wall, LPSU, Wall, Slot);
  volume.AddRule(tcELEL, Wall, DPE0, DSE0, DSE0, DSE0, LBSU, LKSU, DKE0);
  volume.AddRule(tcELEL, Wall, DPE1, Slot, DSE1, DSE1, LBSU, LKSU, DKE1);

  volume.AddRule(tcSUWL, Wall, Wall, Wall, Slot, Wall, Wall, LHNL, LHNU);
  volume.AddRule(tcSUWL, Wall, Wall, Wall, Slot, Wall, LBNL, DKWR, LKNU);
  volume.AddRule(tcSUWL, LBNL, Wall, Wall, LBNL, Wall, Wall, LBNL, LBNU);
  volume.AddRule(tcSUWL, LHNL, Wall, Wall, LBNL, Wall, Wall, LBNL, LBNU);
  volume.AddRule(tcSUWL, LPNL, Wall, Wall, LBNL, Wall, Wall, LBNL, LBNU);
  volume.AddRule(tcSUWL, LBNL, Wall, Wall, LBNL, Wall, Wall, LTNL, LTNU);
  volume.AddRule(tcSUWL, LBNL, Slot, Wall, LBNL, Wall, Wall, LPNL, LPNU);
  volume.AddRule(tcSUWL, Slot, Wall, Wall, LHNL, Wall, Wall, LBNL, LBNU);
  volume.AddRule(tcSUWL, LBNL, Wall, Wall, LTNL, Wall, Wall, Wall, Slot);
  volume.AddRule(tcSUWL, LBNL, DKWR, Wall, LPNL, Wall, Wall, LBNL, LBNU);
  volume.AddRule(tcSUWL, Slot, DPWR, Slot, DKWR, DSWR, LPNL, Wall, DSWR);

  volume.AddRule(tcSLWL, LTNU, Wall, Wall, Slot, Wall, Wall, Wall, LTNL);
  volume.AddRule(tcSLWL, LBNU, Wall, Wall, LBNU, Wall, Wall, LBNU, LBNL);
  volume.AddRule(tcSLWL, LHNU, Wall, Wall, LBNU, Wall, Wall, LBNU, LHNL);
  volume.AddRule(tcSLWL, LPNU, DSWR, Wall, LBNU, Wall, Wall, LBNU, LPNL);
  volume.AddRule(tcSLWL, LBNU, Wall, Wall, LBNU, Wall, Wall, LTNU, LBNL);
  volume.AddRule(tcSLWL, LBNU, Wall, Wall, LBNU, Wall, Wall, LPNU, LBNL);
  volume.AddRule(tcSLWL, Wall, Wall, Wall, LHNU, Wall, Wall, LBNU, Slot);
  volume.AddRule(tcSLWL, LBNU, Wall, Wall, LTNU, Wall, Wall, Slot, LBNL);
  volume.AddRule(tcSLWL, LBNU, LKNU, Wall, LPNU, Wall, Wall, LBNU, LBNL);
  volume.AddRule(tcSLWL, Wall, Wall, Wall, LKNU, Wall, LPNU, DSWR, Slot);
  volume.AddRule(tcSLWL, LKNU, DPWR, Slot, DSWR, DSWR, LBNU, Wall, DKWR);

  volume.AddRule(tcSUSL, Wall, Wall, Wall, Slot, LHWL, Wall, Wall, LHWU);
  volume.AddRule(tcSUSL, Wall, LBWL, Wall, Slot, DKSR, Wall, Wall, LKWU);
  volume.AddRule(tcSUSL, Wall, Wall, LBWL, LBWL, LBWL, Wall, Wall, LBWU);
  volume.AddRule(tcSUSL, Wall, Wall, LHWL, LBWL, LBWL, Wall, Wall, LBWU);
  volume.AddRule(tcSUSL, Wall, Wall, LPWL, LBWL, LBWL, Wall, Wall, LBWU);
  volume.AddRule(tcSUSL, Wall, Wall, LBWL, LBWL, LTWL, Wall, Wall, LTWU);
  volume.AddRule(tcSUSL, Wall, Wall, LBWL, LBWL, LPWL, Slot, Wall, LPWU);
  volume.AddRule(tcSUSL, Wall, Wall, Slot, LHWL, LBWL, Wall, Wall, LBWU);
  volume.AddRule(tcSUSL, Wall, Wall, LBWL, LTWL, Wall, Wall, Wall, Slot);
  volume.AddRule(tcSUSL, Wall, Wall, LBWL, LPWL, LBWL, DKSR, Wall, LBWU);
  volume.AddRule(tcSUSL, DSSR, LPWL, Slot, DKSR, Wall, DPSR, Slot, DSSR);

  volume.AddRule(tcSSS0, Slot, Wall, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcSSS0, Slot, DBSR, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcSSS0, Slot, DHSR, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcSSS0, Slot, Wall, Wall, Slot, Wall, DPSR, Wall, Slot);
  volume.AddRule(tcSSS0, DHSR, Wall, Wall, Slot, Wall, Wall, Slot, DHS0);
  volume.AddRule(tcSSS0, DQSR, DBSR, Wall, Slot, Wall, Wall, Slot, DQS0);
  volume.AddRule(tcSSS0, DQSR, DBSR, DGE0, Slot, Wall, DBE0, Slot, DQS0);
  volume.AddRule(tcSSS0, DSSR, Wall, Wall, Slot, Wall, DBSR, Slot, DSS0);
  volume.AddRule(tcSSS0, DQSR, DBSR, Slot, Slot, DQE1, DBE1, DQSR, DQS0);
  volume.AddRule(tcSSS0, Slot, DBSR, DGWR, Slot, DQWR, DBWR, Slot, Slot);
  volume.AddRule(tcSSS0, DBSR, Wall, Wall, DBSR, Wall, Wall, DBSR, DBS0);
  volume.AddRule(tcSSS0, DBSR, Wall, Wall, DBSR, Wall, Slot, DBSR, DBS0);
  volume.AddRule(tcSSS0, DPSR, Wall, Wall, DBSR, Wall, Slot, DBSR, DPS0);
  volume.AddRule(tcSSS0, DBSR, Wall, Wall, DBSR, Wall, Slot, DHSR, DBS0);
  volume.AddRule(tcSSS0, DBSR, Wall, Wall, DBSR, Wall, Wall, DPSR, DBS0);
  volume.AddRule(tcSSS0, DPSR, Slot, Wall, DBSR, Wall, Wall, DPSR, DPS0);
  volume.AddRule(tcSSS0, DPSR, Wall, Wall, DBSR, Wall, Slot, DPSR, DPS0);
  volume.AddRule(tcSSS0, DBSR, Wall, Wall, DHSR, Wall, Slot, Slot, DBS0);
  volume.AddRule(tcSSS0, DPSR, DSSR, Wall, DPSR, Wall, Wall, DBSR, DPS0);
  volume.AddRule(tcSSS0, DBSR, Wall, Wall, DPSR, Wall, DQSR, DBSR, DBS0);
  volume.AddRule(tcSSS0, DBSR, Slot, Wall, DPSR, Wall, DQSR, DBSR, DBS0);
  volume.AddRule(tcSSS0, Wall, DSSR, Wall, DPSR, Wall, Wall, DPSR, Slot);
  volume.AddRule(tcSSS0, DPSR, DSSR, Wall, DPSR, Wall, Wall, DPSR, DPS0);
  volume.AddRule(tcSSS0, DSSR, LBWU, LKWU, DSSR, Wall, DPSR, Slot, DSS0);
  volume.AddRule(tcSSS0, Wall, Wall, Wall, DSSR, Wall, DPSR, DSSR, Slot);
  volume.AddRule(tcSSS0, DSSR, Wall, Wall, DSSR, Wall, DPSR, DSSR, DSS0);
  volume.AddRule(tcSSS0, Wall, DPSR, Wall, DQSR, Wall, Wall, Slot, Slot);
  volume.AddRule(tcSSS0, Slot, DPSR, Wall, DQSR, Wall, Wall, Slot, Slot);

  volume.AddRule(tcSSS0, DGE0, DBSR, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcSSS0, DGE0, DBSR, Wall, Slot, Wall, Wall, DQSR, Slot);
  volume.AddRule(tcSSS0, Slot, DBSR, Slot, Slot, DQWR, DBWR, Slot, Slot);
  volume.AddRule(tcSSS0, DPSR, Wall, Wall, DBSR, Wall, DGE0, DBSR, DPS0);
  volume.AddRule(tcSSS0, DBSR, Wall, Wall, DBSR, Wall, Slot, DPSR, DBS0);
  volume.AddRule(tcSSS0, Wall, DPSR, Wall, DQSR, Wall, Wall, DGE0, Slot);
  volume.AddRule(tcSSS0, Slot, DPSR, Wall, DQSR, Wall, Wall, DGE0, Slot);
  volume.AddRule(tcSSS0, DQSR, DBSR, Slot, DGE0, Slot, DPE0, Slot, DJE0);

  volume.AddRule(tcSSS1, Slot, DBS0, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcSSS1, DHS0, Wall, Wall, Slot, Wall, Wall, Wall, DHS1);
  volume.AddRule(tcSSS1, DQS0, DBS0, Wall, Slot, Wall, Wall, Wall, DQS1);
  volume.AddRule(tcSSS1, DSS0, Wall, Wall, Slot, Wall, DBS0, Wall, DSS1);
  volume.AddRule(tcSSS1, Wall, Slot, Wall, Slot, Wall, Wall, DPS0, Slot);
  volume.AddRule(tcSSS1, Wall, Wall, Wall, Slot, Wall, Slot, DSS0, Slot);
  volume.AddRule(tcSSS1, Wall, DBS0, Wall, Slot, Wall, Wall, DQS0, Slot);
  volume.AddRule(tcSSS1, DQS0, DBS0, Wall, Slot, Wall, Wall, DQS0, DQS1);
  volume.AddRule(tcSSS1, DQS0, DBS0, DGWR, Slot, DQWR, DBWR, Slot, DQS1);
  volume.AddRule(tcSSS1, DBS0, Wall, Wall, DBS0, Wall, Wall, DBS0, DBS1);
  volume.AddRule(tcSSS1, DPS0, Wall, Wall, DBS0, Wall, Slot, DBS0, DPS1);
  volume.AddRule(tcSSS1, DBS0, Wall, Wall, DBS0, Wall, Slot, DHS0, DBS1);
  volume.AddRule(tcSSS1, DBS0, Wall, Wall, DBS0, Wall, Slot, DPS0, DBS1);
  volume.AddRule(tcSSS1, DPS0, Wall, Wall, DBS0, Wall, Slot, DPS0, DPS1);
  volume.AddRule(tcSSS1, DPS0, Slot, Wall, DBS0, Wall, Slot, DPS0, DPS1);
  volume.AddRule(tcSSS1, DBS0, Wall, Wall, DHS0, Wall, Wall, Slot, DBS1);
  volume.AddRule(tcSSS1, DPS0, DSS0, Wall, DPS0, Wall, Wall, DBS0, DPS1);
  volume.AddRule(tcSSS1, DBS0, Wall, Wall, DPS0, Wall, DQS0, DBS0, DBS1);
  volume.AddRule(tcSSS1, Slot, DSS0, Wall, DPS0, Wall, Wall, DPS0, Slot);
  volume.AddRule(tcSSS1, DPS0, DSS0, Wall, DPS0, Wall, Wall, DPS0, DPS1);
  volume.AddRule(tcSSS1, DSS0, Wall, Wall, DSS0, Wall, DPS0, Slot, DSS1);
  volume.AddRule(tcSSS1, Slot, Wall, Wall, DSS0, Wall, DPS0, DSS0, Slot);
  volume.AddRule(tcSSS1, DSS0, LBWU, LKWU, DSS0, Wall, DPS0, DSS0, DSS1);
  volume.AddRule(tcSSS1, Slot, DPS0, Wall, DQS0, Wall, Wall, Slot, Slot);
  volume.AddRule(tcSSS1, Slot, DPS0, DGE0, DQS0, Wall, DBE0, Slot, Slot);
  volume.AddRule(tcSSS1, Slot, DPS0, Slot, DQS0, DQE1, DBE1, Slot, Slot);

  volume.AddRule(tcWUNL, Wall, Wall, LHEL, Slot, Wall, Wall, Wall, LHEU);
  volume.AddRule(tcWUNL, Wall, LBEL, DKNR, Slot, Wall, Wall, Wall, LKEU);
  volume.AddRule(tcWUNL, Wall, Wall, LBEL, LBEL, LBEL, Wall, Wall, LBEU);
  volume.AddRule(tcWUNL, Wall, Wall, LTEL, LBEL, LBEL, Wall, Wall, LTEU);
  volume.AddRule(tcWUNL, Wall, Wall, LPEL, LBEL, LBEL, Slot, Wall, LPEU);
  volume.AddRule(tcWUNL, Wall, Wall, LBEL, LBEL, LHEL, Wall, Wall, LBEU);
  volume.AddRule(tcWUNL, Wall, Wall, LBEL, LBEL, LPEL, Wall, Wall, LBEU);
  volume.AddRule(tcWUNL, Wall, Wall, LBEL, LHEL, Slot, Wall, Wall, LBEU);
  volume.AddRule(tcWUNL, Wall, Wall, Wall, LTEL, LBEL, Wall, Wall, Slot);
  volume.AddRule(tcWUNL, Wall, Wall, LBEL, LPEL, LBEL, DKNR, Wall, LBEU);
  volume.AddRule(tcWUNL, Slot, LPEL, Wall, DKNR, Slot, DPNR, DSNR, DSNR);

  volume.AddRule(tcSLSL,
        Wall, Wall,
     LTWU, Slot, Wall,
        Wall, Wall,
     LTWL
    );
  volume.AddRule(tcSLSL,
        Wall, Wall,
     LBWU, LBWU, LBWU,
        Wall, Wall,
     LBWL
    );
  volume.AddRule(tcSLSL,
        Wall, Wall,
     LHWU, LBWU, LBWU,
        Wall, Wall,
     LHWL
    );
  volume.AddRule(tcSLSL,
        Wall, Wall,
     LPWU, LBWU, LBWU,
        DSS0, Wall,
     LPWL
    );
  volume.AddRule(tcSLSL,
        Wall, Wall,
     LPWU, LBWU, LBWU,
        DSS1, Wall,
     LPWL
    );
  volume.AddRule(tcSLSL,
        Wall, Wall,
     LBWU, LBWU, LTWU,
        Wall, Wall,
     LBWL
    );
  volume.AddRule(tcSLSL,
        Wall, Wall,
     LBWU, LBWU, LPWU,
        Wall, Wall,
     Unkn
    );
  volume.AddRule(tcSLSL,
        Wall, Wall,
     Wall, LHWU, LBWU,
        Wall, Wall,
     Unkn
    );
  volume.AddRule(tcSLSL,
        Wall, Wall,
     LBWU, LTWU, Slot,
        Wall, Wall,
     Unkn
    );
  volume.AddRule(tcSLSL,
        Wall, Wall,
     LBWU, LPWU, LBWU,
        LKWU, Wall,
     Unkn
    );
  volume.AddRule(tcSLSL,
        Wall, LPWU,
     Wall, LKWU, DSS0,
        Wall, Wall,
     Unkn
    );
  volume.AddRule(tcSLSL,
        Wall, LPWU,
     Wall, LKWU, DSS1,
        Wall, Wall,
     Unkn
    );
  volume.AddRule(tcSLSL,
        DSS0, LBWU,
     LKWU, DSS0, Wall,
        DPS0, DSS0,
     Unkn
    );
  volume.AddRule(tcSLSL,
        Slot, LBWU,
     LKWU, DSS1, Wall,
        DPS1, DSS1,
     Unkn
    );

#endif // CAM_8_RULES_H
