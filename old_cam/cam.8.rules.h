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

  volume.AddRule(tcSLSL, Wall, Wall, LTWU, Slot, Wall, Wall, Wall, LTWL);
  volume.AddRule(tcSLSL, Wall, Wall, LBWU, LBWU, LBWU, Wall, Wall, LBWL);
  volume.AddRule(tcSLSL, Wall, Wall, LHWU, LBWU, LBWU, Wall, Wall, LHWL);
  volume.AddRule(tcSLSL, Wall, Wall, LPWU, LBWU, LBWU, DSS0, Wall, LPWL);
  volume.AddRule(tcSLSL, Wall, Wall, LPWU, LBWU, LBWU, DSS1, Wall, LPWL);
  volume.AddRule(tcSLSL, Wall, Wall, LBWU, LBWU, LTWU, Wall, Wall, LBWL);
  volume.AddRule(tcSLSL, Wall, Wall, LBWU, LBWU, LPWU, Wall, Wall, LBWL);
  volume.AddRule(tcSLSL, Wall, Wall, Wall, LHWU, LBWU, Wall, Wall, Slot);
  volume.AddRule(tcSLSL, Wall, Wall, LBWU, LTWU, Slot, Wall, Wall, LBWL);
  volume.AddRule(tcSLSL, Wall, Wall, LBWU, LPWU, LBWU, LKWU, Wall, LBWL);
  volume.AddRule(tcSLSL, Wall, LPWU, Wall, LKWU, DSS0, Wall, Wall, Slot);
  volume.AddRule(tcSLSL, Wall, LPWU, Wall, LKWU, DSS1, Wall, Wall, Slot);
  volume.AddRule(tcSLSL, DSS0, LBWU, LKWU, DSS0, Wall, DPS0, DSS0, DKS0);
  volume.AddRule(tcSLSL, Slot, LBWU, LKWU, DSS1, Wall, DPS1, DSS1, DKS1);

  volume.AddRule(tcWLNL, Wall, Wall, Wall, Slot, LTEU, Wall, Wall, LTEL);
  volume.AddRule(tcWLNL, Wall, Wall, LBEU, LBEU, LBEU, Wall, Wall, LBEL);
  volume.AddRule(tcWLNL, Wall, Wall, LTEU, LBEU, LBEU, Wall, Wall, LBEL);
  volume.AddRule(tcWLNL, Wall, Wall, LPEU, LBEU, LBEU, Wall, Wall, LBEL);
  volume.AddRule(tcWLNL, Wall, Wall, LBEU, LBEU, LHEU, Wall, Wall, LHEL);
  volume.AddRule(tcWLNL, Wall, Wall, LBEU, LBEU, LPEU, DSNR, Wall, LPEL);
  volume.AddRule(tcWLNL, Wall, Wall, LBEU, LHEU, Wall, Wall, Wall, Slot);
  volume.AddRule(tcWLNL, Wall, Wall, Slot, LTEU, LBEU, Wall, Wall, LBEL);
  volume.AddRule(tcWLNL, Wall, Wall, LBEU, LPEU, LBEU, LKEU, Wall, LBEL);
  volume.AddRule(tcWLNL, Wall, LPEU, DSNR, LKEU, Wall, Wall, Wall, Slot);
  volume.AddRule(tcWLNL, Slot, LBEU, Wall, DSNR, LKEU, DPNR, DSNR, DKNR);

  volume.AddRule(tcWUWL, Wall, Wall, Wall, Slot, Wall, Wall, LHNL, LHNU);
  volume.AddRule(tcWUWL, Wall, Wall, Wall, Slot, Wall, LBNL, DKWR, LKNU);
  volume.AddRule(tcWUWL, LBNL, Wall, Wall, LBNL, Wall, Wall, LBNL, LBNU);
  volume.AddRule(tcWUWL, LHNL, Wall, Wall, LBNL, Wall, Wall, LBNL, LBNU);
  volume.AddRule(tcWUWL, LPNL, Wall, Wall, LBNL, Wall, Wall, LBNL, LBNU);
  volume.AddRule(tcWUWL, LBNL, Wall, Wall, LBNL, Wall, Wall, LTNL, LTNU);
  volume.AddRule(tcWUWL, LBNL, Slot, Wall, LBNL, Wall, Wall, LPNL, LPNU);
  volume.AddRule(tcWUWL, Slot, Wall, Wall, LHNL, Wall, Wall, LBNL, LBNU);
  volume.AddRule(tcWUWL, LBNL, Wall, Wall, LTNL, Wall, Wall, Wall, Slot);
  volume.AddRule(tcWUWL, LBNL, DKWR, Wall, LPNL, Wall, Wall, LBNL, LBNU);
  volume.AddRule(tcWUWL, Slot, DPWR, Slot, DKWR, DSWR, LPNL, Wall, DSWR);

  volume.AddRule(tcWLWL, LTNU, Wall, Wall, Slot, Wall, Wall, Wall, LTNL);
  volume.AddRule(tcWLWL, LBNU, Wall, Wall, LBNU, Wall, Wall, LBNU, LBNL);
  volume.AddRule(tcWLWL, LHNU, Wall, Wall, LBNU, Wall, Wall, LBNU, LHNL);
  volume.AddRule(tcWLWL, LPNU, DSWR, Wall, LBNU, Wall, Wall, LBNU, LPNL);
  volume.AddRule(tcWLWL, LBNU, Wall, Wall, LBNU, Wall, Wall, LTNU, LBNL);
  volume.AddRule(tcWLWL, LBNU, Wall, Wall, LBNU, Wall, Wall, LPNU, LBNL);
  volume.AddRule(tcWLWL, Wall, Wall, Wall, LHNU, Wall, Wall, LBNU, Slot);
  volume.AddRule(tcWLWL, LBNU, Wall, Wall, LTNU, Wall, Wall, Slot, LBNL);
  volume.AddRule(tcWLWL, LBNU, LKNU, Wall, LPNU, Wall, Wall, LBNU, LBNL);
  volume.AddRule(tcWLWL, Wall, Wall, Wall, LKNU, Wall, LPNU, DSWR, Slot);
  volume.AddRule(tcWLWL, LKNU, DPWR, Slot, DSWR, DSWR, LBNU, Wall, DKWR);

  volume.AddRule(tcNUEL, LHSL, Wall, Wall, Slot, Wall, Wall, Wall, LHSU);
  volume.AddRule(tcNUEL, DKE0, Wall, Wall, Slot, Wall, LBSL, Wall, LKSU);
  volume.AddRule(tcNUEL, DKE1, Wall, Wall, Slot, Wall, LBSL, Wall, LKSU);
  volume.AddRule(tcNUEL, LBSL, Wall, Wall, LBSL, Wall, Wall, LBSL, LBSU);
  volume.AddRule(tcNUEL, LTSL, Wall, Wall, LBSL, Wall, Wall, LBSL, LTSU);
  volume.AddRule(tcNUEL, LPSL, Slot, Wall, LBSL, Wall, Wall, LBSL, LPSU);
  volume.AddRule(tcNUEL, LBSL, Wall, Wall, LBSL, Wall, Wall, LHSL, LBSU);
  volume.AddRule(tcNUEL, LBSL, Wall, Wall, LBSL, Wall, Wall, LPSL, LBSU);
  volume.AddRule(tcNUEL, LBSL, Wall, Wall, LHSL, Wall, Wall, Slot, LBSU);
  volume.AddRule(tcNUEL, Wall, Wall, Wall, LTSL, Wall, Wall, LBSL, Slot);
  volume.AddRule(tcNUEL, LBSL, DKE0, Wall, LPSL, Wall, Wall, LBSL, LBSU);
  volume.AddRule(tcNUEL, LBSL, DKE1, Wall, LPSL, Wall, Wall, LBSL, LBSU);
  volume.AddRule(tcNUEL, Wall, DPE0, DSE0, DKE0, DSE0, LPSL, Slot, DSE0);
  volume.AddRule(tcNUEL, Wall, DPE1, Slot, DKE1, DSE1, LPSL, Slot, DSE1);

  volume.AddRule(tcNRE1, Wall, Wall, DHE0, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcNRE1, Wall, DBE0, DSE0, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcNRE1, Wall, Wall, DJE0, Slot, Wall, DBE0, Wall, Slot);
  volume.AddRule(tcNRE1, Slot, DBS1, DGE0, Slot, Wall, DBE0, DQS1, Slot);
  volume.AddRule(tcNRE1, Wall, Slot, Wall, Slot, Slot, Wall, Wall, Slot);
  volume.AddRule(tcNRE1, Wall, Wall, Wall, Slot, Slot, Slot, Wall, Slot);
  volume.AddRule(tcNRE1, Wall, Wall, DQE1, Slot, Slot, DBE1, Wall, Slot);
  volume.AddRule(tcNRE1, Wall, Wall, Wall, Slot, Slot, DPE1, Wall, Slot);
  volume.AddRule(tcNRE1, Wall, Wall, Slot, Slot, DPE1, Slot, Wall, Slot);
  volume.AddRule(tcNRE1, Wall, DBE0, Wall, Slot, DSE0, Wall, Wall, Slot);
  volume.AddRule(tcNRE1, Wall, Slot, Slot, Slot, DSE1, Wall, Wall, DSE0);
  volume.AddRule(tcNRE1, Slot, DBS1, Slot, Slot, DQE1, DBE1, DQS1, DQE0);
  volume.AddRule(tcNRE1, Slot, DBNR, Slot, Slot, DQE1, DBE1, DQNR, DQE0);
  volume.AddRule(tcNRE1, Wall, Wall, Wall, Slot, DGE0, DBE0, Wall, Slot);
  volume.AddRule(tcNRE1, Wall, Wall, Wall, Slot, DJE0, DBE0, Wall, Slot);
  volume.AddRule(tcNRE1, Wall, Wall, DBE0, DBE0, DBE0, Wall, Wall, DBE0);
  volume.AddRule(tcNRE1, Wall, Slot, DPE0, DBE0, DBE0, Wall, Wall, DBE0);
  volume.AddRule(tcNRE1, Wall, Wall, DIE0, DBE0, DBE0, Wall, Wall, DBE0);
  volume.AddRule(tcNRE1, Wall, Wall, DPE0, DBE0, DBE0, Slot, Wall, DBE0);
  volume.AddRule(tcNRE1, Wall, Slot, DPE0, DBE0, DHE0, Wall, Wall, DBE0);
  volume.AddRule(tcNRE1, Wall, Slot, DBE0, DBE0, DPE0, Wall, Wall, DBE0);
  volume.AddRule(tcNRE1, Wall, Wall, DBE0, DBE0, DPE0, Slot, Wall, DBE0);
  volume.AddRule(tcNRE1, Wall, Wall, DBE1, DBE1, DBE1, Wall, Wall, DBE0);
  volume.AddRule(tcNRE1, Wall, Wall, DPE1, DBE1, DBE1, Wall, Wall, DBE0);
  volume.AddRule(tcNRE1, Wall, Slot, DPE1, DBE1, DBE1, Wall, Wall, DBE0);
  volume.AddRule(tcNRE1, Wall, Slot, DBE1, DBE1, DPE1, Wall, Wall, DPE0);
  volume.AddRule(tcNRE1, Wall, Slot, DPE1, DBE1, DPE1, Wall, Wall, DPE0);
  volume.AddRule(tcNRE1, Wall, Wall, DBE1, DBE1, DOE1, Wall, Wall, DBE0);
  volume.AddRule(tcNRE1, Wall, Wall, DBE0, DHE0, Slot, Wall, Wall, DHE0);
  volume.AddRule(tcNRE1, Wall, DGE0, DBE0, DPE0, DBE0, Wall, Wall, DPE0);
  volume.AddRule(tcNRE1, Wall, DJE0, DBE0, DPE0, DBE0, Wall, Wall, DPE0);
  volume.AddRule(tcNRE1, Wall, Wall, DPE0, DPE0, DBE0, DSE0, Wall, DPE0);
  volume.AddRule(tcNRE1, Wall, Wall, DBE0, DPE0, DPE0, DSE0, Wall, DPE0);
  volume.AddRule(tcNRE1, Wall, Wall, DPE0, DPE0, DPE0, DSE0, Wall, DPE0);
  volume.AddRule(tcNRE1, Wall, DQE1, DBE1, DPE1, DBE1, Wall, Wall, DBE0);
  volume.AddRule(tcNRE1, Wall, Slot, DPE1, DPE1, DBE1, DSE1, Wall, DBE0);
  volume.AddRule(tcNRE1, Wall, Wall, Slot, DPE1, DPE1, DSE1, Wall, DPE0);
  volume.AddRule(tcNRE1, Wall, Wall, DPE1, DPE1, DPE1, DSE1, Wall, DPE0);
  volume.AddRule(tcNRE1, Wall, DPE0, DSE0, DSE0, Slot, Wall, Wall, DSE0);
  volume.AddRule(tcNRE1, Wall, DPE0, Slot, DSE0, DSE0, Wall, Wall, DSE0);
  volume.AddRule(tcNRE1, Wall, DPE0, DSE0, DSE0, DSE0, LBSU, LKSU, DSE0);
  volume.AddRule(tcNRE1, Wall, DPE1, DSE1, DSE1, Wall, Wall, Wall, Slot);
  volume.AddRule(tcNRE1, Wall, DPE1, DSE1, DSE1, DSE1, Wall, Wall, DSE0);
  volume.AddRule(tcNRE1, Wall, DPE1, Slot, DSE1, DSE1, LBSU, LKSU, DSE0);
  volume.AddRule(tcNRE1, Wall, Wall, Slot, DQE1, Wall, DPE1, Wall, Slot);
  volume.AddRule(tcNRE1, Wall, Wall, Slot, DQE1, Slot, DPE1, Wall, Slot);
  volume.AddRule(tcNRE1, Wall, Wall, Slot, DGE0, Slot, DPE0, Wall, DGE0);
  volume.AddRule(tcNRE1, Slot, DPS0, Slot, DJE0, Slot, DPE0, Slot, DJE0);
  volume.AddRule(tcNRE1, Wall, Wall, Wall, DIE0, DBE0, Wall, Wall, DIE0);
  volume.AddRule(tcNRE1, Wall, Wall, DBE1, DOE1, Wall, Wall, Wall, DOE0);

  volume.AddRule(tcNRE0, Wall, Wall, DHE0, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcNRE0, Wall, DBE0, DSE0, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcNRE0, Wall, Wall, DQE0, Slot, Wall, DBE0, Wall, Slot);
  volume.AddRule(tcNRE0, Wall, Wall, DJE0, Slot, Wall, DBE0, Wall, Slot);
  volume.AddRule(tcNRE0, Slot, DBS1, DGE0, Slot, Wall, DBE0, DQS1, Slot);
  volume.AddRule(tcNRE0, Wall, Wall, Wall, Slot, Slot, Slot, Wall, Slot);
  volume.AddRule(tcNRE0, Wall, Wall, DQE0, Slot, Slot, DBE0, Wall, Slot);
  volume.AddRule(tcNRE0, Wall, Wall, Slot, Slot, DPE0, DSE0, Wall, DPER);
  volume.AddRule(tcNRE0, Wall, Slot, Wall, Slot, DSE0, Wall, Wall, DSER);
  volume.AddRule(tcNRE0, Wall, DBE0, Wall, Slot, DSE0, Wall, Wall, DSER);
  volume.AddRule(tcNRE0, Wall, Wall, Wall, Slot, DQE0, DBE0, Wall, DQER);
  volume.AddRule(tcNRE0, Wall, Wall, Slot, Slot, DQE0, DBE0, Wall, DQER);
  volume.AddRule(tcNRE0, Wall, Wall, Wall, Slot, DGE0, DBE0, Wall, DGER);
  volume.AddRule(tcNRE0, Wall, Wall, Wall, Slot, DJE0, DBE0, Wall, DGER);
  volume.AddRule(tcNRE0, Wall, Wall, DBE0, DBE0, DBE0, Wall, Wall, DBER);
  volume.AddRule(tcNRE0, Wall, Slot, DPE0, DBE0, DBE0, Wall, Wall, DBER);
  volume.AddRule(tcNRE0, Wall, Wall, DIE0, DBE0, DBE0, Wall, Wall, DBER);
  volume.AddRule(tcNRE0, Wall, Wall, DPE0, DBE0, DBE0, Slot, Wall, DBER);
  volume.AddRule(tcNRE0, Wall, Slot, DPE0, DBE0, DHE0, Wall, Wall, DHER);
  volume.AddRule(tcNRE0, Wall, Slot, DBE0, DBE0, DPE0, Wall, Wall, DPER);
  volume.AddRule(tcNRE0, Wall, Wall, DBE0, DBE0, DPE0, Slot, Wall, DPER);
  volume.AddRule(tcNRE0, Wall, Slot, DPE0, DBE0, DPE0, Slot, Wall, DPER);
  volume.AddRule(tcNRE0, Wall, Wall, DBE0, DBE0, DOE0, Wall, Wall, DBER);
  volume.AddRule(tcNRE0, Wall, Wall, DBE0, DHE0, Slot, Wall, Wall, Slot);
  volume.AddRule(tcNRE0, Wall, DQE0, DBE0, DPE0, DBE0, Wall, Wall, DBER);
  volume.AddRule(tcNRE0, Wall, DGE0, DBE0, DPE0, DBE0, Wall, Wall, DBER);
  volume.AddRule(tcNRE0, Wall, DJE0, DBE0, DPE0, DBE0, Wall, Wall, DBER);
  volume.AddRule(tcNRE0, Wall, Wall, DPE0, DPE0, DBE0, DSE0, Wall, DBER);
  volume.AddRule(tcNRE0, Wall, Wall, Slot, DPE0, DPE0, DSE0, Wall, DPER);
  volume.AddRule(tcNRE0, Wall, Wall, DBE0, DPE0, DPE0, DSE0, Wall, DPER);
  volume.AddRule(tcNRE0, Wall, Wall, DPE0, DPE0, DPE0, DSE0, Wall, DPER);
  volume.AddRule(tcNRE0, Wall, DPE0, DSE0, DSE0, Slot, Wall, Wall, Slot);
  volume.AddRule(tcNRE0, Wall, Slot, Slot, DSE0, DSE0, Wall, Wall, DSER);
  volume.AddRule(tcNRE0, Wall, DPE0, Slot, DSE0, DSE0, Wall, Wall, DSER);
  volume.AddRule(tcNRE0, Wall, DPE0, DSE0, DSE0, DSE0, LBSU, LKSU, DSER);
  volume.AddRule(tcNRE0, Slot, DBS1, Slot, DQE0, Slot, DPE0, DQS1, Slot);
  volume.AddRule(tcNRE0, Slot, DBNR, Slot, DQE0, Slot, DPE0, DQNR, Slot);
  volume.AddRule(tcNRE0, Wall, Wall, Slot, DGE0, Slot, DPE0, Wall, Slot);
  volume.AddRule(tcNRE0, Slot, DPS0, Slot, DJE0, Slot, DPE0, Slot, DQS0);
  volume.AddRule(tcNRE0, Wall, Wall, Wall, DIE0, DBE0, Wall, Wall, DIER);
  volume.AddRule(tcNRE0, Wall, Wall, DBE0, DOE0, Wall, Wall, Wall, DOER);

  volume.AddRule(tcNLEL, Wall, Wall, Wall, Slot, Wall, Wall, LTSU, LTSL);
  volume.AddRule(tcNLEL, LBSU, Wall, Wall, LBSU, Wall, Wall, LBSU, LBSL);
  volume.AddRule(tcNLEL, LTSU, Wall, Wall, LBSU, Wall, Wall, LBSU, LBSL);
  volume.AddRule(tcNLEL, LPSU, Wall, Wall, LBSU, Wall, Wall, LBSU, LBSL);
  volume.AddRule(tcNLEL, LBSU, Wall, Wall, LBSU, Wall, Wall, LHSU, LHSL);
  volume.AddRule(tcNLEL, LBSU, DSER, Wall, LBSU, Wall, Wall, LPSU, LPSL);
  volume.AddRule(tcNLEL, LBSU, Wall, Wall, LHSU, Wall, Wall, Wall, Slot);
  volume.AddRule(tcNLEL, Slot, Wall, Wall, LTSU, Wall, Wall, LBSU, LBSL);
  volume.AddRule(tcNLEL, LBSU, LKSU, Wall, LPSU, Wall, Wall, LBSU, LBSL);
  volume.AddRule(tcNLEL, DSER, Wall, Wall, LKSU, Wall, LPSU, Wall, Slot);
  volume.AddRule(tcNLEL, Wall, DPER, DSER, DSER, Slot, LBSU, LKSU, DKER);

  volume.AddRule(tcNUNL, Wall, Wall, LHEL, Slot, Wall, Wall, Wall, LHEU);
  volume.AddRule(tcNUNL, Wall, LBEL, DKNR, Slot, Wall, Wall, Wall, LKEU);
  volume.AddRule(tcNUNL, Wall, Wall, LBEL, LBEL, LBEL, Wall, Wall, LBEU);
  volume.AddRule(tcNUNL, Wall, Wall, LTEL, LBEL, LBEL, Wall, Wall, LTEU);
  volume.AddRule(tcNUNL, Wall, Wall, LPEL, LBEL, LBEL, Slot, Wall, LPEU);
  volume.AddRule(tcNUNL, Wall, Wall, LBEL, LBEL, LHEL, Wall, Wall, LBEU);
  volume.AddRule(tcNUNL, Wall, Wall, LBEL, LBEL, LPEL, Wall, Wall, LBEU);
  volume.AddRule(tcNUNL, Wall, Wall, LBEL, LHEL, Slot, Wall, Wall, LBEU);
  volume.AddRule(tcNUNL, Wall, Wall, Wall, LTEL, LBEL, Wall, Wall, Slot);
  volume.AddRule(tcNUNL, Wall, Wall, LBEL, LPEL, LBEL, DKNR, Wall, LBEU);
  volume.AddRule(tcNUNL, Slot, LPEL, Wall, DKNR, Slot, DPNR, DSNR, DSNR);

  volume.AddRule(tcNSN0, Wall, Wall, Wall, Slot, Wall, Wall, Slot, Slot);
  volume.AddRule(tcNSN0, Wall, DHNR, Wall, Slot, Wall, Wall, Slot, Slot);
  volume.AddRule(tcNSN0, DQNR, DBNR, Wall, Slot, Wall, Wall, Slot, Slot);
  volume.AddRule(tcNSN0, Wall, Wall, Wall, Slot, Wall, DPNR, Slot, Slot);
  volume.AddRule(tcNSN0, Slot, Wall, Wall, Slot, Wall, Wall, DHNR, DHN0);
  volume.AddRule(tcNSN0, Slot, Wall, Wall, Slot, Wall, DBNR, DSNR, DSN0);
  volume.AddRule(tcNSN0, Slot, DBNR, DQER, Slot, Slot, DBER, DQNR, DQN0);
  volume.AddRule(tcNSN0, Slot, DBNR, Slot, Slot, Slot, DBWR, DQNR, DQN0);
  volume.AddRule(tcNSN0, DPNR, Wall, Wall, DBNR, Wall, Slot, DBNR, DBN0);
  volume.AddRule(tcNSN0, DPNR, Slot, Wall, DBNR, Wall, Wall, DPNR, DPN0);
  volume.AddRule(tcNSN0, DBNR, Wall, Wall, DBNR, Wall, Slot, DPNR, DPN0);
  volume.AddRule(tcNSN0, DHNR, Wall, Wall, DBNR, Wall, Slot, DPNR, DPN0);
  volume.AddRule(tcNSN0, Slot, Wall, Wall, DHNR, Wall, Slot, DBNR, DBN0);
  volume.AddRule(tcNSN0, DPNR, DSNR, Wall, DPNR, Wall, Wall, Wall, Slot);
  volume.AddRule(tcNSN0, DBNR, Wall, Wall, DPNR, Wall, DQNR, DBNR, DBN0);
  volume.AddRule(tcNSN0, DBNR, Slot, Wall, DPNR, Wall, DQNR, DBNR, DBN0);
  volume.AddRule(tcNSN0, DBNR, DSNR, Wall, DPNR, Wall, Wall, DPNR, DPN0);
  volume.AddRule(tcNSN0, DPNR, DSNR, Wall, DPNR, Wall, Wall, DPNR, DPN0);
  volume.AddRule(tcNSN0, DSNR, Wall, Wall, DSNR, Wall, DPNR, Wall, Slot);
  volume.AddRule(tcNSN0, DSNR, Wall, Wall, DSNR, Wall, DPNR, DSNR, DSN0);
  volume.AddRule(tcNSN0, Slot, LBEU, Wall, DSNR, LKEU, DPNR, DSNR, DSN0);
  volume.AddRule(tcNSN0, Slot, DPNR, Wall, DQNR, Wall, Wall, Wall, Slot);
  volume.AddRule(tcNSN0, Slot, DPNR, Wall, DQNR, Wall, Wall, Slot, Slot);

  volume.AddRule(tcNSN1, DPN0, Slot, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcNSN1, DQN0, DBN0, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcNSN1, DSN0, Wall, Wall, Slot, Wall, Slot, Wall, Slot);
  volume.AddRule(tcNSN1, DQN0, DBN0, Wall, Slot, Wall, Wall, Slot, Slot);
  volume.AddRule(tcNSN1, Wall, Wall, Wall, Slot, Wall, Wall, DHN0, DHN1);
  volume.AddRule(tcNSN1, Wall, Wall, Wall, Slot, Wall, DBN0, DSN0, DSN1);
  volume.AddRule(tcNSN1, Wall, DBN0, Wall, Slot, Wall, Wall, DQN0, DQN1);
  volume.AddRule(tcNSN1, Slot, DBN0, Wall, Slot, Wall, Wall, DQN0, DQN1);
  volume.AddRule(tcNSN1, DPN0, Wall, Wall, DBN0, Wall, Slot, DBN0, DBN1);
  volume.AddRule(tcNSN1, DBN0, Wall, Wall, DBN0, Wall, Slot, DPN0, DPN1);
  volume.AddRule(tcNSN1, DHN0, Wall, Wall, DBN0, Wall, Slot, DPN0, DPN1);
  volume.AddRule(tcNSN1, DPN0, Slot, Wall, DBN0, Wall, Slot, DPN0, DPN1);
  volume.AddRule(tcNSN1, Slot, Wall, Wall, DHN0, Wall, Wall, DBN0, DBN1);
  volume.AddRule(tcNSN1, DPN0, DSN0, Wall, DPN0, Wall, Wall, Slot, Slot);
  volume.AddRule(tcNSN1, DBN0, Wall, Wall, DPN0, Wall, DQN0, DBN0, DBN1);
  volume.AddRule(tcNSN1, DBN0, DSN0, Wall, DPN0, Wall, Wall, DPN0, DPN1);
  volume.AddRule(tcNSN1, DPN0, DSN0, Wall, DPN0, Wall, Wall, DPN0, DPN1);
  volume.AddRule(tcNSN1, DSN0, Wall, Wall, DSN0, Wall, DPN0, Slot, Slot);
  volume.AddRule(tcNSN1, Slot, Wall, Wall, DSN0, Wall, DPN0, DSN0, DSN1);
  volume.AddRule(tcNSN1, DSN0, LBEU, Wall, DSN0, LKEU, DPN0, DSN0, DSN1);
  volume.AddRule(tcNSN1, Slot, DPN0, DQER, DQN0, Slot, DBER, Slot, Slot);
  volume.AddRule(tcNSN1, Slot, DPN0, Slot, DQN0, Slot, DBWR, Slot, Slot);

  volume.AddRule(tcNLNL, Wall, Wall, Wall, Slot, LTEU, Wall, Wall, LTEL);
  volume.AddRule(tcNLNL, Wall, Wall, LBEU, LBEU, LBEU, Wall, Wall, LBEL);
  volume.AddRule(tcNLNL, Wall, Wall, LTEU, LBEU, LBEU, Wall, Wall, LBEL);
  volume.AddRule(tcNLNL, Wall, Wall, LPEU, LBEU, LBEU, Wall, Wall, LBEL);
  volume.AddRule(tcNLNL, Wall, Wall, LBEU, LBEU, LHEU, Wall, Wall, LHEL);
  volume.AddRule(tcNLNL, Wall, Wall, LBEU, LBEU, LPEU, DSN1, Wall, LPEL);
  volume.AddRule(tcNLNL, Wall, Wall, LBEU, LHEU, Wall, Wall, Wall, Slot);
  volume.AddRule(tcNLNL, Wall, Wall, Slot, LTEU, LBEU, Wall, Wall, LBEL);
  volume.AddRule(tcNLNL, Wall, Wall, LBEU, LPEU, LBEU, LKEU, Wall, LBEL);
  volume.AddRule(tcNLNL, Wall, LPEU, DSN1, LKEU, Wall, Wall, Wall, Slot);
  volume.AddRule(tcNLNL, DSN1, LBEU, Wall, DSN1, LKEU, DPN1, Slot, DKN1);

  volume.AddRule(tcEUSL, Wall, LBWL, Wall, Slot, DKS0, Wall, Wall, LKWU);
  volume.AddRule(tcEUSL, Wall, LBWL, Wall, Slot, DKS1, Wall, Wall, LKWU);
  volume.AddRule(tcEUSL, Wall, Wall, LBWL, LPWL, LBWL, DKS0, Wall, LBWU);
  volume.AddRule(tcEUSL, Wall, Wall, LBWL, LPWL, LBWL, DKS1, Wall, LBWU);
  volume.AddRule(tcEUSL, DSS0, LPWL, Slot, DKS0, Wall, DPS0, DSS0, DSS0);
  volume.AddRule(tcEUSL, Slot, LPWL, Slot, DKS1, Wall, DPS1, DSS1, DSS1);

  volume.AddRule(tcERS1, Slot, DBS0, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcERS1, DHS0, Wall, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcERS1, DQS1, DBS1, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcERS1, DGS0, DBS0, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcERS1, DSS0, Wall, Wall, Slot, Wall, DBS0, Wall, Slot);
  volume.AddRule(tcERS1, Wall, Slot, Wall, Slot, Wall, Wall, Slot, Slot);
  volume.AddRule(tcERS1, Wall, DBS1, Wall, Slot, Wall, Wall, Slot, Slot);
  volume.AddRule(tcERS1, Wall, DPS1, Wall, Slot, Wall, Wall, Slot, Slot);
  volume.AddRule(tcERS1, DGS0, DBS0, Wall, Slot, Wall, Wall, Slot, Slot);
  volume.AddRule(tcERS1, Wall, Wall, Wall, Slot, Wall, Slot, Slot, Slot);
  volume.AddRule(tcERS1, Wall, Slot, Wall, Slot, Wall, Wall, DPS0, Slot);
  volume.AddRule(tcERS1, Slot, Slot, Wall, Slot, Wall, Wall, DPS1, DPS0);
  volume.AddRule(tcERS1, Wall, Wall, Wall, Slot, Wall, Slot, DSS0, Slot);
  volume.AddRule(tcERS1, Slot, Wall, Wall, Slot, Wall, Slot, DSS1, DSS0);
  volume.AddRule(tcERS1, Wall, DBS0, Wall, Slot, Wall, Wall, DQS0, Slot);
  volume.AddRule(tcERS1, DQS1, DBS1, Wall, Slot, Wall, Wall, DQS1, DQS0);
  volume.AddRule(tcERS1, Slot, DBS1, Slot, Slot, Wall, DBER, DQS1, DQS0);
  volume.AddRule(tcERS1, Wall, DBS0, Wall, Slot, Wall, Wall, DGS0, Slot);
  volume.AddRule(tcERS1, Slot, DBS0, Wall, Slot, Wall, Wall, DGS0, Slot);
  volume.AddRule(tcERS1, Slot, DBS1, DQER, Slot, Slot, DBER, DQS1, DQS0);
  volume.AddRule(tcERS1, DQS0, DBS0, Slot, Slot, DQWR, DBWR, Slot, Slot);
  volume.AddRule(tcERS1, DBS0, Wall, Wall, DBS0, Wall, Wall, DBS0, DBS0);
  volume.AddRule(tcERS1, DPS0, Wall, Wall, DBS0, Wall, Slot, DBS0, DBS0);
  volume.AddRule(tcERS1, DBS0, Wall, Wall, DBS0, Wall, Slot, DHS0, DBS0);
  volume.AddRule(tcERS1, DBS0, Wall, Wall, DBS0, Wall, Slot, DPS0, DBS0);
  volume.AddRule(tcERS1, DPS0, Slot, Wall, DBS0, Wall, Slot, DPS0, DBS0);
  volume.AddRule(tcERS1, DBS1, Wall, Wall, DBS1, Wall, Wall, DBS1, DBS0);
  volume.AddRule(tcERS1, DPS1, Wall, Wall, DBS1, Wall, Wall, DBS1, DBS0);
  volume.AddRule(tcERS1, DBS1, Wall, Wall, DBS1, Wall, Slot, DBS1, DBS0);
  volume.AddRule(tcERS1, DPS1, Wall, Wall, DBS1, Wall, Slot, DBS1, DBS0);
  volume.AddRule(tcERS1, DBS1, Wall, Wall, DBS1, Wall, Wall, DHS1, DHS0);
  volume.AddRule(tcERS1, DBS1, Wall, Wall, DBS1, Wall, Slot, DPS1, DPS0);
  volume.AddRule(tcERS1, DPS1, Wall, Wall, DBS1, Wall, Slot, DPS1, DPS0);
  volume.AddRule(tcERS1, DBS0, Wall, Wall, DHS0, Wall, Wall, Slot, DHS0);
  volume.AddRule(tcERS1, DBS1, Wall, Wall, DHS1, Wall, Wall, Wall, Slot);
  volume.AddRule(tcERS1, DPS0, DSS0, Wall, DPS0, Wall, Wall, DBS0, DPS0);
  volume.AddRule(tcERS1, DBS0, Wall, Wall, DPS0, Wall, DQS0, DBS0, DPS0);
  volume.AddRule(tcERS1, DBS0, Wall, Wall, DPS0, Wall, DGS0, DBS0, DPS0);
  volume.AddRule(tcERS1, Slot, DSS0, Wall, DPS0, Wall, Wall, DPS0, DPS0);
  volume.AddRule(tcERS1, DPS0, DSS0, Wall, DPS0, Wall, Wall, DPS0, DPS0);
  volume.AddRule(tcERS1, DPS1, DSS1, Wall, DPS1, Wall, Slot, DBS1, DBS0);
  volume.AddRule(tcERS1, DBS1, Wall, Wall, DPS1, Wall, DQS1, DBS1, DBS0);
  volume.AddRule(tcERS1, Slot, DSS1, Wall, DPS1, Wall, Wall, DPS1, DPS0);
  volume.AddRule(tcERS1, DPS1, DSS1, Wall, DPS1, Wall, Wall, DPS1, DPS0);
  volume.AddRule(tcERS1, DSS0, Wall, Wall, DSS0, Wall, DPS0, Slot, DSS0);
  volume.AddRule(tcERS1, Slot, Wall, Wall, DSS0, Wall, DPS0, DSS0, DSS0);
  volume.AddRule(tcERS1, DSS0, LBWU, LKWU, DSS0, Wall, DPS0, DSS0, DSS0);
  volume.AddRule(tcERS1, DSS1, Wall, Wall, DSS1, Wall, DPS1, Wall, Slot);
  volume.AddRule(tcERS1, DSS1, Wall, Wall, DSS1, Wall, DPS1, DSS1, DSS0);
  volume.AddRule(tcERS1, Slot, LBWU, LKWU, DSS1, Wall, DPS1, DSS1, DSS0);
  volume.AddRule(tcERS1, Slot, DPS0, Wall, DQS0, Wall, Wall, Slot, DQS0);
  volume.AddRule(tcERS1, Slot, DPS1, Wall, DQS1, Wall, Wall, Wall, Slot);
  volume.AddRule(tcERS1, Slot, DPS1, Wall, DQS1, Wall, Wall, Slot, Slot);
  volume.AddRule(tcERS1, Slot, DPS1, DGWR, DQS1, DQWR, DBWR, Slot, Slot);
  volume.AddRule(tcERS1, Slot, DPS0, DGER, DGS0, Slot, DBER, Slot, DGS0);

  volume.AddRule(tcERS0, Slot, DBS0, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcERS0, DHS0, Wall, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcERS0, DQS0, DBS0, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcERS0, DGS0, DBS0, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcERS0, DSS0, Wall, Wall, Slot, Wall, DBS0, Wall, Slot);
  volume.AddRule(tcERS0, DGS0, DBS0, Wall, Slot, Wall, Wall, Slot, Slot);
  volume.AddRule(tcERS0, Wall, Slot, Wall, Slot, Wall, Wall, DPS0, DPSR);
  volume.AddRule(tcERS0, Wall, Wall, Wall, Slot, Wall, Slot, DSS0, DSSR);
  volume.AddRule(tcERS0, Wall, DBS0, Wall, Slot, Wall, Wall, DQS0, DQSR);
  volume.AddRule(tcERS0, DQS0, DBS0, Wall, Slot, Wall, Wall, DQS0, DQSR);
  volume.AddRule(tcERS0, Wall, DBS0, Wall, Slot, Wall, Wall, DGS0, DGSR);
  volume.AddRule(tcERS0, Slot, DBS0, Wall, Slot, Wall, Wall, DGS0, DGSR);
  volume.AddRule(tcERS0, DQS0, DBS0, Slot, Slot, DQWR, DBWR, Slot, Slot);
  volume.AddRule(tcERS0, DQS0, DBS0, DGWR, Slot, DQWR, DBWR, Slot, Slot);
  volume.AddRule(tcERS0, DBS0, Wall, Wall, DBS0, Wall, Wall, DBS0, DBSR);
  volume.AddRule(tcERS0, DPS0, Wall, Wall, DBS0, Wall, Slot, DBS0, DBSR);
  volume.AddRule(tcERS0, DBS0, Wall, Wall, DBS0, Wall, Slot, DHS0, DHSR);
  volume.AddRule(tcERS0, DBS0, Wall, Wall, DBS0, Wall, Slot, DPS0, DPSR);
  volume.AddRule(tcERS0, DPS0, Wall, Wall, DBS0, Wall, Slot, DPS0, DPSR);
  volume.AddRule(tcERS0, DPS0, Slot, Wall, DBS0, Wall, Slot, DPS0, DPSR);
  volume.AddRule(tcERS0, DBS0, Wall, Wall, DHS0, Wall, Wall, Slot, Slot);
  volume.AddRule(tcERS0, DPS0, DSS0, Wall, DPS0, Wall, Wall, DBS0, DBSR);
  volume.AddRule(tcERS0, DBS0, Wall, Wall, DPS0, Wall, DQS0, DBS0, DBSR);
  volume.AddRule(tcERS0, DBS0, Wall, Wall, DPS0, Wall, DGS0, DBS0, DBSR);
  volume.AddRule(tcERS0, Slot, DSS0, Wall, DPS0, Wall, Wall, DPS0, DPSR);
  volume.AddRule(tcERS0, DPS0, DSS0, Wall, DPS0, Wall, Wall, DPS0, DPSR);
  volume.AddRule(tcERS0, DSS0, Wall, Wall, DSS0, Wall, DPS0, Slot, Slot);
  volume.AddRule(tcERS0, Slot, Wall, Wall, DSS0, Wall, DPS0, DSS0, DSSR);
  volume.AddRule(tcERS0, DSS0, LBWU, LKWU, DSS0, Wall, DPS0, DSS0, DSSR);
  volume.AddRule(tcERS0, Slot, DPS0, Wall, DQS0, Wall, Wall, Slot, Slot);
  volume.AddRule(tcERS0, Slot, DPS0, Slot, DQS0, Wall, DBER, Slot, Slot);
  volume.AddRule(tcERS0, Slot, DPS0, DQER, DQS0, Slot, DBER, Slot, Slot);
  volume.AddRule(tcERS0, Slot, DPS0, DGER, DGS0, Slot, DBER, Slot, Slot);

  volume.AddRule(tcESE0, DQN1, DBN1, DQER, Slot, Slot, DBER, Slot, DQE0);
  volume.AddRule(tcESE0, DGSR, DBSR, DGER, Slot, Slot, DBER, Slot, DGE0);
  volume.AddRule(tcESE0, Wall, Wall, Wall, Slot, DPER, DSER, Wall, Slot);
  volume.AddRule(tcESE0, Wall, Wall, Slot, DPER, DPER, DSER, Wall, Slot);
  volume.AddRule(tcESE0, Wall, Slot, Wall, DSER, DSER, Wall, Wall, Slot);

  volume.AddRule(tcESE1, Wall, Wall, Wall, Slot, Slot, Slot, Wall, Slot);
  volume.AddRule(tcESE1, Wall, Wall, Slot, Slot, DPE0, DSE0, Wall, Slot);
  volume.AddRule(tcESE1, Wall, Slot, Slot, DSE0, DSE0, Wall, Wall, Slot);
  volume.AddRule(tcESE1, DQN1, DBN1, Slot, DQE0, Slot, DPE0, Slot, Slot);

  volume.AddRule(tcELEL, Wall, Slot, Slot, DSE1, DSE1, LBSU, LKSU, DKE1);

  volume.AddRule(tcERS1, DQS0, DBS0, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcERS1, DQS0, DBS0, Wall, Slot, Wall, Wall, Slot, Slot);
  volume.AddRule(tcERS1, Slot, DBS0, Wall, Slot, Wall, Wall, DQS0, Slot);
  volume.AddRule(tcERS1, Slot, DPS0, DGER, DQS0, Slot, DBER, Slot, DQS0);

  volume.AddRule(tcERS0, DQS0, DBS0, Wall, Slot, Wall, Wall, Slot, Slot);
  volume.AddRule(tcERS0, Slot, DBS0, Wall, Slot, Wall, Wall, DQS0, DQSR);
  volume.AddRule(tcERS0, Slot, DPS0, DGER, DQS0, Slot, DBER, Slot, Slot);

  volume.AddRule(tcWUNL, Wall, LBEL, DKN1, Slot, Wall, Wall, Wall, LKEU);
  volume.AddRule(tcWUNL, Wall, Wall, LBEL, LPEL, LBEL, DKN1, Wall, LBEU);
  volume.AddRule(tcWUNL, DSN1, LPEL, Wall, DKN1, Slot, DPN1, Slot, DSN1);

  volume.AddRule(tcWRN1, Slot, Slot, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcWRN1, Slot, DPN1, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcWRN1, Slot, Wall, Wall, Slot, Wall, Slot, Wall, Slot);
  volume.AddRule(tcWRN1, DPN1, Slot, Wall, Slot, Wall, Wall, Slot, DPN0);
  volume.AddRule(tcWRN1, DSN1, Wall, Wall, Slot, Wall, Slot, Slot, DSN0);
  volume.AddRule(tcWRN1, Slot, DBN1, Wall, Slot, Wall, Wall, DQN1, Slot);
  volume.AddRule(tcWRN1, DQN1, DBN1, Slot, Slot, Slot, DBWR, Slot, DQN0);
  volume.AddRule(tcWRN1, DQN1, DBN1, Slot, Slot, DQE1, DBE1, Slot, DQN0);
  volume.AddRule(tcWRN1, DPN1, Wall, Wall, DBN1, Wall, Slot, DBN1, DPN0);
  volume.AddRule(tcWRN1, DHN1, Wall, Wall, DBN1, Wall, Wall, DPN1, DHN0);
  volume.AddRule(tcWRN1, DBN1, Wall, Wall, DBN1, Wall, Slot, DPN1, DBN0);
  volume.AddRule(tcWRN1, DPN1, Wall, Wall, DBN1, Wall, Slot, DPN1, DPN0);
  volume.AddRule(tcWRN1, Wall, Wall, Wall, DHN1, Wall, Wall, DBN1, Slot);
  volume.AddRule(tcWRN1, DPN1, DSN1, Wall, DPN1, Wall, Wall, Slot, DPN0);
  volume.AddRule(tcWRN1, DBN1, Wall, Wall, DPN1, Wall, DQN1, DBN1, DBN0);
  volume.AddRule(tcWRN1, DPN1, DSN1, Wall, DPN1, Wall, Wall, DPN1, DPN0);
  volume.AddRule(tcWRN1, DBN1, DSN1, Wall, DPN1, Wall, Slot, DPN1, DBN0);
  volume.AddRule(tcWRN1, Wall, Wall, Wall, DSN1, Wall, DPN1, DSN1, Slot);
  volume.AddRule(tcWRN1, DSN1, Wall, Wall, DSN1, Wall, DPN1, DSN1, DSN0);
  volume.AddRule(tcWRN1, DSN1, LBEU, Wall, DSN1, LKEU, DPN1, Slot, DSN0);
  volume.AddRule(tcWRN1, Wall, DPN1, Wall, DQN1, Wall, Wall, Slot, Slot);
  volume.AddRule(tcWRN1, Slot, DPN1, Wall, DQN1, Wall, Wall, Slot, Slot);

  volume.AddRule(tcWRN0, DPN0, Slot, Wall, Slot, Wall, Wall, Wall, DPNR);
  volume.AddRule(tcWRN0, DQN0, DBN0, Wall, Slot, Wall, Wall, Wall, DQNR);
  volume.AddRule(tcWRN0, DSN0, Wall, Wall, Slot, Wall, Slot, Wall, DSNR);
  volume.AddRule(tcWRN0, DQN0, DBN0, Wall, Slot, Wall, Wall, Slot, DQNR);
  volume.AddRule(tcWRN0, Wall, Wall, Wall, Slot, Wall, Wall, DHN0, Slot);
  volume.AddRule(tcWRN0, Wall, Wall, Wall, Slot, Wall, DBN0, DSN0, Slot);
  volume.AddRule(tcWRN0, Wall, DBN0, Wall, Slot, Wall, Wall, DQN0, Slot);
  volume.AddRule(tcWRN0, Slot, DBN0, Wall, Slot, Wall, Wall, DQN0, Slot);
  volume.AddRule(tcWRN0, DPN0, Wall, Wall, DBN0, Wall, Slot, DBN0, DPNR);
  volume.AddRule(tcWRN0, DBN0, Wall, Wall, DBN0, Wall, Slot, DPN0, DBNR);
  volume.AddRule(tcWRN0, DHN0, Wall, Wall, DBN0, Wall, Slot, DPN0, DHNR);
  volume.AddRule(tcWRN0, DPN0, Slot, Wall, DBN0, Wall, Slot, DPN0, DPNR);
  volume.AddRule(tcWRN0, Slot, Wall, Wall, DHN0, Wall, Wall, DBN0, Slot);
  volume.AddRule(tcWRN0, DPN0, DSN0, Wall, DPN0, Wall, Wall, Slot, DPNR);
  volume.AddRule(tcWRN0, DBN0, Wall, Wall, DPN0, Wall, DQN0, DBN0, DBNR);
  volume.AddRule(tcWRN0, DBN0, DSN0, Wall, DPN0, Wall, Wall, DPN0, DBNR);
  volume.AddRule(tcWRN0, DPN0, DSN0, Wall, DPN0, Wall, Wall, DPN0, DPNR);
  volume.AddRule(tcWRN0, DSN0, Wall, Wall, DSN0, Wall, DPN0, Slot, DSNR);
  volume.AddRule(tcWRN0, Slot, Wall, Wall, DSN0, Wall, DPN0, DSN0, Slot);
  volume.AddRule(tcWRN0, DSN0, LBEU, Wall, DSN0, LKEU, DPN0, DSN0, DSNR);
  volume.AddRule(tcWRN0, Slot, DPN0, Slot, DQN0, Slot, DBWR, Slot, Slot);
  volume.AddRule(tcWRN0, Slot, DPN0, Slot, DQN0, DQE1, DBE1, Slot, Slot);

  volume.AddRule(tcESE0, DQSR, DBSR, DGER, Slot, Slot, DBER, DGSR, DGE0);
  volume.AddRule(tcESE0, Slot, DBNR, DQER, Slot, Slot, DBER, DGNR, DQE0);
  volume.AddRule(tcESE0, Wall, Wall, Slot, Slot, DGER, DBER, Wall, Slot);
  volume.AddRule(tcESE0, Wall, Wall, Slot, DGER, Slot, DPER, Wall, Slot);

  volume.AddRule(tcESE1, Wall, Wall, DGE0, Slot, Wall, DBE0, Wall, DGE1);
  volume.AddRule(tcESE1, Wall, Wall, Slot, Slot, DGE0, DBE0, Wall, Slot);
  volume.AddRule(tcESE1, Wall, DGE0, DBE0, DPE0, DBE0, Wall, Wall, DBE1);
  volume.AddRule(tcESE1, Slot, DBNR, Slot, DQE0, Slot, DPE0, DGNR, Slot);
  volume.AddRule(tcESE1, DQSR, DBSR, Slot, DGE0, Slot, DPE0, DGSR, Slot);

  volume.AddRule(tcSSS0, DGSR, DBSR, Wall, Slot, Wall, Wall, Slot, DGS0);
  volume.AddRule(tcSSS0, DQSR, DBSR, Slot, Slot, DGE1, DBE1, DGSR, DQS0);
  volume.AddRule(tcSSS0, DBSR, Wall, Wall, DPSR, Wall, DGSR, DBSR, DBS0);
  volume.AddRule(tcSSS0, Wall, DPSR, Wall, DGSR, Wall, Wall, Slot, Slot);
  volume.AddRule(tcSSS0, Slot, DPSR, Wall, DGSR, Wall, Wall, Slot, Slot);

  volume.AddRule(tcSSS1, DQS0, DBS0, Wall, Slot, Wall, Wall, DGS0, DQS1);
  volume.AddRule(tcSSS1, DGS0, DBS0, DGWR, Slot, DQWR, DBWR, Slot, DGS1);
  volume.AddRule(tcSSS1, DBS0, Wall, Wall, DPS0, Wall, DGS0, DBS0, DBS1);
  volume.AddRule(tcSSS1, Slot, DPS0, Slot, DQS0, DGE1, DBE1, Slot, Slot);
  volume.AddRule(tcSSS1, Slot, DPS0, Wall, DGS0, Wall, Wall, Slot, Slot);

  volume.AddRule(tcWSW0, Wall, Wall, Wall, Slot, Slot, Wall, Wall, Slot);
  volume.AddRule(tcWSW0, Wall, DPWR, Wall, Slot, Slot, Wall, Wall, Slot);
  volume.AddRule(tcWSW0, Wall, Wall, DQWR, Slot, Slot, DBWR, Wall, Slot);
  volume.AddRule(tcWSW0, Wall, Wall, Wall, Slot, Slot, DHWR, Wall, Slot);
  volume.AddRule(tcWSW0, Slot, DBNR, Slot, Slot, Slot, DBWR, DQNR, Slot);
  volume.AddRule(tcWSW0, Wall, Wall, Slot, Slot, DHWR, Wall, Wall, DHW0);
  volume.AddRule(tcWSW0, Wall, DBWR, Slot, Slot, DSWR, Wall, Wall, DSW0);
  volume.AddRule(tcWSW0, DGS0, DBS0, Slot, Slot, DQWR, DBWR, Slot, DQW0);
  volume.AddRule(tcWSW0, Wall, Wall, Slot, Slot, DGWR, DBWR, Wall, DGW0);
  volume.AddRule(tcWSW0, Wall, Slot, DHWR, DBWR, DBWR, Wall, Wall, DBW0);
  volume.AddRule(tcWSW0, Wall, Slot, DPWR, DBWR, DBWR, Wall, Wall, DBW0);
  volume.AddRule(tcWSW0, Wall, Slot, DBWR, DBWR, DPWR, Wall, Wall, DPW0);
  volume.AddRule(tcWSW0, Wall, DGS1, DPWR, DBWR, DPWR, Wall, Wall, DPW0);
  volume.AddRule(tcWSW0, Wall, Wall, DPWR, DBWR, DPWR, Slot, Wall, DPW0);
  volume.AddRule(tcWSW0, Wall, Slot, Slot, DHWR, DBWR, Wall, Wall, DBW0);
  volume.AddRule(tcWSW0, Wall, Wall, DPWR, DPWR, Wall, DSWR, Wall, Slot);
  volume.AddRule(tcWSW0, Wall, DQWR, DBWR, DPWR, DBWR, Wall, Wall, DBW0);
  volume.AddRule(tcWSW0, Wall, DGWR, DBWR, DPWR, DBWR, Wall, Wall, DBW0);
  volume.AddRule(tcWSW0, Wall, DQWR, DBWR, DPWR, DBWR, Slot, Wall, DBW0);
  volume.AddRule(tcWSW0, Wall, Wall, DBWR, DPWR, DPWR, DSWR, Wall, DPW0);
  volume.AddRule(tcWSW0, Wall, Wall, DPWR, DPWR, DPWR, DSWR, Wall, DPW0);
  volume.AddRule(tcWSW0, Wall, DPWR, DSWR, DSWR, Wall, Wall, Wall, Slot);
  volume.AddRule(tcWSW0, Wall, DPWR, DSWR, DSWR, DSWR, Wall, Wall, DSW0);
  volume.AddRule(tcWSW0, LKNU, DPWR, Slot, DSWR, DSWR, LBNU, Wall, DSW0);
  volume.AddRule(tcWSW0, Wall, Wall, Slot, DQWR, Wall, DPWR, Wall, Slot);
  volume.AddRule(tcWSW0, Wall, Wall, DGS1, DQWR, Slot, DPWR, Wall, Slot);
  volume.AddRule(tcWSW0, Slot, DPS1, DGWR, DGS1, DQWR, DBWR, Slot, DJS1);
  volume.AddRule(tcWSW0, Wall, Wall, Slot, DGWR, DGS1, DPWR, Wall, Slot);

  volume.AddRule(tcWLWL, LPNU, DSW0, Wall, LBNU, Wall, Wall, LBNU, LPNL);
  volume.AddRule(tcWLWL, Wall, Wall, Wall, LKNU, Wall, LPNU, DSW0, Slot);
  volume.AddRule(tcWLWL, LKNU, DPW0, DSW0, DSW0, DSW0, LBNU, Wall, DKS0);

  volume.AddRule(tcNRE1, Slot, DBNR, Slot, Slot, DQE1, DBE1, DGNR, DQE0);
  volume.AddRule(tcNRE1, Slot, DBS1, Slot, Slot, DGE1, DBE1, DQS1, DGE0);
  volume.AddRule(tcNRE1, Wall, DGE1, DBE1, DPE1, DBE1, Wall, Wall, DBE0);
  volume.AddRule(tcNRE1, Wall, Wall, Slot, DGE1, Wall, DPE1, Wall, Slot);

  volume.AddRule(tcNRE0, Wall, Wall, DGE0, Slot, Wall, DBE0, Wall, Slot);
  volume.AddRule(tcNRE0, Wall, Wall, Slot, Slot, DGE0, DBE0, Wall, DGER);
  volume.AddRule(tcNRE0, Slot, DBNR, Slot, DQE0, Slot, DPE0, DGNR, Slot);
  volume.AddRule(tcNRE0, Slot, DBS1, Slot, DGE0, Slot, DPE0, DQS1, Slot);

  volume.AddRule(tcNSN0, DGNR, DBNR, Wall, Slot, Wall, Wall, Slot, Slot);
  volume.AddRule(tcNSN0, Slot, DBNR, DQER, Slot, Slot, DBER, DGNR, DGN0);
  volume.AddRule(tcNSN0, Slot, DBNR, Slot, Slot, DGW0, DBW0, DQNR, DQN0);
  volume.AddRule(tcNSN0, DBNR, Wall, Wall, DPNR, Wall, DGNR, DBNR, DBN0);
  volume.AddRule(tcNSN0, Slot, DPNR, Wall, DGNR, Wall, Wall, Slot, Slot);

  volume.AddRule(tcNSN1, DGN0, DBN0, Wall, Slot, Wall, Wall, Slot, Slot);
  volume.AddRule(tcNSN1, Wall, DBN0, Wall, Slot, Wall, Wall, DGN0, DGN1);
  volume.AddRule(tcNSN1, DBN0, Wall, Wall, DPN0, Wall, DGN0, DBN0, DBN1);
  volume.AddRule(tcNSN1, Slot, DPN0, Slot, DQN0, DGW0, DBW0, Slot, Slot);
  volume.AddRule(tcNSN1, Slot, DPN0, DQER, DGN0, Slot, DBER, Slot, Slot);

  volume.AddRule(tcERS1, DQW0, DBS0, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcERS1, DJS1, DBS1, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcERS1, DQS1, DBS1, Wall, Slot, Wall, Wall, DJS1, DGS0);
  volume.AddRule(tcERS1, Slot, DBS1, DGER, Slot, Slot, DBER, DQS1, DQS0);
  volume.AddRule(tcERS1, DPS0, Wall, Wall, DBS0, Wall, DQW0, DBS0, DBS0);
  volume.AddRule(tcERS1, DBS1, Wall, Wall, DPS1, Wall, DJS1, DBS1, DBS0);
  volume.AddRule(tcERS1, DGS0, DBS0, Slot, DQW0, Slot, DPW0, Slot, DQW0);
  volume.AddRule(tcERS1, Slot, DPS0, Wall, DGS0, Wall, Wall, DQW0, DGS0);
  volume.AddRule(tcERS1, Slot, DPS1, Slot, DJS1, Slot, DPW0, Slot, DQW0);

  volume.AddRule(tcERS0, DQW0, DBS0, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcERS0, DQS0, DBS0, Wall, Slot, Wall, Wall, DGS0, DGSR);
  volume.AddRule(tcERS0, DPS0, Wall, Wall, DBS0, Wall, DQS0, DBS0, DBSR);
  volume.AddRule(tcERS0, DPS0, Wall, Wall, DBS0, Wall, DQW0, DBS0, DBSR);
  volume.AddRule(tcERS0, DGS0, DBS0, Slot, DQS0, Slot, DPW0, Slot, Slot);
  volume.AddRule(tcERS0, DGS0, DBS0, Slot, DQW0, Slot, DPW0, Slot, DQW0);
  volume.AddRule(tcERS0, Slot, DPS0, Wall, DGS0, Wall, Wall, DQS0, DQSR);
  volume.AddRule(tcERS0, Slot, DPS0, Wall, DGS0, Wall, Wall, DQW0, Slot);

  volume.AddRule(tcESE0, DGN1, DBN1, DQER, Slot, Slot, DBER, Slot, DQE0);

  volume.AddRule(tcESE1, DGN1, DBN1, Slot, DQE0, Slot, DPE0, Slot, Slot);

  volume.AddRule(tcSUWL, Wall, Wall, Wall, Slot, Wall, LBNL, DKS0, LKNU);
  volume.AddRule(tcSUWL, LBNL, DKS0, Wall, LPNL, Wall, Wall, LBNL, LBNU);
  volume.AddRule(tcSUWL, Slot, DPW0, DSW0, DKS0, DSW0, LPNL, Wall, DSW0);

  volume.AddRule(tcSRW1, Wall, Slot, DSW0, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcSRW1, Wall, Wall, DPW0, Slot, Wall, Slot, Wall, Slot);
  volume.AddRule(tcSRW1, Wall, Wall, Slot, Slot, Wall, DBW0, Wall, Slot);
  volume.AddRule(tcSRW1, Wall, Wall, Wall, Slot, Slot, DBW0, Wall, Slot);
  volume.AddRule(tcSRW1, Wall, Wall, Slot, Slot, Slot, DBW0, Wall, Slot);
  volume.AddRule(tcSRW1, Wall, Wall, DGW0, Slot, Slot, DBW0, Wall, Slot);
  volume.AddRule(tcSRW1, Slot, DBSR, Slot, Slot, Slot, DPW0, Slot, Slot);
  volume.AddRule(tcSRW1, DQSR, DBSR, Slot, Slot, Slot, DPW0, Slot, Slot);
  volume.AddRule(tcSRW1, Wall, Wall, Wall, Slot, DHW0, Wall, Wall, Slot);
  volume.AddRule(tcSRW1, Wall, DBW0, Wall, Slot, DSW0, Wall, Wall, Slot);
  volume.AddRule(tcSRW1, DQN1, DBN1, Slot, Slot, DGW0, DBW0, Slot, Slot);
  volume.AddRule(tcSRW1, Wall, Slot, DHW0, DBW0, DBW0, Wall, Wall, DBW0);
  volume.AddRule(tcSRW1, Wall, Slot, DPW0, DBW0, DBW0, Wall, Wall, DBW0);
  volume.AddRule(tcSRW1, Wall, Slot, DBW0, DBW0, DPW0, Wall, Wall, DBW0);
  volume.AddRule(tcSRW1, Wall, Slot, DPW0, DBW0, DPW0, Wall, Wall, DBW0);
  volume.AddRule(tcSRW1, Wall, Slot, DPW0, DBW0, DPW0, Slot, Wall, DBW0);
  volume.AddRule(tcSRW1, Wall, Wall, Slot, DHW0, DBW0, Wall, Wall, DHW0);
  volume.AddRule(tcSRW1, Wall, Wall, DPW0, DPW0, Slot, DSW0, Wall, DPW0);
  volume.AddRule(tcSRW1, Wall, Slot, DBW0, DPW0, DBW0, Wall, Wall, DPW0);
  volume.AddRule(tcSRW1, Wall, DGW0, DBW0, DPW0, DBW0, Wall, Wall, DPW0);
  volume.AddRule(tcSRW1, Wall, Wall, DBW0, DPW0, DPW0, DSW0, Wall, DPW0);
  volume.AddRule(tcSRW1, Wall, Wall, DPW0, DPW0, DPW0, DSW0, Wall, DPW0);
  volume.AddRule(tcSRW1, Wall, DPW0, DSW0, DSW0, Slot, Wall, Wall, DSW0);
  volume.AddRule(tcSRW1, Wall, DPW0, Slot, DSW0, DSW0, Wall, Wall, DSW0);
  volume.AddRule(tcSRW1, LKNU, DPW0, DSW0, DSW0, DSW0, LBNU, Wall, DSW0);
  volume.AddRule(tcSRW1, Wall, Wall, Slot, DGW0, Slot, DPW0, Wall, DGW0);

  volume.AddRule(tcSRW0, Wall, Slot, DSW0, Slot, Wall, Wall, Wall, DSWR);
  volume.AddRule(tcSRW0, Wall, Wall, DPW0, Slot, Wall, Slot, Wall, DPWR);
  volume.AddRule(tcSRW0, Wall, Wall, Slot, Slot, Wall, DBW0, Wall, Slot);
  volume.AddRule(tcSRW0, Wall, Wall, Wall, Slot, Slot, DBW0, Wall, Slot);
  volume.AddRule(tcSRW0, Wall, Wall, Slot, Slot, Slot, DBW0, Wall, Slot);
  volume.AddRule(tcSRW0, Wall, Wall, DGW0, Slot, Slot, DBW0, Wall, DGWR);
  volume.AddRule(tcSRW0, Slot, DBSR, Slot, Slot, Slot, DPW0, Slot, Slot);
  volume.AddRule(tcSRW0, DQSR, DBSR, Slot, Slot, Slot, DPW0, Slot, Slot);
  volume.AddRule(tcSRW0, Wall, Wall, Wall, Slot, DHW0, Wall, Wall, Slot);
  volume.AddRule(tcSRW0, Wall, DBW0, Wall, Slot, DSW0, Wall, Wall, Slot);
  volume.AddRule(tcSRW0, DQN1, DBN1, Slot, Slot, DGW0, DBW0, Slot, Slot);
  volume.AddRule(tcSRW0, Wall, Slot, DHW0, DBW0, DBW0, Wall, Wall, DHWR);
  volume.AddRule(tcSRW0, Wall, Slot, DPW0, DBW0, DBW0, Wall, Wall, DPWR);
  volume.AddRule(tcSRW0, Wall, Slot, DBW0, DBW0, DPW0, Wall, Wall, DBWR);
  volume.AddRule(tcSRW0, Wall, Slot, DPW0, DBW0, DPW0, Wall, Wall, DPWR);
  volume.AddRule(tcSRW0, Wall, Slot, DPW0, DBW0, DPW0, Slot, Wall, DPWR);
  volume.AddRule(tcSRW0, Wall, Wall, Slot, DHW0, DBW0, Wall, Wall, Slot);
  volume.AddRule(tcSRW0, Wall, Wall, DPW0, DPW0, Slot, DSW0, Wall, DPWR);
  volume.AddRule(tcSRW0, Wall, Slot, DBW0, DPW0, DBW0, Wall, Wall, DBWR);
  volume.AddRule(tcSRW0, Wall, DGW0, DBW0, DPW0, DBW0, Wall, Wall, DBWR);
  volume.AddRule(tcSRW0, Wall, Wall, DBW0, DPW0, DPW0, DSW0, Wall, DBWR);
  volume.AddRule(tcSRW0, Wall, Wall, DPW0, DPW0, DPW0, DSW0, Wall, DPWR);
  volume.AddRule(tcSRW0, Wall, DPW0, DSW0, DSW0, Slot, Wall, Wall, DSWR);
  volume.AddRule(tcSRW0, Wall, DPW0, Slot, DSW0, DSW0, Wall, Wall, Slot);
  volume.AddRule(tcSRW0, LKNU, DPW0, DSW0, DSW0, DSW0, LBNU, Wall, DSWR);
  volume.AddRule(tcSRW0, Wall, Wall, Slot, DGW0, Slot, DPW0, Wall, Slot);

  volume.AddRule(tcSSS0, DQSR, DBSR, Slot, Slot, Slot, DBWR, Slot, DQS0);
  volume.AddRule(tcSSS0, Slot, DBSR, DGWR, Slot, Slot, DBWR, Slot, Slot);
  volume.AddRule(tcSSS0, DPSR, Wall, Wall, DBSR, Wall, DQSR, DBSR, DPS0);
  volume.AddRule(tcSSS0, DGSR, DBSR, Wall, DQSR, Wall, Wall, Slot, DGS0);
  volume.AddRule(tcSSS0, Wall, DPSR, Wall, DGSR, Wall, Wall, DQSR, Slot);

  volume.AddRule(tcSRW1, Wall, Wall, DQW0, Slot, Wall, DBW0, Wall, Slot);
  volume.AddRule(tcSRW1, Wall, Wall, DQW0, Slot, Slot, DBW0, Wall, Slot);
  volume.AddRule(tcSRW1, Wall, Wall, Slot, Slot, DQW0, DBW0, Wall, Slot);
  volume.AddRule(tcSRW1, Wall, Wall, DGW0, Slot, DQW0, DBW0, Wall, Slot);
  volume.AddRule(tcSRW1, Wall, DQW0, DBW0, DPW0, DBW0, Wall, Wall, DPW0);
  volume.AddRule(tcSRW1, Slot, DBSR, Slot, DQW0, Slot, DPW0, Slot, DQW0);

  volume.AddRule(tcSRW0, Wall, Wall, DQW0, Slot, Wall, DBW0, Wall, DQWR);
  volume.AddRule(tcSRW0, Wall, Wall, DQW0, Slot, Slot, DBW0, Wall, DQWR);
  volume.AddRule(tcSRW0, Wall, Wall, Slot, Slot, DQW0, DBW0, Wall, Slot);
  volume.AddRule(tcSRW0, Wall, Wall, DGW0, Slot, DQW0, DBW0, Wall, DGWR);
  volume.AddRule(tcSRW0, Wall, DQW0, DBW0, DPW0, DBW0, Wall, Wall, DBWR);
  volume.AddRule(tcSRW0, Slot, DBSR, Slot, DQW0, Slot, DPW0, Slot, Slot);

  volume.AddRule(tcWRN1, DGN1, DBN1, Slot, Slot, DQE1, DBE1, Slot, DGN0);
  volume.AddRule(tcWRN1, DBN1, Wall, Wall, DPN1, Wall, DGN1, DBN1, DBN0);
  volume.AddRule(tcWRN1, Wall, DPN1, Wall, DGN1, Wall, Wall, Slot, Slot);

  volume.AddRule(tcWRN0, DGN0, DBN0, Wall, Slot, Wall, Wall, Slot, DGNR);
  volume.AddRule(tcWRN0, Wall, DBN0, Wall, Slot, Wall, Wall, DGN0, Slot);
  volume.AddRule(tcWRN0, DBN0, Wall, Wall, DPN0, Wall, DGN0, DBN0, DBNR);
  volume.AddRule(tcWRN0, Slot, DPN0, Slot, DGN0, DQE1, DBE1, Slot, Slot);

  volume.AddRule(tcNUEL, DKS1, Wall, Wall, Slot, Wall, LBSL, Wall, LKSU);
  volume.AddRule(tcNUEL, LBSL, DKS1, Wall, LPSL, Wall, Wall, LBSL, LBSU);
  volume.AddRule(tcNUEL, Wall, Slot, Slot, DKE1, DSE1, LPSL, Slot, DSE1);

  volume.AddRule(tcNRE1, Wall, Wall, Slot, Slot, Slot, Slot, Wall, Slot);
  volume.AddRule(tcNRE1, Wall, Wall, Slot, Slot, DPE1, DSE1, Wall, DPE0);
  volume.AddRule(tcNRE1, Wall, Slot, Slot, DSE1, DSE1, LBSU, LKSU, DSE0);

  volume.AddRule(tcESE1, Wall, Wall, DHE0, Slot, Wall, Wall, Wall, DHE1);
  volume.AddRule(tcESE1, Wall, DBE0, Wall, Slot, DSE0, Wall, Wall, Slot);
  volume.AddRule(tcESE1, Wall, Wall, Wall, Slot, DGE0, DBE0, Wall, Slot);
  volume.AddRule(tcESE1, Wall, Wall, DIE0, DBE0, DBE0, Wall, Wall, DBE1);
  volume.AddRule(tcESE1, Wall, Wall, DPE0, DBE0, DBE0, Slot, Wall, DPE1);
  volume.AddRule(tcESE1, Wall, Slot, DPE0, DBE0, DHE0, Wall, Wall, DPE1);
  volume.AddRule(tcESE1, Wall, Wall, DBE0, DBE0, DPE0, Slot, Wall, DBE1);
  volume.AddRule(tcESE1, Wall, Wall, DBE0, DHE0, Slot, Wall, Wall, DBE1);
  volume.AddRule(tcESE1, Wall, Wall, DBE0, DPE0, DPE0, DSE0, Wall, DBE1);
  volume.AddRule(tcESE1, DQSR, DBSR, Slot, DGE0, Slot, DPE0, Slot, Slot);
  volume.AddRule(tcESE1, Wall, Wall, Wall, DIE0, DBE0, Wall, Wall, DIE1);

  volume.AddRule(tcSSS0, DQSR, DBSR, Slot, Slot, DGE1, DBE1, Slot, DQS0);

  volume.AddRule(tcNLEL, LBSU, DSE1, Wall, LBSU, Wall, Wall, LPSU, LPSL);
  volume.AddRule(tcNLEL, DSE1, Wall, Wall, LKSU, Wall, LPSU, Wall, Slot);
  volume.AddRule(tcNLEL, Wall, DPE1, Slot, DSE1, DSE1, LBSU, LKSU, DKE1);

  volume.AddRule(tcERS1, Slot, DPS0, Slot, DQS0, DGE1, DBE1, Slot, DQS0);

  volume.AddRule(tcERS0, Slot, DPS0, Slot, DQS0, DGE1, DBE1, Slot, Slot);

  volume.AddRule(tcEUEL, DKE1, Wall, Wall, Slot, Wall, LBSL, Wall, LKSU);
  volume.AddRule(tcEUEL, LBSL, DKE1, Wall, LPSL, Wall, Wall, LBSL, LBSU);
  volume.AddRule(tcEUEL, Wall, DPE1, Slot, DKE1, DSE1, LPSL, Slot, DSE1);

  volume.AddRule(tcESE1, DQSR, DBSR, DGE0, Slot, Wall, DBE0, Slot, DGE1);
  volume.AddRule(tcESE1, Wall, Wall, Slot, DGE0, Slot, DPE0, Wall, Slot);

  volume.AddRule(tcSSS0, DGE1, DBSR, Wall, Slot, Wall, Wall, Wall, Slot);
  volume.AddRule(tcSSS0, Slot, DBSR, Wall, Slot, Wall, Wall, DQSR, Slot);
  volume.AddRule(tcSSS0, DPSR, Wall, Wall, DBSR, Wall, DGE1, DBSR, DPS0);
  volume.AddRule(tcSSS0, Wall, DPSR, Wall, DQSR, Wall, Wall, DGE1, Slot);
  volume.AddRule(tcSSS0, DQSR, DBSR, Slot, DGE1, Wall, DPE1, Slot, DJE1);

  volume.AddRule(tcSSS1, DQS0, DBS0, Wall, Slot, Wall, Wall, Slot, DQS1);
  volume.AddRule(tcSSS1, Slot, DBS0, Wall, Slot, Wall, Wall, DQS0, Slot);
  volume.AddRule(tcSSS1, Wall, DBS0, Wall, Slot, Wall, Wall, DGS0, Slot);
  volume.AddRule(tcSSS1, DGS0, DBS0, Slot, Slot, DQWR, DBWR, Slot, DGS1);

  volume.AddRule(tcWSW0, DGS0, DBS0, DGWR, Slot, DQWR, DBWR, Slot, DQW0);
  volume.AddRule(tcWSW0, Wall, Wall, DQWR, Slot, DGS1, DBWR, Wall, Slot);
  volume.AddRule(tcWSW0, Wall, DGS1, DBWR, DBWR, DPWR, Wall, Wall, DPW0);
  volume.AddRule(tcWSW0, Wall, Slot, DPWR, DBWR, DPWR, Wall, Wall, DPW0);
  volume.AddRule(tcWSW0, Wall, Wall, DGS1, DQWR, Wall, DPWR, Wall, Slot);
  volume.AddRule(tcWSW0, Wall, Wall, Slot, DQWR, Slot, DPWR, Wall, Slot);
  volume.AddRule(tcWSW0, Slot, DPS1, Slot, DGS1, DQWR, DBWR, Slot, DJS1);
  volume.AddRule(tcWSW0, Wall, Wall, Slot, DGWR, Slot, DPWR, Wall, Slot);

  volume.AddRule(tcNRE1,
        Wall, DBE1,
     Wall, Slot, Slot,
        Wall, Wall,
     Slot
    );
  volume.AddRule(tcNRE1,
        Wall, Wall,
     Wall, Slot, Slot,
        DBE1, Wall,
     Slot
    );
  volume.AddRule(tcNRE1,
        Wall, DBE1,
     Slot, Slot, DSE1,
        Wall, Wall,
     DSE0
    );
  volume.AddRule(tcNRE1,
        Wall, Wall,
     Slot, Slot, DJE1,
        DBE1, Wall,
     DGE0
    );
  volume.AddRule(tcNRE1,
        Wall, Slot,
     DBE1, DBE1, DBE1,
        Wall, Wall,
     DBE0
    );
  volume.AddRule(tcNRE1,
        Wall, Wall,
     DIE1, DBE1, DBE1,
        Wall, Wall,
     DBE0
    );
  volume.AddRule(tcNRE1,
        Wall, Wall,
     DBE1, DBE1, DBE1,
        Slot, Wall,
     DBE0
    );
  volume.AddRule(tcNRE1,
        Wall, Wall,
     DPE1, DBE1, DHE1,
        Wall, Wall,
     DHE0
    );
  volume.AddRule(tcNRE1,
        Wall, Wall,
     DBE1, DBE1, DPE1,
        Slot, Wall,
     DPE0
    );
  volume.AddRule(tcNRE1,
        Wall, Wall,
     DBE1, DHE1, Wall,
        Wall, Wall,
     Slot
    );
  volume.AddRule(tcNRE1,
        Wall, DJE1,
     DBE1, DPE1, DBE1,
        Wall, Wall,
     DBE0
    );
  volume.AddRule(tcNRE1,
        Wall, Wall,
     DPE1, DPE1, DBE1,
        DSE1, Wall,
     DBE0
    );
  volume.AddRule(tcNRE1,
        Wall, Wall,
     DBE1, DPE1, DPE1,
        DSE1, Wall,
     DPE0
    );
  volume.AddRule(tcNRE1,
        Slot, DPS0,
     Slot, DJE1, Wall,
        DPE1, Slot,
     DQS0
    );
  volume.AddRule(tcNRE1,
        Wall, Wall,
     Wall, DIE1, DBE1,
        Wall, Wall,
     DIE0
    );

#endif // CAM_8_RULES_H
