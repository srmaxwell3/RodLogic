#include <cstdio>

int main(int argc, char *const argv[]) {
  static unsigned vs[16] = {
        0 % 4096,
        1 % 4096,
        3 % 4096,
        7 % 4096,
       15 % 4096,
       30 % 4096,
       60 % 4096,
      120 % 4096,
      240 % 4096,
      480 % 4096,
      960 % 4096,
     1920 % 4096,
     3840 % 4096,
     7680 % 4096,
    15360 % 4096,
    30720 % 4096
  };

#define function(function) { f = (function); fString = #function; }
  char const *fString;

  printf("{ Start, S.3..0, M, C0, B.b..0, A.b..0 }\n");
  for (unsigned s = 0; s < 16; s += 1) {
    if (s != 9) continue;
    for (unsigned m = 0; m < 2; m += 1) {
      for (unsigned c = 0; c < 2; c += 1) {
        if (c != 0) continue;

        for (unsigned bi = 0; bi < 16; bi += 1) {
          for (unsigned ai = 0; ai < 16; ai += 1) {
            unsigned b = vs[bi];
            unsigned a = vs[ai];
            unsigned f;
            switch (m) {
              case /* m = */ 0:
                switch (c) {
                  case /* m = 0, c = */ 0:
                    switch (s) {
                      case /* m = 0, c = 0, s = */  0:
                        f = a-1;
                        break;
                      case /* m = 0, c = 0, s = */  1:
                        f = (a&b)-1;
                        break;
                      case /* m = 0, c = 0, s = */  2:
                        f = (a&!b)-1;
                        break;
                      case /* m = 0, c = 0, s = */  3:
                        f = -1;
                        break;
                      case /* m = 0, c = 0, s = */  4:
                        f = a+(a|!b);
                        break;
                      case /* m = 0, c = 0, s = */  5:
                        f = (a&b)+(a|!b);
                        break;
                      case /* m = 0, c = 0, s = */  6:
                        f = a-b-1;
                        break;
                      case /* m = 0, c = 0, s = */  7:
                        f = a|!b;
                        break;
                      case /* m = 0, c = 0, s = */  8:
                        f = a+(a|b);
                        break;
                      case /* m = 0, c = 0, s = */  9:
                        f = a+b;
                        break;
                      case /* m = 0, c = 0, s = */ 10:
                        f = (a&!b)+(a|b);
                        break;
                      case /* m = 0, c = 0, s = */ 11:
                        f = a|b;
                        break;
                      case /* m = 0, c = 0, s = */ 12:
                        f = a+a;
                        break;
                      case /* m = 0, c = 0, s = */ 13:
                        f = (a&b)+a;
                        break;
                      case /* m = 0, c = 0, s = */ 14:
                        f = (a&!b)+a;
                        break;
                      case /* m = 0, c = 0, s = */ 15:
                        f = a;
                        break;
                    };
                    break;
                  case /* m = 0, c = */ 1:
                    switch (s) {
                      case /* m = 0, c = 1, s = */  0:
                        f = a;
                        break;
                      case /* m = 0, c = 1, s = */  1:
                        f = a|b;
                        break;
                      case /* m = 0, c = 1, s = */  2:
                        f = a|!b;
                        break;
                      case /* m = 0, c = 1, s = */  3:
                        f = -1;
                        break;
                      case /* m = 0, c = 1, s = */  4:
                        f = a+(a&!b);
                        break;
                      case /* m = 0, c = 1, s = */  5:
                        f = (a|b)+(a&!b);
                        break;
                      case /* m = 0, c = 1, s = */  6:
                        f = a-b-1;
                        break;
                      case /* m = 0, c = 1, s = */  7:
                        f = (a&!b)-1;
                        break;
                      case /* m = 0, c = 1, s = */  8:
                        f = a+(a&b);
                        break;
                      case /* m = 0, c = 1, s = */  9:
                        f = a+b;
                        break;
                      case /* m = 0, c = 1, s = */ 10:
                        f = (a+!b)+(a&b);
                        break;
                      case /* m = 0, c = 1, s = */ 11:
                        f = (a&b)-1;
                        break;
                      case /* m = 0, c = 1, s = */ 12:
                        f = a+a;
                        break;
                      case /* m = 0, c = 1, s = */ 13:
                        f = (a|b)+a;
                        break;
                      case /* m = 0, c = 1, s = */ 14:
                        f = (a|!b)+a;
                        break;
                      case /* m = 0, c = 1, s = */ 15:
                        f = a-1;
                        break;
                    };
                    break;
                };
                break;
              case /* m = */ 1:
                switch (c) {
                  case /* m = 1, c = 0, s = */ 0:
                    switch (s) {
                      case /* m = 1, c = 0, s = */  0:
                        f = !a;
                        break;
                      case /* m = 1, c = 0, s = */  1:
                        f = !(a|b);
                        break;
                      case /* m = 1, c = 0, s = */  2:
                        f = !a|b;
                        break;
                      case /* m = 1, c = 0, s = */  3:
                        f = 0;
                        break;
                      case /* m = 1, c = 0, s = */  4:
                        f = !(a&b);
                        break;
                      case /* m = 1, c = 0, s = */  5:
                        f = !b;
                        break;
                      case /* m = 1, c = 0, s = */  6:
                        f = a^b;
                        break;
                      case /* m = 1, c = 0, s = */  7:
                        f = a&!b;
                        break;
                      case /* m = 1, c = 0, s = */  8:
                        f = !a|b;
                        break;
                      case /* m = 1, c = 0, s = */  9:
                        f = !(a^b);
                        break;
                      case /* m = 1, c = 0, s = */ 10:
                        f = b;
                        break;
                      case /* m = 1, c = 0, s = */ 11:
                        f = a&b;
                        break;
                      case /* m = 1, c = 0, s = */ 12:
                        f = 1;
                        break;
                      case /* m = 1, c = 0, s = */ 13:
                        f = a|!b;
                        break;
                      case /* m = 1, c = 0, s = */ 14:
                        f = a|b;
                        break;
                      case /* m = 1, c = 0, s = */ 15:
                        f = a;
                        break;
                    };
                    break;
                  case /* m = 1, c = */ 1:
                    switch (s) {
                      case /* m = 1, c = 1, s = */  0:
                        f = !a;
                        break;
                      case /* m = 1, c = 1, s = */  1:
                        f = !(a|b);
                        break;
                      case /* m = 1, c = 1, s = */  2:
                        f = !a|b;
                        break;
                      case /* m = 1, c = 1, s = */  3:
                        f = 0;
                        break;
                      case /* m = 1, c = 1, s = */  4:
                        f = !(a&b);
                        break;
                      case /* m = 1, c = 1, s = */  5:
                        f = !b;
                        break;
                      case /* m = 1, c = 1, s = */  6:
                        f = a^b;
                        break;
                      case /* m = 1, c = 1, s = */  7:
                        f = a&!b;
                        break;
                      case /* m = 1, c = 1, s = */  8:
                        f = !a|b;
                        break;
                      case /* m = 1, c = 1, s = */  9:
                        f = !(a^b);
                        break;
                      case /* m = 1, c = 1, s = */ 10:
                        f = b;
                        break;
                      case /* m = 1, c = 1, s = */ 11:
                        f = a&b;
                        break;
                      case /* m = 1, c = 1, s = */ 12:
                        f = 1;
                        break;
                      case /* m = 1, c = 1, s = */ 13:
                        f = a|!b;
                        break;
                      case /* m = 1, c = 1, s = */ 14:
                        f = a|b;
                        break;
                      case /* m = 1, c = 1, s = */ 15:
                        f = a;
                        break;
                    };
                    break;
                };
                break;
            };

            printf("# %s (f = 0x%03x b = 0x%03x, a = 0x%03x)\n", fString, f, b, a);
            printf("{ 1, 0x%1x, %1d, %1d, 0x%03x, 0x%03x }\n", s, m, c, b, a);
            printf("{ 0, 0x%1x, %1d, %1d, 0x%03x, 0x%03x }\n", s, m, c, b, a);
            printf("{ 0, 0x%1x, %1d, %1d, 0x%03x, 0x%03x }\n", s, m, c, b, a);
            printf("{ 0, 0x%1x, %1d, %1d, 0x%03x, 0x%03x }\n", s, m, c, b, a);
            printf("{ 0, 0x%1x, %1d, %1d, 0x%03x, 0x%03x }\n", s, m, c, b, a);
          }
        }
      }
    }
  }
  return 0;
}
