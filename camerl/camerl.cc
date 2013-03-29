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
#include "datastate.h"

void processInputFrom(istream &in, map<string, deque<DataState>> &inputs) {
  unsigned lineNumber = 0;

  inputs.clear();

  do {
    string line;
    getline(in, line);
    if (!in.good()) {
      continue;
    }

    lineNumber += 1;

    char const *sStart = line.c_str();
    char const *sPtr = sStart;
    unsigned sCnt;
    char sBuf[1024];

    // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

    if (sscanf(sPtr, " %[#]", sBuf) == 1) {
      continue;
    }

    if (sscanf(sPtr, " %[{]%n", sBuf, &sCnt) != 1) {
      fprintf(stderr,
              "rlic: Error: %d, %ld: Input syntax error; "
              "expected '^ >[#{]<' at '%s'!\n",
              lineNumber,
              ((sPtr + sCnt) - sStart),
	      sPtr
             );
      continue;
    }
    sPtr += sCnt;

    // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

    string name;
    if (sscanf(sPtr, " \"%[^\",]\" , %n", sBuf, &sCnt) == 1 ||
        sscanf(sPtr, " \'%[^\',]\' , %n", sBuf, &sCnt) == 1 ||
        sscanf(sPtr, " %[^, ] , %n", sBuf, &sCnt) == 1
        )
    {
      name = sBuf;
    } else {
      fprintf(stderr,
              "rlic: Error: %d, %ld: Input syntax error; "
              "expected '^{ >\\w+< ,' (symbol name) at '%s'!\n",
              lineNumber,
              ((sPtr + sCnt) - sStart),
	      sPtr
             );
      continue;
    }
    sPtr += sCnt;

    // fprintf(stderr, "%d: name='%s'\n", lineNumber, name.c_str());
    // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

    if (sscanf(sPtr, " %[{]%n", sBuf, &sCnt) != 1) {
      fprintf(stderr,
              "rlic: Error: %d, %ld: Input syntax error; "
              "expected '^{ \\w+ , >{<' at '%s'!\n",
              lineNumber,
              ((sPtr + sCnt) - sStart),
	      sPtr
             );
      continue;
    }
    sPtr += sCnt;

    deque<DataState> values;
    char symbol[128];
    int value;

    // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

    if (sscanf(sPtr, " %i %n", &value, &sCnt) == 1) {
      values.push_back(value == 0 ? dsSet0 : dsSet1);
      sPtr += sCnt;

      // fprintf(stderr, "%d: value=%d\n", lineNumber, value);
      // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

      while (sscanf(sPtr, " , %i %n", &value, &sCnt) == 1) {
        values.push_back(value == 0 ? dsSet0 : dsSet1);
        sPtr += sCnt;

        // fprintf(stderr, "%d: , value=%d\n", lineNumber, value);
      }

      // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

      if (sscanf(sPtr, " %[}]%n", sBuf, &sCnt) != 1) {
        fprintf(stderr,
                "rlic: Error: %d, %ld: Input syntax error; "
                "expected '^{ \\w+, { \\d+ ( , \\d+ )* >}<' at '%s'!\n",
                lineNumber,
                ((sPtr + sCnt) - sStart),
		sPtr
               );
        continue;
      }
      sPtr += sCnt;
    } else if (sscanf(sPtr, " %[^,} ] %n", symbol, &sCnt) == 1) {
      DataState state = toDataState(symbol);
      if (state == eoDataState) {
        fprintf(stderr,
                "rlic: Error: %d, %ld: Input syntax error; "
                "expected '^{ \\w+ , { ><DataState>< ( , <DataState> )* }' at '%s'!\n",
                lineNumber,
                ((sPtr + sCnt) - sStart),
		sPtr
               );
        continue;
      }
      values.push_back(state);
      sPtr += sCnt;

      // fprintf(stderr, "%d: value=%d\n", lineNumber, value);
      // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

      while (sscanf(sPtr, " , %[^,} ] %n", symbol, &sCnt) == 1) {
        DataState state = toDataState(symbol);
        if (state == eoDataState) {
          fprintf(stderr,
                  "rlic: Error: %d, %ld: Input syntax error; "
                  "expected '^{ \\w+ , { <DataState> ( , ><DataState>< )* }' at '%s'!\n",
                  lineNumber,
                  ((sPtr + sCnt) - sStart),
                  sPtr
                  );
          continue;
        }
        values.push_back(state);
        sPtr += sCnt;

        // fprintf(stderr, "%d: , value=%d\n", lineNumber, value);
      }

      // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

      if (sscanf(sPtr, " %[}]%n", sBuf, &sCnt) != 1) {
        fprintf(stderr,
                "rlic: Error: %d, %ld: Input syntax error; "
                "expected '^{ \\w+ , { <DataState> ( , <DataState> )* >}<' at '%s'!\n",
                lineNumber,
                ((sPtr + sCnt) - sStart),
		sPtr
               );
        continue;
      }
      sPtr += sCnt;
    } else {
      fprintf(stderr,
              "rlic: Error: %d, %ld: Input syntax error; "
              "expected '^{ \\w+, { >\\d+<' (integer->bool value) at '%s'!\n",
              lineNumber,
              ((sPtr + sCnt) - sStart),
	      sPtr
             );
      continue;
    }

    // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

    if (sscanf(sPtr, " %[}]%n", sBuf, &sCnt) != 1) {
      fprintf(stderr,
              "rlic: Error: %d, %ld: Input syntax error; "
              "expected '^{ \\w+, { \\d+ ( , \\d+ ) } >}<' at '%s'!\n",
              lineNumber,
              ((sPtr + sCnt) - sStart),
	      sPtr
             );
      continue;
    }
    sPtr += sCnt;

    inputs[name] = values;
  } while (in.good() && !in.eof());
}

