#include <cassert>
#include <cstdlib>
#include <ctype.h>
#include <algorithm>
#include <fstream>
using std::ifstream;
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

bool optLittleEndian = false;
bool optCycleCountSet = false;
int optCycleCount = 0;
string optInputPath = "";
bool optEchoInput = false;
string optTableInputPath = "";
bool optRodsInitialValue = false;
bool optShowEvaluatingRods = false;
bool optShowPerfrmance = false;
bool optShowRods = false;
bool optShowChangedStateEveryTick = false;
bool optShowStateEveryTick = false;
bool optWarnings = false;

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
              "cam.r: Error: %d, %ld: Input syntax error; "
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
              "cam.r: Error: %d, %ld: Input syntax error; "
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
              "cam.r: Error: %d, %ld: Input syntax error; "
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
                "cam.r: Error: %d, %ld: Input syntax error; "
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
              "cam.r: Error: %d, %ld: Input syntax error; "
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
              "cam.r: Error: %d, %ld: Input syntax error; "
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
              "cam.r: Error: %d, %ld: Input syntax error; "
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
              "cam.r: Error: %d, %ld: Input syntax error; "
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
              "cam.r: Error: %d, %ld: Input syntax error; "
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
              "cam.r: Error: %d, %ld: Input syntax error; "
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
                "cam.r: Error: %d, %ld: Input syntax error; "
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
              "cam.r: Error: %d, %ld: Input syntax error; "
              "expected '^{ \\w+, { >\\d+<' (integer->bool value) at '%s'!\n",
              lineNumber,
              ((sPtr + sCnt) - sStart),
	      sPtr
             );
      continue;
    }

    if (names.size() < values.size()) {
      fprintf(stderr,
              "cam.r: Warning: "
              "Table row %d has too many values (has %lu, expected %lu); "
              "ignoring extra.\n",
              lineNumber,
              values.size(),
              names.size()
             );
    } else if (values.size() < names.size()) {
      fprintf(stderr,
              "cam.r: Warning: "
              "Table row %d has too few values (has %lu, expected %lu); "
              "filling with 0's.\n",
              lineNumber,
              values.size(),
              names.size()
             );
    }

    for (size_t n = 0; n < names.size(); n += 1) {
      inputs[names[n]].push_back(n < values.size() ? values[n] : 0);
    }
  } while (in.good() && !in.eof());
}

