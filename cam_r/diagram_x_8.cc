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

bool optCycleCountSet = false;
int optCycleCount = 0;
string optInputPath = "";
bool optShowEvaluatingRods = false;
bool optShowRods = false;
bool optShowStateEveryTick = false;
bool optWarnings = false;

struct Diagram2D: public array<vector<string>, 8> {
  Diagram2D(istream &in);
  void Scan();
  void Dump() const;

  int yMax;
  int xMax;
};

Diagram2D::Diagram2D(istream &in) :
    array<vector<string>, 8>(),
    xMax(0),
    yMax(0)
{
  do {
    string line;
    getline(in, line);
    if (in.good()) {
      (*this)[0].push_back(line);
      if (xMax < line.size()) {
        xMax = line.size();
      }
    }
  } while (in.good() && !in.eof());

  for (auto &line : (*this)[0]) {
    line.resize(xMax, ' ');
  }

  yMax = size();
}

void Diagram2D::Scan() {
  map<char, array<char, 4>> const rotators = {
    { '|', { '|', '-', '|', '-' } },
    { '-', { '-', '|', '-', '|' } },
    { '>', { '>', 'v', '<', '^' } },
    { 'v', { 'v', '<', '^', '>' } },
    { '<', { '<', '^', '>', 'v' } },
    { '^', { '^', '>', 'v', '<' } }
  };

  // Rotate right 90 degrees, 3 times.
  for (size_t x = 0; x < xMax; x += 1) {
    string line(yMax, ' ');
    for (size_t y = yMax; 0 < y; y -= 1) {
      char c = (*this)[0][y];
      if (rotators.find(c) != rotators.end()) {
        line[yMax - y] = rotator[c][1];
      } else {
        line[yMax - y] = c;
      }
    }
    (*this)[1].push_back(line);
  }
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
  for (auto const &nameAndValues : inputs) {
    string const &name = nameAndValues.first;
    vector<int> const &values = nameAndValues.second;

    diagram.setInputFor(name, values);
  }

  if (optCycleCount) {
    int nPhasesWithoutChanges = 0;

    fprintf(stdout, "Executing up to %d cycles....\n", optCycleCount);
    while (diagram.CurrentTick() < (optCycleCount * int(eoDirections))) {
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
  } else {
    fprintf(stdout, "Not executing (optCycleCount == %d).\n", optCycleCount);
  }
}

int main(int argc, char *const argv[]) {
  int c;
  while ((c = getopt(argc, argv, "c:ei:rsw")) != -1) {
    switch (c) {
      case 'c':
        if (sscanf(optarg, "%i", &optCycleCount) != 1) {
          fprintf(stderr, "Option -%c requires a [decimal] integer argument.\n", c);
          return 1;
        }
        optCycleCountSet = true;
        break;
      case 'e':
        optShowEvaluatingRods = true;
        break;
      case 'i':
        optInputPath = optarg;
        break;
      case 'r':
        optShowRods = true;
        break;
      case 's':
        optShowStateEveryTick = true;
        break;
      case 'w':
        optWarnings = true;
        break;
      case '?':
        if (optopt == 'c') {
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
        } else if (isprint(optopt)) {
          fprintf(stderr, "Unknown option `-%c'.\n", optopt);
        } else {
          fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
        }
        return 1;
      default:
        abort();
    }
  }

  if (optind < argc) {
    for (int a = optind; a < argc; a += 1) {
      ifstream dFile(argv[a]);
      if (!dFile.is_open()) {
        fprintf(stderr, "Unable to open diagram %s for reading!\n", argv[a]);
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
