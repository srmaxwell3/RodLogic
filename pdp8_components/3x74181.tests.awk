#!/usr/bin/awk -f

BEGIN {
  printf "{ Start, S.3..0, M, C0, B.b..0, A.b..0 }\n";
  s = 8;
  m = 0;
  c0 = 1;
  # for (s = 0; s < 16; s += 1) {
  #   for (m = 0; m < 2; m += 1) {
  #     for (c = 0; c < 2; c += 1) {
        for (b = 0; b < 4096; b += 1) {
          for (a = 0; a < 4096; a += 1) {
            printf "{ 1, 0x%1x, %1d, %1d, 0x%03x, 0x%03x }\n", s, m, c0, b, a;
          }
        }
  #     }
  #   }
  # }
}
