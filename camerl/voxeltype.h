#ifndef VOXELTYPE_H
#define VOXELTYPE_H

enum VoxelType {
  vtUnkn,

  vtWall,
  vtSlot,
  vtLock,
  vtData,

  eoVoxelType
};

char const *c_str(VoxelType t);

#endif // VOXELTYPE_H
