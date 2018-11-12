#!/usr/bin/awk -f

/^[0-9]+: inputs={/ {
  tick = 0 + $1;
  inInputs = 1;
}

/ +}, outputs={/ {
  inInputs = 0;
}

inInputs == 1 && /->/ {
  sub(/< /, "", $1);
  sub(/\.>/, "", $1);

  if (!($1 in iln)) {
    iln[$1] = ++ins;
    ils[ins] = $1;
    # printf "iln['%s'] = %d, ils[%d] = '%s', ", $1, ins, ins, $1;
  }
  sub(/,$/, "", $3);
  ivs[iln[$1]] = $3;
  # printf "ivs[iln['%s'](%d)] = %d\n", $1, iln[$1], $3;
}

inInputs == 0 && /->/ {
  sub(/< /, "", $1);
  sub(/\.>/, "", $1);

  if (!($1 in oln)) {
    oln[$1] = ++ons;
    ols[ons] = $1;
    # printf "oln['%s'] = %d, ols[%d] = '%s', ", $1, ons, ons, $1;
  }
  sub(/,$/, "", $3);
  ovs[oln[$1]] = $3;
  # printf "ovs[oln['%s'](%d)] = %d\n", $1, oln[$1], $3;
}

inInputs == 0 && / +}$/ {
  printf "%d: ", tick;
  if (0 < ins) {
    printf "%s -> %s", ils[1], ivs[1];
    for (i = 2; i < ins + 1; i += 1) {
      printf ", %s -> %s", ils[i], ivs[i];
    }
  }
  printf "; ";
  if (0 < ons) {
    printf "%s -> %s", ols[1], ovs[1];
    for (o = 2; o < ons + 1; o += 1) {
      printf ", %s -> %s", ols[o], ovs[o];
    }
  }
  printf "\n";
}
