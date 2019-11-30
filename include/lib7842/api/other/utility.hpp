#pragma once

namespace lib7842::util {

/////////////////
//    Sign    //
/////////////////
template <typename T> int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

} // namespace lib7842::util