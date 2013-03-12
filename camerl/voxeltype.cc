#include "voxeltype.h"

char const *toConstCharPointer(VoxelType t) {
  switch (t) {
    case VTUnkn: return "VTUnkn";
    case VTWall: return "VTWall";
    case VTSlot: return "VTSlot";
    case VTLock: return "VTLock";
    case VTData: return "VTData";
    case eoVoxelType:
      return "eoVoxelType";
  }
  return "VoxelType(?)";
}
