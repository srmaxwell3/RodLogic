// ┃┇━┅▲△▶▷▼▽◀◁⬛
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <array>
using std::array;
#include <map>
using std::map;
#include <set>
using std::set;
#include <string>
using std::string;
#include <deque>
using std::deque;
#include <initializer_list>
using std::initializer_list;

bool optVerbose = true;

#include "direction.h"
#include "fwdorbwd.h"
#include "phasepercycle.h"
#include "tickperphase.h"
#include "tickpercycle.h"
#include "voxelcoordinant.h"
#include "fblkstate.h"
#include "rblkstate.h"
#include "voxeltype.h"
#include "locktype.h"
#include "lockstate.h"
#include "datatype.h"
#include "datastate.h"
#include "blockstate.h"
#include "rodtype.h"
#include "voxel.h"
#include "scenario.h"
#include "rule.h"
#include "volume.h"

typedef map<string, Voxel> StringToVoxel;
StringToVoxel stringToVoxel;

  //   abcdefghijklmnopqrstuvwxy
  // a ......v........... ...... a
  // b ......|...........^...... b
  // c ......|...........|...... c
  // d ......|...........|...... d
  // e ......|.....>..>..|...... e
  // f ......|.....>..>..|...... f
  // g  <----------X- X--------< g
  // h ......|.....|..|..|...... h
  // i ......|.....v..v..|...... i
  // j I---^^X---0-|-1|>.|.....C j
  // k ......|.....|..|..|...... k
  // l ......|.....|..v..|...... l
  // m I---^^X-----|-1|>.|.....D m
  // n ......|.....|..|..|...... n
  // o ......|..^..v..|..|...... o
  // p ....^^X->|-1|--|--------O p
  // q ......|..1..0..0..|...... q
  // r ......|..|..|..|..|...... r
  // s ......|.<|-0|<-|<-Xvv.... s
  // t ......|..^..v..v..|...... t
  // u ......|..|........|...... u
  // v >--------X------------->  v
  // w ......|..<........|...... w
  // x ......|..<........|...... x
  // y ......|...........|...... y
  // z ......|...........|...... z
  // A ......v...........|...... A
  // B ...... ...........^...... B
  //   abcdefghijklmnopqrstuvwxy

