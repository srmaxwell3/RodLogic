#ifndef EDGED_BOOL_H
#define EDGED_BOOL_H

class EdgedBool {
 public:
  EdgedBool(bool initalValue = false) {
    values[0] = initalValue;
    values[1] = initalValue;
  }
  operator bool() const { return values[0]; }
  EdgedBool &operator=(bool newValue) {
    values[1] = values[0];
    values[0] = newValue;
    return *this;
  }
  bool isALeadingEdge() const { return values[0] && !values[1]; }
  bool isATrailingEdge() const { return !values[0] && values[1]; }
  bool isAnEdge() const { return values[0] != values[1]; }
  bool hasChanged() const { return isAnEdge(); }
  bool isSteady() const { return !hasChanged(); }

 private:
  bool values[2];
};

#endif // EDGED_BOOL_H
