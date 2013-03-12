function randInt(n) {
  return int(n * rand());
}

function shuffle(a, n,   i, j, t) {
  for (i = 0; i < n; i += 1) {
    j = randInt(n);
    if (i != j) {
      t = a[j];
      a[j] = a[i];
      a[i] = t;
    }
  }
}

BEGIN {
  nWords = 64;
  nWrites = 64;
  nReads = 64;

  for (i = 0; i < nWrites; i += 1) {
    wData[i] = randInt(4096);
    wAdrs[i] = randInt(4096);
  }
  # shuffle(wAdrs, nWrites);
  for (i = 0; i < nReads; i += 1) {
      rAdrs[i] = randInt(4096);
  }
  # shuffle(rAdrs, nReads);

  for (i = 0; i < nWrites; i += 1) {
    md[i] = wData[i];
    ma[i] = wAdrs[i];
    wr[i] = 1;
    rd[i] = 0;
  }
  for (; i < nReads; i += 1) {
    md[i] = wData[i];
    ma[i] = rAdrs[i];
    wr[i] = 0;
    rd[i] = 1;
  }

  printf "{ MA.0..b, { 0x%03x", ma[0];
  for (i = 0; i < (nWrites + nReads); i += 1) {
    printf ", 0x%03x", ma[i];
  }
  printf " } }\n";

  printf "{ MD.0..b, { 0x%03x", md[0];
  for (i = 0; i < (nWrites + nReads); i += 1) {
    printf ", 0x%03x", md[i];
  }
  printf " } }\n";

  printf "{ Write,   { %5d", wr[0];
  for (i = 0; i < (nWrites + nReads); i += 1) {
    printf ", %5d", wr[i];
  }
  printf " } }\n";

  printf "{ Read,    { %5d", rd[0];
  for (i = 0; i < (nRdites + nReads); i += 1) {
    printf ", %5d", rd[i];
  }
  printf " } }\n";
  exit 0;
}

