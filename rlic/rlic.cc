#include <cassert>
#include <cstdlib>
#include <ctype.h>
#include <algorithm>
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <iostream>
using std::istream;
using std::cin;
using std::cerr;
using std::cout;
#include <array>
using std::array;
#include <vector>
using std::vector;
#include <map>
using std::map;
#include <set>
using std::set;
#include <string>
using std::string;
#include <unistd.h>

#include "diagram2d.h"
#include "edged_bool.h"
#define EBQT EdgedBool::QueryType
#include "voxel.h"

EBQT optLogStateOnLabelState = EBQT::eoQueryType;
string optLogStateOnLabel = "";
bool optLittleEndian = false;
bool optCycleCountSet = false;
bool optShowDebugOutput = false;
int optCycleCount = 0;
string optInputPath = "";
bool optEchoInput = false;
string optTableInputPath = "";
string optOutputPath = "";
bool optRodsInitialValue = false;
bool optShowEvaluatingRods = false;
bool optShowPerformance = false;
bool optShowRods = false;
bool optShowChangedStateEveryTick = false;
bool optShowStateEveryTick = false;
bool optVerbose = false;
bool optShowWaveforms = false;
bool optWarnings = false;
bool optShowExpressions = false;

void processInputFrom(istream &in, map<string, vector<int>> &inputs) {
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

    vector<int> values;
    int value;

    // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

    if (sscanf(sPtr, " %i %n", &value, &sCnt) == 1) {
      values.push_back(value);
      sPtr += sCnt;

      // fprintf(stderr, "%d: value=%d\n", lineNumber, value);
      // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

      while (sscanf(sPtr, " , %i %n", &value, &sCnt) == 1) {
        values.push_back(value);
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

void processTableInputFrom(istream &in, map<string, vector<int>> &inputs) {
  unsigned lineNumber = 0;

  inputs.clear();

  vector<string> names;
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

  size_t nInputValues = 0;
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

    vector<int> values;
    int value;

    // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

    if (sscanf(sPtr, " %i %n", &value, &sCnt) == 1) {
      values.push_back(value);
      sPtr += sCnt;

      // fprintf(stderr, "%d: value=%d\n", lineNumber, value);
      // fprintf(stderr, "%d: scanning >%s\n", lineNumber, sPtr);

      while (sscanf(sPtr, " , %i %n", &value, &sCnt) == 1) {
        values.push_back(value);
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

    // fprintf(stdout, "inputs[%lu]: {", nInputValues++);
    // char const *comma = "";
    for (size_t n = 0; n < names.size(); n += 1) {
      inputs[names[n]].push_back(n < values.size() ? values[n] : 0);
      // fprintf(stdout, "%s %s=%d", comma, names[n].c_str(), inputs[names[n]].back());
      // comma = ",";
    }
    // fprintf(stdout, "\n");
  } while (in.good() && !in.eof());
}

template<> void Plate<char>::DumpPixelAt(size_t y, size_t x) const {
  fprintf(stdout, "%c", (*this)[y][x]);
}

template<> void Plate<Voxel>::DumpPixelAt(size_t y, size_t x) const {
  VoxelProperties const &vp = voxelProperties[(*this)[y][x]];
  fprintf(stdout, "%c", vp.text);
}

void processDiagramFrom(istream &in, map<string, vector<int>> const &inputs) {
  Diagram2D diagram(in);
  diagram.scan();
  diagram.dump();
  diagram.refactor();

  int maxInputLength = optCycleCount;
  for (auto const &nameAndValues : inputs) {
    string const &name = nameAndValues.first;
    vector<int> const &values = nameAndValues.second;

    diagram.setInputFor(name, values);
    maxInputLength = std::max(maxInputLength, int(values.size()));
  }

  if (!inputs.empty()) {
    int const allowedPhasesWithoutChanges = 6;
    int nPhasesWithoutChanges = 0;
    int nPhasesWithoutInputs = 0;
    Label label(optLogStateOnLabelState != EBQT::eoQueryType ?
                optLogStateOnLabel : ""
               );

    // fprintf(stdout, "Executing up to %d cycles....\n", maxInputLength);
    // while (diagram.CurrentTick() < (maxInputLength * int(eoDirection))) {
    while (diagram.hasUnreadInput()) {
      if (0 < optCycleCount) {
        if (optCycleCount < (diagram.CurrentTick() / int(eoDirection))) {
          break;
        }
      }

      if (!diagram.rodsWereChangedDuringLastEvaluation()) {
        nPhasesWithoutChanges += 1;
        if (allowedPhasesWithoutChanges * int(eoDirection) <=
            nPhasesWithoutChanges
           )
        {
          fprintf(stdout,
                  "Bailing early (cycle %d): "
                  "the last %d cycle(s) had no rod changes....\n",
                  diagram.CurrentTick() / int(eoDirection),
        	  allowedPhasesWithoutChanges
                  );
          break;
        }
      } else {
        nPhasesWithoutChanges = 0;
      }

      diagram.evaluate();

      // diagram.inputsWereReadDuringLastEvaluation() ||
      // diagram.outputsWereWrittenDuringLastEvaluation() ||
      if ((optLogStateOnLabelState != EBQT::eoQueryType &&
           diagram.getOutputFor(label).is(optLogStateOnLabelState)
          ) ||
          optShowStateEveryTick
	 )
      {
	diagram.dumpState();
      }
    };

    if (optShowWaveforms) {
      diagram.dumpWaveforms();
    }
    if (optShowPerformance) {
      diagram.dumpPerformance();
    }
  } else {
    fprintf(stdout, "Not executing (optCycleCount == %d).\n", optCycleCount);

    MapP3dToLabel p3dToLabel[3];
    diagram.getIODPointsAndLabels(p3dToLabel);

    Plate<char> plate(' ', 1, diagram.yMax, diagram.xMax);
    diagram.RebuildWithChar(plate);
    Diagram2D newDiagram(plate);
    newDiagram.scan();
    newDiagram.dump();

    if (!optOutputPath.empty()) {
      Brick<Voxel> brick(Wall, 4, 7, newDiagram.yMax, newDiagram.xMax);
      newDiagram.RebuildWithEnum(brick);

      ofstream oFile(optOutputPath);
      if (!oFile.is_open()) {
        fprintf(stderr,
                "rlic: Error: Unable to open output %s for writing!\n",
                optOutputPath.c_str()
                );
        exit(1);
      }

      brick.WriteTo(oFile);
      oFile << "\n";
      static char const *iToIOD[3] = { "Inputs", "Outputs", "DebugOutputs" };
      for (size_t i = 0; i < 3; i += 1) {
        oFile << iToIOD[i] << "={ ";
        char const *comma = "";
        for (auto const &pl : p3dToLabel[i]) {
          oFile << comma << pl.second;
          comma = ", ";
        }
        oFile << " }\n";
      }
    }
  }
}

char const *ARGV0 = "rlic";

void Usage(bool terminate) {
  fprintf(stderr,
          "\
%s [ <option>... ] [ <diagram>... ]\n\
where <option> is:\n\
-/ <label>	Log state when <label> changes from 0 to 1 (default: no) [%s].\n\
-\\ <label>	Log state when <label> changes from 1 to 0 (default: no) [%s].\n\
-0		Set the default initial value of a rod to 0 (default: false) [%s].\n\
-1		Set the default initial value of a rod to 1 (default: true) [%s].\n\
-b		Assume Big Endian for multi-rod variables (default: on) [%s].\n\
-c <n>		Execute up to <n> cycles (default: 0; execute until input is exhausted) [%d].\n\
-d		Log debug output (in addition to input and output) variables (default: no) [%s].\n\
-e		Log rod evaluation (default: no) [%s].\n\
-H <label>	Log state when <label> == 1 (default: no) [%s].\n\
-h		Show this usage.\n\
-i <path>	Read (variable-per-row) input from <path>, without echo (default: no) [%s].\n\
-I <path>	Read (variable-per-row) input from <path>, with echo (default: no) [%s].\n\
-L <label>	Log state when <label> == 0 (default: no) [%s].\n\
-l		Assume Little Endian for multi-rod variables (default: off) [%s].\n\
-o <path>       Create, and write CAM output to <path> (default: no) [%s].\n\
-p		Log performance metrics (default: off) [%s].\n\
-r		Log rods (default: off) [%s].\n\
-s		Log whole state after each tick (default: off) [%s].\n\
-S		Log changed state after each tick (default: off) [%s].\n\
-t <path>	Read (tabular, variable-per-column) input from <path>, without echo (default: no) [%s].\n\
-T <path>	Read (tabular, variable-per-column) input from <path>, with echo (default: no) [%s].\n\
-w		Log input/output/debug output waveforms following execution (default: no) [%s].\n\
-W		Log verification warnings (default: no) [%s].\n\
",
          ARGV0,
          optLogStateOnLabelState == EBQT::aLeadingEdge ?
              optLogStateOnLabel.c_str() : "",
          optLogStateOnLabelState == EBQT::aTrailingEdge ?
              optLogStateOnLabel.c_str() : "",
          optRodsInitialValue ? "" : "-0",
          optRodsInitialValue ? "-1" : "",
          optLittleEndian ? "" : "-b",
          optCycleCount,
          optShowDebugOutput ? "-d" : "",
          optShowEvaluatingRods ? "-e" : "",
          optLogStateOnLabelState == EBQT::aHi ?
              optLogStateOnLabel.c_str() : "",
          !optInputPath.empty() && !optEchoInput ? optInputPath.c_str() : "",
          !optInputPath.empty() &&  optEchoInput ? optInputPath.c_str() : "",
          optLogStateOnLabelState == EBQT::aLo ?
              optLogStateOnLabel.c_str() : "",
          optLittleEndian ? "-l" : "",
          optOutputPath.c_str(),
          optShowPerformance ? "-p" : "",
          optShowRods ? "-s" : "",
          optShowStateEveryTick && !optShowChangedStateEveryTick ? "-s" : "",
          optShowStateEveryTick &&  optShowChangedStateEveryTick ? "-S" : "",
          !optTableInputPath.empty() && !optEchoInput ? optTableInputPath.c_str() : "",
          !optTableInputPath.empty() &&  optEchoInput ? optTableInputPath.c_str() : "",
          optShowWaveforms ? "-w" : "",
          optWarnings ? "-W" : ""
         );
  if (terminate) {
    exit(1);
  }
}

int main(int argc, char *const argv[]) {
  ARGV0 = argv[0];

  bool optShowHelp = false;
  int c;
  while ((c = getopt(argc, argv, "/:\\:01bc:deH:hi:I:L:lpo:rsSt:T:vwWx")) != -1) {
    switch (c) {
    case '/':
      optLogStateOnLabelState = EBQT::aLeadingEdge;
      optLogStateOnLabel = optarg;
      break;
    case '\\':
      optLogStateOnLabelState = EBQT::aTrailingEdge;
      optLogStateOnLabel = optarg;
      break;
    case '0':
      optRodsInitialValue = false;
      break;
    case '1':
      optRodsInitialValue = true;
      break;
    case 'b':
      optLittleEndian = false;
      break;
    case 'c':
      if (sscanf(optarg, "%i", &optCycleCount) != 1) {
        fprintf(stderr, "rlic: Error: Option -%c requires a [decimal] integer argument.\n", c);
        return 1;
      }
      optCycleCountSet = true;
      break;
    case 'd':
      optShowDebugOutput = true;
      break;
    case 'e':
      optShowEvaluatingRods = true;
      break;
    case 'H':
      optLogStateOnLabelState = EBQT::aHi;
      optLogStateOnLabel = optarg;
      break;
    case 'h':
      optShowHelp = true;
      break;
    case 'i':
      optInputPath = optarg;
      optEchoInput = false;
      break;
    case 'I':
      optInputPath = optarg;
      optEchoInput = true;
      break;
    case 'L':
      optLogStateOnLabelState = EBQT::aLo;
      optLogStateOnLabel = optarg;
      break;
    case 'l':
      optLittleEndian = true;
      break;
    case 'o':
      optOutputPath = optarg;
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
      optShowWaveforms = true;
      break;
    case 'W':
      optWarnings = true;
      break;
    case 'x':
      optShowExpressions = true;
      break;
    case '?':
      if (optopt == 'c') {
        fprintf(stderr, "rlic: Error: Option -%c requires an argument.\n", optopt);
      } else if (isprint(optopt)) {
        fprintf(stderr, "rlic: Error: Unknown option `-%c'.\n", optopt);
      } else {
        fprintf(stderr, "rlic: Error: Unknown option character `\\x%x'.\n", optopt);
      }
      return 1;
    default:
      abort();
    }
  }

  if (optShowHelp) {
    Usage(false);
  }

  fprintf(stdout, "%s", argv[0]);
  fprintf(stdout, " -%s", optRodsInitialValue ? "1" : "0");
  fprintf(stdout, " -%s", optLittleEndian ? "l" : "b");
  fprintf(stdout, " -c %d", optCycleCount);
  switch (optLogStateOnLabelState) {
  case EBQT::eoQueryType:
    break;
  case EBQT::aLeadingEdge:
    fprintf(stdout, " -/ %s", optLogStateOnLabel.c_str());
    break;
  case EBQT::aHi:
    fprintf(stdout, " -H %s", optLogStateOnLabel.c_str());
    break;
  case EBQT::aTrailingEdge:
    fprintf(stdout, " -\\ %s", optLogStateOnLabel.c_str());
    break;
  case EBQT::aLo:
    fprintf(stdout, " -L %s", optLogStateOnLabel.c_str());
    break;
  }
  if (optShowDebugOutput) {
    fprintf(stdout, " -d");
  }
  if (optShowEvaluatingRods) {
    fprintf(stdout, " -e");
  }
  if (!optInputPath.empty()) {
    fprintf(stdout, " -%s %s",
            optEchoInput ? "I" : "i",
            optInputPath.c_str()
           );
  }
  if (optShowPerformance) {
    fprintf(stdout, " -p");
  }
  if (optShowRods) {
    fprintf(stdout, " -r");
  }
  if (optShowStateEveryTick) {
    fprintf(stdout, " -%s", optShowChangedStateEveryTick ? "S" : "s");
  }
  if (!optTableInputPath.empty()) {
    fprintf(stdout, " -%s %s",
            optEchoInput ? "T" : "t",
            optTableInputPath.c_str()
           );
  }
  if (optWarnings) {
    fprintf(stdout, " -w");
  }
  if (optind < argc) {
    for (int a = optind; a < argc; a += 1) {
      fprintf(stdout, " %s", argv[a]);
    }
  } else {
    fprintf(stdout, " <stdin>");
  }
  fprintf(stdout, "\n");

  map<string, vector<int>> inputs;

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
                "rlic: Error: Unable to open diagram %s for reading!\n",
                argv[a]
               );
        return 1;
      }
      processDiagramFrom(dFile, inputs);
      dFile.close();
    }
  } else {
    processDiagramFrom(cin, inputs);
  }

  return 0;
}
