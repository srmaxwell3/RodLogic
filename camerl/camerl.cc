// ┃┇━┅▲△▶▷▼▽◀◁⬛
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <array>
using std::array;
#include <fstream>
using std::ifstream;
#include <map>
using std::map;
#include <set>
using std::set;
#include <string>
using std::string;
#include <deque>
using std::deque;
#include <initializer_list>
using std::initializer_list;
#include <iostream>
using std::istream;
using std::cin;
using std::cerr;
using std::cout;
#include <unistd.h>

#include "ncurses.h"

bool optLittleEndian = false;
bool optCycleCountSet = false;
int optCycleCount = 0;
bool optDelayMSecSet = false;
int optDelayMSec = 0;
string optInputPath = "";
bool optEchoInput = false;
string optTableInputPath = "";
bool optShowEvaluatingRods = false;
bool optShowPerformance = false;
bool optShowRods = false;
bool optShowChangedStateEveryTick = false;
bool optShowStateEveryTick = false;
bool optVerbose = false;
bool optWarnings = false;

#include "volume.h"

void processVolumeFrom(istream &in) {
  Volume volume(in);

  // #include "cam.8.rules.h"

  volume.AddInput
      (VoxelCoordinant(2, 16, 8),
       { dsSet0, dsSet1, dsSet0, dsSet1,
         dsSet0, dsSet0, dsSet0, dsSet1,
         dsSet0, dsSet1, dsSet0, dsSet0
       }
      );
  volume.AddInput
      (VoxelCoordinant(2, 20, 8),
       { dsSet0, dsSet0, dsSet0, dsSet1,
         dsSet0, dsSet1, dsSet0, dsSet0,
         dsSet0, dsSet1, dsSet0, dsSet0
       }
      );

  if (optDelayMSecSet) {
   initscr();			// Start curses mode
   mvprintw(0, 0, "Press any key to start.");
   getch();
   refresh();
  }

  for (size_t t = 0; t < (8 * NTicksPerCycle); t += 1) {
    volume.PrintViewFlat();
    volume.ProceedOneTick();
    if (volume.FoundNewRules()) {
      break;
    }
    if (optDelayMSecSet) {
      usleep(optDelayMSec);
    }
  }
  volume.PrintViewFlat();

  if (optDelayMSec) {
    refresh();
    printw("\n\nPress any key to exit.");
    refresh();
    getch();
    endwin();
  } else {
    volume.DumpUnusedRules();
    volume.DumpPerformance();
  }
}

int main(int argc, char *const argv[]) {
  int c;
  while ((c = getopt(argc, argv, "bc:d:ei:I:lprsSt:T:vw")) != -1) {
    switch (c) {
      case 'b':
        optLittleEndian = false;
        break;
      case 'c':
        if (sscanf(optarg, "%i", &optCycleCount) != 1 || optCycleCount < 0) {
          fprintf(stderr, "cam.r: Error: Option -%c requires a [positive] integer argument.\n", c);
          return 1;
        }
        optCycleCountSet = true;
        break;
      case 'd':
        if (sscanf(optarg, "%i", &optDelayMSec) != 1 || optDelayMSec < 0) {
          fprintf(stderr, "cam.r: Error: Option -%c requires a [positive] integer argument.\n", c);
          return 1;
        }
        optDelayMSecSet = true;
        optDelayMSec *= 1000;
        break;
      case 'e':
        optShowEvaluatingRods = true;
        break;
      case 'i':
        optInputPath = optarg;
        optEchoInput = false;
        break;
      case 'I':
        optInputPath = optarg;
        optEchoInput = true;
        break;
      case 'l':
        optLittleEndian = true;
        break;
      case 'p':
        optShowPerformance = true;
        break;
      case 'r':
        optShowRods = true;
        break;
      case 's':
        optShowChangedStateEveryTick = false;
        optShowStateEveryTick = true;
        break;
      case 'S':
        optShowChangedStateEveryTick = true;
        optShowStateEveryTick = true;
        break;
      case 't':
        optTableInputPath = optarg;
        optEchoInput = false;
        break;
      case 'T':
        optTableInputPath = optarg;
        optEchoInput = true;
        break;
      case 'v':
        optVerbose = true;
        break;
      case 'w':
        optWarnings = true;
        break;
      case '?':
        if (optopt == 'c') {
          fprintf(stderr, "cam.r: Error: Option -%c requires an argument.\n", optopt);
        } else if (isprint(optopt)) {
          fprintf(stderr, "cam.r: Error: Unknown option `-%c'.\n", optopt);
        } else {
          fprintf(stderr, "cam.r: Error: Unknown option character `\\x%x'.\n", optopt);
        }
        return 1;
      default:
        abort();
    }
  }

  if (!optDelayMSecSet) {
    fprintf(stdout, "%s -c %d", argv[0], optCycleCount);
    if (optShowEvaluatingRods) {
      fprintf(stdout, " -e");
    }
    if (!optInputPath.empty()) {
      fprintf(stdout, " -i %s", optInputPath.c_str());
    }
    if (optShowRods) {
      fprintf(stdout, " -s");
    }
    if (!optTableInputPath.empty()) {
      fprintf(stdout, " -t %s", optTableInputPath.c_str());
    }
    if (optind < argc) {
      for (int a = optind; a < argc; a += 1) {
        fprintf(stdout, " %s", argv[a]);
      }
    } else {
      fprintf(stdout, " <stdin>");
    }
    fprintf(stdout, "\n");
  }

  if (optind < argc) {
    for (int a = optind; a < argc; a += 1) {
      ifstream dFile(argv[a]);
      if (!dFile.is_open()) {
        fprintf(stderr,
                "cam.r: Error: Unable to open volume %s for reading!\n",
                argv[a]
               );
        return 1;
      }
      processVolumeFrom(dFile);
      dFile.close();
    }
  } else {
    processVolumeFrom(cin);
  }

  return 0;
}