VolArray initialVolume = {
  //   abcdefghijklmnopqrstuvwxy
  // a ......v........... ...... a
  // b ......|...........^...... b
  // c ......|...........|...... c
  // d ......|...........|...... d
  // e ......|...........|...... e
  // f ......|...........|...... f
  // g ......|...........|...... g
  // h ......|...........|...... h
  // i ......|...........|...... i
  // j ......+...........|...... j
  // k ......|...........|...... k
  // l ......|...........|...... l
  // m ......+...........|...... m
  // n ......|...........|...... n
  // o ......|...........|...... o
  // p ......+...........|...... p
  // q ......|...........|...... q
  // r ......|...........|...... r
  // s ......|...........+...... s
  // t ......|...........|...... t
  // u ......|...........|...... u
  // v ......|...........|...... v
  // w ......|...........|...... w
  // x ......|...........|...... x
  // y ......|...........|...... y
  // z ......|...........|...... z
  // A ......v...........|...... A
  // B ...... ...........^...... B
  //   abcdefghijklmnopqrstuvwxy

  //        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y
  /* a */ Wall,Wall,Wall,Wall,Wall,Wall,LTSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall, /* a */
  /* b */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LHNL,Wall,Wall,Wall,Wall,Wall,Wall, /* b */
  /* c */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* c */
  /* d */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* d */
  /* e */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* e */
  /* f */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* f */
  /* g */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* g */
  /* h */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* h */
  /* i */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* i */
  /* j */ Wall,Wall,Wall,Wall,Wall,Wall,LPSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* j */
  /* k */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* k */
  /* l */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* l */
  /* m */ Wall,Wall,Wall,Wall,Wall,Wall,LPSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* m */
  /* n */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* n */
  /* o */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* o */
  /* p */ Wall,Wall,Wall,Wall,Wall,Wall,LPSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* p */
  /* q */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* q */
  /* r */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* r */
  /* s */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LPNL,Wall,Wall,Wall,Wall,Wall,Wall, /* s */
  /* t */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* t */
  /* u */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* u */
  /* v */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* v */
  /* w */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* w */
  /* x */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* x */
  /* y */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* y */
  /* z */ Wall,Wall,Wall,Wall,Wall,Wall,LBSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* z */
  /* A */ Wall,Wall,Wall,Wall,Wall,Wall,LHSL,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LBNL,Wall,Wall,Wall,Wall,Wall,Wall, /* A */
  /* B */ Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,LTNL,Wall,Wall,Wall,Wall,Wall,Wall, /* B */
  //        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ......................... e
  // f ......................... f
  // g ......................... g
  // h ......................... h
  // i ......................... i
  // j ....^^X  ................ j
  // k ...... .................. k
  // l ......................... l
  // m ....^^X  ................ m
  // n ...... .................. n
  // o ......................... o
  // p ....^^X  ................ p
  // q ...... .................. q
  // r .................. ...... r
  // s ................  Xvv.... s
  // t ......................... t
  // u ......................... u
  // v ......................... v
  // w ......................... w
  // x ......................... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

  //        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y
  /* a */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* a */
  /* b */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* b */
  /* c */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* c */
  /* d */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* d */
  /* e */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* e */
  /* f */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* f */
  /* g */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* g */
  /* h */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* h */
  /* i */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* i */
  /* j */ Wall,Wall,Wall,Wall,DSER,DSER,DKER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* j */
  /* k */ Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* k */
  /* l */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* l */
  /* m */ Wall,Wall,Wall,Wall,DSER,DSER,DKER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* m */
  /* n */ Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* n */
  /* o */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* o */
  /* p */ Wall,Wall,Wall,Wall,DSER,DSER,DKER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* p */
  /* q */ Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* q */
  /* r */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall, /* r */
  /* s */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,DKWR,DSWR,DSWR,Wall,Wall,Wall,Wall, /* s */
  /* t */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* t */
  /* u */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* u */
  /* v */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* v */
  /* w */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* w */
  /* x */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* x */
  /* y */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* y */
  /* z */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* z */
  /* A */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* A */
  /* B */ Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall, /* B */
  //        a    b    c    d    e    f    g    h    i    j    k    l    m    n    o    p    q    r    s    t    u    v    w    x    y

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ......................... e
  // f ......................... f
  // g ......................... g
  // h ......................... h
  // i ......................... i
  // j I---^^X---0---1->  ...... j
  // k ......................... k
  // l ......................... l
  // m I---^^X-------1->  ...... m
  // n ......................... n
  // o ......................... o
  // p ....^^X->--1------------O p
  // q ......................... q
  // r ......................... r
  // s ......  <--0-<--<-Xvv.... s
  // t ......................... t
  // u ......................... u
  // v ......................... v
  // w ......................... w
  // x ......................... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
DIER,DBER,DBER,DBER,DPER,DPER,DPER,DBER,DBER,DBER,DPER,DBER,DBER,DBER,DPER,DBER,DHER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
DIER,DBER,DBER,DBER,DPER,DPER,DPER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DPER,DBER,DHER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,DPER,DPER,DPER,DBER,DTER,DBER,DBER,DPER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DBER,DOER,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,DHWR,DBWR,DBWR,DPWR,DBWR,DTWR,DBWR,DBWR,DTWR,DBWR,DPWR,DPWR,DPWR,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ......................... e
  // f ......................... f
  // g ......................... g
  // h ......................... h
  // i ............v..v......... i
  // j ..........0  .1  ........ j
  // k ............ .. ......... k
  // l ...............v......... l
  // m ..............1  ........ m
  // n ............... ......... n
  // o ......... ..v............ o
  // p ........>  1  ........... p
  // q .........1..0..0......... q
  // r ......... .. .. ......... r
  // s ........   0 <  <........ s
  // t .........^.. .. ......... t
  // u ......................... u
  // v ......................... v
  // w ......................... w
  // x ......................... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQSR,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DGER,Slot,Slot,Wall,DGER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DGER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQER,Slot,Slot,DQER,Slot,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQNR,Wall,Wall,DQSR,Wall,Wall,DQSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Slot,Slot,DGWR,Slot,DQWR,Slot,Slot,DQWR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DQNR,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ............>..>......... e
  // f ............>..>......... f
  // g ............X..X......... g
  // h ............|..|......... h
  // i ............v..v......... i
  // j ............|..|......... j
  // k ............|..|......... k
  // l ............|..v......... l
  // m ......... ..|..|......... m
  // n ......... ..|..|......... n
  // o .........^..v..|......... o
  // p .........|..|..|......... p
  // q .........1..0..0......... q
  // r .........|..|..|......... r
  // s .........|..|..|......... s
  // t .........^..v..v......... t
  // u .........|.. .. ......... u
  // v .........X.. .. ......... v
  // w .........<............... w
  // x .........<............... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DHNR,Wall,Wall,DPSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,DPSR,Wall,Wall,DPSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,DBSR,Wall,Wall,DBSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,DHSR,Wall,Wall,DHSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DBNR,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DPNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ............>..>......... e
  // f ............>..>......... f
  // g ........... X. X......... g
  // h ............ .. ......... h
  // i ............ .. ......... i
  // j ......................... j
  // k ......................... k
  // l ......................... l
  // m ......................... m
  // n ......................... n
  // o ......................... o
  // p ......................... p
  // q ......................... q
  // r ......................... r
  // s ......................... s
  // t ......... ............... t
  // u ......... ............... u
  // v .........X .............. v
  // w .........<............... w
  // x .........<............... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSSR,Wall,Wall,DSSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSSR,Wall,Wall,DSSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,DKSR,Wall,Slot,DKSR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DKNR,Slot,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,DSNR,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,

  //   abcdefghijklmnopqrstuvwxy
  // a ......................... a
  // b ......................... b
  // c ......................... c
  // d ......................... d
  // e ......................... e
  // f ......................... f
  // g  <----------X--X--------< g
  // h ......................... h
  // i ......................... i
  // j ......................... j
  // k ......................... k
  // l ......................... l
  // m ......................... m
  // n ......................... n
  // o ......................... o
  // p ......................... p
  // q ......................... q
  // r ......................... r
  // s ......................... s
  // t ......................... t
  // u ......................... u
  // v >--------X------------->  v
  // w ......................... w
  // x ......................... x
  // y ......................... y
  // z ......................... z
  // A ......................... A
  // B ......................... B
  //   abcdefghijklmnopqrstuvwxy

Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Slot,LHWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LPWL,LBWL,LBWL,LPWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LTWL,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
LTWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LPWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LBWL,LHWL,Slot,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,Wall,
};

int main(int argc, char const *argv[]) {
  for (size_t i = 0; i < eoVoxel; i += 1) {
    Voxel v = Voxel(i);

    stringToVoxel[toConstCharPointer(v)] = v;
  }

  Volume volume(initialVolume);
  volume.AddInput
      (VoxelCoordinant(2,  8, 0),
       { DSSet1, DSSet0, DSSet0, DSSet1, DSSet0, DSSet0 }
      );
  volume.AddInput
      (VoxelCoordinant(2, 12, 0),
       { DSSet0, DSSet0, DSSet1, DSSet1, DSSet0, DSSet1 }
      );

  volume.PrintViewFlat();
  for (size_t t = 0; t < (6 * NTicksPerCycle); t += 1) {
    volume.ProceedOneTick();
    volume.PrintViewFlat();
    // for (auto const &r : volume.GetRules()) {
    //   if (r.second == Unkn) {
    //     fprintf(stdout, "  volume.Add");
    //     Rule rule(r.first, r.second);
    //     rule.Dump();
    //     fprintf(stdout, ";\n");
    //   }
    // }
  }
  return 0;
}
