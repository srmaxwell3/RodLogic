#include "scenario.h"
#include "volume.h"

Scenario::Scenario(Volume const *volume,
                   TickPerCycle t,
                   VoxelCoordinant const &vc
                  ) :
    tick(t),
    self(volume->voxelAt(vc))
{
  area[E] = volume->voxelAt(vc.To(E));
  area[S] = volume->voxelAt(vc.To(S));
  area[D] = volume->voxelAt(vc.To(D));
  area[W] = volume->voxelAt(vc.To(W));
  area[N] = volume->voxelAt(vc.To(N));
  area[U] = volume->voxelAt(vc.To(U));
}
