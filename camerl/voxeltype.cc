#include "voxeltype.h"

char const *toConstCharPointer(VoxelType t) {
  switch (t) {
    case vtUnkn: return "vtUnkn";
    case vtWall: return "vtWall";
    case vtSlot: return "vtSlot";
    case vtLock: return "vtLock";
    case vtData: return "vtData";
    case eoVoxelType:
      return "eoVoxelType";
  }
  return "VoxelType(?)";
}
