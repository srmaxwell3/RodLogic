#include "directions.h"

Directions const directions[eoDirections] = { E, S, D, W, N, U };

char const *c_str(Directions d) {
  switch (d) {
    case E: return "E";
    case S: return "S";
    case D: return "D";
    case W: return "W";
    case N: return "N";
    case U: return "U";
    case eoDirections:
      return "eoDirections";
  }
  return "Directions(?)";
}

Directions Next(Directions d) {
  switch (d) {
    case E: return S;
    case S: return D;
    case D: return W;
    case W: return N;
    case N: return U;
    case U: return E;
    case eoDirections:
      return eoDirections;
  }
  return eoDirections;
}

Directions NNext(Directions d) {
  return Next(Next(d));
}

Directions Last(Directions d) {
  switch (d) {
    case E: return U;
    case S: return E;
    case D: return S;
    case W: return D;
    case N: return W;
    case U: return N;
    case eoDirections:
      return eoDirections;
  }
  return eoDirections;
}

Directions LLast(Directions d) {
  return Last(Last(d));
}

Directions FWard(Directions d) {
  switch (d) {
    case E: return E;
    case S: return S;
    case D: return D;
    case W: return W;
    case N: return N;
    case U: return U;
    case eoDirections:
      return eoDirections;
  }
  return eoDirections;
}

Directions BWard(Directions d) {
  switch (d) {
    case E: return W;
    case S: return N;
    case D: return U;
    case W: return E;
    case N: return S;
    case U: return D;
    case eoDirections:
      return eoDirections;
  }
  return eoDirections;
}

Directions LWard(Directions d) {
  switch (d) {
    case E: return N;
    case S: return E;
    case D: return eoDirections;
    case W: return S;
    case N: return W;
    case U: return eoDirections;
    case eoDirections:
      return eoDirections;
  }
  return eoDirections;
}

Directions RWard(Directions d) {
  switch (d) {
    case E: return S;
    case S: return W;
    case D: return eoDirections;
    case W: return N;
    case N: return E;
    case U: return eoDirections;
    case eoDirections:
      return eoDirections;
  }
  return eoDirections;
}
