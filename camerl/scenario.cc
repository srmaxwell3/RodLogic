#include "scenario.h"
#include "volume.h"

Scenario::Scenario(Volume const *volume,
                   TickPerCycle t,
                   VoxelCoordinant const &vc
                  ) :
    tick(t),
    self(volume->voxelAt(vc))
{
  area[DirE] = volume->voxelAt(vc.To(DirE));
  area[DirS] = volume->voxelAt(vc.To(DirS));
  area[DirD] = volume->voxelAt(vc.To(DirD));
  area[DirW] = volume->voxelAt(vc.To(DirW));
  area[DirN] = volume->voxelAt(vc.To(DirN));
  area[DirU] = volume->voxelAt(vc.To(DirU));
}
