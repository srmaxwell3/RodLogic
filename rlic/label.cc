#include "label.h"

#include <climits>
#include <cstdio>
#include <iostream>
#include <regex>
using std::regex;
using std::smatch;
using std::regex_match;

extern bool optLittleEndian;

CombinedLabel::CombinedLabel() :
    string(),
    minBitNumber(INT_MAX),
    maxBitNumber(INT_MIN),
    isBigEndian(!optLittleEndian)
{
}

void CombinedLabel::Update(Label const &label) {
  string::operator=(label.Name());

  if (label.BitNumber() < minBitNumber) {
    minBitNumber = label.BitNumber();
  }
  if (maxBitNumber < label.BitNumber()) {
    maxBitNumber = label.BitNumber();
  }
}

void CombinedLabel::SetEndianness(string const &name, bool isBigEndian) {
  if (allCombinedLabels.find(name) != allCombinedLabels.end()) {
    allCombinedLabels[name].isBigEndian = isBigEndian;
  }
}

string CombinedLabel::ToString() const {
  static char buffer[128];
  if (IsLittleEndian()) {
    sprintf(buffer, "%s.%x..%x<", c_str(), maxBitNumber, minBitNumber);
  } else {
    sprintf(buffer, "%s.>%x..%x", c_str(), minBitNumber, maxBitNumber);
  }
  return buffer;
}

void CombinedLabel::Dump() const {
  if (0 <= minBitNumber) {
    fprintf(stdout,
	    "(CombinedLabel *)(%p)->{ string()=\"%s\", minBitNumber=%d, maxBitNumber=%d, %sisBigEndian }\n",
	    this,
	    c_str(),
	    minBitNumber,
	    maxBitNumber,
	    isBigEndian ? "" : "!"
	    );
  } else {
    fprintf(stdout,
	    "(CombinedLabel *)(%p)->{ string()=\"%s\", %sisBigEndian }\n",
	    this,
	    c_str(),
	    isBigEndian ? "" : "!"
	    );
  }
}

CombinedLabel const *CombinedLabel::LookupCombinedLabelFor(string const &label) {
  if (allCombinedLabels.find(label) != allCombinedLabels.end()) {
    // fprintf(stdout, "CombinedLabel::LookupCombinedLabelFor(\"%s\") -> ", label.c_str());
    // allCombinedLabels[label].Dump();
    return &allCombinedLabels[label];
  }
  // fprintf(stdout, "CombinedLabel::LookupCombinedLabelFor(\"%s\") -> nullptr\n", label.c_str());
  return nullptr;
}

CombinedLabel const *CombinedLabel::LookupCombinedLabelFor(Label const &label) {
  if (allCombinedLabels.find(label.Name()) != allCombinedLabels.end()) {
    // fprintf(stdout, "CombinedLabel::LookupCombinedLabelFor(Label(\"%s\").Name()=\"%s\") -> ", label.ToString().c_str(), label.Name().c_str());
    // allCombinedLabels[label.Name()].Dump();
    return &allCombinedLabels[label.Name()];
  }
  // fprintf(stdout, "CombinedLabel::LookupCombinedLabelFor(Label(\"%s\").Name()=\"%s\") -> nullptr\n", label.ToString().c_str(), label.Name().c_str());
  return nullptr;
}

CombinedLabel const *CombinedLabel::UpdateCombinedLabelFor(Label const &label) {
  CombinedLabel *cLabel = &allCombinedLabels[label.Name()];
  cLabel->Update(label);
  
  // fprintf(stdout, "CombinedLabel::UpdateCombinedLabelFor(Label(\"%s\").Name()=\"%s\") -> ", label.ToString().c_str(), label.Name().c_str());
  // cLabel->Dump();
  return cLabel;
}

map<string, CombinedLabel> CombinedLabel::allCombinedLabels;

// Label::Label(string const &_name, int _bitNumber) :
//   string(_name),
//   isDefined(true),
//   bitNumber(_bitNumber)
// {
// }

Label::Label(string const &_name, int _bitNumber) :
  string(_name),
  isDefined(false),
  bitNumber(_bitNumber)
{
  if (0 <= bitNumber) {
    static char buffer[128];
    sprintf(buffer, "%s.%x", c_str(), bitNumber);
    SetFrom(buffer);
  } else {
    SetFrom(*this);
  }
}

Label::Label(string const &label) :
  string(),
  isDefined(false),
  bitNumber(-1)
{
  SetFrom(label);
}

Label::Label() :
  string(),
  isDefined(false),
  bitNumber(-1)
{
}

Label &Label::operator=(string const &label) {
  SetFrom(label);
  return *this;
}

string Label::ToString() const {
  if (!isDefined) {
    return "<undefined>";
  }
  if (0 <= bitNumber) {
    static char buffer[128];
    sprintf(buffer, "%s.%x", c_str(), bitNumber);
    return buffer;
  }
  return *this;
}

void Label::Dump() const {
  if (0 <= bitNumber) {
    fprintf(stdout,
	    "(Label *)(%p)->{ string()=\"%s\", %sisDefined, bitNumber=%d, ToString()=\"%s\" }\n",
	    this,
	    c_str(),
	    isDefined ? "" : "!",
	    bitNumber,
	    ToString().c_str()
	    );
  } else {
    fprintf(stdout,
	    "(Label *)(%p)->{ string()=\"%s\", %sisDefined, ToString()=\"%s\" }\n",
	    this,
	    c_str(),
	    isDefined ? "" : "!",
	    ToString().c_str()
	    );
  }
}

void Label::DumpWithInputs(BitStreamInput const &inputs) const {
  if (!inputs.empty()) {
    fprintf(stdout, "%s = { %d", ToString().c_str(), inputs[0]);
    for (size_t i = 1; i < inputs.size(); i += 1) {
      fprintf(stdout, ",%d", inputs[i]);
    }
    fprintf(stdout, "\n");
  } else {
    fprintf(stdout, "%s = { }\n", ToString().c_str());
  }
}

void Label::SetFrom(string const &label) {
  string::operator=(label);

  // try {
  //   static regex labelWithBitNumberPattern("^(.+)[.]([0-9a-fA-F]+)$", std::regex::extended);
  //   smatch labelParts;
  //   if (regex_match(label, labelParts, labelWithBitNumberPattern)) {
  //     string::operator=(labelParts[1].str());
  //     sscanf(labelParts[2].str().c_str(), "%x", &bitNumber);
  //   }
  // } catch (const std::regex_error &e) {
  //   fprintf(stderr,
  //           "(Label *)(%p)->SetFrom(label=\"%s\"): Caught std::regex_error e; e.what()=\"%s\"\n",
  //           this,
  //           label.c_str(),
  //           e.what()
  //          );
  //   fflush(stderr);
  //   exit(1);
  // }

  size_t dotAt = label.find_last_of('.');
  if (dotAt != string::npos) {
    size_t eoHex = label.find_first_not_of("0123456789abcdefABCDEF", dotAt + 1);
    if (eoHex == string::npos) {
      string name = label.substr(0, dotAt);
      string nmbr = label.substr(dotAt + 1);
      string::operator=(name);
      sscanf(nmbr.c_str(), "%x", &bitNumber);

      CombinedLabel::UpdateCombinedLabelFor(*this);
    }
  }

  isDefined = true;
  // Dump();
}
