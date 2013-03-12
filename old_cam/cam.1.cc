class Interactions {
 public:
  Interactions() {}

  void RecheckInteractions() = 0;

  bool isBlockable() const = 0;
  bool isBlocked() const = 0;
  void block() const = 0;
  void unblock() const = 0;

  bool isLatchable() const = 0;
  bool isLatched() const = 0;
  void latch() const = 0;
  void unlatch() const = 0;
};

struct VoxelLocator {
  size_t lvl;
  size_t row;
  size_t col;
};

class Entity: public set<VoxelLocator> {
 public:
  Entity(string const &_name);
  Entity();
  ~Entity();

  // From Interactions....

  void RecheckInteractions();

  bool isBlockable();
  bool isBlocked();
  void block();
  void unblock();

  bool isLatchable();
  bool isLatched();
  void latch();
  void unlatch();

 private:
  string name;
};

class Voxel: public Interactions {
  Voxel(int _state = 0);
  ~Voxel();

  // From Interactions....

  void RecheckInteractions();

  bool isBlockable();
  bool isBlocked();
  void block();
  void unblock();

  bool isLatchable();
  bool isLatched();
  void latch();
  void unlatch();

 private:
  Entity *entity;
};

enum VoxelKind {
  Wall,
  Slot,

  LHead,
  LBody,
  LTail,
  LLock,
  LGate,
  LProb,
  LInpt,
  LOutp,

  KBody,
  KLtch,

  EoVoxelKind
};

enum Dir {
  N,
  E,
  S,
  W,

  EoDir
};

class Direction {
 public:
  Direction(Dir _activeDirection = EoDir) : activeDirection(_activeDirection) {}
  Dir forward() const { return activeDirection; }
  Dir backward() const { return backDir[activeDirection]; }
  Dir next() const { return nextDir[activeDirection]; }
  Dir last() const { return lastDir[activeDirection]; }

 private:
  static Dir backDir[EoDir];
  static Dir nextDir[EoDir];
  static Dir lastDir[EoDir];

  Dir activeDirection;
};

//                                N  E  S  W
Dir Direction::backDir[EoDir] = { S, W, N, E };
Dir Direction::nextDir[EoDir] = { E, S, W, N };
Dir Direction::lastDir[EoDir] = { W, N, E, S };

struct VoxelStateAttributes {
  VoxelKind kind;
  Dir activeDirection;
  bool isBlockable;
  bool isLatchable;
} cellStateAttributes[] = {
  { LHead, E, true,  false, },
  { LBody, E, false, false, },
  { LTail, E, true,  false, },
  { LLock, E, false, true,  },
  { LGate, E, false, false, },
  { LProb, E, true,  false, },
  { LInpt, E, false, false, },
  { LOutp, E, false, false, },

  { KBody, E, false, false, },
  { KLtch, E, true,  false, },
};

template<size_t NLvls, size_t NRows, size_t NCols>
class Volume: public array<array<array<size_t, NCols>, NRows>, NLvls> {
 public:
  Volume();
  ~Volume();

 private:
  vector<Entity *> entities;
};

Volume::Volume() {
  fill(0);
}

Volume::~Volume() {
  for (auto e: entities) {
    delete *e;
  }
  entities.clear();
}
