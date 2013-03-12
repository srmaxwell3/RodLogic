// abcdefghijklmnopqrstuvwxyzABCDEFGHILKLMN
// ........................................ a
// ...........|.......>...>...|............ b
// ...........|.......|...|...|............ c
// ...........|.......>...>...|............ d
// .------------------>--->---------------. e
// ...........|.......|...|...|............ f
// ...........|.......|...v...|............ g
// ...........|.......|...|...|............ h
// .>------^-^^----0--|--1|>..|...........C i
// ...........|.......|...|...|............ j
// ...........|.......v...|...|............ k
// ...........|.......|...|...|............ l
// .>------^-^^-------|--1|>..|...........D m
// ...........|.......|...|...|............ n
// ...........|.......v...|...|............ o
// ...........|...^...|...|...|............ p
// ........^-^^->-|--1|---|----------->...Q q
// ...........|...1...0...0...|............ r
// ...........|...|...|...|...|............ s
// ...........|...|...|...|...|............ t
// ...........|..<|--0|-<-|-<-vv-v......... u
// ...........|...|...v...v...|............ v
// ...........|...^...........|............ w
// ...........|...|...........|............ x
// .--------------<-----------------------. y
// ...........|...<...........|............ z
// ...........|...|...........|............ A
// ...........|...<...........|............ B
// ........................................ C

char const *initialLayers[4][29] = {
  {
    // Level 0, latch rods for Stages A (left) and C (right).

    //abcdefghijklmnopqrstuvwxyzABCDEFGHILKLMN
    "........................................", // a
    "...........^............... ............", // b
    "...........|...............v............", // c
    "...........|...............|............", // d
    "...........|...............|............", // e
    "...........|...............|............", // f
    "...........|...............|............", // g
    "...........|...............|............", // h
    "...........^...............|............", // i
    "...........|...............|............", // j
    "...........|...............|............", // k
    "...........|...............|............", // l
    "...........^...............|............", // m
    "...........|...............|............", // n
    "...........|...............|............", // o
    "...........|...............|............", // p
    "...........^...............|............", // q
    "...........|...............|............", // r
    "...........|...............|............", // s
    "...........|...............|............", // t
    "...........|...............v............", // u
    "...........|...............|............", // v
    "...........|...............|............", // w
    "...........|...............|............", // x
    "...........|...............|............", // y
    "...........|...............|............", // z
    "...........^...............|............", // A
    "........... ...............v............", // B
    "........................................"  // C
  },
  {
    // Level 1, logic rods for Stages A (upper 3) and B (lower).

    "........................................", // a
    "........................................", // b
    "........................................", // c
    "........................................", // d
    "........................................", // e
    "........................................", // f
    "........................................", // g
    "........................................", // h
    ".>------^-^^----------1.>   ............", // i
    "........................................", // j
    "........................................", // k
    "........................................", // l
    ".>------^-^^----0------->   ............", // m
    "........................................", // n
    "........................................", // o
    "........................................", // p
    "........^-^^->----1.--------------->   .", // q
    "........................................", // r
    "........................................", // s
    "........................................", // t
    "...........   <---0--<---<-vv-v.........", // u
    "........................................", // .
    "........................................", // w
    "........................................", // x
    "........................................", // y
    "........................................", // z
    "........................................", // A
    "........................................", // B
    "........................................"  // C
  },
  {
    // Level 2, logic rods for Stages D (left-most) and B (right
    // pair).

    "........................................", // a
    "...................>...>................", // b
    "...................|...|................", // c
    "...................>...>................", // d
    "...................>...>................", // e
    "...................|...|................", // f
    "...................|...v................", // g
    "...................|...|................", // h
    "...................|...|................", // i
    "...................|...|................", // j
    "...................v...|................", // k
    "...................|...|................", // l
    "............... ...|...|................", // m
    "............... ...|...|................", // n
    "............... ...v...|................", // o
    "...............^...|...|................", // p
    "...............|...|...|................", // q
    "...............1...0...0................", // r
    "...............|...|...|................", // s
    "...............|...|...|................", // t
    "...............|...|...|................", // u
    "...............|...v...v................", // v
    "...............^... ... ................", // w
    "...............|... ... ................", // x
    "...............<... ... ................", // y
    "...............<........................", // z
    "...............|........................", // A
    "...............<........................", // B
    "........................................"  // C
  },
  {
    // Level 3, latch rods for Stages B (upper) and D (lower).

    "........................................", // a
    "........................................", // b
    "........................................", // c
    "........................................", // d
    ". >---------------->--->------------->-.", // e
    "........................................", // f
    "........................................", // g
    "........................................", // h
    "........................................", // i
    "........................................", // j
    "........................................", // k
    "........................................", // l
    "........................................", // m
    "........................................", // n
    "........................................", // o
    "........................................", // p
    "........................................", // q
    "........................................", // r
    "........................................", // s
    "........................................", // t
    "........................................", // u
    "........................................", // .
    "........................................", // w
    "........................................", // x
    ".<-------------<---------------------< .", // y
    "........................................", // z
    "........................................", // A
    "........................................", // B
    "........................................"  // C
  }
};

