#ifndef DIAGRAM2D_H
#define DIAGRAM2D_H

#include <array>
using std::array;
#include <fstream>
using std::ofstream;
#include <iostream>
using std::istream;
#include <map>
using std::map;
#include <string>
using std::string;
#include <utility>
using std::pair;
#include <vector>
using std::vector;

#include "bit_stream_input.h"
#include "direction.h"
#include "label.h"
#include "p2d.h"
#include "p3d.h"
#include "voxel.h"

struct Rod2D;
typedef set<Rod2D *> SetOfRod2Ds;
typedef map<P3D, string> MapP3dToLabel;

template<typename T> struct Plate : public vector<vector<T>> {
  typedef T PixelType;

  Plate() : vector<vector<PixelType>>(), defaultPixel(), scaleBy(), yMax(), xMax() { }

  Plate(PixelType _defaultPixel, size_t _scaleBy, size_t _yMax, size_t _xMax) :
    vector<vector<PixelType>>(),
    defaultPixel(_defaultPixel),
    scaleBy(_scaleBy),
    yMax((_yMax + 2) * scaleBy),
    xMax((_xMax + 2) * scaleBy),
    nDigits(1)
  {
    for (size_t w = 10; w <= xMax; w *= 10) {
      nDigits += 1;
    }

    vector<vector<T>>::resize(yMax);
    for (size_t y = 0; y < yMax; y += 1) {
      (*this)[y].vector<PixelType>::resize(xMax, defaultPixel);
    }
  }

  Plate(Plate const &that) :
    vector<vector<PixelType>>(that.yMax),
    defaultPixel(that.defaultPixel),
    scaleBy(that.scaleBy),
    yMax(that.yMax),
    xMax(that.xMax)
  {
    for (size_t w = 10; w <= xMax; w *= 10) {
      scaleBy += 1;
    }
    for (size_t y = 0; y < yMax; y += 1) {
      (*this)[y].resize(xMax, defaultPixel);
    }
  }

  PixelType const &at(P2D const &p) const {
    return at(p.y, p.x);
  }
  PixelType &at(P2D const &p) {
    return at(p.y, p.x);
  }

  PixelType const &at(int y, int x) const {
    return vector<vector<PixelType>>::at(y).at(x);
  }
  PixelType &at(int y, int x) {
    return vector<vector<PixelType>>::at(y).at(x);
  }

  void insertRow(size_t atY);
  void insertCol(size_t atX);
  void deleteRow(size_t atY);
  void deleteCol(size_t atX);
  bool compareRowAndRowBelow(size_t atY) const;
  bool compareColAndColToRight(size_t atX) const;
  bool isRowEmpty(size_t atY) const;
  bool isColEmpty(size_t atX) const;
  bool isRowSqueezable(size_t atY) const;
  bool isColSqueezable(size_t atX) const;

  void DumpPixelAt(size_t y, size_t x) const;
  void Dump() const;
  ofstream &WriteTo(ofstream &o) const {
    char const *oComma = "";
    o << "  {\n";
    for (auto const &row : *this) {
      o << oComma;
      o << "    { ";
      char const *iComma = "";
      for (auto const &v : row) {
        o << iComma;
        o << c_str(v);
        iComma = ",";
      }
      o << " }";
      oComma = ",\n";
    }
    o << "\n  }";
    return o;
  }

  PixelType defaultPixel;
  size_t scaleBy;
  size_t yMax;
  size_t xMax;
  int nDigits;
};

