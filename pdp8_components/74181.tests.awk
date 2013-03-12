#!/usr/bin/awk -f

# inputs={ "A0"=0, "A1"=0, "A2"=0, "A3"=0, "B0"=0, "B1"=0, "B2"=0, "B3"=0, "C0"=0, "M"=0, "S0"=0, "S1"=0, "S2"=0, "S3"=0, "Start"=1 }
# outputs={ "Busy"=0, "C4"=0, "F0"=1, "F1"=1, "F2"=1, "F3"=1, "G!"=0, "P!"=0, "Ready"=1 }

function print4BitsOf(v) {
  return sprintf("%d,%d,%d,%d", (v / 1) % 2, (v / 2) % 2, (v / 4) % 2, (v / 8) % 2);
}

function print1BitOf(v) {
  return sprintf("%d", (v / 1) % 2);
}

BEGIN {
  printf "'A0','A1','A2','A3','B0','B1','B2','B3','C0','M','S0','S1','S2','S3','Start'\n";
  for (s = 0; s < 16; s += 1) {
    for (m = 0; m < 2; m += 1) {
      for (c = 0; c < 2; c += 1) {
        for (b = 0; b < 16; b += 1) {
          for (a = 0; a < 16; a += 1) {
            if (m == 1) {
              r = 
            } else {
            }
            printf "%s,%s,%s,%s,%s,1\n",
                print4BitsOf(a),
                print4BitsOf(b),
                print1BitOf(c),
                print1BitOf(m),
                print4BitsOf(s);
          }
        }
      }
    }
  }
}
