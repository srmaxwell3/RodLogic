#include <tuple>
#include <set>

// Density of plexiglass: 1.18 g/cm^3, 0.042630205 lb/in^3
// 0.25x0.25x0.25 in voxel: 0.015625 in^3
// Mass of voxel (full):
//     0.015625 in^3 * 0.042630205 lb/in^3 = 0.000666097 lb
// Mass of voxel, probe/key (triangle): mass of voxel / 2 = 0.000333048 lb
// Mass of voxel, lock (anti-triangle): mass of voxel / 2 = 0.000333048 lb
// Mass of identity/compliment (full): mass of voxel = 0.000666097 lb
// Mass of head/tail (hemi-circle):
//     mass of voxel * ((pi * 0.5^2) / 2) = 0.000261576 lb
// Mass of shortest logic rod (e.g. from osc.txt):
//     hed *  1 = 0.000261576 *  1 = 0.000261576
//     rod * 12 = 0.000666097 * 12 = 0.007993164
//     idn *  1 = 0.000666097 *  1 = 0.000666097
//     prb *  1 = 0.000333048 *  1 = 0.000333048
//     lck *  3 = 0.000333048 *  3 = 0.000999144
//                                 = 0.012607213 lb
// Coefficient of friction, static, plexiglass: 0.8
// Area of 1 face: 0.25^2 in = 0.0625 in^2
// Force on 1 face, at 1 lbf/in^2 = 0.0625 lbf
// Acceleration on shortest logic rod (no friction):
//     F = ma ->
//     a = F/m ->
//     a = 0.0625 lbf / 0.012607213 lb = 160.8 ft/s^2 = 1929.6 in/s^2
// Time to change (no friction) (0.25", 0.50", 0.75"):
//     s = 0.5 * a * t^2 ->
//     t = sqrt(2 * s / a) ->
//     t = sqrt(2 * 0.25 in / 1929.6 in/s^2) = 0.016097238 s
//     t = sqrt(2 * 0.50 in / 1929.6 in/s^2) = 0.022764932 s
//     t = sqrt(2 * 0.75 in / 1929.6 in/s^2) = 0.027881234 s
// Velocity at change (no friction) (0.25", 0.50", 0.75"):
//     v = at ->
//     v @ 0.25 in = 160.8 ft/s^2 * 0.016097238 s = 2.5884358 ft/s
//     v @ 0.55 in = 160.8 ft/s^2 * 0.022764932 s = 3.6606010 ft/s
//     v @ 0.75 in = 160.8 ft/s^2 * 0.027881234 s = 4.4833024 ft/s
// Kenetic energy at change (no friction) (0.25", 0.50", 0.75"):
//     E = 0.5 * m * v^2
//     E @ 0.25 in =
//         0.5 * 0.012607213 lb * (2.5884358 ft/s)^2 = 0.042234163 ft*lbf
//     E @ 0.50 in =
//         0.5 * 0.012607213 lb * (3.6606010 ft/s)^2 = 0.084468325 ft*lbf
//     E @ 0.25 in =
//         0.5 * 0.012607213 lb * (4.4833024 ft/s)^2 = 0.126702493 ft*lbf
class Entity;

class EntityProperties {
 public:
  EntityProperties();
  virtual ~EntityProperties() = 0;
  virtual void PreTickReset() = 0;
  virtual bool IsBlocked() = 0;
  virtual bool IsLatched() = 0;
};

typedef size_t VoxelState;
typedef VoxelState Neighborhood[5];

struct VoxelPosition {
  VoxelPosition(size_t _l, size_t _r, size_t _c) :
      l(_l), r(_r), c(_c)
  {
  }
  size_t l;
  size_t r;
  size_t c;
  bool operator<(VoxelPosition const &that) const {
    return std::tie(l, r, c) < std::tie(that.l, that.r, that.c);
  }
};

class Voxel: public EntityProperties {
  Voxel(Entity *_parent = , VoxelState initialState = 0);
  ~Voxel();

  // From EntityProperties

  VoxelState State() const { return state; }
  void PreTickReset();
  bool IsBlocked();
  bool IsLatched();

 private:
  Entity *parent;
  VoxelState state;
};

class Entity: public EntityProperties : set<VoxelPosition> {
  Entity();
  ~Entity();

  // From EntityProperties

  void PreTickReset();
  bool IsBlocked();
  bool IsLatched();
};

struct VoxelProperties {
};

typedef map<VoxelState, VoxelProperties *> MapVoxelStateToProperties;

template<size_t nLayers, size_t nRows, size_t nCols> class Volume {
 public:
  size_t const NLayers = nLayers;
  size_t const NRows = nRows;
  size_t const NCols = nCols;

  typedef array<array<array<nCols, VoxelState>, nRows>, nLayers> Voxels;

  Volume();
  ~Volume();

  Entity *AddEntity(string const &_name);
  void Scan();

  Voxels volume[2];
  size_t activeVolume;
};

