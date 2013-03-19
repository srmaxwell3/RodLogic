#ifndef EDGED_BOOL_H
#define EDGED_BOOL_H

class EdgedBool {
 public:

  enum QueryType {
    aLeadingEdge,
    aHi,
    aTrailingEdge,
    aLo,

    eoQueryType
  };

  EdgedBool(bool initalValue = false, int atTick = -1) {
    reset(initalValue, atTick);
  }

  operator bool() const { return history[0].value; }
  // EdgedBool &operator=(bool newValue) {
  //   history[1] = history[0];
  //   history[0] = { history[1].atTick + 1, newValue };
  //   return *this;
  // }

  void reset(bool initalValue = false, int atTick = -1) {
    history[0] = { atTick, initalValue };
    history[1] = { atTick, initalValue };
  }

  EdgedBool &updateAt(int atTick, bool newValue) {
    history[1] = history[0];
    history[0] = { atTick, newValue };
    return *this;
  }

  EdgedBool &getAt(int atTick) {
    if (history[0].atTick != atTick) {
      history[1] = history[0];
      history[0] = { atTick, history[1].value };
    }
    return *this;
  }

  int dTicks() const {
    return history[0].atTick - history[1].atTick;
  }

  bool is(QueryType t) const {
    switch (t) {
    case aLeadingEdge:
      return isALeadingEdge();
    case aHi:
      return isAHi();
    case aTrailingEdge:
      return isATrailingEdge();
    case aLo:
      return isALo();
    }
    return false;
  }

  bool isAHi() const {
    return history[0].value;
  }

  bool isALo() const {
    return !history[0].value;
  }

  bool isALeadingEdge() const {
    return history[0].value && isAnEdge();
  }
  bool isATrailingEdge() const {
    return !history[0].value && isAnEdge();
  }
  bool isAnEdge() const {
    return history[0].value != history[1].value;
  }
  bool hasChanged() const {
    return isAnEdge();
  }
  bool isSteady() const {
    return !hasChanged();
  }

 private:
  struct {
    int atTick;
    bool value;
  } history[2];
};

#endif // EDGED_BOOL_H
