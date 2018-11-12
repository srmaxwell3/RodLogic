#!/usr/bin/awk -f 

# 0 [0.E]: inputs={  A.b..0< -> 000000000000 [000],  B.b..0< -> 000000000000 [000],  C0 -> 0,  Cx -> 0,  Cy -> 0,  M -> 0,  S.3..0< -> 0000 [0],  Start -> 0 }, outputs={  Busy -> 0,  C0 -> 0,  C4 -> 0,  C8 -> 0,  Cc -> 0,  F.b..0< -> 000000000000 [000],  G0! -> 0,  G1! -> 0,  G2! -> 0,  P0! -> 0,  P1! -> 0,  P2! -> 0,  Ready -> 0,  Start -> 0 }

/inputs={ / && title == "" {
    title = $0;
    gsub(/ +/, "", title);
    gsub(/->[^,]+/, "", title);
    gsub(/[<>]/, "", title);
    sub(/^.+inputs={/, "", title);
    sub(/},outputs={/, ",", title);
    sub(/}$/, "", title);
    n = split(title, a, /,/);
    title = "\"Tick\",\"Cycle.Phase\"";
    for (i = 1; i <= n; i += 1) {
	title = title ",\"" a[i] "\"";
	if (a[i] ~ /\.\.0/) {
	    title = title ",\"" a[i] "\"";
	}
    }
    print title;
}

/inputs={ / {
    gsub(/ +/, "");
    sub(/\[/, ",");
    sub(/\]:inputs={/, ",");
    sub(/},outputs={/, ",");
    sub(/}/, "");
    # gsub(/\],/, ",");
    # gsub(/\[/, ",");
    n = split($0, a, /,/);
    comma = "";
    for (i = 1; i <= n; i += 1) {
	sub(/^.+->/, "", a[i]);
	if (a[i] ~ /\[[0-9a-f]+\]/) {
	    b = a[i];
	    h = a[i];
	    sub(/\[.+\]/, "", b);
	    sub(/.*\[/, "", h);
	    sub(/\]/, "", h);
	    printf "%s0b%s,0x%s", comma, b, h;
	} else if (a[i] ~ /[0-9]+\.[ESDWNU]/) {
	    printf "%s\"%s\"", comma, a[i];
	} else {
	    printf "%s%s", comma, a[i];
	}
	comma = ",";
    }
    printf "\n";
    next;
}

{
    next;
}
