#pragma once
#include "state.hpp"

#include <any>
#include <map>
#include <stdexcept>
#include <string>

namespace lib7842 {

/**
 * A point that contains extra information of any type.
 */
class DataState : public State {
public:
  using State::State;

  /**
   * Convert a State into a DataState
   *
   * @param ipoint The point
   */
  explicit DataState(const State& ipoint);

  /**
   * Set the point data.
   *
   * @param iid   The data name
   * @param idata The data
   */
  void setData(const std::string& iid, const std::any& idata);

  /**
   * Get the point data.
   *
   * @param  iid The data name
   * @tparam T   The data type
   * @return The data
   */
  template <typename T> T getData(const std::string& iid) const {
    const std::any& idata = getID(iid);
    try {
      return std::any_cast<T>(idata);
    } catch (const std::bad_any_cast& e) {
      throw std::runtime_error("DataState::getData:: \"" + iid + "\" contains wrong type \"" +
                               idata.type().name() + "\"");
    }
  }

protected:
  std::map<std::string, std::any> data {};

private:
  const std::any& getID(const std::string& iid) const;
};
} // namespace lib7842