void processTableInputFrom(istream &in, map<string, deque<DataState>> &inputs) {
  unsigned lineNumber = 0;

  inputs.clear();

  deque<string> names;
  do {
    string line;
    getline(in, line);
    if (!in.good()) {
      continue;
    }

    lineNumber += 1;

    char const *sStart = line.c_str();
    char const *sPtr = sStart;
    unsigned sCnt;
    char sBuf[1024];

    // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

    if (sscanf(sPtr, " %[{]%n", sBuf, &sCnt) != 1) {
      fprintf(stderr,
              "rlic: Error: %d, %ld: Input syntax error; "
              "expected '^ >[#{]<' at '%s'!\n",
              lineNumber,
              ((sPtr + sCnt) - sStart),
	      sPtr
             );
      continue;
    }
    sPtr += sCnt;

    // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

    if (sscanf(sPtr, " %[#]", sBuf) == 1) {
      continue;
    }

    string name;
    if (sscanf(sPtr, " \"%[^\",]\"%n", sBuf, &sCnt) == 1 ||
        sscanf(sPtr, " \'%[^\',]\'%n", sBuf, &sCnt) == 1 ||
        sscanf(sPtr, " %[^,} ]%n", sBuf, &sCnt) == 1
        )
    {
      name = sBuf;

      // fprintf(stderr, "%d: name='%s'\n", lineNumber, name.c_str());
      names.push_back(name);
      sPtr += sCnt;
    } else {
      fprintf(stderr,
              "rlic: Error: %d, %ld: Input syntax error; "
              "expected '^ >\\w+<' (first symbol name) at '%s'!\n",
              lineNumber,
              ((sPtr + sCnt) - sStart),
	      sPtr
             );
      continue;
    }
    while (sscanf(sPtr, " , \"%[^\"]\"%n", sBuf, &sCnt) == 1 ||
           sscanf(sPtr, " , \'%[^\']\'%n", sBuf, &sCnt) == 1 ||
           sscanf(sPtr, " , %[^,} ]%n", sBuf, &sCnt) == 1
          )
    {
      name = sBuf;

      // fprintf(stderr, "%d: name='%s'\n", lineNumber, name.c_str());

      names.push_back(name);
      sPtr += sCnt;
    }

    // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

    if (sscanf(sPtr, " %[}]%n", sBuf, &sCnt) != 1) {
      fprintf(stderr,
              "rlic: Error: %d, %ld: Input syntax error; "
              "expected '^{ \\w+, >( , \\w+ ) }<' (additional symbol name(s), and '}') at '%s'!\n",
              lineNumber,
              ((sPtr + sCnt) - sStart),
	      sPtr
             );
    }
  } while (names.empty() && in.good() && !in.eof());

  do {
    string line;
    getline(in, line);
    if (!in.good()) {
      continue;
    }

    lineNumber += 1;

    char const *sStart = line.c_str();
    char const *sPtr = sStart;
    unsigned sCnt;
    char sBuf[1024];

    // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

    if (sscanf(sPtr, " %[#]", sBuf) == 1) {
      continue;
    }

    if (sscanf(sPtr, " %[{]%n", sBuf, &sCnt) != 1) {
      fprintf(stderr,
              "rlic: Error: %d, %ld: Input syntax error; "
              "expected '^ >[#{]<' at '%s'!\n",
              lineNumber,
              ((sPtr + sCnt) - sStart),
	      sPtr
             );
      continue;
    }
    sPtr += sCnt;

    deque<DataState> values;
    char symbol[128];
    int value;

    // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

    if (sscanf(sPtr, " %i %n", &value, &sCnt) == 1) {
      values.push_back(value == 0 ? dsSet0 : dsSet1);
      sPtr += sCnt;

      // fprintf(stderr, "%d: value=%d\n", lineNumber, value);
      // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

      while (sscanf(sPtr, " , %i %n", &value, &sCnt) == 1) {
        values.push_back(value == 0 ? dsSet0 : dsSet1);
        sPtr += sCnt;

        // fprintf(stderr, "%d: , value=%d\n", lineNumber, value);
      }

      // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

      if (sscanf(sPtr, " %[}]%n", sBuf, &sCnt) != 1) {
        fprintf(stderr,
                "rlic: Error: %d, %ld: Input syntax error; "
                "expected '^{ \\w+, { \\d+ ( , \\d+ )* >}<' at '%s'!\n",
                lineNumber,
                ((sPtr + sCnt) - sStart),
		sPtr
               );
        continue;
      }
      sPtr += sCnt;
    } else if (sscanf(sPtr, " %[^,} ] %n", symbol, &sCnt) == 1) {
      DataState state = toDataState(symbol);
      if (state == eoDataState) {
        fprintf(stderr,
                "rlic: Error: %d, %ld: Input syntax error; "
                "expected '^{ \\w+ , { ><DataState>< ( , <DataState> )* }' at '%s'!\n",
                lineNumber,
                ((sPtr + sCnt) - sStart),
		sPtr
               );
        continue;
      }
      values.push_back(state);
      sPtr += sCnt;

      // fprintf(stderr, "%d: value=%d\n", lineNumber, value);
      // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

      while (sscanf(sPtr, " , %[^,} ] %n", symbol, &sCnt) == 1) {
        DataState state = toDataState(symbol);
        if (state == eoDataState) {
          fprintf(stderr,
                  "rlic: Error: %d, %ld: Input syntax error; "
                  "expected '^{ \\w+ , { <DataState> ( , ><DataState>< )* }' at '%s'!\n",
                  lineNumber,
                  ((sPtr + sCnt) - sStart),
                  sPtr
                  );
          continue;
        }
        values.push_back(state);
        sPtr += sCnt;

        // fprintf(stderr, "%d: , value=%d\n", lineNumber, value);
      }

      // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

      if (sscanf(sPtr, " %[}]%n", sBuf, &sCnt) != 1) {
        fprintf(stderr,
                "rlic: Error: %d, %ld: Input syntax error; "
                "expected '^{ \\w+ , { <DataState> ( , <DataState> )* >}<' at '%s'!\n",
                lineNumber,
                ((sPtr + sCnt) - sStart),
		sPtr
               );
        continue;
      }
      sPtr += sCnt;
    } else {
      fprintf(stderr,
              "rlic: Error: %d, %ld: Input syntax error; "
              "expected '^{ \\w+, { >\\d+<' (integer->bool value) at '%s'!\n",
              lineNumber,
              ((sPtr + sCnt) - sStart),
	      sPtr
             );
      continue;
    }

    if (names.size() < values.size()) {
      fprintf(stderr,
              "rlic: Warning: "
              "Table row %d has too many values (has %lu, expected %lu); "
              "ignoring extra.\n",
              lineNumber,
              values.size(),
              names.size()
             );
    } else if (values.size() < names.size()) {
      fprintf(stderr,
              "rlic: Warning: "
              "Table row %d has too few values (has %lu, expected %lu); "
              "filling with 0's.\n",
              lineNumber,
              values.size(),
              names.size()
             );
    }

    for (size_t n = 0; n < names.size(); n += 1) {
      inputs[names[n]].push_back(n < values.size() ? values[n] : dsUnkn);
    }
  } while (in.good() && !in.eof());
}

