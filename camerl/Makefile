HOSTNAME = $(shell hostname)
ifeq ($(HOSTNAME),Sidneys-MacBook-Pro.local)
CXX = clang++ -std=c++11 -stdlib=libc++ $(DEBUG) $(OPT)
else
CXX = g++ -std=c++0x $(DEBUG) $(OPT)
endif
# CXX = clang++ -std=c++11 -stdlib=libc++ $(DEBUG) $(OPT)
DEBUG = -g
OPT = -O0
LIBS = -lncurses
zzzzzzzz =

ALL_OBJS = \
	camerl.o \
	direction.o \
	fwdorbwd.o \
	tickperphase.o \
	tickpercycle.o \
	tickpercycleproperties.o \
	voxelcoordinant.o \
	fblkstate.o \
	rblkstate.o \
	voxeltype.o \
	locktype.o \
	lockstate.o \
	datatype.o \
	datastate.o \
	blockstate.o \
	rodtype.o \
	voxel.o \
	scenario.o \
	volume.o \
	item.o \
	lockrod.o \
	datarod.o \
	$(zzzzzzzz)

all:	camerl

clean:
	$(RM) $(ALL_OBJS)

clobber:	clean
	$(RM) ./camerl

camerl:		$(ALL_OBJS)
	$(CXX) -o camerl $(ALL_OBJS) $(LIBS)

camerl.o:	camerl.cc

direction.o:	direction.cc direction.h

fwdorbwd.o:	fwdorbwd.cc fwdorbwd.h

tickperphase.o:	tickperphase.cc tickperphase.h

tickpercycle.o:	tickpercycle.cc tickpercycle.h rodtype.h fwdorbwd.h

tickpercycleproperties.o:	tickpercycleproperties.cc tickpercycleproperties.h rodtype.h fwdorbwd.h

voxelcoordinant.o:	voxelcoordinant.cc voxelcoordinant.h volume.h

fblkstate.o:	fblkstate.cc fblkstate.h

rblkstate.o:	rblkstate.cc rblkstate.h

voxeltype.o:	voxeltype.cc voxeltype.h

locktype.o:	locktype.cc locktype.h

lockstate.o:	lockstate.cc lockstate.h

datatype.o:	datatype.cc datatype.h

datastate.o:	datastate.cc datastate.h

blockstate.o:	blockstate.cc blockstate.h

rodtype.o:	rodtype.cc rodtype.h voxeltype.h tickpercycle.h direction.h

voxel.o:	voxel.cc voxel.h voxeltype.h rodtype.h \
		locktype.h lockstate.h datatype.h datastate.h \
		blockstate.h direction.h

scenario.o:	scenario.cc scenario.h tickpercycle.h voxel.h direction.h \
		voxelcoordinant.h

volume.o:	volume.cc volume.h

item.o:		item.cc item.h

lockrod.o:	lockrod.cc lockrod.h

datarod.o:	datarod.cc datarod.h

