#ifndef VOXELTYPE_H
#define VOXELTYPE_H

enum VoxelType {
  VTUnkn,

  VTWall,
  VTSlot,
  VTLock,
  VTData,

  eoVoxelType
};

char const *toConstCharPointer(VoxelType t);

#endif // VOXELTYPE_H
