#pragma once
#include "vector.hpp"

#include "lib7842/api/other/utility.hpp"
#include <any>
#include <map>
#include <stdexcept>
#include <string>

namespace lib7842 {

/**
 * A point that contains extra information of any type.
 */
class DataPoint : public Vector {
public:
  using Vector::Vector;

  /**
   * Convert a Vector into a DataPoint
   *
   * @param ipoint The point
   */
  explicit DataPoint(const Vector& ipoint);

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
    const std::any& data = getID(iid);
    try {
      return std::any_cast<T>(data);
    } catch (const std::bad_any_cast& e) {
      throw std::runtime_error("DataPoint::getData:: \"" + iid + "\" contains wrong type \"" +
                               data.type().name() + "\"");
    }
  }

protected:
  std::map<std::string, std::any> pathData {};

private:
  const std::any& getID(const std::string& iid) const;
};

} // namespace lib7842