void processVolumeFrom(istream &in, map<string, deque<DataState>> &inputs) {
  Volume volume(in);

  // #include "cam.8.rules.h"

  // volume.AddInput
  //     ("C",
  //      { dsSet0, dsSet1, dsSet0, dsSet1,
  //        dsSet0, dsSet0, dsSet0, dsSet1,
  //        dsSet0, dsSet1, dsSet0, dsSet0
  //      }
  //     );
  // volume.AddInput
  //     ("D",
  //      { dsSet0, dsSet0, dsSet0, dsSet1,
  //        dsSet0, dsSet1, dsSet0, dsSet0,
  //        dsSet0, dsSet1, dsSet0, dsSet0
  //      }
  //     );

  int maxInputLength = optCycleCount;
  for (auto const &nameAndValues : inputs) {
    string const &name = nameAndValues.first;
    deque<DataState> const &values = nameAndValues.second;

    volume.AddInput(name, values);
    maxInputLength = std::max(maxInputLength, int(values.size()));
  }

  if (optDelayMSecSet) {
    fprintf(stdout, "Press any key to start.");
    fgetc(stdin);

    initscr();			// Start curses mode
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

  map<string, deque<DataState>> inputs;

  if (!optInputPath.empty()) {
    ifstream iFile(optInputPath);
    if (!iFile.is_open()) {
      fprintf(stderr,
              "rlic: Error: Unable to open input %s for reading!\n",
              optInputPath.c_str()
             );
      return 1;
    }
    processInputFrom(iFile, inputs);
  }

  if (!optTableInputPath.empty()) {
    ifstream iFile(optTableInputPath);
    if (!iFile.is_open()) {
      fprintf(stderr,
              "rlic: Error: Unable to open [table] input %s for reading!\n",
              optTableInputPath.c_str()
             );
      return 1;
    }
    processTableInputFrom(iFile, inputs);
  }

  if (optind < argc) {
    for (int a = optind; a < argc; a += 1) {
      ifstream dFile(argv[a]);
      if (!dFile.is_open()) {
        fprintf(stderr,
                "rlic: Error: Unable to open volume %s for reading!\n",
                argv[a]
               );
        return 1;
      }
      processVolumeFrom(dFile, inputs);
      dFile.close();
    }
  } else {
    processVolumeFrom(cin, inputs);
  }

  return 0;
}
