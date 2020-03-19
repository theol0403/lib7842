#pragma once

#include <any>
#include <map>
#include <stdexcept>
#include <string>

#include "state.hpp"
#include "vector.hpp"

namespace lib7842 {

// forward declaration
template <typename T> class Data;
using DataPoint = Data<Vector>;
using DataState = Data<State>;

/**
 * A class that contains information of any type. 
 */
template <typename T> class Data : public T {
public:
  using T::T;

  /**
   * Convert a T into a Data T
   *
   * @param ibase The base
   */
  explicit Data(const T& ibase) : T(ibase) {}

  /**
   * Set the data.
   *
   * @param iid   The data name
   * @param idata The data
   */
  void setData(const std::string& iid, const std::any& idata) {
    data[iid] = idata;
  }

  /**
   * Get the data, given the name and the type.
   *
   * @param  iid The data name
   * @tparam U   The data type
   * @return The data
   */
  template <typename U> U getData(const std::string& iid) const {
    const std::any& idata = getID(iid);
    try {
      return std::any_cast<U>(idata);
    } catch (const std::bad_any_cast&) {
      throw std::runtime_error("Data::getData:: \"" + iid + "\" contains wrong type \"" +
                               idata.type().name() + "\"");
    }
  }

protected:
  /**
   * Get the data associated with a name.
   *
   * @param  iid The iid
   * @return The data.
   */
  const std::any& getID(const std::string& iid) const {
    try {
      return data.at(iid);
    } catch (const std::out_of_range&) {
      throw std::runtime_error("Data::getID:: \"" + iid + "\" does not exist in data");
    }
  }

  std::map<std::string, std::any> data {};
};
} // namespace lib7842