Volume::Volume() : activeVolume(0) {
}

Volume::~Volume() {
}

void Volume::Scan() {
  Voxels const &pVolume = volume[(activeVolume + 0) % 2];
  Voxels const &aVolume = volume[(activeVolume + 1) % 2];

  for (size_t l = 0; l < NLayers; ++l) {
    for (size_t r = 0; r < NRows; ++r) {
      for (size_t c = 0; c < NCols; ++c) {
        Neighborhood n = { 0, 0, 0, 0, 0 };

        n[0] =                   pVolume[l][r + 0][c + 0].State();
        n[1] = 0 < r ?           pVolume[l][r - 1][c + 0].State() : 0;
        n[2] = 0 < c ?           pVolume[l][r + 0][c - 1].State() : 0;
        n[3] = c < (NCols - 1) ? pVolume[l][r + 0][c + 1].State() : 0;
        n[4] = r < (NRows - 1) ? pVolume[l][r + 1][c + 0].State() : 0;
      }
    }
  }
}

int main(int argc, char const *argv[]) {
  size_t const NLayers = 7;
  size_t const NRows = 21;
  size_t const NCols = 21;

  Volume<NLayers, NRows, NCols> osc;

  // Layer 0 (1)

  Entity *NELatch = osc->AddEntity("NELatch");
  {
    size_t const l =  0;
    size_t const c =  4;
    for (size_t r = 0; r < NRows; ++r) {
      switch (r) {
        case  9:
          NELatch->Add(l + 0, r, c, KKey, N);
          NELatch->Add(l + 1, r, c, KKey, N);
          break;
        case 10:
          osc[l + 1][r][c] = Slot;
          break;
        default:
          NELatch->Add(l + 0, r, c, KRod, N);
          break;
      }
    }
  }

  Entity *SWLatch = osc->AddEntity("SWLatch");
  {
    size_t const l =  0;
    size_t const c = 16;
    for (size_t r = 0; r < NRows; ++r) {
      switch (r) {
        case 10:
          osc[l + 1][r][c] = Slot;
          break;
        case 11:
          NELatch->Add(l + 0, r, c, KKey, S);
          NELatch->Add(l + 1, r, c, KKey, S);
          break;
        default:
          NELatch->Add(l + 0, r, c, KRod, S);
          break;
      }
    }
  }


  // Layer (1) 2 (3)

  Entity *ELogic = osc->AddELogic("ELogic");
  {
    size_t const l =  2;
    size_t const r = 8;
    for (size_t c = 0; c < NCols; ++c) {
      switch (c) {
        case  0:
          break;
        case  1:
        case  3:
        case  4:
          ELogic->Add(l + 0, r, c, LLck, E);
          ELogic->Add(l - 1, r, c, LLck, E);
          break;
        case  7:
          ELogic->Add(l + 0, r, c, LPrb, E);
          ELogic->Add(l + 1, r, c, LPrb, E);
          break;
        case  9:
          ELogic->Add(l + 0, r, c, LCom, E);
          ELogic->Add(l + 1, r, c, LCom, E);
          break;
        case 20:
          ELogic->Add(l + 0, r, c, LOut, E);
          break;
        default:
          ELogic->Add(l + 0, r, c, LRod, E);
          break;
      }
    }
    osc[l - 1][r][ 2] = Slot;
    osc[l - 1][r][ 5] = Slot;
    osc[l - 1][r][ 6] = Slot;
    osc[l - 1][r][ 7] = Slot;
  }

  Entity *WLogic = osc->AddELogic("WLogic");
  for (size_t c = 0; c < NCols; ++c) {
    switch (c) {
      case  0:
      case  1:
      case  2:
      case  3:
        break;
      case  4:
      case  5:
      case  6:
        osc[1][12][c] = Slot;
        break;
      case  7:
        WLogic->Add(1, r, 4, LHed, W);
        break;
      case  9:
        WLogic->Add(1, r, 4, LIdn, W);
        WLogic->Add(2, r, 4, LIdn, W);
        break;
      case 13:
        WLogic->Add(1, r, 4, LPrb, W);
        WLogic->Add(2, r, 4, LPrb, W);
        break;
      case 20:
        WLogic->Add(1, r, 4, LOut, W);
        break;
      case 16:
      case 17:
      case 19:
        WLogic->Add(0, r, 4, LLck, W);
        WLogic->Add(1, r, 4, LLck, W);
        break;
      default:
        WLogic->Add(1, r, 4, LRod, W);
        break;
    }
  }

  // Layer (3) 4 (5)

  Entity *SLogic = osc->AddELogic("SLogic");
  Entity *NLogic = osc->AddELogic("NLogic");

  // Layer (5) 6

  Entity *ESLatch = osc->AddEntity("ESLatch");
  Entity *WNLatch = osc->AddEntity("WNLatch");

  return 0;
}
