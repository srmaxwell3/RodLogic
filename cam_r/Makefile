HOSTNAME = $(shell hostname)
ifeq ($(HOSTNAME),Sidneys-MacBook-Pro.local)
CXX = clang++ -std=c++11 -stdlib=libc++ $(DEBUG) $(OPT)
else
CXX = g++ -std=c++0x $(DEBUG) $(OPT)
endif
DEBUG = -g
OPT = -O0
LIBS = -lncurses

ALL_OBJS = \
	cam.r.o \
	diagram2d.o \
	directions.o \
	label.o \
	p2d.o \
	rod_connection.o \
	rod2d.o

all:	cam.r test.dcff test.74181 test.16x16bytes

clean:
	$(RM) $(ALL_OBJS) dcff.log 74181.log 16x16bytes.log

clobber:	clean
	$(RM) ./cam.r

cam.r:	$(ALL_OBJS)
	$(CXX) -o cam.r $(ALL_OBJS) $(LIBS)

cam.r.o: \
	cam.r.cc \
	diagram2d.h
	$(CXX) -c cam.r.cc

diagram2d.o: \
	diagram2d.cc \
	diagram2d.h \
	rod2d.h
	$(CXX) -c diagram2d.cc

directions.o: \
	directions.cc \
	directions.h
	$(CXX) -c directions.cc

label.o: \
	label.cc \
	label.h
	$(CXX) -c label.cc

p2d.o: \
	p2d.cc \
	p2d.h \
	diagram2d.h
	$(CXX) -c p2d.cc

rod_connection.o: \
	rod_connection.cc \
	rod_connection.h \
	directions.h
	$(CXX) -c rod_connection.cc

rod2d.o: \
	rod2d.cc \
	rod2d.h \
	directions.h
	$(CXX) -c rod2d.cc

test.dcff:	dcff.txt dcff.input.txt dcff.log.good
	./cam.r -c 15 -e -i dcff.input.txt -s dcff.txt > dcff.log
	diff -c dcff.log.good dcff.log


test.74181:	74181.txt 74181.log.good
	./cam.r 74181.txt > 74181.log
	diff -c 74181.log.good 74181.log

test.16x16bytes:	16x16bytes.txt 16x16bytes.log.good
	./cam.r 16x16bytes.txt > 16x16bytes.log
	diff -c 16x16bytes.log.good 16x16bytes.log
