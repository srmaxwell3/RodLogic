#include "voxelcoordinant.h"
#include "volume.h"

void VoxelCoordinant::Dump(Volume const *volume) const {
  if (volume) {
    Voxel v = volume->voxelAt(*this);
    fprintf(stdout,
            "%s(%d,%d,%d)",
            c_str(v),
            l,
            r,
            c
           );
  } else {
    fprintf(stdout,
            "(VoxelCoordinant *)(%p)->{ l=%d, r=%d, c=%d }",
            this,
            l,
            r,
            c
           );
  }
}
