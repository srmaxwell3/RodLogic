#include "direction.h"

char const *c_str(Direction d) {
  switch (d) {
    case E: return "E";
    case S: return "S";
    case D: return "D";
    case W: return "W";
    case N: return "N";
    case U: return "U";
    case eoDirection:
      return "eoDirection";
  }
  return "Direction(?)";
}

Direction const direction[eoDirection] = {
  E, S, D, W, N, U
};

Direction Next(Direction d) {
  switch (d) {
    case E: return S;
    case S: return D;
    case D: return W;
    case W: return N;
    case N: return U;
    case U: return E;
    case eoDirection:
      return eoDirection;
  }
  return eoDirection;
}

Direction NNext(Direction d) {
  return Next(Next(d));
}

Direction Last(Direction d) {
  switch (d) {
    case E: return U;
    case S: return E;
    case D: return S;
    case W: return D;
    case N: return W;
    case U: return N;
    case eoDirection:
      return eoDirection;
  }
  return eoDirection;
}

Direction LLast(Direction d) {
  return Last(Last(d));
}

Direction FWard(Direction d) {
  switch (d) {
    case E: return E;
    case S: return S;
    case D: return D;
    case W: return W;
    case N: return N;
    case U: return U;
    case eoDirection:
      return eoDirection;
  }
  return eoDirection;
}

Direction BWard(Direction d) {
  switch (d) {
    case E: return W;
    case S: return N;
    case D: return U;
    case W: return E;
    case N: return S;
    case U: return D;
    case eoDirection:
      return eoDirection;
  }
  return eoDirection;
}

Direction LWard(Direction d) {
  switch (d) {
    case E: return N;
    case S: return E;
    case D: return eoDirection;
    case W: return S;
    case N: return W;
    case U: return eoDirection;
    case eoDirection:
      return eoDirection;
  }
  return eoDirection;
}

Direction RWard(Direction d) {
  switch (d) {
    case E: return S;
    case S: return W;
    case D: return eoDirection;
    case W: return N;
    case N: return E;
    case U: return eoDirection;
    case eoDirection:
      return eoDirection;
  }
  return eoDirection;
}

Direction DWard(Direction d) {
  switch (d) {
    case E: return D;
    case S: return D;
    case D: return D;
    case W: return D;
    case N: return D;
    case U: return D;
    case eoDirection:
      return eoDirection;
  }
  return eoDirection;
}

Direction UWard(Direction d) {
  switch (d) {
    case E: return U;
    case S: return U;
    case D: return U;
    case W: return U;
    case N: return U;
    case U: return U;
    case eoDirection:
      return eoDirection;
  }
  return eoDirection;
}
