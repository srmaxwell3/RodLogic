#include <cstdio>

int main(int argc, char const *argv[]) {
  printf("{ 'S.3..0', 'M', 'A.3..0', 'B.3..0', 'C0', 'F.3..0', 'C4', 'G', 'P' }\n");
  for (unsigned s = 0; s < 16; s += 1) {
    for (unsigned m = 0; m < 2; m += 1) {
      for (unsigned c0 = 0; c0 < 2; c0 += 1) {
        for (unsigned b = 0; b < 16; b += 1) {
          for (unsigned a = 0; a < 16; a += 1) {
            unsigned c4 = 0;
            unsigned f = 0;
	    unsigned g = 0;
	    unsigned p = 0;
            if (m == 0) {
	      if (c0 == 0) {
		switch (s) {
                case  0: f = ( a                   ) - 1; break;
                case  1: f = ( a &  b              ) - 1; break;
                case  2: f = ( a & ~b              ) - 1; break;
                case  3: f = ( 0                   ) - 1; break;
                case  4: f = ( a +        ( a | ~b));     break;
                case  5: f = (( a &  b) + ( a | ~b));     break;
                case  6: f = ( a -  b              ) - 1; break;
                case  7: f = ( a | ~b              );     break;
                case  8: f = ( a +        ( a |  b));     break;
                case  9: f = ( a +  b              );     break;
                case 10: f = (( a & ~b) + ( a |  b));     break;
                case 11: f = ( a |  b              );     break;
                case 12: f = ( a +  a              );     break;
                case 13: f = ( a +        ( a &  b));     break;
                case 14: f = ( a +        ( a & ~b));     break;
                case 15: f = ( a                   );     break;
		}
	      } else {
		switch (s) {
                case  0: f = ( a                   );     break;
                case  1: f = ( a |  b              );     break;
                case  2: f = ( a | ~b              );     break;
                case  3: f = ( 0                   ) - 1; break;
                case  4: f = ( a +        ( a & ~b));     break;
                case  5: f = (( a |  b) + ( a & ~b));     break;
                case  6: f = ( a -  b              ) - 1; break;
                case  7: f = ( a &  b              ) - 1; break;
                case  8: f = ( a +        ( a &  b));     break;
                case  9: f = ( a +  b              );     break;
                case 10: f = (( a | ~b) + ( a &  b));     break;
                case 11: f = ( a &  b              ) - 1; break;
                case 12: f = ( a +  a              );     break;
                case 13: f = ( a +        ( a |  b));     break;
                case 14: f = ( a +        ( a | ~b));     break;
                case 15: f = ( a                   ) - 1; break;
		}
	      }
	      c4 = (f & 0x10000) ? 1 : 0;
	      g = 15 < f;
	      p = 15 <= f;
            } else {
	      if (c0 == 0) {
		switch (s) {
                case  0: f =  (~a     ); break;
                case  1: f = ~( a &  b); break;
                case  2: f =  (~a |  b); break;
                case  3: f =  ( 1     ); break;
                case  4: f = ~( a |  b); break;
                case  5: f =  (     ~b); break;
                case  6: f = ~( a ^  b); break;
                case  7: f =  ( a | ~b); break;
                case  8: f =  (~a &  b); break;
                case  9: f = ~( a ^  b); break;
                case 10: f =  (      b); break;
                case 11: f =  ( a |  b); break;
                case 12: f =  ( 0     ); break;
                case 13: f =  ( a & ~b); break;
                case 14: f =  ( a &  b); break;
                case 15: f =  ( a     ); break;
		}
	      } else {
		switch (s) {
                case  0: f =  (~a     ); break;
                case  1: f = ~( a |  b); break;
                case  2: f =  (~a &  b); break;
                case  3: f =  ( 0     ); break;
                case  4: f = ~( a &  b); break;
                case  5: f =  (     ~b); break;
                case  6: f =  ( a ^  b); break;
                case  7: f =  ( a & ~b); break;
                case  8: f =  (~a |  b); break;
                case  9: f = ~( a ^  b); break;
                case 10: f =  (      b); break;
                case 11: f =  ( a &  b); break;
                case 12: f =  ( 1     ); break;
                case 13: f =  ( a | ~b); break;
                case 14: f =  ( a |  b); break;
                case 15: f =  ( a     ); break;
		}
	      }
            }
	    f &= 0xF;

            printf("{ 0x%04x, %1u, 0x%04x, 0x%04x, %1u, 0x%04x, %1u, %1u, %1u }\n",
                   s,
                   m,
                   a,
                   b,
                   c0,
                   f,
                   c4,
		   g,
		   p
                  );
          }
        }
      }
    }
  }
  return 0;
}