template<typename T> struct Brick : public vector<Plate<T>> {
  typedef T VoxelType;

  Brick() : vector<Plate<VoxelType>>(), defaultVoxel(), scaleBy(), zMax(), yMax(), xMax(), nDigits() { }

  Brick(VoxelType _defaultVoxel, size_t _scaleBy, size_t _zMax, size_t _yMax, size_t _xMax) :
    vector<Plate<VoxelType>>(),
    defaultVoxel(_defaultVoxel),
    scaleBy(_scaleBy),
    zMax(_zMax),
    yMax((_yMax + 2) * scaleBy),
    xMax((_xMax + 2) * scaleBy),
    nDigits(1)
  {
    for (size_t w = 10; w <= std::max(yMax, xMax); w *= 10) {
      nDigits += 1;
    }

    vector<Plate<T>>::resize(zMax);
    for (size_t z = 0; z < zMax; z += 1) {
      (*this)[z].scaleBy = scaleBy;
      (*this)[z].yMax = yMax;
      (*this)[z].xMax = xMax;
      (*this)[z].nDigits = nDigits;
      (*this)[z].resize(yMax);
      for (size_t y = 0; y < yMax; y += 1) {
	(*this)[z][y].vector<VoxelType>::resize(xMax, defaultVoxel);
      }
    }
  }

  Brick(Brick const &that) :
    vector<Plate<VoxelType>>(that.zMax),
    defaultVoxel(that.defaultVoxel),
    scaleBy(that.scaleBy),
    zMax(that.zMax),
    yMax(that.yMax),
    xMax(that.xMax),
    nDigits(that.nDigits)
  {
    for (size_t z = 0; z < zMax; z += 1) {
      (*this)[z].scaleBy = scaleBy;
      (*this)[z].yMax = yMax;
      (*this)[z].xMax = xMax;
      (*this)[z].nDigits = nDigits;
      (*this)[z].resize(yMax);
      for (size_t y = 0; y < yMax; y += 1) {
	(*this)[z][y].resize(xMax);
	for (size_t x = 0; x < xMax; x += 1) {
	  (*this)[z][y][z] = that[z][y][x];
	}
      }
    }
  }

  bool isInBounds(P3D const &p) const {
    return 0 <= p.x && p.x < xMax && 0 <= p.y && p.y < yMax && 0 <= p.z && p.z < zMax;
  }

  VoxelType const &at(P3D const &p) const {
    return at(p.z, p.y, p.x);
  }
  VoxelType const &at(int z, int y, int x) const {
    return vector<Plate<VoxelType>>::at(z).at(y, x);
  }

  void Dump() const;
  ofstream &WriteTo(ofstream &o) const {
    char const *comma = "";
    o << "{\n";
    for (auto const &plate : *this) {
      o << comma;
      plate.WriteTo(o);
      comma = ",\n";
    }
    o << "\n}\n";
    return o;
  }

  VoxelType const defaultVoxel;
  size_t scaleBy;
  size_t zMax;
  size_t yMax;
  size_t xMax;
  int nDigits;
};

struct Diagram2D : public Plate<char> {
  Diagram2D(Plate<char> const &p);
  Diagram2D(istream &in);

  bool isInBounds(P2D const &p) const {
    return 0 <= p.x && p.x < xMax && 0 <= p.y && p.y < yMax;
  }

  bool isLegalEW(P2D const &p) const {
    return isInBounds(p) && isLegalEWChar(at(p));
  }

  bool isLegalSN(P2D const &p) const {
    return isInBounds(p) && isLegalSNChar(at(p));
  }

  bool hasAlreadyBeenSeen(P2D const &p) {
    return pointsAlreadySeen.find(p) != pointsAlreadySeen.end();
  }

  void saw(P2D const &p) {
    pointsAlreadySeen.insert(p);
  }

  char const &at(P2D const &p) const {
    return at(p.y, p.x);
  }
  char &at(P2D const &p) {
    return at(p.y, p.x);
  }

  char const &at(int y, int x) const {
    return Plate::at(y, x);
  }
  char &at(int y, int x) {
    return Plate::at(y, x);
  }

  void newRodAt(P2D const &p, Direction d);
  void newIncompleteEWRodAt(P2D const &p);
  void newIncompleteSNRodAt(P2D const &p);

  void rodSharedAt(Rod2D *r, P2D const &p);

  void scan();

  void setInputFor(Label const &label, vector<int> const &values);
  void setInputFor(string const &label, vector<int> const &values);
  void addInputFor(Label const &label, bool value);
  EdgedBool const &getInputFor(Label const &label);
  EdgedBool const &getInputFor(string const &name, int bitNumber);