enum Stage {
  StageA,  // W->E
  StageB,  // N->S
  StageC,  // E->W
  StageD,  // S->N

  eoStage
};

enum Phase {
  Phase1,  // Unlatch StageB logic
  Phase2,  // Set StageB logic to T
  Phase3,  // Latch StageB logic
  Phase4,  // Unlatch StageA logic
  Phase5,  // Set StageA logic to 0/1
  Phase6,  // Latch StageA logic

  eoPhase
};

class Clock {
 public:
  Clock(long tInitial = 0): t(tInitial) { }
  Clock(Clock const &that): t(that.t) { }
  Clock &operator=(Clock const &that) {
    if (this != &that) {
      t = that.t;
    } return *this;
  }
  Clock &operator++() { ++t; return *this; }
  Stage stage() const { return Stage((t / long(eoPhase)) % long(eoStage)); }
  Phase phase() const { return Phase(t % long(eoPhase)); }
};

enum Parts {
  Wall,
  Slot,

  XHead,
  Head,
  Body,
  Tail,
  XTail,

  Probe,

  KnobT,
  Knob0,
  Knob1,

  DentT,
  Dent0,
  Dent1,

  Latch,

  HeadOrTailOrLatch,
  HeadOrTailOrProbe,

  eoRodParts
};

class CellKey {
 public:
  CellKey(Parts _p, Stage _s = eoStage, Phase _f = eoPhase);
  CellKey(CellKey const &that): p(that.p), s(that.s), f(.that.f) {}
  CellKey &operator=(CellKey const &that) {
    if (this != &that) {
      p = that.p;
      s = that.s;
      f = that.f;
    }
    return *this;
  }
  bool operator<(CellKey const &that) const {
    return
        p < that.p ||
        p == that.p && (s < that.s ||
                        s == that.s && (f < that.f)
                       );
  }

  Part part() const { return p; }
  Stage stage() const { return s; }
  Phase phase() const { return f; }

 private:
  Part p;
  Stage s;
  Phase f;
};

class Cell {
 public:
  Cell(CellKey k, size_t l, size_t i, size_t j);
  Cell(Cell const &that): k(that.k), l(that.l), i(that.i), j(.that.j) {}
  Cell &operator=(Cell const &that) {
    if (this != &that) {
      k = that.k;
      l = that.l;
      i = that.i;
      j = that.j;
    }
    return *this;
  }
  bool operator<(Cell const &that) const {
    return
        k < that.k ||
        k == that.k && (l < that.l ||
                        l == that.l && (i < that.i ||
                                        i == that.i && (j < that.j)
                                       )
                       );
  }

  Part part() const { return k.part(); }
  Stage stage() const { return k.stage(); }
  Phase phase() const { return k.phase(); }

 private:
};

class Block {
 public:
  Block(char const *(*initialLayers)[4][29]);
};

CellKey Block::CharToCellKey(size_t layer, size_t i, size_t j, char c) const {
  switch (c) {
    case '.':
      return CellKey(Wall);
    case ' ':
      return CellKey(Slot);
    case '>':
      swtich (layer) {
        case 0:
          break;
        case 1:
          return CellKey(HeadOrTailOrProbe, StageA);
        case 2:
          return CellKey(Dent, StageB);
        case 3:
          return CellKey(HeadOrTailOrLatch, StageB);
      }
      break;
    case 'v':
      swtich (layer) {
        case 0:
          return CellKey(HeadOrTailOrLatch, StageA);
        case 1:
          return CellKey(Dent, StageC);
        case 2:
          return CellKey(HeadOrTailOrProbe, StageB);
        case 3:
          break;
      }
      break;
    case '<':
      swtich (layer) {
        case 0:
          break;
        case 1:
          return CellKey(HeadOrTailOrProbe, StageC);
        case 2:
          return CellKey(Dent, StageD);
        case 3:
          return CellKey(HeadOrTailOrLatch, StageC);
      }
      break;
    case '^':
      swtich (layer) {
        case 0:
          return CellKey(HeadOrTailOrLatch, StageD);
        case 1:
          return CellKey(Dent, StageD);
        case 2:
          return CellKey(HeadOrTailOrProbe, StageD);
        case 3:
          break;
      }
      break;
    case '-':
    case '|':
      return CellKey(Body);
    case '0':
      return CellKey(Knob0);
    case '1':
      return CellKey(Knob1);
    default:
      break;
  }
  fprintf(stderr,
          "CharToCellKey(level=%ld, i=%ld, j=%ld, c='%c'):"
          " Invalid inital level character!\n",
          level,
          i,
          j,
          c
         );
  return CellKey(Illegal);
}

