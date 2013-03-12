#ifndef FWDORBWD_H
#define FWDORBWD_H

enum FwdOrBwd {
  Fwd, Bwd,

  eoFwdOrBwd
};

char const *toConstCharPointer(FwdOrBwd d);

#endif // FWDORBWD_H