  void reset();
  void evaluateAt(int tick);
  void evaluate();
  bool rodsWereChangedDuringLastEvaluation() const {
    return 0 < lastEvaluatedTickNChangedRods;
  }
  bool inputsWereReadDuringLastEvaluation() const {
    return 0 < lastEvaluatedTickNInputsRead;
  }
  bool unreadInputsWereReadDuringLastEvaluation() const {
    return 0 < lastEvaluatedTickNUnreadInputsRead;
  }
  bool outputsWereWrittenDuringLastEvaluation() const {
    return 0 < lastEvaluatedTickNOutputsWritten;
  }
  bool debugOutputsWereWrittenDuringLastEvaluation() const {
    return 0 < lastEvaluatedTickNDebugOutputsWritten;
  }
  size_t maxUnreadInput() const;
  bool hasUnreadInput() const;
  bool hasInputFor(Label const &label);
  EdgedBool const &readInputFor(Label const &label);

  void writeOutputFor(Label const &label, bool value);
  EdgedBool const &getOutputFor(Label const &label);
  EdgedBool const &getOutputFor(string const &name, int bitNumber);

  void writeDebugOutputFor(Label const &label, bool value);
  EdgedBool const &getDebugOutputFor(Label const &label);
  EdgedBool const &getDebugOutputFor(string const &name, int bitNumber);

  int CurrentTick() const { return lastEvaluatedTick; }

  void refactor();

  bool dumpInputLabelState
    (Label const &label,
     char const *comma,
     CombinedLabel const *&lastCLabel
    );
  bool dumpOutputLabelState
    (Label const &label,
     char const *comma,
     CombinedLabel const *&lastCLabel
    );
  bool dumpDebugOutputLabelState
    (Label const &label,
     char const *comma,
     CombinedLabel const *&lastCLabel
    );
  bool dumpLabelState
    (int isIOD,
     Label const &label,
     char const *comma,
     CombinedLabel const *&lastCLabel
    );
  void dumpState();
  void dumpPerformance() const;
  void dumpWaveforms() const;
  void dump() const;

  void RebuildWithChar(Plate<char> &plane) const;
  void RebuildWithChar(Brick<char> &plane) const;
  void RebuildWithEnum(Brick<Voxel> &plane) const;

  void getIODPointsAndLabels(MapP3dToLabel (&p3dToLabel)[3]) const;

  static bool isLegalEWCharNotLabel1st(char c);
  static bool isLegalEWChar(char c);
  static bool isLegalSNCharNotLabel1st(char c);
  static bool isLegalSNChar(char c);

  set<P2D> pointsAlreadySeen;
  map<P2D, SetOfRod2Ds> pointsShared;
  array<SetOfRod2Ds, eoDirection> rods;

  array<SetOfRod2Ds, eoDirection> rodsWithInputs;
  Direction earliestInput;
  map<Label, BitStreamInput> currentInputs;
  map<string, CombinedLabel> currentCombinedInputs;

  array<SetOfRod2Ds, eoDirection> rodsWithOutputs;
  Direction earliestOutput;
  map<Label, EdgedBool> currentOutputs;
  map<string, CombinedLabel> currentCombinedOutputs;

  array<SetOfRod2Ds, eoDirection> rodsWithDebugOutputs;
  Direction earliestDebugOutput;
  map<Label, EdgedBool> currentDebugOutputs;
  map<string, CombinedLabel> currentCombinedDebugOutputs;

  map<Label, vector<bool>> inputsByTick;
  map<Label, vector<bool>> outputsByTick;
  map<Label, vector<bool>> debugOutputsByTick;

  array<long, eoDirection> totalEvaluatedUSecsPerDirection;
  long totalEvaluatedUSecs;
  long totalEvaluatedTicks;

  int lastEvaluatedTick;
  int lastEvaluatedTickNChangedRods;
  int lastEvaluatedTickNInputsRead;
  int lastEvaluatedTickNUnreadInputsRead;
  int lastEvaluatedTickNOutputsWritten;
  int lastEvaluatedTickNDebugOutputsWritten;
};

#endif // DIAGRAM2D