void processDiagramFrom(istream &in, map<string, vector<int>> const &inputs) {
  Diagram2D diagram(in);
  diagram.scan();
  diagram.dump();

  // for (unsigned s = 0; s < 16; s += 1) {
  //   for (unsigned m = 0; m < 2; m += 1) {
  //     for (unsigned c = 0; c < 2; c += 1) {
  //       for (unsigned b = 0; b < 16; b += 1) {
  //         for (unsigned a = 0; a < 16; a += 1) {
  //           diagram.addInputFor("A0", a & 1);
  //           diagram.addInputFor("A1", a & 2);
  //           diagram.addInputFor("A2", a & 4);
  //           diagram.addInputFor("A3", a & 8);
  //           diagram.addInputFor("B0", b & 1);
  //           diagram.addInputFor("B1", b & 2);
  //           diagram.addInputFor("B2", b & 4);
  //           diagram.addInputFor("B3", b & 8);
  //           diagram.addInputFor("C0", c & 1);
  //           diagram.addInputFor("M",  m & 1);
  //           diagram.addInputFor("S0", s & 1);
  //           diagram.addInputFor("S1", s & 2);
  //           diagram.addInputFor("S2", s & 4);
  //           diagram.addInputFor("S3", s & 8);
  //         }
  //       }
  //     }
  //   }
  // }
  //
  // for (bool oldReady = diagram.getOutputFor("Ready"), newReady = true;
  //      /* diagram.CurrentTick() < 100000 && */ diagram.hasInputFor("A0");
  //      oldReady = newReady, newReady = diagram.getOutputFor("Ready")
  //     )
  // {
  //   if (!oldReady && newReady) {
  //     diagram.dumpState();
  //     diagram.addInputFor("Start", true);
  //   }
  //   diagram.evaluate();
  // }
  // diagram.setInputFor("MAdr.XX.0", { 0, 1, 0, 1, 0, 1 });
  // diagram.setInputFor("MAdr.XX.1", { 0, 0, 0, 0, 0, 0 });
  // diagram.setInputFor("MAdr.XX.2", { 0, 0, 0, 0, 0, 0 });
  // diagram.setInputFor("MAdr.XX.3", { 0, 0, 0, 0, 0, 0 });
  // diagram.setInputFor("MAdr.XX.4", { 0, 0, 0, 0, 0, 0 });
  // diagram.setInputFor("MAdr.XX.5", { 0, 0, 0, 0, 0, 0 });
  // diagram.setInputFor("MAdr.XX.6", { 0, 0, 0, 0, 0, 0 });
  // diagram.setInputFor("MAdr.XX.7", { 0, 0, 0, 0, 0, 0 });
  // diagram.setInputFor("MDWr.bb.0", { 0, 0, 1, 1, 0, 0 });
  // diagram.setInputFor("MDWr.bb.1", { 0, 0, 1, 0, 0, 0 });
  // diagram.setInputFor("MDWr.bb.2", { 0, 0, 1, 1, 0, 0 });
  // diagram.setInputFor("MDWr.bb.3", { 0, 0, 1, 1, 0, 0 });
  // diagram.setInputFor("MDWr.bb.4", { 0, 0, 1, 0, 0, 0 });
  // diagram.setInputFor("MDWr.bb.5", { 0, 0, 1, 1, 0, 0 });
  // diagram.setInputFor("MDWr.bb.6", { 0, 0, 1, 0, 0, 0 });
  // diagram.setInputFor("MDWr.bb.7", { 0, 0, 1, 0, 0, 0 });
  // diagram.setInputFor("Read",      { 1, 1, 0, 0, 1, 1 });
  // diagram.setInputFor("Write",     { 0, 0, 1, 1, 0, 0 });
  // while (diagram.CurrentTick() < 100 && diagram.hasInputFor("Read")) {
  //   diagram.evaluate();
  // }
  int maxInputLength = optCycleCount;
  for (auto const &nameAndValues : inputs) {
    string const &name = nameAndValues.first;
    vector<int> const &values = nameAndValues.second;

    diagram.setInputFor(name, values);
    maxInputLength = std::max(maxInputLength, int(values.size()));
  }

  if (maxInputLength) {
    int nPhasesWithoutChanges = 0;

    fprintf(stdout, "Executing up to %d cycles....\n", maxInputLength);
    while (diagram.CurrentTick() < (maxInputLength * int(eoDirections))) {
      if (!diagram.rodsWereChangedDuringLastEvaluation()) {
        nPhasesWithoutChanges += 1;
        if (int(eoDirections) <= nPhasesWithoutChanges) {
          fprintf(stdout,
                  "Bailing early (cycle %d): the last cycle had no rod changes....\n",
                  diagram.CurrentTick()
                  );
          break;
        } else {
          nPhasesWithoutChanges = 0;
        }
      }

      diagram.evaluate();

      if (optShowStateEveryTick ||
	  diagram.inputsWereReadDuringLastEvaluation() ||
	  diagram.outputsWereWrittenDuringLastEvaluation()
	 )
      {
	diagram.dumpState();
      }
    }

    if (optShowPerfrmance) {
      diagram.dumpPerformance();
    }
  } else {
    fprintf(stdout, "Not executing (optCycleCount == %d).\n", optCycleCount);
  }
}

int main(int argc, char *const argv[]) {
  int c;
  while ((c = getopt(argc, argv, "01bc:ei:I:lprsSt:T:w")) != -1) {
    switch (c) {
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
          fprintf(stderr, "cam.r: Error: Option -%c requires a [decimal] integer argument.\n", c);
          return 1;
        }
        optCycleCountSet = true;
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
	optShowPerfrmance = true;
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

  fprintf(stdout, "%s -c %d", argv[0], optCycleCount);
  if (optShowEvaluatingRods) {
    fprintf(stdout, " -e");
  }
  if (!optRodsInitialValue) {
    fprintf(stdout, " -F");
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
  if (optRodsInitialValue) {
    fprintf(stdout, " -T");
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
              "cam.r: Error: Unable to open input %s for reading!\n",
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
              "cam.r: Error: Unable to open [table] input %s for reading!\n",
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
                "cam.r: Error: Unable to open diagram %s for reading!\n",
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
