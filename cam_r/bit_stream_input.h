#ifndef BIT_STREAM_INPUT
#define BIT_STREAM_INPUT

#include <deque>
using std::deque;
#include "edged_bool.h"

class BitStreamInput : public deque<bool> {
 public:
  BitStreamInput(bool initalValue = false) :
      deque<bool>(),
      edgedBool(initalValue)
  {
  }
  EdgedBool const &get() const { return edgedBool; }
  EdgedBool const &read() {
    if (!empty()) {
      edgedBool = front();
      pop_front();
    } else {
      edgedBool = false;
    }
    return get();
  }
 private:
  EdgedBool edgedBool;
};

#endif // BIT_STREAM_INPUT