void Block::InitializeFromInitialLayers(char const *(*initialLayers)[4][29]) {

  // Copy the 'picture' of the initial layers into our cell layer-state
  // space, converting the 'picture' characters.

  for (size_t layer = 0; i < 4; ++i) {
    for (size_t i = 0; i < 29; ++i) {
      for (size_t j = 0; j < 40; ++j) {
        layers[layer][i][j] =
            CharToCellKey(layer, i, j, initialLayers[layer][i][j]);
      }
    }
  }

  // Make sure that the border is all wall....

  for (size_t layer = 0; i < 4; ++i) {
    for (size_t j = 0; j < 40; ++j) {
      assert(layers[layer][ 0][j].part == Wall);
      assert(layers[layer][28][j].part == Wall);
    }
    for (size_t i = 1; i < 28; ++i) {
      assert(layers[layer][i][ 0].part == Wall);
      assert(layers[layer][i][39].part == Wall);
    }
  }

  // Now, walk each layer, building a collection of rods implied by
  // the individual cells.  We'll do some cell translations (since
  // some of the 'picture' characters are overloaded, e.g. ">" is a
  // head, tail, probe, or dent), to remove ambiguity.

  SetOfCells layerHeads[4];

  for (size_t layer = 0; i < 4; ++i) {
    for (size_t i = 1; i < 28; ++i) {
      for (size_t j = 1; j < 39; ++j) {
        Cell &c = layers[layer][i + 0][j + 0];
        Cell &n = layers[layer][i - 1][j + 0];
        Cell &e = layers[layer][i + 0][j + 1];
        Cell &s = layers[layer][i + 1][j + 0];
        Cell &w = layers[layer][i + 0][j - 1];

        switch (c.part()) {
          case Wall:
          case Slot:
          case Body:
            continue;

          case Knob0:
          case Knob1:
          case Dent:
            continue;

          case HeadOrTailOrLatch:
          case HeadOrTailOrProbe:
            switch (c.Stage) {
              case StageA:
                if (e.part() == Slot) {
                  assert(w.part() == Body);
                  c.setPart(Head);
                  layerHeads[layer].insert(c);
                } else if (w.part() == Wall) {
                  assert(e.part() == Body);
                  c.setPart(Tail);
                } else {
                  assert(w.part() == Body && e.part() == Body);
                  c.setPart(c.part() == HeadOrTailOrProbe ? Probe : Latch);
                }
                break;

              case StageB:
                if (s.part() == Slot) {
                  assert(n.part() == Body);
                  c.setPart(Head);
                  layerHeads[layer].insert(c);
                } else if (n.part() == Wall) {
                  assert(s.part() == Body);
                  c.setPart(Tail);
                } else {
                  assert(n.part() == Body && s.part() == Body);
                  c.setPart(c.part() == HeadOrTailOrProbe ? Probe : Latch);
                }
                break;

              case StageC:
                if (w.part() == Slot) {
                  assert(e.part() == Body);
                  c.setPart(Head);
                  layerHeads[layer].insert(c);
                } else if (e.part() == Wall) {
                  assert(e.part() == Body);
                  c.setPart(Tail);
                } else {
                  assert(w.part() == Body && e.part() == Body);
                  c.setPart(c.part() == HeadOrTailOrProbe ? Probe : Latch);
                }
                break;

              case StageD:
                if (n.part() == Slot) {
                  assert(s.part() == Body);
                  c.setPart(Head);
                  layerHeads[layer].insert(c);
                } else if (s.part() == Wall) {
                  assert(n.part() == Body);
                  c.setPart(Tail);
                } else {
                  assert(n.part() == Body && s.part() == Body);
                  c.setPart(c.part() == HeadOrTailOrProbe ? Probe : Latch);
                }
                break;
            }
            break;
        }
      }
    }
  }
}
