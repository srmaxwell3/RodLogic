function randInt(n) {
  return int(n * rand());
}

BEGIN {
  nWords = 128;
  nWrites = 128;
  nReads = 128;

  w = 0
  r = 0;
  for (i = 0; w < nWrites && r < nReads; i += 1) {
    if (0 < randInt(2)) {
      md[i] = 0;
      ma[i] = randInt(4096);
      wr[i] = 0;
      rd[i] = 1;
      r += 1;
    } else {
      md[i] = randInt(4096);
      ma[i] = randInt(4096);
      wr[i] = 1;
      rd[i] = 0;
      w += 1;
    }
  }
  for (; w < nWrites; i += 1) {
    md[i] = randInt(4096);
    ma[i] = randInt(4096);
    wr[i] = 1;
    rd[i] = 0;
    w += 1;
  }
  for (; r < nReads; i += 1) {
    md[i] = 0;
    ma[i] = randInt(4096);
    wr[i] = 0;
    rd[i] = 1;
    r += 1;
  }

  printf "{ MA.0..b, MD.0..b, Write, Read }\n";
  for (i = 0; i < (nWrites + nReads); i += 1) {
    printf "{ 0x%03x, 0x%03x, %1d, %1d }\n", ma[i], md[i], wr[i], rd[i];
  }
  exit 0;
}

