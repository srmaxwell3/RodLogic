#ifndef LABEL_H
#define LABEL_H

#include "bit_stream_input.h"

#include <functional>
#include <string>
using std::string;
#include <map>
using std::map;

class Label;

class CombinedLabel : string {
 public:
  CombinedLabel(Label const &label);
  CombinedLabel();
  void Update(Label const &label);

  string const &Name() const { return *this; }
  int Span() const { return 1 + (maxBitNumber - minBitNumber); }
  bool isSingleBit() const { return Span() == 1; }
  bool isMultipleBits() const { return 1 < Span(); }
  int MinBitNumber() const { return minBitNumber; }
  int MaxBitNumber() const { return maxBitNumber; }
  int IsBigEndian() const { return isBigEndian; }
  int IsLittleEndian() const { return !isBigEndian; }
  string ToString() const;
  void Dump() const;

  static void SetEndianness(string const &name, bool isBigEndian);
  static CombinedLabel const *LookupCombinedLabelFor(string const &label);
  static CombinedLabel const *LookupCombinedLabelFor(Label const &label);
  static CombinedLabel const *UpdateCombinedLabelFor(Label const &label);

 private:
  int minBitNumber;
  int maxBitNumber;
  bool isBigEndian;

  static map<string, CombinedLabel> allCombinedLabels;
};

class Label: string {
 public:
  Label(string const &_name, int _bitNumber);
  Label(string const &label);
  Label();
  Label &operator=(string const &label);
  bool operator<(Label const &that) const {
    return compare(that) < 0 ||
      (compare(that) == 0 && bitNumber < that.bitNumber);
  }
  bool IsDefined() const { return isDefined; }
  string ToString() const;
  string const &Name() const { return *this; }
  int BitNumber() const { return bitNumber; }
  void Dump() const;
  void DumpWithInputs(BitStreamInput const &inputs) const;
 private:
  void SetFrom(string const &label);
  bool isDefined;
  int bitNumber;
};

#endif // LABEL_H
