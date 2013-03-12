function randInt(n) {
  return int(n * rand());
}

BEGIN {
    printf "{ Clk, S.1..0, X.0..b, L }\n";
    for (i = 0; i < 256; i += 1) {
	s = randInt(4);
	if (s == 0) {
	    printf "{ 1, %1d, 0x%03x, %1d }\n", s,             0,          0;
	} else if (s == 1) {
	    printf "{ 1, %1d, 0x%03x, %1d }\n", s,             0, randInt(2);
	} else if (s == 2) {
	    printf "{ 1, %1d, 0x%03x, %1d }\n", s,             0, randInt(2);
	} else {
	    printf "{ 1, %1d, 0x%03x, %1d }\n", s, randInt(4096),          0;
	}
    }
    exit 0;
}

